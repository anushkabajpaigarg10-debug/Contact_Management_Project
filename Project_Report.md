
# PROJECT REPORT

## Dynamic Contact Management with Duplicate Detection

**Subject:** Problem Solving using C (PPS)

**Project Number:** 24

**Project Title:** Dynamic Contact Management with Duplicate Detection

**Programming Language:** C

---

# 1. ABSTRACT

The Dynamic Contact Management System is a menu-driven application developed using the C programming language.

The main purpose of this project is to manage contact information efficiently and identify duplicate contact records.

The system allows users to add, display, edit, delete, search, and merge contacts.

It supports multiple phone numbers and email addresses for each contact.

Dynamic memory allocation techniques such as malloc(), calloc(), realloc(), and free() are used to manage memory efficiently.

File handling is implemented to import and export contact information.

This project demonstrates the practical application of important C programming concepts.

# 2. INTRODUCTION

Contact management is an important requirement in everyday life.

Sometimes contact lists contain duplicate entries, repeated phone numbers, and multiple email addresses belonging to the same person.

Managing these records manually can be difficult.

This project provides a simple solution by developing a contact management application that identifies possible duplicates and allows users to merge contact records.

# 3. PROBLEM STATEMENT

Develop a contact management system that can store, search, update, delete, and merge contact records.

The system should detect duplicate contacts using phone numbers, email addresses, and simple name-matching techniques.

It should also support dynamic memory allocation and file handling.

# 4. OBJECTIVES

1. Develop a menu-driven contact management application.
2. Store contact information using structures.
3. Support multiple phone numbers and emails.
4. Implement searching and sorting-related string operations.
5. Detect duplicate contact records.
6. Merge duplicate contact information.
7. Use pointers and dynamic memory allocation.
8. Implement file import and export functionality.

# 5. SOFTWARE AND HARDWARE REQUIREMENTS

## Software Requirements

- Operating System: Windows/Linux
- Programming Language: C
- Compiler: GCC
- IDE: Visual Studio Code

## Hardware Requirements

- Computer or laptop
- Minimum 4 GB RAM recommended
- Keyboard and monitor

# 6. CONCEPTS USED

## 6.1 Structures

Structures are used to store contact information such as name, address, phone numbers, and email addresses.

## 6.2 Arrays

Arrays are used to maintain multiple contact records and contact details.

## 6.3 Pointers

Pointers are used to access dynamically allocated memory and connect merged records.

## 6.4 Dynamic Memory Allocation

The project uses:

- malloc(): Allocates memory dynamically.
- calloc(): Allocates and initializes memory.
- realloc(): Changes the allocated memory size.
- free(): Releases allocated memory.

## 6.5 String Handling

String functions such as strcmp(), strlen(), strcpy(), strstr(), and strcspn() are used.

## 6.6 Functions

Functions divide the program into smaller modules and make the code easier to understand.

## 6.7 File Handling

File handling functions such as fopen(), fprintf(), fgets(), and fclose() are used to store and retrieve contact information.

# 7. ALGORITHM

Step 1: Start the program.

Step 2: Load previously saved contacts from the file.

Step 3: Display the main menu.

Step 4: Accept the user's choice.

Step 5: Perform the selected operation.

Step 6: Add, display, edit, delete, or search contacts as required.

Step 7: Compare contact information to identify duplicates.

Step 8: Merge duplicate contact records when requested.

Step 9: Import or export contact information using files.

Step 10: Repeat until the user selects Exit.

Step 11: Save contact information and release allocated memory.

Step 12: Stop the program.

# 8. DUPLICATE DETECTION LOGIC

The application identifies possible duplicate records using three basic conditions:

1. Matching phone numbers after removing non-digit characters.
2. Matching email addresses after normalization.
3. Matching names after ignoring case, spaces, and punctuation.

If any of these conditions match, the records are reported as possible duplicates.

These are heuristic rules and may sometimes identify different people with similar names.

# 9. MODULES OF THE PROJECT

| Module | Function |
|---|---|
| Add Contact | addContact() |
| Display Contact | listContacts() |
| Edit Contact | editContact() |
| Delete Contact | deleteContact() |
| Search Contact | searchContacts() |
| Duplicate Detection | detectDuplicates() |
| Merge Contacts | mergeContacts() |
| Export Data | exportContacts() |
| Import Data | loadContacts() |
| Memory Cleanup | cleanup() |

# 10. SAMPLE TEST CASES

| Test Case | Expected Result |
|---|---|
| Add a new contact | Contact added |
| Display contacts | All contacts displayed |
| Search by name | Matching contact displayed |
| Search by phone | Matching contact displayed |
| Add duplicate phone | Duplicate detected |
| Merge two contacts | Contact details combined |
| Delete a contact | Contact removed |
| Export contacts | Data saved to file |
| Import contacts | Data loaded from file |

# 11. ADVANTAGES

- Easy to use.
- Efficient contact management.
- Supports multiple phone numbers and emails.
- Identifies possible duplicate records.
- Uses dynamic memory allocation.
- Supports file-based storage.
- Demonstrates modular programming.

# 12. LIMITATIONS

- Uses a text file for storage.
- Duplicate detection uses simple heuristics.
- Name-based matching can produce false positives.
- The interface is command-line based.
- The text storage format has delimiter restrictions.

# 13. FUTURE SCOPE

The project can be improved by adding:

1. Graphical User Interface.
2. Database connectivity.
3. Advanced fuzzy matching.
4. Contact sorting.
5. Better input validation.
6. Password protection.
7. Cloud-based contact synchronization.

# 14. RESULT

The Dynamic Contact Management System was developed successfully using C programming.

It demonstrates contact management, duplicate detection, merging, searching, dynamic memory allocation, and file handling.

# 15. CONCLUSION

This project provides practical knowledge of C programming concepts.

It demonstrates how structures, arrays, pointers, functions, strings, dynamic memory allocation, and file handling can be combined to develop a useful real-world application.

The project successfully implements the basic requirements of a dynamic contact management system.
