#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "student.h"

void addAcademicRecord(Student students[], int studentCount);
void viewAcademicHistory(const Student students[], int studentCount);
void displayPerformanceAnalysis(const Student students[], int studentCount);
void displayAcademicRisk(const Student students[], int studentCount);
void displayAnalytics(const Student students[], int studentCount);
void generateStudentReport(const Student students[], int studentCount);

#endif
