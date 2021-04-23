#include <curses.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>



int headx;
int heady;
int dir = 1;
int addSize;


// A structure to represent a queue
struct Queue {
    int front, rear, size;
    unsigned capacity;
    int* array;
};


 
// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*)malloc(
        sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
 
    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(
        queue->capacity * sizeof(int));
    return queue;
}
 
// Queue is full when size becomes
// equal to the capacity
int isFull(struct Queue* queue)
{
    return (queue->size == queue->capacity);
}
 
// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}
 
// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}
 
// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}
 
// Function to get front of queue
int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}
 
// Function to get rear of queue
int rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

void borders(){

    //creates horizontal borders
    for (int i = 0; i <= LINES; i++) {
        if ((i == 0) || (i == LINES-1)) {
            for (int j = 0; j <= COLS; j++) {
                move(i,j);
                addch('*');
            }
            
        }
    }

    //creates vertical borders
    for (int i = 0; i <= COLS; i++) {
        if ((i == 0) || (i == COLS-1)) {
            for (int j = 0; j <= LINES; j++){
                move(j,i);
                addch('*');
            }
        }
    }
    refresh();
};
void input() {
    usleep(150000);
        
        
        
        switch (getch()) {
            case 'd':
                dir = 1;
                break;
            case 'w':
                dir = 2;
                break;
            case 'a':
                dir = 3;
                break;
            case 's':
                dir = 4;
                break;
        }
        
        if (dir == 1) {
            headx = headx + 1;
        }
        if (dir == 2) {
            heady = heady - 1;
        }
        if (dir == 3) {
            headx = headx - 1;
        }
        if (dir == 4) {
            heady = heady + 1;
        }
        
        move(heady,headx);
        addch('@');
        refresh();
}




int main() {

    

    initscr();
    noecho();
    nodelay(stdscr, TRUE);    
    curs_set(0); //doesn't seem to work


    int perimeter = (LINES*2)+(COLS*2);
    

    //creates borders
    borders();

    int isalive = 1;
    heady = LINES/2;
    headx = COLS/2;
    addSize = 4;

    //creates queue for both x and y snake parts
    struct Queue* queuex = createQueue(perimeter/2);
    struct Queue* queuey = createQueue(perimeter/2);
    

    move(heady,headx);
    addch('@');
    enqueue(queuex, headx);
    enqueue(queuey, heady);
    
    refresh();

  
      
    
    while (isalive == 1) {
         
        refresh();
        input();
        //adds new point for snake segment into queues
        enqueue(queuex, headx);
        enqueue(queuey, heady);
        
        //to dequeue from snake parts and remove tail         
        if(addSize == 0){
            move(dequeue(queuey),dequeue(queuex));
            addch(' ');
        }
        else {
            addSize--;
        }
        
    }
    
    endwin();

    return 0;
}
