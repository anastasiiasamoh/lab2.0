#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

// Create a student node
struct Student* createStud(const char* surname, const char* name, int d, int m, int y, int g1, int g2, int g3) {
    struct Student* s = (struct Student*)malloc(sizeof(struct Student));
    if (!s) {
        printf("Not enough memory\n");
        return NULL;
    }
    strcpy_s(s->surname, sizeof(s->surname), surname);
    strcpy_s(s->name, sizeof(s->name), name);
    s->data.day = d;
    s->data.month = m;
    s->data.year = y;
    s->grades[0] = g1;
    s->grades[1] = g2;
    s->grades[2] = g3;
    s->next = NULL;
    return s;
}

// Add a new student to the list (sorted by surname)
void addStudent(struct Student** start, const char* surname, const char* name, int d, int m, int y, int g1, int g2, int g3) {
    struct Student* newStud = createStud(surname, name, d, m, y, g1, g2, g3);
    if (!newStud) return;

    if (*start == NULL || strcmp((*start)->surname, newStud->surname) > 0) {
        newStud->next = *start;
        *start = newStud;
        return;
    }

    struct Student* curr = *start;
    while (curr->next && strcmp(curr->next->surname, newStud->surname) < 0) {
        curr = curr->next;
    }
    newStud->next = curr->next;
    curr->next = newStud;
}

// Sort existing student into list
void sortStud(struct Student** start, struct Student* s) {
    if (*start == NULL || strcmp((*start)->surname, s->surname) > 0) {
        s->next = *start;
        *start = s;
        return;
    }
    struct Student* curr = *start;
    while (curr->next && strcmp(curr->next->surname, s->surname) < 0) {
        curr = curr->next;
    }
    s->next = curr->next;
    curr->next = s;
}

// Free the list
void freeList(struct Student* start) {
    while (start != NULL) {
        struct Student* tmp = start;
        start = start->next;
        free(tmp);
    }
}

// Calculate age from birth date
int calculateAge(struct DayOfBirth dob, int currentYear, int currentMonth, int currentDay) {
    int age = currentYear - dob.year;
    if (currentMonth < dob.month || (currentMonth == dob.month && currentDay < dob.day)) {
        age--;
    }
    return age;
}

// Calculate average age of all students in the list
double calculateAverageAge(struct Student* start, int currentYear, int currentMonth, int currentDay) {
    if (start == NULL) return 0.0;

    int count = 0;
    int totalAge = 0;
    struct Student* curr = start;

    while (curr != NULL) {
        totalAge += calculateAge(curr->data, currentYear, currentMonth, currentDay);
        count++;
        curr = curr->next;
    }

    return (double)totalAge / count;
}

// Remove students who do not have all grades as 4 or 5
void removeStudentsWithLowGrades(struct Student** start) {
    struct Student* curr = *start;
    struct Student* prev = NULL;

    while (curr != NULL) {
        int hasLowGrade = 0;
        for (int i = 0; i < 3; i++) {
            if (curr->grades[i] < 4) {
                hasLowGrade = 1;
                break;
            }
        }

        if (hasLowGrade) {
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

// Filter students younger than average age
void filterYoungerThanAverage(struct Student** start, int currentYear, int currentMonth, int currentDay) {
    double avgAge = calculateAverageAge(*start, currentYear, currentMonth, currentDay);
    printf("Average age: %.2f\n", avgAge);

    struct Student* curr = *start;
    struct Student* prev = NULL;

    while (curr != NULL) {
        int age = calculateAge(curr->data, currentYear, currentMonth, currentDay);

        if (age >= avgAge) {
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

// Print the list
void printList(struct Student* start) {
    struct Student* curr = start;
    while (curr != NULL) {
        printf("%s %s | %d.%d.%d | (%d, %d, %d)\n",
            curr->surname, curr->name,
            curr->data.day, curr->data.month, curr->data.year,
            curr->grades[0], curr->grades[1], curr->grades[2]);
        curr = curr->next;
    }
}

int main() {
    // Get current date for age calculation
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    int currentYear = tm_info->tm_year + 1900;
    int currentMonth = tm_info->tm_mon + 1;
    int currentDay = tm_info->tm_mday;

    struct Student* start = NULL;

    // Build initial list
    sortStud(&start, createStud("Melnichuk", "Mariia", 8, 1, 2008, 3, 5, 5));
    sortStud(&start, createStud("Fedolak", "Lubomur", 7, 4, 2009, 3, 4, 5));
    sortStud(&start, createStud("Samokhodska", "Anastasiia", 21, 3, 2010, 5, 5, 5));
    sortStud(&start, createStud("Pushchak", "Andrii", 1, 2, 2007, 5, 4, 3));
    sortStud(&start, createStud("Sydor", "Liliia", 12, 9, 2006, 5, 5, 5));
    sortStud(&start, createStud("Ivanov", "Ivan", 1, 2, 2007, 2, 3, 3));

    printf("Initial list (sorted by surname):\n");
    printList(start);
    printf("\n");

    // Step 1: Remove students with grades below 4
    printf("Step 1: Removing students with any grade below 4...\n");
    removeStudentsWithLowGrades(&start);
    printList(start);
    printf("\n");

    // Step 2: Filter students younger than average age
    printf("Step 2: Filtering students younger than average age...\n");
    filterYoungerThanAverage(&start, currentYear, currentMonth, currentDay);

    printf("\nFinal result (younger than average, good grades, sorted alphabetically):\n");
    printList(start);
    printf("\n");

    // Demonstrate adding a new student
    printf("Demonstration: Adding new student 'Kovalenko Anton'...\n");
    addStudent(&start, "Kovalenko", "Anton", 15, 6, 2009, 5, 4, 5);
    printList(start);

    freeList(start);
    return 0;
}