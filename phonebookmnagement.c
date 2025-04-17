#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LIGHT_GREEN "\033[1;92m"
#define RESET "\033[0m"
typedef struct Contact //typedef use korle pore bar bar contact lekha lagbena, simply contact er kisu ekta character type korlei hobe
{
    char name[50];
    char phone[5][15]; //ekjon manush er 5 ta number rakha jabe highest, eketa number er length hobe 15 ta digits highest
    int phoneCount;
    struct Contact* next;
} Contact;
Contact* head = NULL;

Contact* createContact(char name[ ], char phone[ ])  //ei funtion ta use kora hobe new kore kono contact add korar jonno, valo kotha, number gulo integer na ,character hishebe saved hobe
{
    Contact* newContact = malloc(sizeof(Contact));
    strcpy(newContact->name, name); //strcpy string copy korte use kora hoy, normally input niye string show jhamela hoi onek time, eijonno strcpy better option
    strcpy(newContact->phone[0], phone);
    newContact->phoneCount = 1;
    newContact->next = NULL;
    return newContact;
}

void insertContact(char name[], char phone[]) //jokhon kono ekta user er under e multiple numbers add kora dorkar pore, tokhon shei number ta oi user er under add korte ei function ta help kore
{
    Contact* temp = head;
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
            /* ei strcmp er kaj ta hochche duita string soman ki na sheta check kora, dhoro duita variable, a = 'ami', b= 'tumi' . tomra kheyal korso kina jani na duita string ke kokhono == use kore same ki na check kora
            jay na.
               same ki na eta check korte strcmp use kora lage. Ekhane shetai kora hoyeche. check kore dekhtese je temp name duita same ki na, jodi same na na hoy taile porer line e phone count jodi 5 er kom thake add
               korte dibe new number, karon amader system e ek name e ekjon manush 5 ta number hold korte oarbe!
               */
        {
            if (temp->phoneCount < 5)
            {
                strcpy(temp->phone[temp->phoneCount], phone);
                temp->phoneCount++;
                printf(LIGHT_GREEN "Number added under existing contact!\n" RESET);
                return;
            }
            else
            {
                printf(LIGHT_GREEN "Cannot add more than 5 numbers for a contact.\n" RESET);
                return;
            }
        }
        temp = temp->next;
    }
    Contact* newContact = createContact(name, phone);
    newContact->next = head;
    head = newContact;
    printf(LIGHT_GREEN "Contact added successfully!\n" RESET);
}


// Function to save contacts to a file when exiting
void saveContacts()
{
    FILE* file = fopen("contacts.txt", "w");
    if (!file) return;
    Contact* temp = head;
    while (temp)
    {
        fprintf(file, "%s %d\n", temp->name, temp->phoneCount);
        for (int i = 0; i < temp->phoneCount; i++)
        {
            fprintf(file, "%s\n", temp->phone[i]);
        }
        temp = temp->next;
    }
    fclose(file);
}

// Function to load contacts from a file at startup
void loadContacts()
{
    FILE* file = fopen("contacts.txt", "r");
    if (!file) return;
    char name[50], phone[15];
    int phoneCount;
    while (fscanf(file, "%49s %d", name, &phoneCount) == 2)
    {
        Contact* newContact = (Contact*)malloc(sizeof(Contact));
        strcpy(newContact->name, name);
        newContact->phoneCount = phoneCount;
        newContact->next = head;
        head = newContact;
        for (int i = 0; i < phoneCount; i++)
        {
            fscanf(file, "%14s", phone);
            strcpy(newContact->phone[i], phone);
        }
    }
    fclose(file);
}

