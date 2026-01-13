#ifndef MAIN_H
#define MAIN_H
// Header guard to avoid multiple inclusions of this file

#include <stdio.h>      // Standard input/output functions
#include <stdlib.h>     // Memory allocation functions
#include <string.h>     // String handling functions

#define max_table 27    // Hash table size: 0–25 for a–z, 26 for digits
#define max_len 20      // Maximum length for words and file names

/*----------------------------------------------------------
  Status enum for function return values
----------------------------------------------------------*/
typedef enum
{
    FAILURE,            // Operation failed
    SUCCESS             // Operation successful
} Status;

/*----------------------------------------------------------
  Sub node structure
  Stores file-wise information for a word
----------------------------------------------------------*/
typedef struct sub_node
{
    char file_name[max_len];     // Name of the file
    int word_count;              // Frequency of word in that file
    struct sub_node* sub_link;   // Pointer to next sub node
} Sub;

/*----------------------------------------------------------
  Main node structure
  Stores unique words in the database
----------------------------------------------------------*/
typedef struct main_node
{
    char word[max_len];          // Indexed word
    int file_count;              // Number of files containing the word
    struct main_node* main_link; // Pointer to next main node
    struct sub_node* sub_link;   // Pointer to sub node list
} Main;

/*----------------------------------------------------------
  Hash table structure
  Each index points to a main linked list
----------------------------------------------------------*/
typedef struct hash_table
{
    int key;                     // Hash table index
    struct main_node* main_link; // Pointer to main node list
} Table;

/*----------------------------------------------------------
  Singly linked list for input file names
----------------------------------------------------------*/
typedef struct node
{
    char file_name[max_len];     // Input file name
    struct node* link;           // Pointer to next file node
} Slist;

/*----------------------------------------------------------
  Function Prototypes
----------------------------------------------------------*/

// Validate command-line arguments and create file list
Status read_and_validate_arguments(char* argv[], int argc, Slist** head);

// Create inverted search database from input files
void create_database(Slist* head, Table* table);

// Display the entire database
void display_database(Table* table);

// Save database to a file
void save_database(Table* table);

// Search a word in the database
void search_database(Table* table);

// Update database using a saved database file
Status update_database(Table* table, Slist** head, int* real);

// Delete a file node from linked list
int delete_node(Slist** head, char* file_name);

#endif
// End of main.h
