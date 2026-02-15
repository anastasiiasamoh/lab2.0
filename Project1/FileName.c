#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Data Structures
struct DayOfBirth {
    int day;
    int month;
    int year;
};

struct Student {
    char surname[50];
    char name[50];
    struct DayOfBirth data;
    int grades[3];
    struct Student* next;
};

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Helper: Check if student has at least one grade 4 or 5
int hasGrade4or5(struct Student* s) {
    for (int i = 0; i < 3; i++) {
        if (s->grades[i] == 4 || s->grades[i] == 5) {
            return 1;
        }
    }
    return 0;
}

// ============================================================================
// LIST MANAGEMENT FUNCTIONS
// ============================================================================

// Create and add a new student to the sorted list
struct Student* addStudent(struct Student** start, const char* surname, const char* name,
    int d, int m, int y, int g1, int g2, int g3) {
    // Allocate memory
    struct Student* s = (struct Student*)malloc(sizeof(struct Student));
    if (!s) {
        printf("Error: Not enough memory\n");
        return NULL;
    }

    // Initialize student data
    strcpy_s(s->surname, sizeof(s->surname), surname);
    strcpy_s(s->name, sizeof(s->name), name);
    s->data.day = d;
    s->data.month = m;
    s->data.year = y;
    s->grades[0] = g1;
    s->grades[1] = g2;
    s->grades[2] = g3;
    s->next = NULL;

    // Insert in sorted order by surname
    if (*start == NULL || strcmp((*start)->surname, s->surname) > 0) {
        s->next = *start;
        *start = s;
        return s;
    }

    struct Student* curr = *start;
    while (curr->next && strcmp(curr->next->surname, s->surname) < 0) {
        curr = curr->next;
    }
    s->next = curr->next;
    curr->next = s;

    return s;
}

// Delete students who don't have any grades 4 or 5
void deleteStudentsByCondition(struct Student** start) {
    struct Student* curr = *start;
    struct Student* prev = NULL;

    while (curr != NULL) {
        if (!hasGrade4or5(curr)) {
            struct Student* toDelete = curr;

            if (prev == NULL) {
                *start = curr->next;
                curr = *start;
            }
            else {
                prev->next = curr->next;
                curr = prev->next;
            }
            free(toDelete);
        }
        else {
            prev = curr;
            curr = curr->next;
        }
    }
}

// Display all students in the list
void displayStudents(struct Student* start, const char* header) {
    if (header) {
        printf("\n%s\n", header);
        printf("================================================================\n");
    }

    if (start == NULL) {
        printf("No students in the list.\n");
        return;
    }

    struct Student* curr = start;
    while (curr != NULL) {
        printf("%s %s | %02d.%02d.%d | Grades: (%d, %d, %d)\n",
            curr->surname, curr->name,
            curr->data.day, curr->data.month, curr->data.year,
            curr->grades[0], curr->grades[1], curr->grades[2]);
        curr = curr->next;
    }
    printf("================================================================\n");
}

// Free all memory allocated for the list
void freeList(struct Student* start) {
    while (start != NULL) {
        struct Student* tmp = start;
        start = start->next;
        free(tmp);
    }
}

// ============================================================================
// SEARCH FUNCTIONS
// ============================================================================

