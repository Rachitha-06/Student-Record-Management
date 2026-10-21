#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "academic.h"
#include "analytics.h"
#include "student.h"
#include "validation.h"

void addAcademicRecord(Student students[], int studentCount) {
    int id;
    int index;
    int semester;
    float cgpa;
    float attendance;
    SemesterRecord *record;

    printf("\n--- Add Academic Record ---\n");
    if (!readInteger("Enter student ID: ", &id)) {
        printf("Invalid ID.\n");
        return;
    }
    index = findStudentIndexById(students, studentCount, id);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }
    if (!students[index].active) {
        printf("Cannot edit an archived student's academic record.\n");
        return;
    }
    if (!readInteger("Semester number: ", &semester) || semester < 1 ||
        semester > students[index].currentSemester) {
        printf("Enter a semester from 1 to %d.\n", students[index].currentSemester);
        return;
    }

    record = &students[index].semesters[semester - 1];
    if (record->recorded && !confirmYesNo("A record already exists. Replace it? (Y/N): ")) {
        printf("Academic record was not changed.\n");
        return;
    }
    if (!readFloat("CGPA (0.0 - 10.0): ", &cgpa) || cgpa < 0.0f || cgpa > 10.0f) {
        printf("Invalid CGPA.\n");
        return;
    }
    if (!readFloat("Attendance percentage (0 - 100): ", &attendance) || attendance < 0.0f || attendance > 100.0f) {
        printf("Invalid attendance percentage.\n");
        return;
    }

    record->cgpa = cgpa;
    record->attendance = attendance;
    record->recorded = 1;
    printf("Academic record saved successfully.\n");
}

void viewAcademicHistory(const Student students[], int studentCount) {
    int id;
    int index;
    int i;
    int recordCount = 0;

    printf("\n--- View Academic History ---\n");
    if (!readInteger("Enter student ID: ", &id)) { printf("Invalid ID.\n"); return; }
    index = findStudentIndexById(students, studentCount, id);
    if (index == -1) { printf("Student not found.\n"); return; }

    printf("\nAcademic history for %s (ID: %d)\n", students[index].name, students[index].id);
    printf("----------------------------------------\n");
    for (i = 0; i < MAX_SEMESTERS; i++) {
        if (students[index].semesters[i].recorded) {
            printf("Semester %d | CGPA: %.2f | Attendance: %.1f%%\n", i + 1, students[index].semesters[i].cgpa, students[index].semesters[i].attendance);
            recordCount++;
        }
    }
    if (recordCount == 0) printf("No academic records available.\n");
}

void displayPerformanceAnalysis(const Student students[], int studentCount) {
    int id;
    int index;
    float averageCgpa;
    PerformanceTrend trend;

    printf("\n--- Performance Analysis ---\n");
    if (!readInteger("Enter student ID: ", &id)) { printf("Invalid ID.\n"); return; }
    index = findStudentIndexById(students, studentCount, id);
    if (index == -1) { printf("Student not found.\n"); return; }

    averageCgpa = calculateAverageCgpa(&students[index]);
    trend = calculatePerformanceTrend(&students[index]);
    printf("\n========================================\n         PERFORMANCE ANALYSIS\n========================================\n");
    printf("Student      : %s\n", students[index].name);
    if (averageCgpa < 0.0f) printf("Average CGPA : No academic records\n");
    else printf("Average CGPA : %.2f\n", averageCgpa);
    printf("Trend        : %s\n", getTrendLabel(trend));
    printf("========================================\n");
}

