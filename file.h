#ifndef FILE_H
#define FILE_H

#include "student.h"

int saveStudents(const Student students[], int studentCount);
void loadStudents(Student students[], int *studentCount);

#endif