// Find student by surname
struct Student* findStudentBySurname(struct Student* start, const char* surname) {
    struct Student* curr = start;
    while (curr != NULL) {
        if (strcmp(curr->surname, surname) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

// ============================================================================
// FILE I/O FUNCTIONS
// ============================================================================

// Read students from file
// File format: Surname Name Day Month Year Grade1 Grade2 Grade3 (tab-separated)
int readStudentsFromFile(struct Student** start, const char* filename) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "r");

    if (err != 0 || file == NULL) {
        printf("Error: Could not open file '%s' for reading.\n", filename);
        printf("Make sure the file exists in the current directory.\n");
        return 0;
    }

    char line[256];
    int lineNum = 0;
    int studentsRead = 0;

    while (fgets(line, sizeof(line), file)) {
        lineNum++;

        // Skip empty lines
        if (line[0] == '\n' || line[0] == '\r') {
            continue;
        }

        char surname[50], name[50];
        int day, month, year, g1, g2, g3;

        // Parse line
        int result = sscanf_s(line, "%49s %49s %d %d %d %d %d %d",
            surname, (unsigned)sizeof(surname),
            name, (unsigned)sizeof(name),
            &day, &month, &year, &g1, &g2, &g3);

        if (result != 8) {
            printf("Warning: Line %d has incorrect format. Skipping.\n", lineNum);
            continue;
        }

        // Validate data
        if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100) {
            printf("Warning: Line %d has invalid date. Skipping.\n", lineNum);
            continue;
        }

        if (g1 < 1 || g1 > 5 || g2 < 1 || g2 > 5 || g3 < 1 || g3 > 5) {
            printf("Warning: Line %d has invalid grades (must be 1-5). Skipping.\n", lineNum);
            continue;
        }

        // Add student to list
        if (addStudent(start, surname, name, day, month, year, g1, g2, g3)) {
            studentsRead++;
        }
    }

    fclose(file);
    printf("Successfully read %d students from '%s'\n", studentsRead, filename);
    return studentsRead;
}

// Write students to file
int writeStudentsToFile(struct Student* start, const char* filename) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "w");

    if (err != 0 || file == NULL) {
        printf("Error: Could not open file '%s' for writing.\n", filename);
        return 0;
    }

    int studentsWritten = 0;
    struct Student* curr = start;

    while (curr != NULL) {
        fprintf(file, "%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n",
            curr->surname, curr->name,
            curr->data.day, curr->data.month, curr->data.year,
            curr->grades[0], curr->grades[1], curr->grades[2]);
        studentsWritten++;
        curr = curr->next;
    }

    fclose(file);
    printf("Successfully wrote %d students to '%s'\n", studentsWritten, filename);
    return studentsWritten;
}

// ============================================================================
// MAIN PROGRAM
// ============================================================================

int main() {
    struct Student* start = NULL;
    const char* inputFile = "students.txt";
    const char* outputFile = "filtered_students.txt";

    printf("=== Student Management System ===\n\n");

    // Try to read from file first
    int readCount = readStudentsFromFile(&start, inputFile);

    // If file doesn't exist or is empty, use hardcoded data
    if (readCount == 0) {
        printf("\nUsing hardcoded student data...\n");
        addStudent(&start, "Melnichuk", "Mariia", 8, 1, 2008, 3, 5, 5);
        addStudent(&start, "Fedolak", "Lubomur", 7, 4, 2009, 3, 4, 5);
        addStudent(&start, "Samokhodska", "Anastasiia", 21, 3, 2010, 5, 5, 5);
        addStudent(&start, "Pushchak", "Andrii", 1, 2, 2007, 5, 4, 3);
        addStudent(&start, "Sydor", "Liliia", 12, 9, 2006, 5, 5, 3);
        addStudent(&start, "Ivanov", "Ivan", 1, 2, 2007, 2, 3, 3);
    }

    // Display all students
    displayStudents(start, "All Students (sorted by surname):");

    // Example: Search for a student
    printf("\n--- Searching for 'Sydor' ---\n");
    struct Student* found = findStudentBySurname(start, "Sydor");
    if (found) {
        printf("Found: %s %s | Grades: (%d, %d, %d)\n",
            found->surname, found->name,
            found->grades[0], found->grades[1], found->grades[2]);
    }
    else {
        printf("Student not found.\n");
    }

    // Filter students (remove those without grades 4 or 5)
    printf("\n--- Filtering students ---\n");
    printf("Removing students who don't have any grades 4 or 5...\n");
    deleteStudentsByCondition(&start);

    // Display filtered students
    displayStudents(start, "Students with at least one grade 4 or 5:");

    // Save filtered list to file
    writeStudentsToFile(start, outputFile);

    // Clean up
    freeList(start);

    printf("\nProgram completed successfully.\n");
    return 0;
}