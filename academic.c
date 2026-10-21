#include "academic.h"

float calculateAverageCgpa(const Student *student) {
    int i;
    int recordCount = 0;
    float total = 0.0f;

    for (i = 0; i < MAX_SEMESTERS; i++) {
        if (student->semesters[i].recorded) {
            total += student->semesters[i].cgpa;
            recordCount++;
        }
    }

    return recordCount == 0 ? -1.0f : total / recordCount;
}

float calculateAverageAttendance(const Student *student) {
    int i;
    int recordCount = 0;
    float total = 0.0f;

    for (i = 0; i < MAX_SEMESTERS; i++) {
        if (student->semesters[i].recorded) {
            total += student->semesters[i].attendance;
            recordCount++;
        }
    }

    return recordCount == 0 ? -1.0f : total / recordCount;
}

PerformanceTrend calculatePerformanceTrend(const Student *student) {
    int i;
    int found = 0;
    float latest = 0.0f;
    float previous = 0.0f;

    for (i = MAX_SEMESTERS - 1; i >= 0; i--) {
        if (student->semesters[i].recorded) {
            if (found == 0) {
                latest = student->semesters[i].cgpa;
            } else {
                previous = student->semesters[i].cgpa;
                found++;
                break;
            }
            found++;
        }
    }

    if (found < 2) {
        return TREND_INSUFFICIENT_DATA;
    }
    if (latest - previous >= 0.20f) {
        return TREND_IMPROVING;
    }
    if (latest - previous <= -0.20f) {
        return TREND_DECLINING;
    }
    return TREND_STABLE;
}

const char *getTrendLabel(PerformanceTrend trend) {
    switch (trend) {
        case TREND_IMPROVING: return "IMPROVING";
        case TREND_STABLE: return "STABLE";
        case TREND_DECLINING: return "DECLINING";
        default: return "INSUFFICIENT DATA";
    }
}

float getLatestAttendance(const Student *student) {
    int i;

    for (i = MAX_SEMESTERS - 1; i >= 0; i--) {
        if (student->semesters[i].recorded) {
            return student->semesters[i].attendance;
        }
    }
    return -1.0f;
}

int hasRepeatedDecline(const Student *student) {
    int i;
    int found = 0;
    float latest = 0.0f;
    float middle = 0.0f;
    float oldest = 0.0f;

    for (i = MAX_SEMESTERS - 1; i >= 0; i--) {
        if (student->semesters[i].recorded) {
            if (found == 0) {
                latest = student->semesters[i].cgpa;
            } else if (found == 1) {
                middle = student->semesters[i].cgpa;
            } else {
                oldest = student->semesters[i].cgpa;
                found++;
                break;
            }
            found++;
        }
    }

    return found >= 3 && latest <= middle - 0.20f &&
           middle <= oldest - 0.20f;
}

AcademicRisk calculateAcademicRisk(const Student *student) {
    float averageCgpa = calculateAverageCgpa(student);
    float latestAttendance = getLatestAttendance(student);
    PerformanceTrend trend = calculatePerformanceTrend(student);

    if (averageCgpa < 0.0f) {
        return RISK_INSUFFICIENT_DATA;
    }
    if (averageCgpa < 5.0f || latestAttendance < 60.0f ||
        hasRepeatedDecline(student)) {
        return RISK_HIGH;
    }
    if (averageCgpa < 6.5f || latestAttendance < 75.0f ||
        trend == TREND_DECLINING) {
        return RISK_MEDIUM;
    }
    return RISK_LOW;
}

const char *getRiskLabel(AcademicRisk risk) {
    switch (risk) {
        case RISK_LOW: return "LOW RISK";
        case RISK_MEDIUM: return "MEDIUM RISK";
        case RISK_HIGH: return "HIGH RISK";
        default: return "INSUFFICIENT DATA";
    }
}

int findStrongestSemesterIndex(const Student *student) {
    int i;
    int strongest = -1;

    for (i = 0; i < MAX_SEMESTERS; i++) {
        if (student->semesters[i].recorded &&
            (strongest == -1 || student->semesters[i].cgpa >
             student->semesters[strongest].cgpa)) {
            strongest = i;
        }
    }
    return strongest;
}

int findWeakestSemesterIndex(const Student *student) {
    int i;
    int weakest = -1;

    for (i = 0; i < MAX_SEMESTERS; i++) {
        if (student->semesters[i].recorded &&
            (weakest == -1 || student->semesters[i].cgpa <
             student->semesters[weakest].cgpa)) {
            weakest = i;
        }
    }
    return weakest;
}