void updateContact(char name[ ])
/*
dhoro amra ekta number save korsi
save korte giye number er kono ekta digit vul disi or
name e kono ekta letter vul korsi

amader ei function ta ei duita problem solution dite help korbe
 */
{
    Contact* temp = head;
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
        {
            char choice;
            printf(LIGHT_GREEN "Do you want to update the Name or Phone Number? (n/p): " RESET);
            scanf(" %c", &choice);
            getchar(); // To consume the newline character left by scanf

            if (choice == 'n' || choice == 'N')
            {
                // Update Name
                printf(LIGHT_GREEN "Enter the new name: " RESET);
                fgets(temp->name, sizeof(temp->name), stdin);
                temp->name[strcspn(temp->name, "\n")] = 0; ///strcspn asole dekhe je ekta variable e string number koto
                printf(LIGHT_GREEN "Name updated successfully!\n" RESET);
                return;
            }
            else if (choice == 'p' || choice == 'P')
            {
                // Update Phone Number
                printf(LIGHT_GREEN "Select the phone number to update:\n" RESET);
                for (int i = 0; i < temp->phoneCount; i++)
                {
                    printf(LIGHT_GREEN "%d. %s\n" RESET, i + 1, temp->phone[i]);
                }
                int phoneChoice;
                printf(LIGHT_GREEN "Enter the number index: " RESET);
                scanf("%d", &phoneChoice);
                getchar();
                if (phoneChoice < 1 || phoneChoice > temp->phoneCount)
                {
                    printf(LIGHT_GREEN "Invalid selection!\n" RESET);
                    return;
                }
                printf(LIGHT_GREEN "Enter new phone number: " RESET);
                char newPhone[15];
                scanf("%s", newPhone);
                if (!validatePhone(newPhone))
                {
                    printf(LIGHT_GREEN "Invalid phone number! Must be between 10 and 15 digits.\n" RESET);
                    return;
                }
                strcpy(temp->phone[phoneChoice - 1], newPhone);
                printf(LIGHT_GREEN "Phone number updated successfully!\n" RESET);
                return;
            }
            else
            {
                printf(LIGHT_GREEN "Invalid choice! No updates made.\n" RESET);
                return;
            }
        }
        temp = temp->next;
    }
    printf(LIGHT_GREEN "Contact not found!\n" RESET);
}


void displayContacts()
/*
ekta number save korlam ekjoner name diye, ekhon shei contact ta dekhar jonno ei funtion. mane eita use korle tumi joto gula contacts save korso
shobgula contacts show korbe screen e ei r ki
 */
{
    if (head == NULL)
    {
        printf(LIGHT_GREEN "No contacts found!\n" RESET);
        return;
    }
    Contact* temp = head;
    printf(LIGHT_GREEN "\n--- Contact List ---\n" RESET);
    while (temp != NULL)
    {
        printf(LIGHT_GREEN "Name: %s\n" RESET, temp->name);
        for (int i = 0; i < temp->phoneCount; i++)
        {
            printf(LIGHT_GREEN "Phone %d: %s\n" RESET, i + 1, temp->phone[i]);
        }
        temp = temp->next;
    }
}


void searchContact(char name[])
/* ekta contact khujba specific bhabe, shetar jonno ei function ta. dhoro ami amar name e contact save korsi, Mushfiq diye, tokhon ami system e giye jodi exact
'Mushfiq' likhe search dei, amar contact ta show korbe sathe amar koyta number shetao show korbe.But ekhane ektu case sensitivity er jhamela ache, jeta ami fix korte pari nai. Sheta hochche 'Mushfiq'
name e save korle amake 'Mushfiq' likhe search deya lagbe. 'mushfiq' or 'MUSHFIQ' likhle khuje paoa jabena number ta
*/
{
    Contact* temp = head;
    int found = 0;
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)    // case-sensitive
        {
            printf(LIGHT_GREEN "Contact found: %s\n" RESET, temp->name);
            for (int i = 0; i < temp->phoneCount; i++)
            {
                printf(LIGHT_GREEN "Phone %d: %s\n" RESET, i + 1, temp->phone[i]);
            }
            found = 1;
        }
        temp = temp->next;
    }
    if (!found) printf(LIGHT_GREEN "Contact not found!\n" RESET);
}


