#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_node.h"

/**
 * @brief Creates a new node and prepends it to the database list.
 *
 * @param next A Pointer to the next node in the list.
 * @param key The key to store in the node.
 * @param value The value to duplicate and store.
 * @return A pointer to the new node.
 */
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

/**
 * @brief Creates a new node and appends it to the database list.
 *
 * Used when reconstructing a list from the database.
 *
 * @param prev A double pointer to the node preceding the new node, its next value will be modified
 *             to point to the new node.
 * @param key The key to store in the node.
 * @param value The value to duplicate and store.
 * @return A pointer to the new node.
 */
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
        (*prev)->next = node; // Append to previous node it exists
    return node;
}

/**
 * @brief Finds a node by key in the database list.
 *
 * @param head A pointer to the head of the list.
 * @param key The key to search for.
 * @return A pointer to the found node, or nullptr if not found.
 */
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

/**
 * @brief Modifies the value of a node with the given key.
 *
 * @param head A pointer to the head of the list.
 * @param key The key of the node to modify.
 * @param value The new value to set.
 * @return True if the node was found and modified, false otherwise.
 */
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

/**
 * @brief Deletes a node by key from the database list.
 *
 * @param head A double pointer to the head of the list.
 * @param key The key of the node to delete.
 * @return True if a node was deleted, false if it was not found.
 */
bool delete_node(ListNode** head, const int key)
{
    ListNode* curr = *head;
    ListNode* prev = nullptr;
    while (curr != nullptr)
    {
        if (curr->key == key)
        {
            if (curr == *head)
                *head = curr->next; // Update head if deleting the first node
            if (prev != nullptr)
                prev->next = curr->next;

            curr->key = 0;
            curr->next = nullptr;
            free(curr->value);
            curr->value = nullptr;
            free(curr);
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

/**
 * @brief Creates a linked list from entries in a database file.
 *
 * Expects each line to be in the format: `key,value`.
 *
 * @param db A file pointer to the database file.
 * @return A pointer to the head of the reconstructed list.
 */
ListNode* create_list(FILE* db)
{
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    char* line = nullptr;
    size_t len = 0;
    while (getline(&line, &len, db) != -1)
    {
        char* rest = line;
        const char* key = strsep(&rest, ",");
        const char* value = strsep(&rest, "\n"); // Extract value (db is newline terminated)
        if (key == nullptr || value == nullptr)
            break;  // Bad or empty line, ignore
        // Build the list in reverse so it maintains a consistent order
        tail = reverse_create_node(&tail, (int)strtol(key, nullptr, 10), value);
        if (head == nullptr)
            head = tail; // Set head if first node
    }
    free(line);
    return head;
}

/**
 * @brief Prints the database list (either standard output or a database file).
 *
 * Each line will be in the format: `key,value`.
 *
 * @param fp A file pointer to write to.
 * @param head A pointer to the head of the list.
 */
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

/**
 * @brief Frees all memory associated with the database list.
 *
 * @param head A double pointer to the head of the list.
 */
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