void displayAcademicRisk(const Student students[], int studentCount) {
    int id;
    int index;
    float averageCgpa;
    float latestAttendance;
    PerformanceTrend trend;
    AcademicRisk risk;

    printf("\n--- Academic Risk Detection ---\n");
    if (!readInteger("Enter student ID: ", &id)) { printf("Invalid ID.\n"); return; }
    index = findStudentIndexById(students, studentCount, id);
    if (index == -1) { printf("Student not found.\n"); return; }

    averageCgpa = calculateAverageCgpa(&students[index]);
    latestAttendance = getLatestAttendance(&students[index]);
    trend = calculatePerformanceTrend(&students[index]);
    risk = calculateAcademicRisk(&students[index]);
    printf("\n========================================\n          ACADEMIC RISK REPORT\n========================================\n");
    printf("Student           : %s\n", students[index].name);
    if (averageCgpa < 0.0f) {
        printf("Risk Level        : INSUFFICIENT DATA\nReason            : No academic record exists.\n");
    } else {
        printf("Average CGPA      : %.2f\n", averageCgpa);
        printf("Latest Attendance : %.1f%%\n", latestAttendance);
        printf("Performance Trend : %s\n", getTrendLabel(trend));
        printf("Risk Level        : %s\n", getRiskLabel(risk));
        printf("----------------------------------------\n");
        if (averageCgpa < 5.0f) printf("- Average CGPA is below 5.0.\n");
        else if (averageCgpa < 6.5f) printf("- Average CGPA is below 6.5.\n");
        if (latestAttendance < 60.0f) printf("- Latest attendance is below 60%%.\n");
        else if (latestAttendance < 75.0f) printf("- Latest attendance is below 75%%.\n");
        if (hasRepeatedDecline(&students[index])) printf("- The latest three results show repeated decline.\n");
        else if (trend == TREND_DECLINING) printf("- The latest result declined from the previous one.\n");
        if (risk == RISK_LOW) printf("- Current academic indicators are within safe limits.\n");
    }
    printf("========================================\n");
}

static int areEqualIgnoreCase(const char first[], const char second[]) {
    int i = 0;
    while (first[i] != '\0' && second[i] != '\0') {
        if (tolower((unsigned char)first[i]) != tolower((unsigned char)second[i])) return 0;
        i++;
    }
    return first[i] == '\0' && second[i] == '\0';
}

void displayAnalytics(const Student students[], int studentCount) {
    int i, j, active = 0, archived = 0, withRecords = 0;
    int improving = 0, stable = 0, declining = 0;
    int highRisk = 0, mediumRisk = 0, lowRisk = 0;
    int highestIndex = -1, branchCount = 0, attendanceCount = 0;
    float totalAverageCgpa = 0.0f, totalAttendance = 0.0f;
    char branches[MAX_STUDENTS][MAX_BRANCH_LENGTH];
    int branchStudentCounts[MAX_STUDENTS] = {0};

    for (i = 0; i < studentCount; i++) {
        float average;
        PerformanceTrend trend;
        AcademicRisk risk;
        int branchFound = 0;
        if (!students[i].active) { archived++; continue; }
        active++;
        for (j = 0; j < branchCount; j++) if (areEqualIgnoreCase(students[i].branch, branches[j])) { branchStudentCounts[j]++; branchFound = 1; break; }
        if (!branchFound) { strcpy(branches[branchCount], students[i].branch); branchStudentCounts[branchCount] = 1; branchCount++; }

        average = calculateAverageCgpa(&students[i]);
        if (average >= 0.0f) {
            withRecords++;
            totalAverageCgpa += average;
            if (highestIndex == -1 || average > calculateAverageCgpa(&students[highestIndex])) highestIndex = i;
            risk = calculateAcademicRisk(&students[i]);
            if (risk == RISK_HIGH) highRisk++; else if (risk == RISK_MEDIUM) mediumRisk++; else if (risk == RISK_LOW) lowRisk++;
            trend = calculatePerformanceTrend(&students[i]);
            if (trend == TREND_IMPROVING) improving++; else if (trend == TREND_STABLE) stable++; else if (trend == TREND_DECLINING) declining++;
        }
        for (j = 0; j < MAX_SEMESTERS; j++) if (students[i].semesters[j].recorded) { totalAttendance += students[i].semesters[j].attendance; attendanceCount++; }
    }

    printf("\n========================================\n           ANALYTICS DASHBOARD\n========================================\n");
    printf("Total Students             : %d\nActive Students            : %d\nArchived Students          : %d\n", studentCount, active, archived);
    printf("Active Students with Data  : %d\n----------------------------------------\n", withRecords);
    if (withRecords > 0) {
        printf("Average CGPA               : %.2f\n", totalAverageCgpa / withRecords);
        printf("Highest Average CGPA       : %.2f (%s, ID: %d)\n", calculateAverageCgpa(&students[highestIndex]), students[highestIndex].name, students[highestIndex].id);
    } else printf("Average CGPA               : No academic data\nHighest Average CGPA       : No academic data\n");
    if (attendanceCount > 0) printf("Average Attendance         : %.1f%%\n", totalAttendance / attendanceCount);
    else printf("Average Attendance         : No academic data\n");
    printf("----------------------------------------\nImproving Students         : %d\nStable Students            : %d\nDeclining Students         : %d\n", improving, stable, declining);
    printf("High-Risk Students         : %d\nMedium-Risk Students       : %d\nLow-Risk Students          : %d\n", highRisk, mediumRisk, lowRisk);
    printf("----------------------------------------\nBRANCH-WISE ACTIVE STUDENTS\n");
    if (branchCount == 0) printf("No active students.\n");
    else for (i = 0; i < branchCount; i++) printf("%-20s : %d\n", branches[i], branchStudentCounts[i]);
    printf("========================================\n");
}

