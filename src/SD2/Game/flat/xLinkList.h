#ifndef XLINKLIST_H
#define XLINKLIST_H

struct st_ENODE
{
    st_ENODE* next;
    st_ENODE* prev;
};

struct st_ELIST
{
    st_ENODE* head;
    st_ENODE* tail;
    st_ENODE* tailprev;
};

void NewList(st_ELIST* list);
void AddHead(st_ELIST* list, st_ENODE* node);
void AddTail(st_ELIST* list, st_ENODE* node);
st_ENODE* RemHead(st_ELIST* list);
void MoveList(st_ELIST* to, st_ELIST* from);

#endif
