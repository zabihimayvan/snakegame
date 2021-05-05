#include <curses.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>



int headx;
int heady;
int dir;
int tempaddSize;
int addSize;
int isalive;
int trophyx;
int trophyy;
int trophynum;
int trophylive;



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

//Ryan
int qsize(struct Queue* queue)
{
    return (queue->size);
}

//Ryan
//checks both queues for entry that is same as head
void collision(struct Queue* queuex, struct Queue* queuey)
{
    //check for collision with self    
    for (int i = queuex->front; i <= queuex->rear-1; i++ ){
        if (queuex->array[i] == headx && queuey->array[i] == heady) {
            isalive = 0;                
        }
    }

    //check for collisions with border
    for (int i = 0; i <= LINES; i++) {
        if ((i == 0) || (i == LINES-1)) {
            for (int j = 0; j <= COLS; j++) {
                if (headx == j && heady == i) {
                    isalive = 0;
                }
            }
            
        }
    }
    for (int i = 0; i <= COLS; i++) {
        if ((i == 0) || (i == COLS-1)) {
            for (int j = 0; j <= LINES; j++){
                if (headx == i && heady == j) {
                    isalive = 0;
                }
            }
        }
    }
    
    //checks collison with trophy
    if (headx==trophyx && heady==trophyy){
        addSize = tempaddSize;
        trophylive = 0;

    }
    
}

//Ryan
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
//Ryan
void input() {
    
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

//Jesse(speed and speed interval functions)
//takes the query size of the snake as input, then assigns the speed through seconds
//so it can be used in usleep
int speed(int snakeSize)
{
    if(snakeSize < 10)
        return 333333;
    else if(snakeSize >= 10 && snakeSize < 20)
        return 200000;
    else if(snakeSize >= 20 && snakeSize < 30)
        return 100000;
    else if(snakeSize >= 30 && snakeSize < 40)
        return 50000;
    else
        return 25000;
}

//assigns the speed multiplier in order to get number of times
//the machine needs to "sleep" in order for one second to pass
//in other words, how many times it sleeps per second
int speedInterval(int speed)
{
    if(speed == 333333)
        return 3;
    else if(speed == 200000)
        return 5;
    else if(speed == 100000)
        return 10;
    else if(speed == 50000)
        return 20;
    else
        return 40;

}
//Erik 
//Trophy
// Get random value and store them in trophyy as the y value. 
// Get random value and store them in trophyx as the x value
// Get random value and store them in trophyynum as the Random number for the trophy
// Go to the random x and y point and place the random char in that location.
// 
void trophy(struct Queue* queuex, struct Queue* queuey){
    
    trophylive = 1;
    srand((unsigned)time(NULL));
    
    label1:
        trophyy = rand() % LINES-1;
        if (trophyy == 0)
            goto label1;
        
    label2:
        trophyx = rand() % COLS-1;
        if (trophyx == 0)
            goto label2;
    label3:      
        trophynum = rand() % 10;
        if (trophynum == 0)
            goto label1;  
    for(int i= 0; i<qsize(queuex); i++){
        if (trophyx == queuex->array[i] && trophyy == queuey->array[i]) {
            goto label1;
        }
    } 
    move(trophyy,trophyx);
 
    addch(trophynum + '0');
    tempaddSize = trophynum;

}



int main() {
    srand((unsigned)time(NULL));

    initscr();
    noecho();
    nodelay(stdscr, TRUE);    
    curs_set(0);

    int perimeter = (LINES*2)+(COLS*2);
    
    //creates borders
    borders();

    isalive = 1;
    heady = LINES/2;
    headx = COLS/2;
    addSize = 2;

    
    

    //creates queue for both x and y snake parts
    struct Queue* queuex = createQueue((perimeter/2) + 9);
    struct Queue* queuey = createQueue((perimeter/2) + 9);
    
    trophy(queuex,queuey);

    move(heady,headx);
    addch('@');
    enqueue(queuex, headx);
    enqueue(queuey, heady);
    
//Jesse
    int time;
    int counter = 0;
    int seconds = 0;
    int sleeps = 0;
    int random = rand() % 10 + 10; //picks random number between 10 and 20, since 1 and 9 was way too fast
    int total = 100;
    char totalA[8];
    char goal[8];
    //puts current score in the top left
    move(0,0);
    addstr("Score: ");
    sprintf(totalA, "%d", total);
    addstr(totalA);
    //puts goal score in the middle of the board
    move(0,COLS/2);
    addstr("Goal: ");
    sprintf(goal, "%d", perimeter/2 * 100); //dynamic borders call for dynamic goal score
    addstr(goal);
    dir = rand() % 4 + 1; //direction is randomized when the game has begun
    while (isalive == 1) {

        refresh();
        counter++; //keeps track of refreshes
        time = speed(qsize(queuex)); //takes the size of the snake using speed() function
        usleep(time); //speeds up the snake based on time variable
        sleeps = speedInterval(speed(qsize(queuex))); //initializes variable sleeps using speedInterval() function
        if(counter == sleeps) //if number of refreshes equals the interval then reset counter since once second has passed
        {
            counter = 0;
            seconds++;
        }
        if(seconds == random) //if the number of seconds equals the random variable then the trohpy is deleted and a new one is spawned
        {
            seconds = 0;
            move(trophyy, trophyx);
            addch(' ');
            trophy(queuex,queuey);
            random = rand() % 10 + 10;
        }
//Ryan
        input();
        //adds new point for snake segment into queues
        enqueue(queuex, headx);
        enqueue(queuey, heady);
        
        //to dequeue from snake parts and remove tail         
        if(addSize == 0){
            move(dequeue(queuey),dequeue(queuex));
            addch(' ');
        }
//Jesse(just the total for score creation)
        else { 
            addSize--;
            total = total + 100;
            sprintf(totalA, "%d", total);
            move(0,7);
            addstr(totalA);
        }
        //checks for all collisions
        collision(queuex,queuey);
        
        if (trophylive == 0)
        {
            trophy(queuex,queuey);
        }

        if (qsize(queuex) == perimeter/2) {
            break;
        } 
    }
    
    endwin();
    printf("Game over!\n");
    if (isalive==1){
        printf("You win!\n");
    }
    else{printf("You lose!\n");}
    return 0;
}