/// 
/// File: queueList.h
///
/// Description: header file that contains the nodes and the data structure for the visited
///      node list. Also, This file contains the function headers for those that will be
///      implemented in the corresponding source file.
///
///  @author: Giovanni Coppola gac6151@rit.edu
///

#ifndef QUEUE_LIST_H
#define QUEUE_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <unistd.h>
#include <getopt.h>

// This is what one of the coordinate nodes is going to look like in the 
//     visited list when doing the BFS.
typedef struct Node {
    int row;
    int col;
    //struct Node* parent;
    struct Node* next;
}Node;

// This will be the entire structure of the visited list for the BFS.
typedef struct Visit_List {
    int list_size;
    Node* first_node;
}*Visited_List;


/**
 * Instantiate a list of nodes that will be used for the visited list when
 *      doing the BFS algorithm to find the shortest path through the maze
 * 
 * @return Visited_List - the instantiated visited list of nodes
 */
Visited_List create_list();

/**
 * Check if the visited list is empty
 * 
 * @param list - the list of visited nodes
 * @return true if the list is empty
 * @return false if there are valid nodes in the list
 */
bool list_empty(Visited_List list);

/**
 * Insert the nodes into the list in a LIFO ordering. There does not need to
 *      be special ordering since we want to know in what order the nodes were
 *      visited so we can backtrack.
 * 
 * @param list - the list of visited nodes
 * @param node - a pointer to the node that will be passed in
 */
void list_insert(Visited_List list, Node* node);

/**
 * Pop off the first node of the list and return if needed
 * 
 * @param list - the list of visited nodes
 * @return Node* - a pointer to the node that was removed from the list
 */
Node * list_remove(Visited_List list);

/**
 * Clear the elements in the list by first setting the nodes to NULL so there are 
 *      no elements left after the program is finished running
 * 
 * @param list - the list of visited nodes
 */
void list_clear(Visited_List list);

/**
 * @Destroy the visited nodes list by clearing the nodes, and then clearing the 
 *      first node of the list. Use free on the list when done since it was 
 *      dynamically allocated on the heap/
 * 
 * @param list - the list of visited nodes
 */
void list_destroy(Visited_List list);

#endif