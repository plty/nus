/*************************************
 * Lab 1 Exercise 2
 * Name: Jerrell Ezralemuel
 * Matric No: A0181002B Lab Group: 08
 *************************************/

#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node* next;
};

node* insertAt(node*, int, int, int);
void printList(node*);
void destroyList(node*);



int main()
{
    // sentinelized first value
    node* myList = (node*) malloc(sizeof(node)); 
    myList->next = NULL;

    int pos, copies, value;
    while (scanf("%i%i%i", &pos, &value, &copies) != EOF) {
        insertAt(myList, pos, copies, value);
    }

    //Output code coded for you
    printf("My List:\n");
    printList(myList);

    destroyList(myList);

    printf("My List After Destroy:\n");
    printList(myList);

    return 0;
}

//Actual Function Implementations
node* insertAt(node* head, int position, int copies, int newValue)
{
    node *cur = head;
    for (int i = 0; i < position && cur->next; i++) {
        cur = cur->next;
    }

    for (int i = 0; i < copies; i++) {
        node* n = (node*) malloc(sizeof(node));
        n->data = newValue;
        n->next = cur->next;
        cur->next = n;
    }
    return head;
}

void printList(node* head) {
    node *cur = head;
    while (cur = cur->next, cur)  {
        printf("%i ", cur->data);
    } printf("\n");
}

void destroyList(node* head) {
    if (head->next == NULL) return;
    node *cur = head->next, *prev;
    while(prev = cur, cur = cur->next)
        free(prev);
    head->next = NULL;
}
