/// 
/// File: queueList.c
///
/// Description: This is the implementation of the queueList header file
///

#include "queueList.h"

/**
 * create_list() - this function will create the data that the list will be using
 */
Visited_List create_list() {
    // Create the list by istantiating memory for the new visited list, and then set the members 
    //      of that list to the proper states. Return the NULL if the memory was not allocated
    Visited_List new_list;
    new_list = (Visited_List)malloc(sizeof(struct Visit_List));
    if (new_list != NULL) {
        new_list->list_size = 0;
        new_list->first_node = NULL;
        return new_list;
    }
    return NULL;
}

/**
 * list_empty() - this function will check if the list is empty by seeing if the first node is NULL
 */
bool list_empty(Visited_List list) {
    // Return if the list is empty or not
    return (list->first_node == NULL);
}

/**
 * list_insert() - this function will insert a pair of coordinates into the list in LIFO ordering
 */
void list_insert(Visited_List list, Node* node) {
    if (list_empty(list)) {
        // Set the node to the first element of the list if the list was 
        //      preiously empty
        list->first_node = node;
        list->list_size = 1;
    } else {
        // Find the last node in the list and add to it if the list was 
        //      not previously empty
        Node* current = list->first_node;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
        list->list_size += 1;
    }

    // free(node);
}

/**
 * list_remove() - this function will remove the first node and return it
 */
Node * list_remove(Visited_List list) {
    if (list_empty(list)) {
        // Print an error message if there were no elements on the list to pop off
        fprintf(stderr, "There was no node to remove from the list.\n");
        exit(EXIT_FAILURE);
    } else {
        Node *temp = list->first_node;
        if (list->list_size > 1) {
            list->first_node = list->first_node->next;
        } else {
            list->first_node = NULL;
        }
        list->list_size -= 1;
        return temp;
    }
}

/**
 * list_clear() - this function will clear all of the data in the list
 */
void list_clear(Visited_List list) {
    // Loop through and set all of the nodes in the visited list to NULL
    while (list->first_node) {
        Node* node = list->first_node;
        list->first_node = list->first_node->next;
        free(node);
    }
}

/**
 * list_destroy() - this function will free all of the memory associated with the list
 */
void list_destroy(Visited_List list) {
    // Clear all of the nodes, including the head, and then free the memory that was dynamically
    //      allocated for the list
    if (!list_empty(list)) {
        list_clear(list);
    }
    free(list);
}