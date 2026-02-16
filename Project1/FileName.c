#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void freeList(struct Student* start) {
	while (start != NULL) {
		struct Student* tmp = start;
		start = start->next;
		free(tmp);
	}
}

void cutNo4Or5(struct Student** start) {
	struct Student* curr = *start;
	struct Student* prev = NULL;
	while (curr != NULL) {
		int has4or5 = 0;
		for (int i = 0; i < 3; i++) {
			if (curr->grades[i] == 4 || curr->grades[i] == 5) {
				has4or5 = 1;
				break;
			}
		}
		if (!has4or5) {
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

int main() {
	struct Student* start = NULL;
	sortStud(&start, createStud("Melnichuk", "Mariia", 8, 1, 2008, 3, 5, 5));
	sortStud(&start, createStud("Fedolak", "Lubomur", 7, 4, 2009, 3, 4, 5));
	sortStud(&start, createStud("Samokhodska", "Anastasiia", 21, 3, 2010, 5, 5, 5));
	sortStud(&start, createStud("Pushchak", "Andrii", 1, 2, 2007, 5, 4, 3));
	sortStud(&start, createStud("Sydor", "Liliia", 12, 9, 2006, 5, 5, 3));
	sortStud(&start, createStud("Ivanov", "Ivan", 1, 2, 2007, 2, 3, 3));
	struct Student* curr = start;
	while (curr != NULL) {
		printf("%s %s | %d.%d.%d | (%d, %d, %d)\n",
			curr->surname, curr->name,
			curr->data.day, curr->data.month, curr->data.year,
			curr->grades[0], curr->grades[1], curr->grades[2]);
		curr = curr->next;
	}
	cutNo4Or5(&start);
	printf("Students who has 4 or 5: \n");
	curr = start;
	while (curr != NULL) {
		printf("%s %s | %d.%d.%d | (%d, %d, %d)\n",
			curr->surname, curr->name,
			curr->data.day, curr->data.month, curr->data.year,
			curr->grades[0], curr->grades[1], curr->grades[2]);
		curr = curr->next;
	}
	freeList(start);
	return 0;
}
