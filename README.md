
# Dynamic Contact Management with Duplicate Detection

## 1. Project Overview

The Dynamic Contact Management System is a menu-driven application developed using the C programming language.

It helps users manage contact information, identify duplicate records, merge contacts, and store data using file handling.

## 2. Project Objectives

- Store and manage contact information.
- Support multiple phone numbers per contact.
- Support multiple email addresses per contact.
- Search contacts by name, phone number, and email.
- Detect duplicate contacts.
- Merge duplicate records.
- Import and export contact information.
- Demonstrate dynamic memory allocation.

## 3. Technologies Used

- Programming Language: C
- Compiler: GCC
- IDE: Visual Studio Code
- Data Storage: Text File

## 4. Concepts Used

1. Structures
2. Arrays
3. Strings
4. Pointers
5. Functions
6. Loops
7. Conditional Statements
8. Dynamic Memory Allocation
9. File Handling

## 5. Project Features

1. Add Contact
2. Display Contacts
3. Edit Contact
4. Delete Contact
5. Search Contact
6. Detect Duplicates
7. Merge Contacts
8. Export Contacts
9. Import Contacts

## 6. Project Structure

DynamicContactManagement/

- main.c
- contacts.txt
- README.md
- Project_Report.md

## 7. Compilation

Compile using GCC:

gcc -std=c11 -Wall -Wextra main.c -o contact_manager

## 8. Execution

Windows:

.\contact_manager.exe

Linux:

./contact_manager

## 9. Duplicate Detection

The system identifies possible duplicates by comparing:

- Phone numbers
- Email addresses
- Contact names

It uses basic normalization techniques to ignore formatting differences.

## 10. Conclusion

This project demonstrates the practical application of C programming concepts in developing a contact management application.

It combines structures, pointers, dynamic memory allocation, string manipulation, functions, and file handling.
