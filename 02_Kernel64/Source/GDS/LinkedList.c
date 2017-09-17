#include "LinkedList.h"
#include <Console/Console.h>
void InitializeLList(LLIST* _Ld)
{
    _Ld->Count = 0;
    _Ld->FirstNode = NULL;
    _Ld->LastNode  = NULL;   
    
}

void Push_Back_LList(LLIST* _Ld, void* _Item)
{   
    LLIST_NODE_HEADER* header = (LLIST_NODE_HEADER*)_Item;
    header->NextNode = NULL;
    if(_Ld->FirstNode == NULL)
    {
        _Ld->FirstNode = _Item;
        _Ld->LastNode  = _Item;
        _Ld->Count     = 1;
        return;
    }
    header = (LLIST_NODE_HEADER*)_Ld->LastNode;
    header->NextNode = _Item;
    _Ld->LastNode = _Item;
    _Ld->Count++;
}
void Push_Front_LList(LLIST* _Ld, void* _Item)
{
    LLIST_NODE_HEADER* header = (LLIST_NODE_HEADER*)_Item;
    header->NextNode = _Ld->FirstNode;
    if(_Ld->FirstNode == NULL)
    {
        _Ld->FirstNode  = _Item;
        _Ld->LastNode   = _Item;
        _Ld->Count      = 1;
        return;
    }
    _Ld->FirstNode = _Item;
    _Ld->Count++;
}
void* Remove_LList(LLIST* _Ld, QWORD _ID)
{
    LLIST_NODE_HEADER* pre_headaer = (LLIST_NODE_HEADER*)_Ld->FirstNode;

    for(LLIST_NODE_HEADER* iter_header = pre_headaer; iter_header != NULL; iter_header = iter_header->NextNode)
    {
        if(iter_header->ID == _ID)
        {
            if((iter_header == _Ld->FirstNode) && (iter_header == _Ld->LastNode) )
            {
                _Ld->FirstNode = NULL;
                _Ld->LastNode  = NULL;   
            }
            else if(iter_header == _Ld->FirstNode)
            {
                _Ld->FirstNode = iter_header->NextNode;
                
            }
            else if(iter_header == _Ld->LastNode)
            {
                _Ld ->LastNode = pre_headaer;

            } 
            else
            {
                pre_headaer->NextNode = iter_header->NextNode;
            }
            _Ld->Count--;
            return iter_header;
        }
        pre_headaer = iter_header;
    }
    
    return NULL;
}
void* Remove_Front_LList(const LLIST* _Ld)
{
    if(_Ld->Count == 0)
        return NULL;
    LLIST_NODE_HEADER* header = (LLIST_NODE_HEADER*)_Ld->FirstNode;
    return Remove_LList(_Ld, header->ID);
}
void* Remove_Back_LList(const LLIST* _Ld)
{
    if(_Ld->Count == 0)
        return NULL;
    LLIST_NODE_HEADER* header    = (LLIST_NODE_HEADER*)_Ld->LastNode;
    return Remove_LList(_Ld, header->ID);
}

void* FindLList(const LLIST* _Ld, QWORD _ID)
{
    for(LLIST_NODE_HEADER* iter = (LLIST_NODE_HEADER*) _Ld->FirstNode; iter!=NULL; iter = iter->NextNode)
    {
        if(iter->ID == _ID)
            return iter;
    }
    return NULL;
}