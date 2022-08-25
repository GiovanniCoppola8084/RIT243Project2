/// 
/// File: mopsolver.c 
///
/// Description: This file will run the code in order to solve the maze and print it 
///     according to rules that the user will put in place through command line arguments
///
/// @author Giovanni Coppola (gac6151@rit.edu)
///

#include <string.h>

// Include the header file that will allow for a linked list implementation for the visited list that 
//      will be used during the BFS
#include "queueList.h"

void print_help(char* output);
void input_graph(FILE* filename, char ***array, int *row_count, int *col_count);
void free_maze(char ***array, int row_count);
void print_matrix(int row_size, int col_size, char **maze, FILE* output);
void solver_for_BFS(int row_size, int col_size, char ***maze, Visited_List queue, int* solved_size);

int main(int argc, char* argv[]) {
    FILE* infile;
    FILE* outfile = stdout;
    bool is_stdin = true;
    bool is_output = false;
    int row_size = 0;
    int col_size = 0;
    bool print_maze = false;
    bool print_solution_step = false;
    bool print_solution_maze = false;
    char** maze;

    int opt;
    while ( (opt = getopt(argc,argv,"hdspi:o:") ) != -1) {
        switch(opt) {
            case 'h':
                // Print the help message (call a function to do this)
                print_help("stdout");
                return EXIT_SUCCESS;
                break;
            case 'd':
                // Pretty-print the maze after reading it in
                print_maze = true;
                break;
            case 's':
                // Print the length of the shortest path or "no solution" if none found
                print_solution_step = true;
                break;
            case 'p':
                // Pretty-print the maze with the path if a solution exists
                print_solution_maze = true;
                break;
            case 'i':
                if (optarg == NULL) {
                    // Read in from stdin as a default if no parameter is given
                    infile = stdin;
                } else {
                    // Read from a given file input if there was a name given
                    infile = fopen(optarg, "r");
                    is_stdin = false;
                }
                break;
            case 'o':
                // Write all of the output to the output file
                // fgets(outfileName, 100, stdin);
                if (optarg == NULL) {
                    outfile = stdout;
                } else {
                    outfile = fopen(optarg, "w");
                    is_output = true;
                }
                break;
            case ':':
                // If the command line is missing an argument, print the usage message and return with failure
                print_help("");
                return(EXIT_FAILURE);
                break;
            case '?':
            default:
                // Display the usage message and exit the code with failure
                print_help("");
                return(EXIT_FAILURE);
                break;
        }
    }

    // Call the function to take in the first graph
    input_graph(infile, &maze, &row_size, &col_size);

    // Functions to print what is necessary to print
    // DO NOT FORGET TO PRINT TO AN OUTPUT FILE IF THE USER WANTS
    if (print_maze) {
        print_matrix(row_size, col_size, maze, outfile);
    }
    
    Visited_List visited = create_list();
    // Make a visited queue
    int solved_size = 0;
    // If there was a solution found by the dfs, then solve by bfs
    // solver_for_BFS(row_size, col_size, maze, visited, predecessors, solved_size);
    solver_for_BFS(row_size, col_size, &maze, visited, &solved_size);
    if (print_solution_step) {
        if (solved_size > 0) {
            fprintf(outfile, "Solution in %d steps.\n", solved_size);
        } else {
            fprintf(outfile, "No solution.\n");
        }
    } 

    if (print_solution_maze) {
        if (solved_size > 0) {
            // Loop through and make sure to add the path to the maze. Pretty print the maze after
            print_matrix(row_size, col_size, maze, outfile);
        }
    }

    // Free the visited list
    list_destroy(visited);

    // Call functions here to free all of the malloced data
    // Free the data for the 1d array since it was orignally malloced
    free_maze(&maze, row_size);

    if (!is_stdin) {
        fclose(infile);
    }

    if (is_output) {
        fclose(outfile);
    }
    return 0;
}

/**
 * This function will print the usage message to either the standard output,
 * or the standard error output depending on where the functions is called
 * 
 * @param output - the string that will determine where the usage message will
 *      be printed
 */
