//This is a basic node of the linked list. You will need to change this line to create a proper doubly-linked list. And please name it something else!
struct ListNode{
    uint32_t data;
    struct ListNode* prev;
    struct ListNode* next;
};

typedef struct LinkedList{
    struct ListNode* head;
    pthread_mutex_t lock;
} LinkedList;

//This function constructs and returns a pointer to the head of the linkedlist
//It should dynamically allocate a head and return a pointer to it
void* Create();

//These functions are not threadsafe
int Insert(void* head, uint32_t value, uint32_t loc); //Inserts an item of the value at the index loc. If loc is out-of-bounds, it should insert at the end of the list.
int Delete(void* head, uint32_t item); //Deletes the item at index loc. If loc is out-of-bounds, it should delete the last item in list.
void* Find(void* head, uint32_t value); //Finds whether a given item is in the list and returns a pointer to it!

//These functions should be threadsafe 
int SafeInsert(void* head, uint32_t value, uint32_t loc); //Inserts an item of the value at the index loc. If loc is out-of-bounds, it should insert at the end of the list.
int SafeDelete(void* head, uint32_t item); //Deletes the item at index loc. If loc is out-of-bounds, it should delete the last item in list.
void* SafeFind(void* head, uint32_t value); //Finds whether a given item is in the list and returns a pointer to it!

//These function do not need to be threadsafe
int Display(void* head); //This function should display a representation of the list and return the length of the list.
int Destroy(void* head); //This should free up the memory used by the list. Be sure to destroy the lock! (once you implement it).