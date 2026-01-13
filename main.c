#include "main.h"
#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------------------
  Function: create_HT
  Purpose : Initialize the hash table
----------------------------------------------------------*/
void create_HT(Table *HT)
{
    // Loop through all 27 hash table indices
    for (int i = 0; i < 27; i++)
    {
        HT[i].key = i;          // Assign index as key
        HT[i].main_link = NULL; // Initialize main node pointer to NULL
    }
}

/*----------------------------------------------------------
  Main Function
----------------------------------------------------------*/
int main(int argc, char *argv[])
{
    Slist *head = NULL;     // Head pointer for input file linked list
    Table table[27];        // Hash table with 27 indices

    create_HT(table);       // Initialize hash table

    // Welcome banner
    printf("\n==============================================\n");
    printf("          WELCOME TO INVERTED SEARCH\n");
    printf("==============================================\n");

    // Check if at least one file name is passed
    if (argc != 1)
    {
        // Validate input files and create file list
        if (read_and_validate_arguments(argv, argc, &head) == FAILURE)
        {
            return 0;       // Exit if no valid files
        }

        int created = 0;    // Flag to track database creation
        int updated = 0;    // Flag to track database update

        // Infinite menu loop
        while (1)
        {
            printf("\n==============================================\n");
            printf("                 MAIN MENU\n");
            printf("==============================================\n");
            printf("1. Create Database\n");
            printf("2. Search Word\n");
            printf("3. Display Database\n");
            printf("4. Update Database\n");
            printf("5. Save Database\n");
            printf("6. Exit\n");
            printf("----------------------------------------------\n");
            printf("Enter your choice : ");

            int opt;

            // Read user option safely
            if (scanf("%d", &opt) != 1)
            {
                printf("Invalid input\n");
                break;
            }

            // Menu option handling
            switch (opt)
            {
                case 1:
                    // Create database only once
                    if (created == 0)
                    {
                        create_database(head, table);
                        created = 1;
                    }
                    else
                    {
                        printf("ERROR: You can't create the database again\n");
                    }
                    break;

                case 2:
                    // Search for a word in the database
                    search_database(table);
                    break;

                case 3:
                    // Display entire database
                    display_database(table);
                    break;

                case 4:
                    // Update database only if not created or updated earlier
                    if (created == 0 && updated == 0)
                    {
                        int real = 0;
                        if (update_database(table, &head, &real) == SUCCESS)
                        {
                            updated = 1;
                        }
                    }
                    else
                    {
                        printf("ERROR: Database already created or updated\n");
                    }
                    break;

                case 5:
                    // Save database to a file
                    save_database(table);
                    break;

                case 6:
                    // Exit program
                    printf("\nExiting... Thank you for using Inverted Search!\n");
                    return 0;

                default:
                    // Invalid menu choice
                    printf("INVALID! Enter a valid option\n");
            }
        }
    }
    else
    {
        // No input files provided
        printf("ERROR: Enter valid number of arguments\n");
    }

    return 0;
}
