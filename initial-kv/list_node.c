#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_node.h"

ListNode* create_node(ListNode* next, const int key, const char* value)
{
    ListNode* node = malloc(sizeof(ListNode));
    if (node == nullptr)
    {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }
    node->key = key;
    node->value = strdup(value);
    node->next = next;
    return node;
}

ListNode* reverse_create_node(ListNode** prev, const int key, const char* value)
{
    ListNode* node = malloc(sizeof(ListNode));
    if (node == nullptr)
    {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }
    node->key = key;
    node->value = strdup(value);
    node->next = nullptr;
    if (*prev != nullptr)
        (*prev)->next = node;
    return node;
}

const ListNode* find_node(const ListNode* head, const int key)
{
    const ListNode* curr = head;
    while (curr != nullptr)
    {
        if (curr->key == key)
            return curr;
        curr = curr->next;
    }
    return nullptr;
}

bool modify_node(ListNode* head, const int key, const char* value)
{
    ListNode* curr = head;
    while (curr != nullptr)
    {
        if (curr->key == key)
        {
            free(curr->value);
            curr->value = strdup(value);
            return true;
        }
        curr = curr->next;
    }
    return false;
}

bool delete_node(ListNode** head, const int key)
{
    ListNode* curr = *head;
    ListNode* prev = nullptr;
    while (curr != nullptr)
    {
        if (curr->key == key)
        {
            if (curr == *head)
                *head = curr->next; // Need to replace head b/c it is being deleted
            if (prev != nullptr)
                prev->next = curr->next; // Need to ensure the list is still linked

            curr->key = 0;
            curr->next = nullptr;
            free(curr->value);
            curr->value = nullptr;
            free(curr);
            curr = nullptr;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

ListNode* create_list(FILE* db)
{
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    char* line = nullptr;
    size_t len = 0;
    while (getline(&line, &len, db) != -1)
    {
        // Need a static pointer to free strdup malloced memory after using strsep
        char* line_copy = strdup(line);
        char* rest = line_copy;
        const char* key = strsep(&rest, ",");
        // Adding to db adds extra newline, so strip it from value
        const char* value = strsep(&rest, "\n");
        if (key == nullptr || value == nullptr) // Ignore empty/bad lines in db
        {
            free(line_copy);
            line_copy = nullptr;
            break;
        }
        // Create the list backwards so the list ordering remains consistent between multiple runs
        tail = reverse_create_node(&tail, (int)strtol(key, nullptr, 10), value);
        if (head == nullptr)
            head = tail;

        free(line_copy);
        line_copy = nullptr;
    }
    free(line);
    line = nullptr;
    return head;
}

void print_list(FILE* fp, const ListNode* head)
{
    const ListNode* curr = head;
    while (curr != nullptr)
    {
        if (curr->value != nullptr)
            fprintf(fp, "%d,%s\n", curr->key, curr->value);
        curr = curr->next;
    }
}

void delete_list(ListNode** head)
{
    ListNode* curr = *head;
    while (curr != nullptr)
    {
        ListNode* next = curr->next;

        curr->key = 0;
        curr->next = nullptr;
        free(curr->value);
        curr->value = nullptr;
        free(curr);

        curr = next;
    }
    *head = nullptr;
}
