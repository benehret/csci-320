#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "linkedlist.h"

int main(int argc, char** argv){
    void* head = Create();

    Insert(head, 50, -1);
    Insert(head, 25, -1);
    Insert(head, 10, -1);
    Insert(head, 11, 2);
    Insert(head, 8, 1);
    Insert(head, 2, 0);

    Display(head); //List should read [2,50,8,25,11,10]
    Delete(head, 2);
    Display(head); //[2,8,25,11,10]
    
    uint32_t target = 25;
    void* a = Find(head, target);
    if (a){
        printf("%d is in the list\n",target);
    }
    else{
        printf("%d is not in the list. That is wrong!\n", target);
    }
    Destroy(head);

}