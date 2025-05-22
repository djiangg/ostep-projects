#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_node.h"

/**
 * @brief Creates a new linked list node containing a data string.
 *
 * @param data Data to store in the new node.
 * @param next A pointer to the next node in the list.
 * @return A pointer to the newly created node.
 */
ListNode* create_node(const char* data, ListNode* next)
{
    ListNode* node = malloc(sizeof(ListNode));
    if (node == nullptr)
    {
        fprintf(stderr, "reverse: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    node->data = strdup(data);
    node->next = next;
    return node;
}

/**
 * @brief Frees all memory associated to the linked list.
 *
 * @param head A double pointer to the head of the list.
 */
void delete_list(ListNode** head)
{
    ListNode* curr = *head;
    while (curr != nullptr)
    {
        ListNode* next = curr->next;

        free(curr->data);
        curr->data = nullptr;
        curr->next = nullptr;
        free(curr);

        curr = next;
    }
    *head = nullptr;
}
