/*
Bennett Ehret
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <inttypes.h>
#include "linkedlist.h"

void* Create(){
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list != NULL) {
        list->head = (struct ListNode*)malloc(sizeof(struct ListNode));
        if (list->head != NULL) {
            list->head->prev = NULL;
            list->head->next = NULL;
            pthread_mutex_init(&(list->lock), NULL);
        } else {
            free(list);
            list = NULL;
        }
    }
    return list;
}

int Display(void* head) {
    LinkedList* list = (LinkedList*)head;
    struct ListNode* current = list->head->next;
    int length = 0;
    while (current != NULL) {
        printf("%u ", current->data);
        current = current->next;
        length++;
    }
    printf("\n");
    return length;
}

int Destroy(void* head) {
    LinkedList* list = (LinkedList*)head;
    struct ListNode* current = list->head;
    while (current != NULL) {
        struct ListNode* temp = current;
        current = current->next;
        free(temp);
    }
    pthread_mutex_destroy(&(list->lock));
    free(list);
    return 1;
}

int Insert(void* head, uint32_t value, uint32_t loc){
    LinkedList* list = (LinkedList*)head;
    struct ListNode* current = list->head;
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->data = value;
    uint32_t pos = 0;
    while(current->next != NULL && pos < loc){
        current = current->next;
        pos++;
    }
    newNode->next = current->next;
    if (current->next != NULL) {
        current->next->prev = newNode;
    }
    current->next = newNode;
    newNode->prev = current;
    return 1;
}

int Delete(void* head, uint32_t item) {
    LinkedList* list = (LinkedList*)head;
    struct ListNode* current = list->head->next;
    for (uint32_t i = 1; i < item && current != NULL; i++) {
        if (current->next == NULL) {
            current->prev->next = NULL;
            free(current);
            return 1;
        }
        current = current->next;
    }
    current->prev->next = current->next;
    if (current->next != NULL) {
        current->next->prev = current->prev;
    }
    free(current);

    return 1;
}


void* Find(void* head, uint32_t value) {
    LinkedList* list = (LinkedList*)head;
    struct ListNode* current = list->head->next;
    while (current != NULL) {
        if (current->data == value) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int SafeInsert(void* head, uint32_t value, uint32_t loc){
    LinkedList* list = (LinkedList*)head;
    struct ListNode* current = list->head;
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->data = value;
    uint32_t pos = 0;
    pthread_mutex_lock(&(list->lock));
    while(current->next != NULL && pos < loc){
        current = current->next;
        pos++;
    }
    newNode->next = current->next;
    if (current->next != NULL) {
        current->next->prev = newNode;
    }
    current->next = newNode;
    newNode->prev = current;
    pthread_mutex_unlock(&(list->lock));
    return 1;
}

int SafeDelete(void* head, uint32_t item) {
    LinkedList* list = (LinkedList*)head;
    struct ListNode* current = list->head->next;
    pthread_mutex_lock(&(list->lock));
    for (uint32_t i = 1; i < item && current != NULL; i++) {
        if (current->next == NULL) {
            current->prev->next = NULL;
            free(current);
            pthread_mutex_unlock(&(list->lock));
            return 1;
        }
        current = current->next;
    }
    current->prev->next = current->next;
    if (current->next != NULL) {
        current->next->prev = current->prev;
    }
    free(current);
    pthread_mutex_unlock(&(list->lock));
    return 1;
}

void* SafeFind(void* head, uint32_t value) {
    LinkedList* list = (LinkedList*)head;
    struct ListNode* current = list->head->next;
    pthread_mutex_lock(&(list->lock));
    while (current != NULL) {
        if (current->data == value) {
            return current;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&(list->lock));
    return NULL;
}