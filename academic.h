#ifndef ACADEMIC_H
#define ACADEMIC_H

#include "student.h"

float calculateAverageCgpa(const Student *student);
float calculateAverageAttendance(const Student *student);
PerformanceTrend calculatePerformanceTrend(const Student *student);
const char *getTrendLabel(PerformanceTrend trend);
float getLatestAttendance(const Student *student);
int hasRepeatedDecline(const Student *student);
AcademicRisk calculateAcademicRisk(const Student *student);
const char *getRiskLabel(AcademicRisk risk);
int findStrongestSemesterIndex(const Student *student);
int findWeakestSemesterIndex(const Student *student);

#endif