void print_help(char* output) {
    if (strcmp(output, "stdout") == 0) {
        // Print the message to standard out
        printf(
            "Usage:\n"
            "mopsolver [-hdsp] [-i INFILE] [-o OUTFILE]\n"
            "Options:\n"
            "  -h          Print usage and options list to stdout only.    (Default: off)\n"
            "  -d          Pretty-print (display) the maze after reading.  (Default: off)\n"
            "  -s          Print length of shortest path or \'No solution\'. (Default: off)\n"
            "  -p          Pretty-print maze with the path, if one exists. (Default: off)\n"
            "  -i infile   Read maze from infile.                          (Default: stdin)\n"
            "  -o outfile  Write all output to outfile.                    (Default: stdout)\n"
        );
    } else {
        // Print the message to standard error
        fprintf(stderr,
            "Usage:\n"
            "mopsolver [-hdsp] [-i INFILE] [-o OUTFILE]\n"
            "Options:\n"
            "  -h          Print usage and options list to stdout only.    (Default: off)\n"
            "  -d          Pretty-print (display) the maze after reading.  (Default: off)\n"
            "  -s          Print length of shortest path or \'No solution\'. (Default: off)\n"
            "  -p          Pretty-print maze with the path, if one exists. (Default: off)\n"
            "  -i infile   Read maze from infile.                          (Default: stdin)\n"
            "  -o outfile  Write all output to outfile.                    (Default: stdout)\n"
        );
    }
}

/**
 * This function will take in data from an input file and store it in a 1d matrix.
 * The array will be later converted to a matrix, for a maze representation
 * 
 * @param filename - the file that will be used to take in the matrix
 * @param array - the array that the data from the file will be stored in
 * @param row_count - the number of rows for the matrix
 * @param col_count - the number of columns for the matrix
 */
void input_graph(FILE* filename, char ***array, int *row_count, int *col_count) {
    // Delete this code if it is not needed when project has been completed!!!
    // This is for putting it directly into a matrix    
    // Dynamically allocate the memory for the 2d array of characters

    char character;
    int column = 0;
    int row = 0;
    *row_count = 0;
    *col_count = 0;

    // Get the length of one line and return the pointer back to the start of the file
    character = fgetc(filename);
    while (character != '\n') {
        if (character != ' ') {
            (*col_count)++;
        }
        character = fgetc(filename);
    }
    fseek(filename, 0, SEEK_SET);

    // Malloc the first row of the matrix
    (*row_count) = 1;

    *array = (char**)malloc((*row_count)*sizeof(char*));
    (*array)[0] = (char*)malloc(((*col_count) + 1)*sizeof(char));

    while ( (character = fgetc(filename)) != EOF) {
        if (character == '0') {
            (*array)[row][column] = '.';
            column++;
            if (column >= (*col_count)) {
                column = 0;
                row++;
            }
        } else if (character == '1') {
            (*array)[row][column] = '#';
            column++;
            if (column >= (*col_count)) {
                column = 0;
                row++;
            }
        } else if (character == '\n') {
            *array = (char**)realloc(*array, ((*row_count) + 1)*sizeof(char*));
            (*array)[(*row_count)] = (char*)malloc((*col_count)*sizeof(char) * 1);
            (*row_count)++;
        }
    }
    (*row_count)--;
}

/**
 * Function to free the memory that was dynamically allocated to the matrix
 * 
 * @param array - the matrix to be freed
 * @param row_count - the number of rows in the matrix
 */
void free_maze(char ***array, int row_count) {
    for (int index = 0; index < row_count; index++) {
        free((*array)[index]);
    }

    free(*array);
}

/**
 * This function will pretty print the matrix to the standard output. It 
 *      will include borders on the top, bottom, and sides of the maze
 *      while also putting a space between each value to make it more
 *      readable.
 * 
 * @param row_size - the number of rows in the maze
 * @param col_size - the number of columns in the maze
 * @param maze - the maze that will be printed out
 * // Possiblly change this so that stdout can be used as a FILE* to get rid of if statement
 * @param output - the name of the output file if necessary
 */
