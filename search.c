#include "main.h"        // User-defined header file
#include <string.h>     // String handling functions
#include <stdio.h>      // Standard I/O functions

/*----------------------------------------------------------
  Function to search a word in the inverted search database
----------------------------------------------------------*/
void search_database(Table* table)
{
    char word[max_len];     // Buffer to store input word

    // Prompt user to enter the word to search
    printf("Enter the word that you want to search: ");
    getchar();              // Clear input buffer
    scanf("%19s", word);    // Read word safely (limit length)

    int number = 0;         // Flag to detect numeric word

    // Convert first letter to lowercase if uppercase
    if (word[0] >= 'A' && word[0] <= 'Z')
    {
        word[0] = word[0] + 32;   // ASCII conversion to lowercase
    }
    // Check if word starts with a digit
    else if (word[0] >= '0' && word[0] <= '9')
    {
        number = 1;              // Numeric word detected
    }

    int index;

    // Calculate hash table index
    if (number == 0)
    {
        index = word[0] % 97;    // a–z mapped to 0–25
    }
    else
    {
        index = 26;              // Digits stored at index 26
    }

    int flag = 0;                // Flag to check word presence

    // Get the main linked list at computed index
    Main* main = table[index].main_link;

    // Traverse main linked list
    while (main != NULL)
    {
        // Compare searched word with database word
        if (strcmp(main->word, word) == 0)
        {
            // Print index, word, and file count
            printf("%d    %s    %d    ",
                   index, main->word, main->file_count);

            // Traverse sub linked list (file details)
            Sub* sub = main->sub_link;
            while (sub != NULL)
            {
                // Print file name and word frequency
                printf("%s    %d    ",
                       sub->file_name, sub->word_count);
                flag = 1;        // Word found
                sub = sub->sub_link;
            }

            printf("\n");        // New line after printing details
            break;               // Exit after finding word
        }

        // Move to next main node
        main = main->main_link;
    }

    // If word not found in database
    if (flag == 0)
    {
        printf("NOTE: Word not in the database\n");
    }
}
