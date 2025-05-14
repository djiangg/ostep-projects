#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_node.h"

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
