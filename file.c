#include <stdio.h>
#include <string.h>

#include "file.h"

#define DATA_FILE "students.dat"
#define TEMP_DATA_FILE "students.tmp"
#define BACKUP_DATA_FILE "students.bak"
#define FILE_MAGIC "STUDENTVAULT_V1"
#define FILE_VERSION 1

typedef struct {
    char magic[20];
    int version;
    int studentCount;
} FileHeader;

static int fileExists(const char fileName[]) {
    FILE *file = fopen(fileName, "rb");
    if (file == NULL) return 0;
    fclose(file);
    return 1;
}

static int copyFile(const char sourceName[], const char destinationName[]) {
    FILE *source;
    FILE *destination;
    char buffer[512];
    size_t bytesRead;
    int success = 1;

    source = fopen(sourceName, "rb");
    if (source == NULL) return 0;
    destination = fopen(destinationName, "wb");
    if (destination == NULL) { fclose(source); return 0; }

    while ((bytesRead = fread(buffer, 1U, sizeof(buffer), source)) > 0U) {
        if (fwrite(buffer, 1U, bytesRead, destination) != bytesRead) { success = 0; break; }
    }
    if (ferror(source)) success = 0;
    if (fclose(source) != 0) success = 0;
    if (fclose(destination) != 0) success = 0;
    if (!success) remove(destinationName);
    return success;
}

static int hasTerminatingNull(const char text[], size_t size) {
    return memchr(text, '\0', size) != NULL;
}

static int isValidStudent(const Student *student) {
    int i;
    if (student->id <= 0 || student->age < 15 || student->age > 100 ||
        student->currentSemester < 1 || student->currentSemester > MAX_SEMESTERS ||
        (student->active != 0 && student->active != 1) ||
        (student->gender != 'M' && student->gender != 'F' &&
         student->gender != 'O' && student->gender != 'U')) return 0;
    if (student->name[0] == '\0' || student->branch[0] == '\0' ||
        student->contact.email[0] == '\0' || student->contact.phone[0] == '\0') return 0;
    if (!hasTerminatingNull(student->name, MAX_NAME_LENGTH) ||
        !hasTerminatingNull(student->branch, MAX_BRANCH_LENGTH) ||
        !hasTerminatingNull(student->contact.email, MAX_EMAIL_LENGTH) ||
        !hasTerminatingNull(student->contact.phone, MAX_PHONE_LENGTH)) return 0;
    for (i = 0; i < MAX_SEMESTERS; i++) {
        if (student->semesters[i].recorded != 0 && student->semesters[i].recorded != 1) return 0;
        if (student->semesters[i].recorded &&
            (!(student->semesters[i].cgpa >= 0.0f && student->semesters[i].cgpa <= 10.0f) ||
             !(student->semesters[i].attendance >= 0.0f && student->semesters[i].attendance <= 100.0f))) return 0;
    }
    return 1;
}

static int validateLoadedStudents(const Student students[], int studentCount) {
    int i, j;
    for (i = 0; i < studentCount; i++) {
        if (!isValidStudent(&students[i])) return 0;
        for (j = i + 1; j < studentCount; j++) if (students[i].id == students[j].id) return 0;
    }
    return 1;
}

static int loadStudentsFromFile(const char fileName[], Student students[], int *studentCount) {
    FILE *file;
    FileHeader header;
    Student loadedStudents[MAX_STUDENTS];
    long fileSize;
    long expectedSize;

    file = fopen(fileName, "rb");
    if (file == NULL) return 0;
    if (fseek(file, 0L, SEEK_END) != 0) { fclose(file); return -1; }
    fileSize = ftell(file);
    rewind(file);
    if (fileSize < (long)sizeof(FileHeader) || fread(&header, sizeof(FileHeader), 1U, file) != 1U) { fclose(file); return -1; }
    if (memcmp(header.magic, FILE_MAGIC, sizeof(FILE_MAGIC)) != 0 ||
        header.version != FILE_VERSION || header.studentCount < 0 ||
        header.studentCount > MAX_STUDENTS) { fclose(file); return -1; }
    expectedSize = (long)sizeof(FileHeader) + (long)header.studentCount * (long)sizeof(Student);
    if (fileSize != expectedSize) {
        fclose(file);
        return -1;
    }
    if (fread(loadedStudents, sizeof(Student), (size_t)header.studentCount, file) != (size_t)header.studentCount) {
        fclose(file);
        return -1;
    }
    if (fclose(file) != 0 || !validateLoadedStudents(loadedStudents, header.studentCount)) return -1;

    memcpy(students, loadedStudents, (size_t)header.studentCount * sizeof(Student));
    *studentCount = header.studentCount;
    return 1;
}

int saveStudents(const Student students[], int studentCount) {
    FILE *file;
    FileHeader header = {0};
    int successful = 1;

    strcpy(header.magic, FILE_MAGIC);
    header.version = FILE_VERSION;
    header.studentCount = studentCount;
    file = fopen(TEMP_DATA_FILE, "wb");
    if (file == NULL) { printf("Could not create the temporary data file.\n"); return 0; }
    if (fwrite(&header, sizeof(FileHeader), 1U, file) != 1U ||
        fwrite(students, sizeof(Student), (size_t)studentCount, file) != (size_t)studentCount) successful = 0;
    if (fclose(file) != 0) successful = 0;
    if (!successful) { remove(TEMP_DATA_FILE); printf("Saving failed. Existing data was not replaced.\n"); return 0; }
    if (fileExists(DATA_FILE) && !copyFile(DATA_FILE, BACKUP_DATA_FILE)) { remove(TEMP_DATA_FILE); printf("Saving stopped because a backup could not be created.\n"); return 0; }
    if (fileExists(DATA_FILE) && remove(DATA_FILE) != 0) { remove(TEMP_DATA_FILE); printf("Could not replace the existing data file.\n"); return 0; }
    if (rename(TEMP_DATA_FILE, DATA_FILE) != 0) {
        printf("Could not activate the newly saved data file.\n");
        if (fileExists(BACKUP_DATA_FILE)) copyFile(BACKUP_DATA_FILE, DATA_FILE);
        return 0;
    }
    printf("Data saved successfully.\n");
    return 1;
}

void loadStudents(Student students[], int *studentCount) {
    int result = loadStudentsFromFile(DATA_FILE, students, studentCount);
    if (result == 1) { printf("Loaded %d student record(s).\n", *studentCount); return; }
    if (result == -1) printf("The main data file is invalid or corrupted.\n");
    result = loadStudentsFromFile(BACKUP_DATA_FILE, students, studentCount);
    if (result == 1) printf("Recovered %d student record(s) from the backup file.\n", *studentCount);
    else if (result == 0) printf("No saved data found. Starting with an empty StudentVault.\n");
    else printf("Could not load student data. Current records were unchanged.\n");
}
