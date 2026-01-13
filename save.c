#include "main.h"        // User-defined header file
#include <stdio.h>      // Standard I/O functions
#include <string.h>     // String handling functions

/*----------------------------------------------------------
  Function to save the inverted search database to a file
----------------------------------------------------------*/
void save_database(Table* table)
{
    int cont = 1;               // Flag to control filename validation loop
    char file[256];             // Buffer to store file name

    // Loop until a valid .txt filename is entered
    while (cont)
    {
        printf("Enter the file that you want to save: ");
        getchar();              // Clear input buffer
        scanf("%255s", file);   // Read file name safely

        // Check if file has .txt extension
        if (strstr(file, ".txt") != NULL)
        {
            cont = 0;           // Valid filename
        }
        else
        {
            printf("Enter valid file name\n");
        }
    }

    // Open file in write mode
    FILE* fptr = fopen(file, "w");
    if (!fptr)
    {
        printf("ERROR: Could not open %s for writing\n", file);
        return;
    }

    // Traverse hash table (0–25 for alphabets, 26 for digits)
    for (int i = 0; i < 27; i++)
    {
        // Skip empty hash table index
        if (table[i].main_link == NULL)
        {
            continue;
        }

        // Traverse main linked list
        Main* main = table[i].main_link;
        while (main != NULL)
        {
            // Write index, word, and file count
            fprintf(fptr, "#%d;%s;%d", i, main->word, main->file_count);

            // Traverse sub linked list
            Sub* sub = main->sub_link;
            while (sub != NULL)
            {
                // Write file name and word count
                fprintf(fptr, ";%s;%d", sub->file_name, sub->word_count);
                sub = sub->sub_link;
            }

            // Mark end of entry
            fprintf(fptr, ";#\n");

            // Move to next main node
            main = main->main_link;
        }
    }

    fclose(fptr);   // Close file

    // Success message
    printf("SUCCESS: Database saved successfully to %s\n", file);
}