void print_matrix(int row_size, int col_size, char **maze, FILE* output) {
    fprintf(output, "|");
    for (int index = 0; index < ((col_size * 2) + 1); index++) {
        fprintf(output, "%c", '-');
    }
    fprintf(output, "|\n");
    for (int row = 0; row < row_size; row++) {
        if (row == 0) {
            fprintf(output, "  ");
        } else {
            fprintf(output, "| ");
        }
        for (int col = 0; col < col_size; col++) {
            fprintf(output, "%c ", maze[row][col]);
        }
        if (row == (row_size-1)) {
            fprintf(output, " \n");
        } else {
            fprintf(output, "|\n");
        }
    }

    fprintf(output, "|");
    for (int index = 0; index < ((col_size * 2) + 1); index++) {
        fprintf(output, "%c", '-');
    }
    fprintf(output, "|\n");
}

/**
 * Function to solve the maze through a BFS. This will take use a Visited_List as the queue, and it will
 *      change the nodes in the graph to show where it actually has been visited.
 * 
 * @param row_size - the total row size of the maze
 * @param col_size - the total col size of the maze
 * @param maze - the maze
 * @param queue - the Visited_List for the BFS
 * @param solved_size - the total size of the solution path, if one exists
 */
void solver_for_BFS(int row_size, int col_size, char ***maze, Visited_List queue, int* solved_size) {
    Node* final_node = NULL;
    Node* node = (Node*)malloc(sizeof(struct Node));
    node->row = 0;
    node->col = 0;
    //node->parent = NULL;
    node->next = NULL;
    list_insert(queue, node);

    while (!list_empty(queue)) {
        Node* current = list_remove(queue);
        // Check if the final node has been reached. Break the while loop if it has
        if (current->row == (row_size-1) && current->col == (col_size-1)) {
            final_node = current;
            break;
        }

        if (((current->row)-1 >= 0) && (*maze)[current->row-1][current->col] == '.') {
            node = (Node*)malloc(sizeof(struct Node));
            node->row = (current->row)-1;
            node->col = current->col;
            node->next = NULL;
            list_insert(queue, node);
            (*maze)[current->row-1][current->col] = 'v';
        }
        if (((current->col)-1 >= 0) && (*maze)[current->row][current->col-1] == '.') {
            node = (Node*)malloc(sizeof(struct Node));
            node->row = current->row;
            node->col = (current->col)-1;
            node->next = NULL;
            list_insert(queue, node);
            (*maze)[current->row][current->col-1] = '>';
        }
        if (((current->row)+1 < row_size) && (*maze)[current->row+1][current->col] == '.') {
            node = (Node*)malloc(sizeof(struct Node));
            node->row = (current->row)+1;
            node->col = current->col;
            node->next = NULL;
            list_insert(queue, node);
            (*maze)[current->row+1][current->col] = '^';
        }
        if (((current->col)+1 < col_size) && (*maze)[current->row][current->col+1] == '.') {
            node = (Node*)malloc(sizeof(struct Node));
            node->row = current->row;
            node->col = (current->col)+1;
            node->next = NULL;
            list_insert(queue, node);
            (*maze)[current->row][current->col+1] = '<';
        }
        free(current);
    }

    print_matrix(row_size, col_size, *maze, stdout);

    if (final_node){
        int row = final_node->row;
        int col = final_node->col;
        // Generate the path by following the arrows
        while (true) {
            if (row == 0 && col == 0) {
                break;
            }
            // Check the 4 directions and find an arrow to move into the proper spot
            if ((row-1 >= 0) && (*maze)[row][col] == '^') {
                (*maze)[row][col] = '+';
                (*solved_size)++;
                row--;
            }
            else if (((col)-1 >= 0) && (*maze)[row][col] == '<') {
                (*maze)[row][col] = '+';
                (*solved_size)++;
                col--;
            }
            else if (((row)+1 < row_size) && (*maze)[row][col] == 'v') {
                (*maze)[row][col] = '+';
                (*solved_size)++;
                row++;
            }
            else if (((col)+1 < col_size) && (*maze)[row][col] == '>') {
                (*maze)[row][col] = '+';
                (*solved_size)++;
                col++;
            }
        }
        (*maze)[0][0] = '+';
        (*solved_size)++;
    }

    // Loop through and change the rest of the arrows now
    for (int index = 0; index < row_size; index++) {
        for (int value = 0; value < col_size; value++) {
            if ((*maze)[index][value] == '^' || (*maze)[index][value] == 'v' || 
            (*maze)[index][value] == '<' || (*maze)[index][value] == '>') {
                (*maze)[index][value] = '.';
            }
        }
    }
}