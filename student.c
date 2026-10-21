#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "academic.h"
#include "student.h"
#include "validation.h"

static void displayStudentSummary(const Student student) {
    printf("\nID       : %d\n", student.id);
    printf("Name     : %s\n", student.name);
    printf("Branch   : %s\n", student.branch);
    printf("Semester : %d\n", student.currentSemester);
    printf("Status   : %s\n", student.active ? "Active" : "Archived");
}

static int studentIdExists(const Student students[], int studentCount, int id) {
    return findStudentIndexById(students, studentCount, id) != -1;
}

int findStudentIndexById(const Student students[], int studentCount, int id) {
    int i;

    for (i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            return i;
        }
    }
    return -1;
}

void addStudent(Student students[], int *studentCount) {
    Student newStudent = {0};
    int i;

    if (*studentCount >= MAX_STUDENTS) {
        printf("\nStudentVault is full. Cannot add more students.\n");
        return;
    }

    printf("\n--- Add Student ---\n");
    if (!readInteger("Student ID: ", &newStudent.id) || newStudent.id <= 0) {
        printf("Invalid ID. Enter a positive whole number.\n");
        return;
    }
    if (studentIdExists(students, *studentCount, newStudent.id)) {
        printf("A student with this ID already exists.\n");
        return;
    }
    if (!readNonEmptyText("Name: ", newStudent.name, MAX_NAME_LENGTH)) {
        printf("Name cannot be empty or too long.\n");
        return;
    }
    if (!readInteger("Age: ", &newStudent.age) ||
        newStudent.age < 15 || newStudent.age > 100) {
        printf("Invalid age. Enter a value from 15 to 100.\n");
        return;
    }
    if (!readGender("Gender (M/F/O/U): ", &newStudent.gender)) {
        printf("Invalid gender. Use M, F, O, or U.\n");
        return;
    }
    if (!readNonEmptyText("Branch: ", newStudent.branch, MAX_BRANCH_LENGTH)) {
        printf("Branch cannot be empty or too long.\n");
        return;
    }
    if (!readInteger("Current semester (1-8): ", &newStudent.currentSemester) ||
        newStudent.currentSemester < 1 ||
        newStudent.currentSemester > MAX_SEMESTERS) {
        printf("Invalid semester.\n");
        return;
    }
    if (!readNonEmptyText("Email: ", newStudent.contact.email, MAX_EMAIL_LENGTH)) {
        printf("Email cannot be empty or too long.\n");
        return;
    }
    if (!readNonEmptyText("Phone: ", newStudent.contact.phone, MAX_PHONE_LENGTH)) {
        printf("Phone cannot be empty or too long.\n");
        return;
    }

    for (i = 0; i < MAX_SEMESTERS; i++) {
        newStudent.semesters[i].recorded = 0;
    }
    newStudent.active = 1;
    students[*studentCount] = newStudent;
    (*studentCount)++;
    printf("Student added successfully.\n");
}

void viewAllStudents(const Student students[], int studentCount) {
    int i;
    int activeCount = 0;

    printf("\n--- Active Students ---\n");
    for (i = 0; i < studentCount; i++) {
        if (students[i].active) {
            displayStudentSummary(students[i]);
            activeCount++;
        }
    }
    if (activeCount == 0) {
        printf("No active students found.\n");
    }
}

void viewStudentProfile(const Student students[], int studentCount) {
    int id;
    int index;
    const Student *student;

    printf("\n--- View Student Profile ---\n");
    if (!readInteger("Enter student ID: ", &id)) {
        printf("Invalid ID.\n");
        return;
    }
    index = findStudentIndexById(students, studentCount, id);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    student = &students[index];
    printf("\n========================================\n");
    printf("            STUDENT PROFILE\n");
    printf("========================================\n");
    printf("Student ID       : %d\n", student->id);
    printf("Name             : %s\n", student->name);
    printf("Age              : %d\n", student->age);
    printf("Gender           : %c\n", student->gender);
    printf("Branch           : %s\n", student->branch);
    printf("Current Semester : %d\n", student->currentSemester);
    printf("Email            : %s\n", student->contact.email);
    printf("Phone            : %s\n", student->contact.phone);
    printf("Status           : %s\n", student->active ? "Active" : "Archived");
    printf("========================================\n");
}

