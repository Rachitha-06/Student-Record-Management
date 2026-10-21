#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "validation.h"

void removeNewline(char text[]) {
    text[strcspn(text, "\n")] = '\0';
}

static int readLine(char text[], size_t size) {
    int character;

    if (fgets(text, (int)size, stdin) == NULL) {
        return 0;
    }

    if (strchr(text, '\n') == NULL) {
        while ((character = getchar()) != '\n' && character != EOF) {
            /* Discard the rest of an overlong input line. */
        }
        return 0;
    }

    removeNewline(text);
    return 1;
}

static void trimWhitespace(char text[]) {
    char *start = text;
    char *end;

    while (isspace((unsigned char)*start)) {
        start++;
    }

    if (start != text) {
        memmove(text, start, strlen(start) + 1U);
    }

    end = text + strlen(text);
    while (end > text && isspace((unsigned char)end[-1])) {
        end--;
    }
    *end = '\0';
}

int readInteger(const char prompt[], int *value) {
    char line[100];
    char *endPointer;
    long number;

    printf("%s", prompt);
    if (!readLine(line, sizeof(line))) {
        return 0;
    }

    errno = 0;
    number = strtol(line, &endPointer, 10);
    if (endPointer == line || errno == ERANGE ||
        number < INT_MIN || number > INT_MAX) {
        return 0;
    }

    while (isspace((unsigned char)*endPointer)) {
        endPointer++;
    }

    if (*endPointer != '\0') {
        return 0;
    }

    *value = (int)number;
    return 1;
}

int readFloat(const char prompt[], float *value) {
    char line[100];
    char *endPointer;
    float number;

    printf("%s", prompt);
    if (!readLine(line, sizeof(line))) {
        return 0;
    }

    errno = 0;
    number = strtof(line, &endPointer);
    if (endPointer == line || errno == ERANGE || !isfinite(number)) {
        return 0;
    }

    while (isspace((unsigned char)*endPointer)) {
        endPointer++;
    }

    if (*endPointer != '\0') {
        return 0;
    }

    *value = number;
    return 1;
}

int readNonEmptyText(const char prompt[], char text[], int size) {
    printf("%s", prompt);
    if (!readLine(text, (size_t)size)) {
        return 0;
    }

    trimWhitespace(text);
    return text[0] != '\0';
}

int readGender(const char prompt[], char *gender) {
    char line[20];
    char value;

    printf("%s", prompt);
    if (!readLine(line, sizeof(line))) {
        return 0;
    }

    trimWhitespace(line);
    if (strlen(line) != 1U) {
        return 0;
    }

    value = (char)toupper((unsigned char)line[0]);
    if (value != 'M' && value != 'F' && value != 'O' && value != 'U') {
        return 0;
    }

    *gender = value;
    return 1;
}

int confirmYesNo(const char prompt[]) {
    char answer[10];

    printf("%s", prompt);
    if (!readLine(answer, sizeof(answer))) {
        return 0;
    }

    trimWhitespace(answer);
    return (answer[0] == 'Y' || answer[0] == 'y') && answer[1] == '\0';
}