void generateStudentReport(const Student students[], int studentCount) {
    int id, index, i, strongest, weakest;
    float averageCgpa, averageAttendance;
    PerformanceTrend trend;
    AcademicRisk risk;
    const Student *student;

    printf("\n--- Generate Student Report ---\n");
    if (!readInteger("Enter student ID: ", &id)) { printf("Invalid ID.\n"); return; }
    index = findStudentIndexById(students, studentCount, id);
    if (index == -1) { printf("Student not found.\n"); return; }
    student = &students[index];
    averageCgpa = calculateAverageCgpa(student);
    averageAttendance = calculateAverageAttendance(student);
    trend = calculatePerformanceTrend(student);
    risk = calculateAcademicRisk(student);
    strongest = findStrongestSemesterIndex(student);
    weakest = findWeakestSemesterIndex(student);

    printf("\n========================================\n        STUDENT ACADEMIC REPORT\n========================================\n");
    printf("Student ID     : %d\nName           : %s\nAge            : %d\nBranch         : %s\nCurrent Sem    : %d\n", student->id, student->name, student->age, student->branch, student->currentSemester);
    printf("Email          : %s\nPhone          : %s\nStatus         : %s\n", student->contact.email, student->contact.phone, student->active ? "Active" : "Archived");
    printf("\n----------------------------------------\nACADEMIC HISTORY\n----------------------------------------\n");
    for (i = 0; i < MAX_SEMESTERS; i++) if (student->semesters[i].recorded) printf("Semester %d | CGPA: %.2f | Attendance: %.1f%%\n", i + 1, student->semesters[i].cgpa, student->semesters[i].attendance);
    if (averageCgpa < 0.0f) printf("No academic records available.\n");
    printf("\n----------------------------------------\nACADEMIC SNAPSHOT\n----------------------------------------\n");
    if (averageCgpa < 0.0f) {
        printf("Average CGPA       : No data\nAverage Attendance : No data\nTrend              : INSUFFICIENT DATA\nRisk Level         : INSUFFICIENT DATA\nStrongest Semester : No data\nWeakest Semester   : No data\n");
    } else {
        printf("Average CGPA       : %.2f\nAverage Attendance : %.1f%%\nTrend              : %s\nRisk Level         : %s\n", averageCgpa, averageAttendance, getTrendLabel(trend), getRiskLabel(risk));
        printf("Strongest Semester : Semester %d (CGPA: %.2f)\n", strongest + 1, student->semesters[strongest].cgpa);
        printf("Weakest Semester   : Semester %d (CGPA: %.2f)\n", weakest + 1, student->semesters[weakest].cgpa);
    }
    printf("========================================\n");
}
