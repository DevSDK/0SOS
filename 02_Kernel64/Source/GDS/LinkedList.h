#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__
#include <Types.h>


#pragma pack(push, 1)
typedef struct _struct_Single_LinekdListNode
{
    void* NextNode; //Single Link Linked List
    QWORD ID; //Address or Data
}   LLIST_NODE_HEADER;

/*
    exmple of element data structure.


typedef struct _Example_Data
{
    LLIST_DATA_HEADER header;
    
    int data1;
    char data2;

} Example;
*/

typedef struct _struct_LinkedListDescriptor
{
    void* FirstNode; 
    void* LastNode;
    QWORD Count; 
} LLIST;

#pragma pack(pop)

void InitializeLList(LLIST* _Ld);

void Push_Back_LList(LLIST* _Ld, void* _Item);
void Push_Front_LList(LLIST* _Ld, void* _Item);
void* Remove_LList(LLIST* _Ld, QWORD _ID);
void* Remove_Front_LList(const LLIST* _Ld);
void* Remove_Back_LList(const LLIST* _Ld);

void* FindLList(const LLIST* _Ld, QWORD _ID);


#endif /*__LINKEDLIST_H__*/`