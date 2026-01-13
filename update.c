#include "main.h"        // User-defined header
#include <string.h>     // String handling functions
#include <stdlib.h>     // malloc, free
#include <stdio.h>      // Standard I/O

/*----------------------------------------------------------
  Function to delete a node from Slist based on file name
----------------------------------------------------------*/
int delete_node(Slist** head, char* file_name)
{
    Slist* temp = *head;     // Pointer to traverse the list
    Slist* prev = NULL;      // Pointer to track previous node

    // Traverse the linked list
    while (temp != NULL)
    {
        // Compare file names
        if (strcmp(temp->file_name, file_name) == 0)
        {
            // If node to be deleted is the first node
            if (prev == NULL)
            {
                *head = temp->link; // Move head to next node
                free(temp);         // Free memory
                return 0;           // Success
            }

            // If node is in middle or end
            prev->link = temp->link; // Bypass current node
            free(temp);              // Free memory
            return 0;                // Success
        }

        // Move pointers forward
        prev = temp;
        temp = temp->link;
    }
    return 1;   // Failure: file not found
}

/*----------------------------------------------------------
  Function to update inverted search database from file
----------------------------------------------------------*/
Status update_database(Table* table, Slist** head, int* real)
{
    printf("Enter the file you want to upload: ");

    char file[256];
    getchar();                     // Clear input buffer
    scanf("%255s", file);          // Read file name safely

    // Check file extension
    if (strstr(file, ".txt") == NULL)
    {
        printf("%s is not an valid argument\n", file);
        *real = 1;
        return FAILURE;
    }

    // Open file
    FILE* fptr = fopen(file, "r");
    if (fptr == NULL)
    {
        printf("ERROR: No file name %s\n", file);
        *real = 1;
        return FAILURE;
    }

    /* Check header format (# at beginning and end) */
    char ch1;
    ch1 = fgetc(fptr);             // Read first character
    fseek(fptr, -1, SEEK_END);     // Move to last character
    char ch2 = fgetc(fptr);
    rewind(fptr);                  // Reset file pointer

    // Validate database file format
    if (ch1 == '#' && ch2 == '#')
    {
        // Read file till end
        while (fscanf(fptr, "%c", &ch1) != EOF)
        {
            char ind[4] = "\0\0\0\0";
            fscanf(fptr, "%[^;];", ind);  // Read index value

            // Convert index string to integer
            int i = 0, index = 0;
            while (ind[i])
            {
                index = index * 10 + (ind[i] - '0');
                i++;
            }

            // Read word
            char word[20];
            fscanf(fptr, "%[^;];", word);

            // Read file count
            char file_cnt[4] = "\0\0\0\0";
            fscanf(fptr, "%[^;];", file_cnt);

            i = 0;
            int file_count = 0;
            while (file_cnt[i])
            {
                file_count = file_count * 10 + (file_cnt[i] - '0');
                i++;
            }

            // Loop through file count
            for (int k = 0; k < file_count; k++)
            {
                char file_name[20];
                fscanf(fptr, "%[^;];", file_name);

                char word_cnt[4] = "\0\0\0\0";
                fscanf(fptr, "%[^;];", word_cnt);

                // Convert word count
                int j = 0, word_count = 0;
                while (word_cnt[j])
                {
                    word_count = word_count * 10 + (word_cnt[j] - '0');
                    j++;
                }

                // If index slot is empty
                if (table[index].main_link == NULL)
                {
                    // Create main node
                    Main* new = (Main*)malloc(sizeof(Main));
                    Sub* sub_new = (Sub*)malloc(sizeof(Sub));

                    table[index].main_link = new;
                    new->file_count = file_count;
                    new->main_link = NULL;
                    new->sub_link = sub_new;

                    strncpy(new->word, word, max_len - 1);
                    new->word[max_len - 1] = '\0';

                    // Create sub node
                    sub_new->word_count = word_count;
                    sub_new->sub_link = NULL;
                    strncpy(sub_new->file_name, file_name, max_len - 1);
                    sub_new->file_name[max_len - 1] = '\0';
                }
                else
                {
                    // Traverse main list
                    Main* main = table[index].main_link;
                    Main* prev = NULL;
                    int flag = 0;

                    while (main != NULL)
                    {
                        // If word already exists
                        if (strcmp(main->word, word) == 0)
                        {
                            // Create new sub node
                            Sub* sub_node_link = (Sub*)malloc(sizeof(Sub));
                            sub_node_link->word_count = word_count;
                            strncpy(sub_node_link->file_name, file_name, max_len - 1);
                            sub_node_link->file_name[max_len - 1] = '\0';
                            sub_node_link->sub_link = NULL;

                            // Append sub node
                            Sub* sub_nlink = main->sub_link;
                            Sub* prev_sub = NULL;

                            while (sub_nlink != NULL)
                            {
                                prev_sub = sub_nlink;
                                sub_nlink = sub_nlink->sub_link;
                            }

                            if (prev_sub == NULL)
                                main->sub_link = sub_node_link;
                            else
                                prev_sub->sub_link = sub_node_link;

                            flag = 1;
                            break;
                        }
                        prev = main;
                        main = main->main_link;
                    }

                    // If word not found, create new main node
                    if (flag == 0)
                    {
                        Main* new = (Main*)malloc(sizeof(Main));
                        new->main_link = NULL;
                        new->sub_link = NULL;
                        new->file_count = file_count;

                        strncpy(new->word, word, max_len - 1);
                        new->word[max_len - 1] = '\0';

                        Sub* sub_new = (Sub*)malloc(sizeof(Sub));
                        sub_new->word_count = word_count;
                        sub_new->sub_link = NULL;
                        strncpy(sub_new->file_name, file_name, max_len - 1);
                        sub_new->file_name[max_len - 1] = '\0';

                        if (prev)
                            prev->main_link = new;
                    }
                }
            }
            fseek(fptr, 2, SEEK_CUR); // Skip ";#"
        }
    }
    else
    {
        printf("ERROR: thats not a valid file\n");
        *real = 1;
        fclose(fptr);
        return FAILURE;
    }

    fclose(fptr);

    /* Remove uploaded files from Slist if already present */
    Slist* temp = *head;
    while (temp != NULL)
    {
        for (int i = 0; i < 27; i++)
        {
            if (table[i].main_link == NULL)
                continue;

            Main* main = table[i].main_link;
            while (main != NULL)
            {
                Sub* sub = main->sub_link;
                while (sub != NULL)
                {
                    if (strcmp(sub->file_name, temp->file_name) == 0)
                    {
                        delete_node(head, temp->file_name);
                        temp = *head;  // Restart scan
                        break;
                    }
                    sub = sub->sub_link;
                }
                main = main->main_link;
            }
        }
        if (temp)
            temp = temp->link;
    }

    printf("SUCCESS: Update Successfully done\n");
    return SUCCESS;
}
