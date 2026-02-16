# Student Management System - Refactoring Documentation

## Overview
This document describes the refactoring of the original student management program into a modular, maintainable system with file I/O capabilities.

---

## Changes Summary

### 1. **Modularization**
The monolithic code has been divided into logical functional groups:

#### **File Operations Module**
- `readStudentsFromFile()` - Loads student data from a text file
- `writeStudentsToFile()` - Saves student data to a text file

#### **List Management Module**
- `addStudent()` - Combines creation and sorted insertion (replaces `createStud()` + `sortStud()`)
- `deleteStudentsByCondition()` - Removes students based on criteria (renamed from `cutNo4Or5()`)
- `displayStudents()` - Centralized output formatting
- `freeList()` - Memory cleanup (kept from original)

#### **Search & Utility Module**
- `findStudentBySurname()` - Searches for specific students
- `hasGrade4or5()` - Extracted grade checking logic

---

## Detailed Function Descriptions

### File I/O Functions

#### `readStudentsFromFile(struct Student** start, const char* filename)`
**Purpose:** Reads student records from a file and builds the linked list.

**Error Handling:**
- File not found → displays error message, returns 0
- Invalid line format → skips line with warning
- Invalid date/grades → skips line with warning
- Empty lines → automatically skipped

**Returns:** Number of students successfully read

**File Format:** Tab-separated values
```
Surname Name Day Month Year Grade1 Grade2 Grade3
```

#### `writeStudentsToFile(struct Student* start, const char* filename)`
**Purpose:** Exports the current student list to a file.

**Error Handling:**
- Cannot open file for writing → displays error message, returns 0

**Returns:** Number of students written

---

### List Management Functions

#### `addStudent(...)`
**Purpose:** Creates a new student and inserts it in alphabetically sorted order by surname.

**Key Improvements:**
- Combines `createStud()` and `sortStud()` logic
- Single function call instead of two
- Better encapsulation

#### `deleteStudentsByCondition(struct Student** start)`
**Purpose:** Removes all students who don't have at least one grade of 4 or 5.

**Key Improvements:**
- More descriptive name (was `cutNo4Or5`)
- Uses helper function `hasGrade4or5()` for better readability

#### `displayStudents(struct Student* start, const char* header)`
**Purpose:** Prints all students with optional header.

**Key Improvements:**
- Eliminates code duplication (was inline in `main()` twice)
- Consistent formatting
- Optional section headers

---

### Search Functions

#### `findStudentBySurname(struct Student* start, const char* surname)`
**Purpose:** Searches the list for a specific student by surname.

**Returns:** Pointer to student if found, NULL otherwise

**New Functionality:** Enables lookup operations (not in original code)

---

### Utility Functions

#### `hasGrade4or5(struct Student* s)`
**Purpose:** Checks if a student has at least one grade of 4 or 5.

**Key Improvements:**
- Extracted from inline logic
- Reusable across different functions
- Improves readability

---

## File Format Specification

### Input File Structure
**Filename:** `students.txt` (configurable)
**Format:** Plain text, tab-separated values
**Encoding:** UTF-8 or ASCII

**Field Order:**
1. Surname (string, max 49 chars)
2. Name (string, max 49 chars)
3. Day (integer, 1-31)
4. Month (integer, 1-12)
5. Year (integer, 1900-2100)
6. Grade1 (integer, 1-5)
7. Grade2 (integer, 1-5)
8. Grade3 (integer, 1-5)

**Example:**
```
Melnichuk	Mariia	8	1	2008	3	5	5
Fedolak	Lubomur	7	4	2009	3	4	5
```

### Output File
**Filename:** `filtered_students.txt` (configurable)
**Format:** Same as input file
**Content:** Only students with at least one grade 4 or 5

---

## Error Handling

