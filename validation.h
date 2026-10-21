#ifndef VALIDATION_H
#define VALIDATION_H

void removeNewline(char text[]);
int readInteger(const char prompt[], int *value);
int readFloat(const char prompt[], float *value);
int readNonEmptyText(const char prompt[], char text[], int size);
int readGender(const char prompt[], char *gender);
int confirmYesNo(const char prompt[]);

#endif