void deleteContact(char name[])
/*
ei function ta diye amra jekono ekta contact delete korte parbo
but ami ekhane ekta kaj korsi
dekho ekjoner to onekgula number thaktei pare. dhoro shei onek number er kono ekta delete kora lagbe
amader ei function ta ke sheivabei sajaichi je jekono ekta specific number or
full ekta contact delete kore dite parbe
 */
{
    Contact* temp = head, *prev = NULL;
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
        {
            printf(LIGHT_GREEN "Contact found: %s\n" RESET, temp->name);
            printf(LIGHT_GREEN "Would you like to delete all phone numbers or a specific one? (a/s): " RESET);
            char choice;
            scanf(" %c", &choice);

            if (choice == 'a' || choice == 'A')
            {
                // Delete all phone numbers
                if (prev == NULL)
                {
                    head = temp->next;
                }
                else
                {
                    prev->next = temp->next;
                }
                free(temp);
                printf(LIGHT_GREEN "All phone numbers for %s deleted successfully!\n" RESET, name);
                return;
            }
            else if (choice == 's' || choice == 'S')
            {
                // Delete specific phone number
                printf(LIGHT_GREEN "Select the phone number to delete:\n" RESET);
                for (int i = 0; i < temp->phoneCount; i++)
                {
                    printf(LIGHT_GREEN "%d. %s\n" RESET, i + 1, temp->phone[i]);
                }
                int phoneChoice;
                printf(LIGHT_GREEN "Enter the number index: " RESET);
                scanf("%d", &phoneChoice);
                if (phoneChoice < 1 || phoneChoice > temp->phoneCount)
                {
                    printf(LIGHT_GREEN "Invalid selection!\n" RESET);
                    return;
                }

                // Shift the remaining numbers
                for (int i = phoneChoice - 1; i < temp->phoneCount - 1; i++)
                {
                    strcpy(temp->phone[i], temp->phone[i + 1]);
                }
                temp->phoneCount--;
                printf(LIGHT_GREEN "Phone number deleted successfully!\n" RESET);
                return;
            }
            else
            {
                printf(LIGHT_GREEN "Invalid choice. Deletion cancelled.\n" RESET);
                return;
            }
        }
        prev = temp;
        temp = temp->next;
    }
    printf(LIGHT_GREEN "Contact not found!\n" RESET);
}


int validatePhone(char phone[])
{
    //phone number is between 10 to 15 digits
    int len = strlen(phone);
    return len >= 10 && len <= 15;
}


int main()
{
    loadContacts(); // Load contacts from the file when the program starts
    int choice;
    while (1)
    {
        printf(LIGHT_GREEN "\n--- Phonebook Menu ---\n Hello Everyone! \n" RESET);
        printf(LIGHT_GREEN "1. Add contact\n" RESET);
        printf(LIGHT_GREEN "2. Update contact\n" RESET);
        printf(LIGHT_GREEN "3. Delete contact\n" RESET);
        printf(LIGHT_GREEN "4. Display all contacts\n" RESET);
        printf(LIGHT_GREEN "5. Search contact\n" RESET);
        printf(LIGHT_GREEN "6. Save & Exit\n" RESET);
        printf(LIGHT_GREEN "Enter your choice: " RESET);
        scanf("%d", &choice);

        char name[50], phone[15];

        switch (choice)
        {
        case 1:
            printf(LIGHT_GREEN "Enter the name: " RESET);
            getchar(); // Consume the newline character
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;

            printf(LIGHT_GREEN "Enter the phone number: " RESET);
            fgets(phone, sizeof(phone), stdin);
            phone[strcspn(phone, "\n")] = 0;

            if (!validatePhone(phone))
            {
                printf(LIGHT_GREEN "Invalid phone number! Must be between 10 and 15 digits.\n" RESET);
                break;
            }
            insertContact(name, phone);
            break;

        case 2:
            printf(LIGHT_GREEN "Enter the name of the contact to update: " RESET);
            getchar();
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;
            updateContact(name);
            break;

        case 3:
            printf(LIGHT_GREEN "Enter the name of the contact to delete: " RESET);
            getchar();
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;
            deleteContact(name);
            break;

        case 4:
            displayContacts();
            break;

        case 5:
            printf(LIGHT_GREEN "Enter the name to search for: " RESET);
            getchar();
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;
            searchContact(name);
            break;

        case 6:
            saveContacts(); // Save contacts to the file before exiting
            printf(LIGHT_GREEN "Exiting program...\n" RESET);
            return 0;

        default:
            printf(LIGHT_GREEN "Invalid choice, please try again.\n" RESET);
            break;
        }
    }
    return 0;
}
