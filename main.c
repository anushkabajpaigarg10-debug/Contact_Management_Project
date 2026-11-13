
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_LEN 100
#define ADDRESS_LEN 200
#define VALUE_LEN 120
#define LINE_LEN 2048
#define DATA_FILE "contacts.txt"

typedef struct Contact {
    int id;
    char name[NAME_LEN];
    char address[ADDRESS_LEN];

    char **phones;
    int phoneCount;
    int phoneCapacity;

    char **emails;
    int emailCount;
    int emailCapacity;

    struct Contact *mergedInto;
    struct Contact *nextMerged;
} Contact;

Contact **contacts = NULL;
int contactCount = 0;
int contactCapacity = 0;
int nextId = 1;

Contact *mergedArchive = NULL;

/* ---------- INPUT AND MEMORY FUNCTIONS ---------- */

void readLine(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);

    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
}

int readInt(const char *prompt) {
    char line[80];
    char *end;
    long value;

    while (1) {
        readLine(prompt, line, sizeof(line));

        value = strtol(line, &end, 10);

        while (isspace((unsigned char)*end))
            end++;

        if (line[0] != '\0' && *end == '\0')
            return (int)value;

        printf("Please enter a valid number.\n");
    }
}

char *copyString(const char *source) {
    char *copy = malloc(strlen(source) + 1);

    if (copy != NULL)
        strcpy(copy, source);

    return copy;
}

void freeStringArray(char **items, int count) {
    for (int i = 0; i < count; i++)
        free(items[i]);

    free(items);
}

void freeContact(Contact *c) {
    if (c == NULL)
        return;

    freeStringArray(c->phones, c->phoneCount);
    freeStringArray(c->emails, c->emailCount);

    free(c);
}

