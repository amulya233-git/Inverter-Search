#include "main.h"          // User-defined header file
#include <stdio.h>         // Standard I/O functions
#include <string.h>        // String handling functions
#include <stdlib.h>        // Memory allocation functions

// Function to read command-line arguments and validate text files
Status read_and_validate_arguments(char* argv[], int argc, Slist** head)
{
    int flag = 0;          // Counts invalid files

    // Loop through all command-line arguments except program name
    for (int i = 1; i < argc; i++)
    {
        // Check if the file has .txt extension
        if (strstr(argv[i], ".txt") == NULL)
        {
            printf("%s is not a valid argument\n", argv[i]);
            flag++;        // Increment invalid file count
            continue;      // Move to next argument
        }

        // Try opening the file in read mode
        FILE* fptr = fopen(argv[i], "r");
        if (fptr == NULL)
        {
            printf("ERROR: No file name %s\n", argv[i]);
            flag++;        // File not found or cannot be opened
            continue;
        }

        // Move file pointer to end to check file size
        fseek(fptr, 0, SEEK_END);
        unsigned long int set = ftell(fptr);

        // Check if the file is empty
        if (set == 0)
        {
            printf("File opened but there is no data in %s\n", argv[i]);
            flag++;        // Empty file
            fclose(fptr); // Close the file
            continue;
        }

        // Close file after validation
        fclose(fptr);
        printf("FILE NAME %s is a valid FILE\n", argv[i]);

        // If linked list is empty, create first node
        if (*head == NULL)
        {
            Slist* new = (Slist*)malloc(sizeof(Slist)); // Allocate memory
            new->link = NULL;                           // Initialize link
            strncpy(new->file_name, argv[i], max_len-1);// Copy filename safely
            new->file_name[max_len-1] = '\0';           // Ensure null termination
            *head = new;                                // Update head
        }
        else
        {
            Slist* temp = *head;   // Pointer to traverse list
            Slist* prev = NULL;    // Pointer to store previous node
            int flag1 = 0;         // Flag to detect duplicate files

            // Traverse the linked list to check duplicates
            while (temp != NULL)
            {
                if (strcmp(temp->file_name, argv[i]) == 0)
                {
                    printf("The file %s is a duplicate file\n", argv[i]);
                    flag1 = 1;     // Duplicate found
                    break;
                }
                prev = temp;       // Move prev pointer
                temp = temp->link; // Move to next node
            }

            // If duplicate file, skip insertion
            if (flag1 == 1)
            {
                flag++;
                continue;
            }

            // Create new node for valid, non-duplicate file
            Slist* new = (Slist*)malloc(sizeof(Slist));
            new->link = NULL;                           
            strncpy(new->file_name, argv[i], max_len-1);
            new->file_name[max_len-1] = '\0';
            prev->link = new;      // Link new node at the end
        }
    }

    // If all provided files are invalid
    if (flag == argc - 1)
    {
        printf("ERROR: No files are valid files\n");
        return FAILURE;
    }

    return SUCCESS;        // At least one valid file found
}
