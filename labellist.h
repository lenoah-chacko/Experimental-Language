#include<stdlib.h>
typedef struct node
{
    char *index;
    int address;
    struct node* next;
} node;

struct node* makeNode(char* index, int address)
{
    node *temp;
    temp = (node*)malloc(sizeof(node));
    temp->index = strdup(index);
    temp->address = address;  
    temp->next=NULL;
    return temp;
}
struct node* pushNode(node* HEAD,char *index, int address)
{
    node *newNode=makeNode(index,address);
    if(HEAD==NULL)
    {
        HEAD=newNode;
    }
    else
        {
            node* temp=HEAD;
            while(temp->next!=NULL)
            {
                temp=temp->next;
            }
            temp->next=newNode;
        }
    return HEAD;
}

void traverse(node* HEAD)
{
    node *temp=HEAD;
    while(temp!=NULL)
    {
        printf("%s<->%d\n",temp->index,temp->address);
        temp=temp->next;
    }
}

int search(node *HEAD,char *index)
{
    node *temp=HEAD;
    strcat(index, ":");
    while(temp!=NULL)
    {
        //printf("[%s,%s]\n",temp->index,index);
        if(!strcmp(temp->index,index))
            return temp->address;
        temp=temp->next;
    }
    return -1;
}