Contact *createContact(void) {
    Contact *c = calloc(1, sizeof(Contact));

    if (c == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    c->id = nextId++;

    return c;
}

/* ---------- DYNAMIC CONTACT ARRAY ---------- */

int reserveContacts(int needed) {

    if (needed <= contactCapacity)
        return 1;

    int newCapacity;

    if (contactCapacity == 0)
        newCapacity = 4;
    else
        newCapacity = contactCapacity * 2;

    while (newCapacity < needed)
        newCapacity *= 2;

    Contact **temp = realloc(
        contacts,
        newCapacity * sizeof(Contact *)
    );

    if (temp == NULL) {
        printf("Unable to grow contact list.\n");
        return 0;
    }

    contacts = temp;
    contactCapacity = newCapacity;

    return 1;
}

/* ---------- PHONE AND EMAIL MANAGEMENT ---------- */

int addString(
    char ***array,
    int *count,
    int *capacity,
    const char *value
) {

    if (value[0] == '\0')
        return 1;

    if (*count == *capacity) {

        int newCapacity;

        if (*capacity == 0)
            newCapacity = 2;
        else
            newCapacity = *capacity * 2;

        char **temp = realloc(
            *array,
            newCapacity * sizeof(char *)
        );

        if (temp == NULL)
            return 0;

        *array = temp;
        *capacity = newCapacity;
    }

    char *copy = copyString(value);

    if (copy == NULL)
        return 0;

    (*array)[(*count)++] = copy;

    return 1;
}

int addPhone(Contact *c, const char *value) {
    return addString(
        &c->phones,
        &c->phoneCount,
        &c->phoneCapacity,
        value
    );
}

int addEmail(Contact *c, const char *value) {
    return addString(
        &c->emails,
        &c->emailCount,
        &c->emailCapacity,
        value
    );
}

/* ---------- STRING NORMALIZATION ---------- */

void normalize(
    const char *source,
    char *target,
    int size,
    int digitsOnly
) {

    int j = 0;

    for (int i = 0;
         source[i] && j < size - 1;
         i++) {

        unsigned char ch = (unsigned char)source[i];

        if (digitsOnly) {

            if (isdigit(ch))
                target[j++] = (char)ch;

        } else if (isalnum(ch)) {

            target[j++] = (char)tolower(ch);
        }
    }

    target[j] = '\0';
}

/* ---------- DUPLICATE DETECTION ---------- */

int samePhone(const char *a, const char *b) {

    char x[VALUE_LEN];
    char y[VALUE_LEN];

    normalize(a, x, sizeof(x), 1);
    normalize(b, y, sizeof(y), 1);

    return x[0] && strcmp(x, y) == 0;
}

int sameEmail(const char *a, const char *b) {

    char x[VALUE_LEN];
    char y[VALUE_LEN];

    normalize(a, x, sizeof(x), 0);
    normalize(b, y, sizeof(y), 0);

    return x[0] && strcmp(x, y) == 0;
}

int sameNameHeuristic(const char *a, const char *b) {

    char x[NAME_LEN];
    char y[NAME_LEN];

    normalize(a, x, sizeof(x), 0);
    normalize(b, y, sizeof(y), 0);

    return x[0] && strcmp(x, y) == 0;
}

int contactsAreDuplicates(Contact *a, Contact *b) {

    for (int i = 0; i < a->phoneCount; i++) {

        for (int j = 0; j < b->phoneCount; j++) {

            if (samePhone(a->phones[i], b->phones[j]))
                return 1;
        }
    }

    for (int i = 0; i < a->emailCount; i++) {

        for (int j = 0; j < b->emailCount; j++) {

            if (sameEmail(a->emails[i], b->emails[j]))
                return 1;
        }
    }

    if (sameNameHeuristic(a->name, b->name))
        return 1;

    return 0;
}

/* ---------- SEARCH HELPERS ---------- */

int containsIgnoreCase(
    const char *text,
    const char *pattern
) {

    if (!*pattern)
        return 1;

    size_t n = strlen(pattern);

    for (; *text; text++) {

        size_t i = 0;

        while (
            i < n &&
            text[i] &&
            tolower((unsigned char)text[i]) ==
            tolower((unsigned char)pattern[i])
        ) {
            i++;
        }

        if (i == n)
            return 1;
    }

    return 0;
}

int startsWithIgnoreCase(
    const char *text,
    const char *prefix
) {

    while (*prefix) {

        if (
            !*text ||
            tolower((unsigned char)*text) !=
            tolower((unsigned char)*prefix)
        )
            return 0;

        text++;
        prefix++;
    }

    return 1;
}

/* ---------- DISPLAY CONTACTS ---------- */

void printContact(Contact *c, int position) {

    printf("\n[%d] ID: %d\n", position + 1, c->id);

    printf("Name: %s\n", c->name);

    printf(
        "Address: %s\n",
        c->address[0] ? c->address : "Not provided"
    );

    printf("Phone(s): ");

    if (!c->phoneCount)
        printf("None");

    for (int i = 0; i < c->phoneCount; i++) {

        printf(
            "%s%s",
            i ? ", " : "",
            c->phones[i]
        );
    }

    printf("\nEmail(s): ");

    if (!c->emailCount)
        printf("None");

    for (int i = 0; i < c->emailCount; i++) {

        printf(
            "%s%s",
            i ? ", " : "",
            c->emails[i]
        );
    }

    printf("\n");
}

void listContacts(void) {

    if (!contactCount) {
        printf("\nNo contacts available.\n");
        return;
    }

    printf(
        "\n========== CONTACT LIST (%d) ==========\n",
        contactCount
    );

    for (int i = 0; i < contactCount; i++)
        printContact(contacts[i], i);
}

/* ---------- ADD CONTACT ---------- */

void addContact(void) {

    Contact *c = createContact();

    char value[ADDRESS_LEN];

    if (c == NULL)
        return;

    readLine(
        "Enter name: ",
        c->name,
        sizeof(c->name)
    );

    if (!c->name[0]) {

        printf("Name cannot be empty. Contact cancelled.\n");

        freeContact(c);
        return;
    }

    readLine(
        "Enter address: ",
        c->address,
        sizeof(c->address)
    );

    int n = readInt("How many phone numbers? ");

    if (n < 0)
        n = 0;

    for (int i = 0; i < n; i++) {

        readLine(
            "Enter phone number: ",
            value,
            sizeof(value)
        );

        if (!addPhone(c, value))
            printf("Could not save this phone number.\n");
    }

    n = readInt("How many email addresses? ");

    if (n < 0)
        n = 0;

    for (int i = 0; i < n; i++) {

        readLine(
            "Enter email: ",
            value,
            sizeof(value)
        );

        if (!addEmail(c, value))
            printf("Could not save this email.\n");
    }

    if (!reserveContacts(contactCount + 1)) {

        freeContact(c);
        return;
    }

    contacts[contactCount++] = c;

    printf(
        "Contact added successfully. ID = %d\n",
        c->id
    );
}

/* ---------- FIND CONTACT BY ID ---------- */

int findById(int id) {

    for (int i = 0; i < contactCount; i++) {

        if (contacts[i]->id == id)
            return i;
    }

    return -1;
}

/* ---------- EDIT CONTACT ---------- */

void editContact(void) {

    int id = readInt("Enter contact ID to edit: ");

    int index = findById(id);

    char value[ADDRESS_LEN];

    if (index < 0) {

        printf("Contact not found.\n");
        return;
    }

    Contact *c = contacts[index];

    printf("Current name: %s\n", c->name);

    readLine(
        "New name (press Enter to keep current): ",
        value,
        sizeof(value)
    );

    if (value[0])
        snprintf(c->name, sizeof(c->name), "%s", value);

    printf("Current address: %s\n", c->address);

    readLine(
        "New address (press Enter to keep current): ",
        value,
        sizeof(value)
    );

    if (value[0])
        snprintf(c->address, sizeof(c->address), "%s", value);

    printf("\n1. Add phone");
    printf("\n2. Add email");
    printf("\n3. Finish\n");

    int choice = readInt("Choose: ");

    if (choice == 1) {

        readLine(
            "Enter new phone: ",
            value,
            sizeof(value)
        );

        if (!addPhone(c, value))
            printf("Memory allocation failed.\n");

    } else if (choice == 2) {

        readLine(
            "Enter new email: ",
            value,
            sizeof(value)
        );

        if (!addEmail(c, value))
            printf("Memory allocation failed.\n");
    }

    printf("Contact updated.\n");
}

/* ---------- DELETE CONTACT ---------- */

void deleteContact(void) {

    int id = readInt("Enter contact ID to delete: ");

    int index = findById(id);

    if (index < 0) {

        printf("Contact not found.\n");
        return;
    }

    freeContact(contacts[index]);

    for (int i = index; i < contactCount - 1; i++)
        contacts[i] = contacts[i + 1];

    contactCount--;

    printf("Contact deleted.\n");
}

/* ---------- SEARCH CONTACT ---------- */

void searchContacts(void) {

    char query[VALUE_LEN];

    int found = 0;

    readLine(
        "Enter name/phone/email search text: ",
        query,
        sizeof(query)
    );

    for (int i = 0; i < contactCount; i++) {

        Contact *c = contacts[i];

        int match = containsIgnoreCase(c->name, query);

        for (int j = 0; j < c->phoneCount; j++) {

            if (strstr(c->phones[j], query))
                match = 1;
        }

        for (int j = 0; j < c->emailCount; j++) {

            if (containsIgnoreCase(c->emails[j], query))
                match = 1;
        }

        if (match) {

            printContact(c, i);
            found = 1;
        }
    }

    if (!found)
        printf("No matching contact found.\n");

    printf(
        "(Search supports substring matches; "
        "name prefixes also match.)\n"
    );
}

/* ---------- DETECT DUPLICATES ---------- */

void detectDuplicates(void) {

    int found = 0;

    printf("\n========== POSSIBLE DUPLICATES ==========\n");

    for (int i = 0; i < contactCount; i++) {

        for (int j = i + 1; j < contactCount; j++) {

            if (contactsAreDuplicates(
                contacts[i],
                contacts[j]
            )) {

                printf(
                    "Contact ID %d (%s) may duplicate "
                    "ID %d (%s)\n",
                    contacts[i]->id,
                    contacts[i]->name,
                    contacts[j]->id,
                    contacts[j]->name
                );

                found = 1;
            }
        }
    }

    if (!found)
        printf("No possible duplicates found.\n");
}

/* ---------- CHECK EXISTING VALUES ---------- */

int stringExists(
    char **items,
    int count,
    const char *value,
    int emailMode
) {

    for (int i = 0; i < count; i++) {

        if (
            emailMode
            ? sameEmail(items[i], value)
            : samePhone(items[i], value)
        )
            return 1;
    }

    return 0;
}

/* ---------- MERGE CONTACTS ---------- */

void mergeContacts(void) {

    int keepId = readInt(
        "Enter ID of contact to KEEP: "
    );

    int removeId = readInt(
        "Enter ID of duplicate contact to MERGE: "
    );

    int a = findById(keepId);
    int b = findById(removeId);

    if (a < 0 || b < 0 || a == b) {

        printf("Invalid contact IDs.\n");
        return;
    }

    Contact *keep = contacts[a];
    Contact *remove = contacts[b];

    /* Merge unique phone numbers */

    for (int i = 0; i < remove->phoneCount; i++) {

        if (!stringExists(
            keep->phones,
            keep->phoneCount,
            remove->phones[i],
            0
        )) {

            if (!addPhone(keep, remove->phones[i]))
                printf("Warning: phone not merged.\n");
        }
    }

    /* Merge unique email addresses */

    for (int i = 0; i < remove->emailCount; i++) {

        if (!stringExists(
            keep->emails,
            keep->emailCount,
            remove->emails[i],
            1
        )) {

            if (!addEmail(keep, remove->emails[i]))
                printf("Warning: email not merged.\n");
        }
    }

    if (!keep->address[0] && remove->address[0]) {

        snprintf(
            keep->address,
            sizeof(keep->address),
            "%s",
            remove->address
        );
    }

    if (!keep->name[0] && remove->name[0]) {

        snprintf(
            keep->name,
            sizeof(keep->name),
            "%s",
            remove->name
        );
    }

    /* Maintain pointer-linked merge archive */

    remove->mergedInto = keep;
    remove->nextMerged = mergedArchive;

    mergedArchive = remove;

    /* Remove merged contact from active array */

    for (int i = b; i < contactCount - 1; i++)
        contacts[i] = contacts[i + 1];

    contactCount--;

    printf(
        "Contacts merged. ID %d is now the main contact.\n",
        keep->id
    );
}

/* ---------- EXPORT CONTACTS ---------- */

int exportContacts(void) {

    FILE *fp = fopen(DATA_FILE, "w");

    if (fp == NULL) {

        printf("Could not open %s for writing.\n", DATA_FILE);
        return 0;
    }

    for (int i = 0; i < contactCount; i++) {

        Contact *c = contacts[i];

        fprintf(
            fp,
            "%s|%s|",
            c->name,
            c->address
        );

        for (int j = 0; j < c->phoneCount; j++) {

            fprintf(
                fp,
                "%s%s",
                j ? "," : "",
                c->phones[j]
            );
        }

        fprintf(fp, "|");

        for (int j = 0; j < c->emailCount; j++) {

            fprintf(
                fp,
                "%s%s",
                j ? "," : "",
                c->emails[j]
            );
        }

        fprintf(fp, "\n");
    }

    fclose(fp);

    printf(
        "Exported %d contact(s) to %s.\n",
        contactCount,
        DATA_FILE
    );

    return 1;
}

/* ---------- IMPORT CONTACTS ---------- */

void loadContacts(void) {

    FILE *fp = fopen(DATA_FILE, "r");

    char line[LINE_LEN];

    if (fp == NULL) {

        printf(
            "No existing %s file. "
            "Starting with an empty list.\n",
            DATA_FILE
        );

        return;
    }

    while (fgets(line, sizeof(line), fp)) {

        line[strcspn(line, "\r\n")] = '\0';

        char *name = strtok(line, "|");
        char *address = strtok(NULL, "|");
        char *phones = strtok(NULL, "|");
        char *emails = strtok(NULL, "|");

        if (!name || !address)
            continue;

        Contact *c = createContact();

        if (c == NULL)
            break;

        snprintf(
            c->name,
            sizeof(c->name),
            "%s",
            name
        );

        snprintf(
            c->address,
            sizeof(c->address),
            "%s",
            address
        );

        if (phones) {

            char *token = strtok(phones, ",");

            while (token) {

                addPhone(c, token);
                token = strtok(NULL, ",");
            }
        }

        if (emails) {

            char *token = strtok(emails, ",");

            while (token) {

                addEmail(c, token);
                token = strtok(NULL, ",");
            }
        }

        if (!reserveContacts(contactCount + 1)) {

            freeContact(c);
            break;
        }

        contacts[contactCount++] = c;
    }

    fclose(fp);

    printf(
        "Loaded %d contact(s) from %s.\n",
        contactCount,
        DATA_FILE
    );
}

/* ---------- MEMORY CLEANUP ---------- */

void cleanup(void) {

    for (int i = 0; i < contactCount; i++)
        freeContact(contacts[i]);

    free(contacts);

    while (mergedArchive != NULL) {

        Contact *next = mergedArchive->nextMerged;

        freeContact(mergedArchive);

        mergedArchive = next;
    }
}

/* ---------- MAIN MENU ---------- */

void showMenu(void) {

    printf(
        "\n========== DYNAMIC CONTACT MANAGEMENT ==========\n"
    );

    printf("1. Add contact\n");
    printf("2. Display all contacts\n");
    printf("3. Edit contact\n");
    printf("4. Delete contact\n");
    printf("5. Search contact\n");
    printf("6. Detect duplicates\n");
    printf("7. Merge duplicate contacts\n");
    printf("8. Export contacts to file\n");
    printf("9. Import contacts from file\n");
    printf("0. Save and exit\n");
}

/* ---------- MAIN FUNCTION ---------- */

int main(void) {

    int choice;

    printf(
        "Dynamic Contact Management with Duplicate Detection\n"
    );

    loadContacts();

    do {

        showMenu();

        choice = readInt("Enter your choice: ");

        switch (choice) {

            case 1:
                addContact();
                break;

            case 2:
                listContacts();
                break;

            case 3:
                editContact();
                break;

            case 4:
                deleteContact();
                break;

            case 5:
                searchContacts();
                break;

            case 6:
                detectDuplicates();
                break;

            case 7:
                mergeContacts();
                break;

            case 8:
                exportContacts();
                break;

            case 9:
                loadContacts();
                break;

            case 0:

                exportContacts();

                printf(
                    "Thank you for using "
                    "the Contact Management System.\n"
                );

                break;

            default:

                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 0);

    cleanup();

    return 0;
}
