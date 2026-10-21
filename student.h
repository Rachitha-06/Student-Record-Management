#ifndef STUDENT_H
#define STUDENT_H

#define MAX_STUDENTS 100
#define MAX_SEMESTERS 8
#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 50
#define MAX_PHONE_LENGTH 15
#define MAX_BRANCH_LENGTH 20

typedef struct {
    char email[MAX_EMAIL_LENGTH];
    char phone[MAX_PHONE_LENGTH];
} Contact;

typedef struct {
    float cgpa;
    float attendance;
    int recorded;
} SemesterRecord;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    int age;
    char gender;
    char branch[MAX_BRANCH_LENGTH];
    int currentSemester;
    Contact contact;
    SemesterRecord semesters[MAX_SEMESTERS];
    int active;
} Student;

typedef enum {
    TREND_INSUFFICIENT_DATA,
    TREND_IMPROVING,
    TREND_STABLE,
    TREND_DECLINING
} PerformanceTrend;

typedef enum {
    RISK_INSUFFICIENT_DATA,
    RISK_LOW,
    RISK_MEDIUM,
    RISK_HIGH
} AcademicRisk;

int findStudentIndexById(const Student students[], int studentCount, int id);
void addStudent(Student students[], int *studentCount);
void viewAllStudents(const Student students[], int studentCount);
void viewStudentProfile(const Student students[], int studentCount);
void updateStudent(Student students[], int studentCount);
void archiveStudent(Student students[], int studentCount);
void viewArchivedStudents(const Student students[], int studentCount);
void restoreStudent(Student students[], int studentCount);
void searchStudents(const Student students[], int studentCount);
void sortStudents(Student students[], int studentCount);

#endif
