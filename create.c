#include "main.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*----------------------------------------------------------
  Function: check_word
  Purpose : Check whether the given file already exists
            in the sub-node list of a word
----------------------------------------------------------*/
int check_word(char* file_name, Sub* temp1, Sub** prev)
{
    // Traverse through sub-node list
    while (temp1 != NULL)
    {
        // If file name matches
        if (strcmp(file_name, temp1->file_name) == 0)
        {
            *prev = temp1;   // Store address of matched sub-node
            return 0;        // File already exists
        }

        *prev = temp1;       // Move prev pointer
        temp1 = temp1->sub_link;
    }

    return 1;                // File not found
}

/*----------------------------------------------------------
  Function: insert_into_hash_table
  Purpose : Insert first word at a given hash index
----------------------------------------------------------*/
void insert_into_hash_table(Table* table, int index, char* word, char* file_name)
{
    // Allocate memory for main node and sub node
    Main* new = (Main*)malloc(sizeof(Main));
    Sub* sub_new = (Sub*)malloc(sizeof(Sub));

    // Link main node to hash table
    table[index].main_link = new;

    // Initialize main node
    new->file_count = 1;
    new->main_link = NULL;
    new->sub_link = sub_new;
    strncpy(new->word, word, max_len - 1);
    new->word[max_len-1] = '\0';

    // Initialize sub node
    sub_new->word_count = 1;
    sub_new->sub_link = NULL;
    strncpy(sub_new->file_name, file_name, max_len - 1);
    sub_new->file_name[max_len-1] = '\0';
}

/*----------------------------------------------------------
  Function: create_database
  Purpose : Read words from files and create inverted index
----------------------------------------------------------*/
void create_database(Slist* head, Table* table)
{
    // Traverse through file list
    Slist* temp = head;

    while (temp != NULL)
    {
        char word[max_len];

        // Open file
        FILE* fptr = fopen(temp->file_name, "r");
        if (!fptr)
        {
            printf("WARNING: Could not open %s - skipping\n", temp->file_name);
            temp = temp->link;
            continue;
        }

        // Read word by word
        while (fscanf(fptr, "%19s", word) != EOF)
        {
            int number = 0;

            // Convert uppercase to lowercase
            if (word[0] >= 'A' && word[0] <= 'Z')
            {
                word[0] = word[0] + 32;
            }
            // If digit → special bucket
            else if (word[0] >= '0' && word[0] <= '9')
            {
                number = 1;
            }
            // Lowercase alphabet
            else if (word[0] >= 'a' && word[0] <= 'z')
            {
                number = 0;
            }
            // Special characters
            else
            {
                number = 1;
            }

            // Calculate hash index
            int index;
            if (number == 0)
                index = word[0] % 97;   // a → 0, b → 1 ...
            else
                index = 26;             // digits/symbols bucket

            // If index is empty
            if (table[index].main_link == NULL)
            {
                insert_into_hash_table(table, index, word, temp->file_name);
            }
            else
            {
                Main* main = table[index].main_link;
                Main* prev = NULL;
                int flag = 0;

                // Traverse main list
                while (main != NULL)
                {
                    // If word already exists
                    if (strcmp(main->word, word) == 0)
                    {
                        Sub* sub = main->sub_link;
                        Sub* sub_prev = NULL;

                        // Check if file already exists
                        int res = check_word(temp->file_name, sub, &sub_prev);

                        if (res == 0)
                        {
                            // Same word & same file → increment count
                            sub_prev->word_count++;
                            flag = 1;
                            break;
                        }
                        else
                        {
                            // Same word, new file → add sub node
                            Sub* new_sub = (Sub*)malloc(sizeof(Sub));
                            new_sub->sub_link = NULL;
                            new_sub->word_count = 1;
                            strncpy(new_sub->file_name, temp->file_name, max_len-1);
                            new_sub->file_name[max_len-1] = '\0';

                            // Attach sub-node at end
                            Sub* it = main->sub_link;
                            while (it->sub_link != NULL)
                                it = it->sub_link;
                            it->sub_link = new_sub;

                            main->file_count++;
                            flag = 1;
                            break;
                        }
                    }

                    prev = main;
                    main = main->main_link;
                }

                // If word not found → create new main node
                if (flag == 0)
                {
                    Main* main_node = (Main*)malloc(sizeof(Main));
                    main_node->main_link = NULL;
                    main_node->file_count = 1;
                    strncpy(main_node->word, word, max_len-1);
                    main_node->word[max_len-1] = '\0';

                    // Create first sub-node
                    Sub* sub_new = (Sub*)malloc(sizeof(Sub));
                    sub_new->sub_link = NULL;
                    sub_new->word_count = 1;
                    strncpy(sub_new->file_name, temp->file_name, max_len-1);
                    sub_new->file_name[max_len-1] = '\0';

                    main_node->sub_link = sub_new;

                    if (prev)
                        prev->main_link = main_node;
                }
            }
        }

        printf("Successfully created database for %s\n", temp->file_name);
        fclose(fptr);
        temp = temp->link;
    }

    printf("SUCCESS: DataBase Created Successfully\n");
}
