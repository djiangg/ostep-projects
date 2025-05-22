#ifndef LIST_NODE_H
#define LIST_NODE_H

typedef struct ListNode
{
    char* data;
    struct ListNode* next;
} ListNode;

ListNode* create_node(const char* data, ListNode* next);
void delete_list(ListNode** head);

#endif //LIST_NODE_H
