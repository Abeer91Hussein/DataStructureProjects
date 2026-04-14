/* Abeer Salah
1220425
sec.2*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

 struct QueueNode{
    int data;
    struct QueueNode*front;
    struct QueueNode*rear;
    struct QueueNode *next;
};
 struct StackNode {
    int data;
    struct StackNode *next;
 };
struct QueueNode* createQueue() {
struct QueueNode *q = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    q->front = NULL;
    q->rear = NULL;
    return q;
}
void enqueue(struct QueueNode*q, int value) {
    struct QueueNode*newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    newNode->data = value;
    newNode->next = NULL;
    if (q->rear) {
        q->rear->next = newNode;
    } else {
        q->front = newNode;
    }
    q->rear = newNode;
}
int dequeue(struct QueueNode *q) {
    if (q->front==NULL)
        return -1; // Queue is empty
    int value = q->front->data;
    struct QueueNode *temp = q->front;
    q->front = q->front->next;
    if (!q->front) {
        q->rear = NULL; // Queue is now empty
    }
    free(temp);
    return value;
}
int isQueueEmpty(struct QueueNode *q) {
    return q->front==NULL;
}
void printQ(struct QueueNode *Q) {
    printf("Printing queue contents:");
    if (isQueueEmpty(Q)) {
        printf("The queue is empty.\n");
        return;
    }
    struct QueueNode *q = Q->front;
    while (q != NULL) {
        printf("%d ", q->data);
        q = q->next;
    }
    printf("\n");
}
struct StackNode* createStack() {
    struct StackNode *s = (struct StackNode*)malloc(sizeof(struct StackNode));
    if (s == NULL) {
        printf("Out of memory!\n");
        return NULL;
    }
    s->next = NULL;
    return s;
}

int isStackEmpty(struct StackNode *s) {
    return s->next == NULL;
}

void push(struct StackNode *s, int teamID) {
   struct StackNode *temp = (struct StackNode*)malloc(sizeof(struct StackNode));
    if (temp == NULL) {
        printf("out of memory!\n");
        return;
    }
    temp->data = teamID;
    temp->next = s->next;
    s->next = temp;
}

int pop(struct StackNode*s){
int x=-1;
struct StackNode*temp;
if(isStackEmpty(s))
    printf("empty stack!\n");
else {
    temp=s->next;
    x=s->next->data;
    s->next=s->next->next;
}
return x;
}
void printStack(struct StackNode *top) {
    printf("Stack contents (top to bottom): ");
struct     StackNode *current = top->next;
    while (current) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}
struct QueueNode* copyQueue(struct QueueNode* original) {
    if (original == NULL || original->front == NULL) {
        return createQueue(); // Return an empty queue
    }

struct QueueNode* newQueue = createQueue();
    struct QueueNode* current = original->front;
    while (current != NULL) {
        enqueue(newQueue, current->data);
        current = current->next;
    }
    return newQueue;
}

int Runner(struct StackNode*sLoser, int stackSize){//return the real runner
    int player1,player2, winner, checker = stackSize, round = 1, tmp = 0;
    if(stackSize == 1){
        int ans = pop(sLoser);
        return ans;
    }
    while(!isStackEmpty(sLoser)){
      if(checker >= 2){
         player1 = pop(sLoser);
         player2 = pop(sLoser);
         if(round % 2 == 0){
           if(player1>player2){
            winner=player1;
           }
           else{
            winner=player2;
           }
        }
        else{
           if(player1<player2){
            winner=player1;
           }
           else{
            winner=player2;
           }
        }
        checker-=2;
        push(sLoser, winner);
        tmp++;
      }

      else{
        round++;
        if(checker == 1)
            checker = tmp + 1;
        else checker = tmp;
        if(checker == 1) break;
        tmp = 0;
      }
    }
    int ans = pop(sLoser);
    return ans;
}

struct StackNode* winner(struct QueueNode*q, int size,int champ, int* cnt){//return the losers stack
    struct StackNode*s=createStack();
    int player1,player2, winner, checker = size, round = 1, tmp = 0;
    while(!isQueueEmpty(q)){
         if(checker >= 2){
            player1 = dequeue(q);
            player2 = dequeue(q);
            if(round % 2 == 0){
               if(player1>player2){
                   winner=player1;
                   if(winner == champ){
                      push(s, player2);
                      *cnt+=1;
                   }
               }
               else{
                 winner=player2;
                 if(winner == champ){
                    push(s, player1);
                    *cnt+=1;
                 }
               }
              enqueue(q, winner);
              tmp++;
            }
            else{
                if(player1<player2){
                    winner=player1;
                    if(winner == champ){
                      push(s, player2);
                      *cnt+=1;
                    }
                }
                else{
                  winner=player2;
                  if(winner == champ){
                    push(s, player1);
                    *cnt+=1;
                  }
                }
               enqueue(q, winner);
               tmp++;
            }
            checker-=2;
        }
        else{
           round++;
           if(checker == 1)
              checker = tmp + 1;
           else checker = tmp;

           if(checker == 1) break;
           tmp = 0;
        }
    }
    return s;
}
int match(struct QueueNode* q,struct StackNode*s){//return the champion
    int round=1;
    int winner ,loser;
    while(q->front&&q->front->next){
    struct QueueNode*nextRound=createQueue();
     int last_team =-1;
while(!isQueueEmpty(q)){
  int t1=dequeue(q);
  if (isQueueEmpty(q)) {  // handle odd number of teams
                last_team = t1;
                break;
            }
   int t2= dequeue(q);
    if(round%2==0){//even round
        if(t1>t2){
            winner=t1;
            loser=t2;
        }
        else{
            winner=t2;
            loser=t1;
        }
    }
    else {//odd round
       if(t1<t2){
        winner=t1;
        loser=t2;
       }
       else{
        winner=t2;
        loser=t1;
       }
    }
      push(s,loser);
    enqueue(nextRound,winner);
}
        if (last_team != -1) {
            struct QueueNode *temp_queue = createQueue();
            enqueue(temp_queue, last_team);  // Enqueue the last team first
            while (!isQueueEmpty(nextRound)) {
                enqueue(temp_queue,dequeue(nextRound));
            }
            free(nextRound);
            nextRound = temp_queue;
        }
free(q);
q=nextRound;
round++;
}
int champ=dequeue(q);
return champ;
}

int main() {
    int n; // Number of teams
    while (1) {
         printf("Enter the team members number:\n");
         scanf("%d", &n);
        if (n == -9999) {
            break;
        }
        struct QueueNode *teams = createQueue();
        struct StackNode *losers = createStack();
        struct StackNode *s = createStack();
        struct StackNode*ss =createStack();
        for (int i = 0; i < n; i++) {
            int id;
            scanf("%d",&id);
            enqueue(teams,id);
        }
struct QueueNode*runningTeams=copyQueue(teams);
        int champion =match(teams, losers);
        printf("champion:%d\n",champion);
struct QueueNode *runner_up_teams = createQueue();
        int counter = 0;
        ss = winner(runningTeams, n, champion, &counter);
        int runner = Runner(ss, counter);
        printf("real runner:%d\n", runner);
    }
    return 0;
}
