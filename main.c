#include <stdio.h>

#include "analytics.h"
#include "file.h"
#include "student.h"
#include "validation.h"

static void displayMainMenu(void) {
    printf("\n========================================\n");
    printf("              STUDENTVAULT\n");
    printf("      Academic Record Management\n");
    printf("========================================\n");
    printf("PROFILE MANAGEMENT\n");
    printf(" 1. Add Student\n");
    printf(" 2. View All Active Students\n");
    printf(" 3. View Student Profile\n");
    printf(" 4. Update Student\n");
    printf(" 5. Archive Student\n");
    printf(" 6. View Archived Students\n");
    printf(" 7. Restore Student\n");
    printf(" 8. Search Students\n");
    printf("\nACADEMIC MANAGEMENT\n");
    printf(" 9. Add Academic Record\n");
    printf("10. View Academic History\n");
    printf("11. Performance Analysis\n");
    printf("12. Academic Risk Detection\n");
    printf("\nINSIGHTS AND REPORTS\n");
    printf("13. Analytics Dashboard\n");
    printf("14. Sort Students\n");
    printf("15. Generate Student Report\n");
    printf("\nDATA MANAGEMENT\n");
    printf("16. Save Data\n");
    printf("17. Load Data\n");
    printf(" 0. Save and Exit\n");
    printf("========================================\n");
}

int main(void) {
    Student students[MAX_STUDENTS] = {0};
    int studentCount = 0;
    int choice;

    loadStudents(students, &studentCount);

    do {
        displayMainMenu();

        if (!readInteger("Choose an option: ", &choice)) {
            printf("Invalid menu choice.\n");
            continue;
        }

        switch (choice) {
            case 1: addStudent(students, &studentCount); break;
            case 2: viewAllStudents(students, studentCount); break;
            case 3: viewStudentProfile(students, studentCount); break;
            case 4: updateStudent(students, studentCount); break;
            case 5: archiveStudent(students, studentCount); break;
            case 6: viewArchivedStudents(students, studentCount); break;
            case 7: restoreStudent(students, studentCount); break;
            case 8: searchStudents(students, studentCount); break;
            case 9: addAcademicRecord(students, studentCount); break;
            case 10: viewAcademicHistory(students, studentCount); break;
            case 11: displayPerformanceAnalysis(students, studentCount); break;
            case 12: displayAcademicRisk(students, studentCount); break;
            case 13: displayAnalytics(students, studentCount); break;
            case 14: sortStudents(students, studentCount); break;
            case 15: generateStudentReport(students, studentCount); break;
            case 16: saveStudents(students, studentCount); break;
            case 17:
                if (confirmYesNo("Reload saved data and discard unsaved changes? (Y/N): ")) {
                    loadStudents(students, &studentCount);
                } else {
                    printf("Load cancelled.\n");
                }
                break;
            case 0:
                if (saveStudents(students, studentCount)) {
                    printf("Goodbye.\n");
                } else {
                    printf("Data could not be saved.\n");
                }
                break;
            default: printf("Choose a valid menu option.\n");
        }
    } while (choice != 0);

    return 0;
}