void updateStudent(Student students[], int studentCount) {
    int id;
    int index;
    int choice;
    int newSemester;
    char newText[MAX_EMAIL_LENGTH];

    printf("\n--- Update Student ---\n");
    if (!readInteger("Enter student ID: ", &id)) {
        printf("Invalid ID.\n");
        return;
    }
    index = findStudentIndexById(students, studentCount, id);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    printf("1. Email\n2. Phone\n3. Branch\n4. Current Semester\n");
    if (!readInteger("Choose an option: ", &choice)) {
        printf("Invalid choice.\n");
        return;
    }

    switch (choice) {
        case 1:
            if (!readNonEmptyText("New email: ", newText, MAX_EMAIL_LENGTH)) {
                printf("Email cannot be empty or too long.\n");
                return;
            }
            strcpy(students[index].contact.email, newText);
            break;
        case 2:
            if (!readNonEmptyText("New phone: ", newText, MAX_PHONE_LENGTH)) {
                printf("Phone cannot be empty or too long.\n");
                return;
            }
            strcpy(students[index].contact.phone, newText);
            break;
        case 3:
            if (!readNonEmptyText("New branch: ", newText, MAX_BRANCH_LENGTH)) {
                printf("Branch cannot be empty or too long.\n");
                return;
            }
            strcpy(students[index].branch, newText);
            break;
        case 4:
            if (!readInteger("New semester (1-8): ", &newSemester) ||
                newSemester < 1 || newSemester > MAX_SEMESTERS) {
                printf("Invalid semester.\n");
                return;
            }
            students[index].currentSemester = newSemester;
            break;
        default:
            printf("Choose a valid update option.\n");
            return;
    }
    printf("Student updated successfully.\n");
}

void archiveStudent(Student students[], int studentCount) {
    int id;
    int index;

    printf("\n--- Archive Student ---\n");
    if (!readInteger("Enter student ID: ", &id)) {
        printf("Invalid ID.\n");
        return;
    }
    index = findStudentIndexById(students, studentCount, id);
    if (index == -1) {
        printf("Student not found.\n");
    } else if (!students[index].active) {
        printf("This student is already archived.\n");
    } else {
        students[index].active = 0;
        printf("%s has been archived successfully.\n", students[index].name);
    }
}

void viewArchivedStudents(const Student students[], int studentCount) {
    int i;
    int archivedCount = 0;

    printf("\n--- Archived Students ---\n");
    for (i = 0; i < studentCount; i++) {
        if (!students[i].active) {
            displayStudentSummary(students[i]);
            archivedCount++;
        }
    }
    if (archivedCount == 0) {
        printf("No archived students found.\n");
    }
}

void restoreStudent(Student students[], int studentCount) {
    int id;
    int index;

    printf("\n--- Restore Student ---\n");
    if (!readInteger("Enter student ID: ", &id)) {
        printf("Invalid ID.\n");
        return;
    }
    index = findStudentIndexById(students, studentCount, id);
    if (index == -1) {
        printf("Student not found.\n");
    } else if (students[index].active) {
        printf("This student is already active.\n");
    } else {
        students[index].active = 1;
        printf("%s has been restored successfully.\n", students[index].name);
    }
}

static int containsIgnoreCase(const char text[], const char query[]) {
    int i;
    int j;

    for (i = 0; text[i] != '\0'; i++) {
        j = 0;
        while (query[j] != '\0' && text[i + j] != '\0' &&
               tolower((unsigned char)text[i + j]) ==
               tolower((unsigned char)query[j])) {
            j++;
        }
        if (query[j] == '\0') {
            return 1;
        }
    }
    return query[0] == '\0';
}

