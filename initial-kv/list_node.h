#ifndef LIST_NODE_H
#define LIST_NODE_H

typedef struct ListNode
{
    int key;
    char* value;
    struct ListNode* next;
} ListNode;

ListNode* create_node(ListNode* next, int key, const char* value);
ListNode* reverse_create_node(ListNode** prev, int key, const char* value);
const ListNode* find_node(const ListNode* head, int key);
bool modify_node(ListNode* head, int key, const char* value);
bool delete_node(ListNode** head, int key);

ListNode* create_list(FILE* db);
void print_list(FILE* fp, const ListNode* head);
void delete_list(ListNode** head);


#endif //LIST_NODE_H
