#include "main.h"
#include <stdio.h>

/*----------------------------------------------------------
  Function: display_database
  Purpose : Display the complete inverted search database
----------------------------------------------------------*/
void display_database(Table* table)
{
    // Print database header
    printf("\nDatabase details:\n");

    // Loop through all 27 hash table indices
    for (int i = 0; i < 27; i++)
    {
        // Skip index if no word is present
        if (table[i].main_link == NULL)
        {
            continue;
        }

        // Point to the first main node at this index
        Main* main = table[i].main_link;

        // Traverse through all main nodes (words)
        while (main != NULL)
        {
            // Print index, word, and number of files containing the word
            printf("%2d    %-10s    %d    ",
                   i,
                   main->word,
                   main->file_count);

            // Pointer to sub-node list (file details)
            Sub* sub = main->sub_link;

            // Traverse through sub nodes
            while (sub != NULL)
            {
                // Print file name and word count in that file
                printf("%-10s --> %d    ",
                       sub->file_name,
                       sub->word_count);

                sub = sub->sub_link;   // Move to next sub node
            }

            main = main->main_link;    // Move to next word in main list
            printf("\n");              // New line after each word entry
        }
    }
}