void searchStudents(const Student students[], int studentCount) {
    int choice;
    int id;
    int semester;
    int statusChoice;
    int riskChoice;
    int i;
    int foundCount = 0;
    float minimum;
    float maximum;
    float value;
    AcademicRisk risk;
    char query[MAX_NAME_LENGTH];

    printf("\n--- Search Students ---\n");
    printf("1. Student ID\n2. Name\n3. Branch\n4. Semester\n");
    printf("5. Average CGPA Range\n6. Average Attendance Range\n");
    printf("7. Student Status\n8. Academic Risk Level\n");
    if (!readInteger("Choose search type: ", &choice)) {
        printf("Invalid choice.\n");
        return;
    }

    switch (choice) {
        case 1:
            if (!readInteger("Enter student ID: ", &id)) {
                printf("Invalid ID.\n");
                return;
            }
            i = findStudentIndexById(students, studentCount, id);
            if (i != -1) {
                displayStudentSummary(students[i]);
                foundCount = 1;
            }
            break;
        case 2:
            if (!readNonEmptyText("Enter name text: ", query, MAX_NAME_LENGTH)) return;
            for (i = 0; i < studentCount; i++) if (students[i].active && containsIgnoreCase(students[i].name, query)) { displayStudentSummary(students[i]); foundCount++; }
            break;
        case 3:
            if (!readNonEmptyText("Enter branch text: ", query, MAX_NAME_LENGTH)) return;
            for (i = 0; i < studentCount; i++) if (students[i].active && containsIgnoreCase(students[i].branch, query)) { displayStudentSummary(students[i]); foundCount++; }
            break;
        case 4:
            if (!readInteger("Enter semester (1-8): ", &semester) || semester < 1 || semester > MAX_SEMESTERS) { printf("Invalid semester.\n"); return; }
            for (i = 0; i < studentCount; i++) if (students[i].active && students[i].currentSemester == semester) { displayStudentSummary(students[i]); foundCount++; }
            break;
        case 5:
            if (!readFloat("Minimum CGPA (0.0-10.0): ", &minimum) || !readFloat("Maximum CGPA (0.0-10.0): ", &maximum) || minimum < 0.0f || maximum > 10.0f || minimum > maximum) { printf("Invalid CGPA range.\n"); return; }
            for (i = 0; i < studentCount; i++) { value = calculateAverageCgpa(&students[i]); if (students[i].active && value >= minimum && value <= maximum) { displayStudentSummary(students[i]); foundCount++; } }
            break;
        case 6:
            if (!readFloat("Minimum attendance (0-100): ", &minimum) || !readFloat("Maximum attendance (0-100): ", &maximum) || minimum < 0.0f || maximum > 100.0f || minimum > maximum) { printf("Invalid attendance range.\n"); return; }
            for (i = 0; i < studentCount; i++) { value = calculateAverageAttendance(&students[i]); if (students[i].active && value >= minimum && value <= maximum) { displayStudentSummary(students[i]); foundCount++; } }
            break;
        case 7:
            printf("1. Active\n2. Archived\n");
            if (!readInteger("Choose status: ", &statusChoice) || (statusChoice != 1 && statusChoice != 2)) { printf("Invalid status choice.\n"); return; }
            for (i = 0; i < studentCount; i++) if ((statusChoice == 1 && students[i].active) || (statusChoice == 2 && !students[i].active)) { displayStudentSummary(students[i]); foundCount++; }
            break;
        case 8:
            printf("1. Low Risk\n2. Medium Risk\n3. High Risk\n");
            if (!readInteger("Choose risk level: ", &riskChoice) || riskChoice < 1 || riskChoice > 3) { printf("Invalid risk choice.\n"); return; }
            for (i = 0; i < studentCount; i++) { risk = calculateAcademicRisk(&students[i]); if (students[i].active && ((riskChoice == 1 && risk == RISK_LOW) || (riskChoice == 2 && risk == RISK_MEDIUM) || (riskChoice == 3 && risk == RISK_HIGH))) { displayStudentSummary(students[i]); foundCount++; } }
            break;
        default:
            printf("Choose a valid search option.\n");
            return;
    }

    if (foundCount == 0) printf("No matching students found.\n");
    else printf("\nMatches found: %d\n", foundCount);
}

static int compareTextIgnoreCase(const char first[], const char second[]) {
    int i = 0;
    while (first[i] != '\0' && second[i] != '\0') {
        char a = (char)tolower((unsigned char)first[i]);
        char b = (char)tolower((unsigned char)second[i]);
        if (a != b) return a < b ? -1 : 1;
        i++;
    }
    return first[i] == second[i] ? 0 : (first[i] == '\0' ? -1 : 1);
}

static int shouldComeBefore(const Student *candidate, const Student *current, int sortChoice) {
    float candidateValue;
    float currentValue;
    switch (sortChoice) {
        case 1: return candidate->id < current->id;
        case 2: return compareTextIgnoreCase(candidate->name, current->name) < 0;
        case 3:
            candidateValue = calculateAverageCgpa(candidate);
            currentValue = calculateAverageCgpa(current);
            return candidateValue > currentValue;
        case 4:
            candidateValue = calculateAverageAttendance(candidate);
            currentValue = calculateAverageAttendance(current);
            return candidateValue > currentValue;
        case 5: return candidate->currentSemester < current->currentSemester;
        default: return 0;
    }
}

void sortStudents(Student students[], int studentCount) {
    int choice;
    int i;
    int j;
    int selected;
    Student temporary;

    if (studentCount < 2) { printf("At least two students are needed for sorting.\n"); return; }
    printf("\n--- Sort Students ---\n1. ID (low to high)\n2. Name (A to Z)\n3. Average CGPA (high to low)\n4. Average Attendance (high to low)\n5. Current Semester (low to high)\n");
    if (!readInteger("Choose sorting method: ", &choice) || choice < 1 || choice > 5) { printf("Invalid sorting choice.\n"); return; }

    for (i = 0; i < studentCount - 1; i++) {
        selected = i;
        for (j = i + 1; j < studentCount; j++) if (shouldComeBefore(&students[j], &students[selected], choice)) selected = j;
        if (selected != i) { temporary = students[i]; students[i] = students[selected]; students[selected] = temporary; }
    }
    printf("Students sorted successfully.\n");
}
