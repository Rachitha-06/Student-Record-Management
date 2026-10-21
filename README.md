# Student-Record-Management
# StudentVault

StudentVault is a console-based C application for managing student profiles, semester records, attendance, academic trends, risk indicators, reports, and persistent local data.

## Features

- Student CRUD with safe archive and restore operations
- Validation for IDs, age, gender, semester, CGPA, attendance, and input buffers
- Search by ID, name, branch, semester, CGPA range, attendance range, status, and risk level
- Semester academic history, average CGPA, attendance, performance trend, and academic-risk analysis
- Analytics dashboard, selection-sort options, and detailed student reports
- Binary persistence with a file header, temporary save file, backup, and invalid-file checks

## Build

Use GCC from the project folder:

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c student.c validation.c academic.c analytics.c file.c -o studentvault
```

On Windows, run:

```bash
studentvault.exe
```

On Linux or macOS, run:

```bash
./studentvault
```

## Project Files

```text
main.c                 Menu controller
student.c / student.h  Student CRUD, search, archive, sorting, data model
validation.c/.h        Safe terminal input and validation
academic.c/.h          CGPA, attendance, trend, risk calculations
analytics.c/.h         Academic-record screens, dashboard, reports
file.c / file.h        Binary save, load, backup, and validation
```

## Data Files

The program creates `students.dat`, `students.bak`, and sometimes `students.tmp` in its working folder. They are intentionally excluded by `.gitignore`: they are local runtime data, not source code.

## Academic Rules

- A latest CGPA increase of at least `0.20` means **Improving**.
- A decrease of at least `0.20` means **Declining**; otherwise, the trend is **Stable**.
- **High Risk** applies for average CGPA below `5.0`, latest attendance below `60%`, or repeated decline.
- **Medium Risk** applies for average CGPA below `6.5`, latest attendance below `75%`, or a declining trend.

The project uses simple rule-based analysis, not machine learning or official academic decisions.

## Limitations

- Maximum 100 students and 8 semesters per student
- Semester CGPAs are equally averaged; subject credits are not yet included
- Designed for one user at a time
- Binary data is not a replacement for a database