### File Operations
| Error Condition | Handling |
|----------------|----------|
| File not found | Display error, use hardcoded data |
| Cannot create output file | Display error, return 0 |
| Invalid line format | Skip line, display warning with line number |
| Invalid date values | Skip line, display warning |
| Invalid grade values (not 1-5) | Skip line, display warning |
| Empty lines | Automatically skip |

### Memory Operations
| Error Condition | Handling |
|----------------|----------|
| malloc() fails | Display error, return NULL |
| Memory properly freed on exit | `freeList()` called before program termination |

---

## Key Improvements Over Original Code

### 1. **Separation of Concerns**
- File I/O separated from business logic
- Display logic centralized
- Each function has a single responsibility

### 2. **Better Function Names**
- `addStudent()` instead of `createStud()` + `sortStud()`
- `deleteStudentsByCondition()` instead of `cutNo4Or5()`
- `hasGrade4or5()` makes the condition explicit

### 3. **Error Handling**
- File operations include error checking
- Data validation for dates and grades
- Graceful degradation (uses hardcoded data if file missing)

### 4. **Code Reusability**
- `displayStudents()` eliminates duplication
- Helper functions can be used in multiple contexts
- File format standardized for easy modification

### 5. **Extensibility**
- Easy to add new search functions
- Easy to add new filtering criteria
- File format can be extended with additional fields

### 6. **User Feedback**
- Progress messages for file operations
- Clear error messages with context
- Success confirmations

---

## Usage Example

### Compilation (Visual Studio)
```bash
cl student_manager.c /Fe:student_manager.exe
```

### Compilation (GCC/Clang)
Replace `strcpy_s`, `fopen_s`, `sscanf_s` with standard versions, then:
```bash
gcc student_manager.c -o student_manager
```

### Running the Program
```bash
./student_manager
```

### Expected Output
```
=== Student Management System ===

Successfully read 8 students from 'students.txt'

All Students (sorted by surname):
================================================================
Bondarenko Petro | 20.11.2009 | Grades: (2, 2, 3)
Fedolak Lubomur | 07.04.2009 | Grades: (3, 4, 5)
Ivanov Ivan | 01.02.2007 | Grades: (2, 3, 3)
Kovalenko Olena | 15.06.2008 | Grades: (5, 4, 5)
Melnichuk Mariia | 08.01.2008 | Grades: (3, 5, 5)
Pushchak Andrii | 01.02.2007 | Grades: (5, 4, 3)
Samokhodska Anastasiia | 21.03.2010 | Grades: (5, 5, 5)
Sydor Liliia | 12.09.2006 | Grades: (5, 5, 3)
================================================================

--- Searching for 'Sydor' ---
Found: Sydor Liliia | Grades: (5, 5, 3)

--- Filtering students ---
Removing students who don't have any grades 4 or 5...

Students with at least one grade 4 or 5:
================================================================
Fedolak Lubomur | 07.04.2009 | Grades: (3, 4, 5)
Kovalenko Olena | 15.06.2008 | Grades: (5, 4, 5)
Melnichuk Mariia | 08.01.2008 | Grades: (3, 5, 5)
Pushchak Andrii | 01.02.2007 | Grades: (5, 4, 3)
Samokhodska Anastasiia | 21.03.2010 | Grades: (5, 5, 5)
Sydor Liliia | 12.09.2006 | Grades: (5, 5, 3)
================================================================

Successfully wrote 6 students to 'filtered_students.txt'

Program completed successfully.
```

---

## Future Enhancement Possibilities

1. **Additional Search Functions**
   - Search by grade criteria
   - Search by birth year range
   - Search by name

2. **Sorting Options**
   - Sort by grades (average)
   - Sort by birth date
   - Sort by name instead of surname

3. **File Format Options**
   - CSV support
   - JSON format
   - Binary format for faster I/O

4. **Data Validation**
   - Duplicate detection
   - More sophisticated date validation
   - Name format checking

5. **Interactive Menu**
   - Add student interactively
   - Delete by surname
   - Update student information
   - Multiple filter options
