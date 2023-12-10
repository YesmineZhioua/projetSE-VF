#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>  
#include <sys/ioctl.h>
#include <unistd.h>

struct node {
   char *data[5]; // [Process_Name, TA, TE, Priority, initValTA]
   struct node *next;
};
 
// The queue, front stores the front node of LL and end stores the last node of LL
struct Queue {
   struct node *front, *end;
};
 
struct ProcessInfo {
  char *processName;
    int turnaroundTime;
    int waitingTime;
    int startTime;
    int endTime; 
     float rotation_moy;
    float attent_moy;
};

typedef struct {
    char algorithmName[50];
    double avgTurnaround;
    double avgWaiting;
} AlgorithmResult;


/* Function to bubble sort the given linked list based on an index to compare with */
void sortByTA(struct node *start);
  
/* Function to swap data of two nodes a and b */
void swap(struct node *a, struct node *b);

/* Function to reverse a linked list recursively */
struct node* inverseLinkedList(struct node *node);

/* Function that reads processes from config file and returns linked list of processes */
struct node *getProcessesListFromFile(char *configFile);

/* Function to remove white spaces from string */
char *remove_white_spaces(char *str);

/* Function to sort by TA and TE or Priority */
void bubbleSortByTwoIndexes(struct node *start, int comparisonIndex1, int comparisonIndex2, bool Desc);

/* Function to sort by TE or Priority */
void sortByTwoIndexes(struct node *head, int comparisonIndex1, int comparisonIndex2, bool Desc);

/* Function that returns minimum of a & b */
int min(int a, int b);

/* Function to print Gantt chart */
void printGanttChart(struct node *head, char *algorithmName);

/* Function to print Table of processes */
void printProcessTable(struct node *head);

/* Function to add Idle nodes to print them in gantt diagram */
void addIdleNodes(struct node *head, char *algorithmName);

/* Function to sort process by TA preemptive */
struct node *sortByTaPreemptive(struct Queue *queue, int quantum);

/* Function to create an empty Queue */
struct Queue* createQueue();

/* The function to add an element to queue q */
void enQueue(struct Queue* q, struct node *element);

/* Function to remove element from given queue q */
void deQueue(struct Queue* q);

/* Function to create Queue from LinkedList */
struct Queue *createQueueFromLinkedList(struct node *head);

/* Function to create a new linked list node */
struct node* newNode(struct node *dataNode);


void printProcessTable(struct node *head){ 
   printf("\n******************** Processes Table *********************\n\n");
   struct node *tmp = head;
   puts(" +---------+---------------+----------------+-----------+");
   puts(" |   PID   | Arrival Time  | Execution Time | Priority  |");
   puts(" +---------+---------------+----------------+-----------+");

   while(tmp){
      printf(" |  %4s   |   %4s        |    %4s        | %4s      |\n"
            , tmp->data[0], tmp->data[1], tmp->data[2], tmp->data[3] );
      puts(" +---------+---------------+----------------+-----------+");
      tmp = tmp->next;
   }
 
}

void printGanttChartFile(struct node *head, char *algorithmName, const char *filename) {
     FILE *fp = fopen("./grantt.txt", "w"); 
   if(fp == NULL){
      printf("Error opening file %s\n", "grantt.txt");
      return;
   }
    printf("file open\n");
 struct node *tmp = head;
int finish = 0; 
int prev_end = 0; 
while (tmp) {
    int end = prev_end + atoi(tmp->data[2]); 
    prev_end = end; 
    finish = end; 
    tmp = tmp->next;
}

tmp = head;
prev_end = 0; 
while (tmp) {
    
    fputs(tmp->data[0], fp); 
    fputc('\t', fp);

    int start = atoi(tmp->data[1]); 
    int end = prev_end + atoi(tmp->data[2]); 
    for (int i = 0; i < finish; i++) { // start from 0 instead of 1
        if (i == 0  && start !=i ){
            fputc('P', fp); 
            i++;
            }
        else if (i == prev_end)
            fputc('E', fp); // the process starts running at the finish time of the previous process
        else if (i < start )
            fputc('P', fp); // the process is ready
        else if (i >= start && i < prev_end)
            fputc('A', fp); // the process is waiting
        else if (i >= prev_end && i < end)
            fputc('E', fp); // the process is running
        else
            fputc('T', fp); // the process is finished
        fputc('\t', fp); // write a tab character
    }


    prev_end = end;
    fputc('\n', fp); 
        tmp = tmp->next;
}

fclose(fp);

 }

void printGanttChart(struct node *head, char *algorithmName){
   printf("\n******************** %s Gantt Chart ***********************\n\n", algorithmName);
   struct node *tmp1, *tmp2, *tmp3, *tmp4;
   tmp1 = tmp2 = tmp3 = tmp4 = head;
   struct winsize terminalSize;
   ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminalSize);
   int finish; // used to count nb of col in Gantt diagram
   int finish2 = atoi(head->data[1]); // to print under each prcoess
   while(tmp1){
      printf(" +");
      finish = 1; // reset finish 
      while(tmp1){
         if(finish > 1 && finish + atoi(tmp1->data[2])*2 + 4 > terminalSize.ws_col)
            break;
         for(int j=0; j <= min(atoi(tmp1->data[2]), (terminalSize.ws_col/2) - 5); ++j)
            printf("--");
         printf("+");
         finish += atoi(tmp1->data[2])*2 + 4;
         tmp1 = tmp1->next;
      }
      printf("\n |");
      finish = 1;
      // printing process id in the middle
      while(tmp2) {
         if(finish > 1 && finish + atoi(tmp2->data[2])*2 + 4 >= terminalSize.ws_col)
            break;
         for(int j=0; j < min(atoi(tmp2->data[2]) - 1, (terminalSize.ws_col / 2) - 5); ++j) printf(" ");
         printf("%3s", tmp2->data[0]);
         for(int j=0; j < min(atoi(tmp2->data[2]), (terminalSize.ws_col / 2) - 6); ++j) printf(" ");
         printf("|");
         finish += atoi(tmp2->data[2])*2 + 4;
         tmp2 = tmp2->next;
      }
      printf("\n +");
      finish = 1;
      // printing bottom bar
      while(tmp3){
         if(finish > 1 && finish + atoi(tmp3->data[2])*2 + 4 > terminalSize.ws_col)
            break;
         for(int j=0; j <= min(atoi(tmp3->data[2]), (terminalSize.ws_col/2) - 5); ++j) printf("--");
         printf("+");
         finish += atoi(tmp3->data[2])*2 + 4;
         tmp3 = tmp3->next;
      }
      printf("\n ");
      // printing the time line
      printf("%d", finish2);
      finish = 1;
      while(tmp4){
         if(finish > 1 && finish + atoi(tmp4->data[2])*2 + 4 > terminalSize.ws_col)
            break;
         for(int j=0; j <= min(atoi(tmp4->data[2]), (terminalSize.ws_col/2) - 5); j++) printf("  ");
         finish2 += atoi(tmp4->data[2]);
         if(finish2 > 9){
            int t = (finish2 + 1) / 10;
            int i = 0;
            do{ // backsapces for the number of digits of finish
               t /= 10;
               printf("\b"); // backspace : remove 1 space
               ++i;
            }while(t != 0);
         }
         printf("%d", finish2);
         finish += atoi(tmp4->data[2])*2 + 4;
         tmp4 = tmp4->next;
      }
      printf("\n\n");
   }
   printf("***********************************************************\n\n");
}

int min(int a, int b){
   return (a > b) ? b : a;
}


void addIdleNodes(struct node *head, char *algorithmName){
   struct node *tmp = head->next, *prev = head;
   int finish = atoi(head->data[1]) + atoi(head->data[2]); // TA + TE
   int count=0;
   int totalTurnaroundTime = 0;
   while(tmp){
      count++;
      int ta = atoi(tmp->data[1]); 
      int te = atoi(tmp->data[2]); 
      int idle = (ta > finish) ? (ta - finish) : 0;
      if(idle > 0){
         struct node *newNode = (struct node*)malloc(sizeof(struct node));
         for(int i=0; i < 4; i++)
            newNode->data[i] = (char *)malloc(sizeof(char) * 255);
         newNode->data[0] = "-";
         sprintf(newNode->data[1], "%d", finish);
         sprintf(newNode->data[2], "%d", idle);
         newNode->data[3] = "0";
         newNode->next = prev->next;
         prev->next = newNode;
         prev = prev->next;
      }
      if(count==1 && (strcmp(algorithmName, "FIFO") == 0  || strcmp(algorithmName, "PRIORITY") == 0 || strcmp(algorithmName, "SJF") == 0)){
         printf("\n********************** %s Turnaround ***********************\n\n", algorithmName);
         printf("Turnaround time for process %s: %d\n", prev->data[0], finish-atoi(prev->data[1]));
         totalTurnaroundTime+= finish-atoi(prev->data[1]);
      }
      finish += te + idle; 
      if (strcmp(algorithmName, "FIFO") == 0  || strcmp(algorithmName, "PRIORITY") == 0 || strcmp(algorithmName, "SJF") == 0){
         printf("Turnaround time for process %s: %d\n", tmp->data[0], finish-atoi(tmp->data[1]));
         totalTurnaroundTime+= finish-atoi(tmp->data[1]);
      }
      tmp = tmp->next;
      prev = prev->next;
   }
   if (strcmp(algorithmName, "FIFO") == 0  || strcmp(algorithmName, "PRIORITY") == 0 || strcmp(algorithmName, "SJF") == 0){
   double averageTurnaroundTime = (double)totalTurnaroundTime / (count+1);
   // printf("Count: %d\n", count);
   printf("Average Turnaround Time: %.2lf\n", averageTurnaroundTime);
   }
}

void sortByTA(struct node *start){
   int swapped;
   struct node *ptr1;
   struct node *lptr = NULL; //last pointer
   /* Checking for empty list */
   if (start == NULL)
      return;
   do{
      swapped = 0;
      ptr1 = start;
      while (ptr1->next != lptr){
         int a = atoi(ptr1->data[1]); //convertir en entier.
         int b = atoi(ptr1->next->data[1]);
         // comparing ascendante
         if (a > b){ 
               swap(ptr1, ptr1->next);
               swapped = 1;
         }
         ptr1 = ptr1->next;
      }
      lptr = ptr1;
   }while (swapped);
}
  
  
/* function to swap data of two nodes a and b*/
void swap(struct node *a, struct node *b){
   for(int i=0; i < 4; i++){
      char *temp = strdup(a->data[i]);
      a->data[i] = strdup(b->data[i]);
      b->data[i] = strdup(temp);
   }
}


struct node* inverseLinkedList(struct node *node){
   //condition d'arret
   if(node->next == NULL)
      return node;
   struct node *node1 = inverseLinkedList(node->next);
   node->next->next = node;
   node->next = NULL;
   return node1;
}

char *remove_white_spaces(char *str){
   char *tmp = strdup(str); //creates a copy of the string pointed to, and returns a pointer to the new copy
	int i = 0, j = 0;
	while (tmp[i]){
		if (tmp[i] != ' ')
          tmp[j++] = tmp[i];
		i++;
	}
	tmp[j] = '\0'; // null terminator
   //in C, strings are represented as arrays of characters, and the end of a string is marked by a null terminator
	return tmp;
}


struct node *getProcessesListFromFile(char *configFile){
   FILE *openedFile;
   openedFile = fopen(configFile, "r");
   if(openedFile == NULL) {
      printf("Unable to open file!");
      exit(1);   
   }
   char * line = NULL;
   size_t len = 0;
   ssize_t read;
   struct node *processesLinkedList = NULL;
   while ((read = getline(&line, &len, openedFile)) != -1) {
      if(remove_white_spaces(line)[0] == '#'){
         printf("Commentaire : %s\n", line);
         continue;
      }
      else if(remove_white_spaces(line)[0] == '\n')
         continue;
      struct node *newNode = (struct node*)malloc(sizeof(struct node));
      char *token = strtok(line, ":"); //splitting string into smaller tokenS(liste chainé kol data thezek lelli mbaadha) based on ':'
      int i=0;
      while(token != NULL && i<4){
         token[strcspn(token, "\n")] = 0; // deleting \n from the end of the token
         newNode->data[i] = strdup(token); //duplicating the data and storing it
         token = strtok(NULL, ":"); //advances the token pointer to the next token in the string.
         i++;
      }
      newNode->data[4] = strdup(newNode->data[1]);
      newNode->next = processesLinkedList; //kenek fehem l linked list taw tefhemha ;)
      processesLinkedList = newNode;
   }
   fclose(openedFile);
   if(processesLinkedList == NULL){
      printf("Processes file is empty!\n");
      exit(1);
   }

   //la liste chainé ajoute chaque process dans la liste au début 
   //==> premier process est placé le dernier dans la liste 
   //==> il faut l'inverser 
   processesLinkedList = inverseLinkedList(processesLinkedList); 
   return processesLinkedList;
}


void bubbleSortByTwoIndexes(struct node *start, int comparisonIndex1, int comparisonIndex2, bool Desc){
   int swapped;
   struct node *ptr1;
   struct node *lptr = NULL;
   /* Checking for empty list */
   if (start == NULL)
      return;
   do{
      swapped = 0;
      ptr1 = start;
      while (ptr1->next != lptr){
         int i1 = atoi(ptr1->data[comparisonIndex1]);
         int ni1 = atoi(ptr1->next->data[comparisonIndex1]);
         int i2 = atoi(ptr1->data[comparisonIndex2]);
         int ni2 = atoi(ptr1->next->data[comparisonIndex2]);
         bool condition = Desc ? i2 < ni2 : i2 > ni2;
         if (i1 > ni1 || (i1 == ni1 && condition)){ 
               swap(ptr1, ptr1->next);
               swapped = 1;
         }
         ptr1 = ptr1->next;
      }
      lptr = ptr1;
   }while (swapped);
}


void sortByTwoIndexes(struct node *head, int comparisonIndex1, int comparisonIndex2, bool Desc){
   struct node *lptr = NULL;
   bool swapped;
   do{
      swapped = false;
      int finish = atoi(head->data[1]) + atoi(head->data[2]);
      struct node *ptr1 = head->next;
      while(ptr1 && ptr1->next != lptr){
         int ni1 = atoi(ptr1->next->data[comparisonIndex1]);
         int i2 = atoi(ptr1->data[comparisonIndex2]);
         int ni2 = atoi(ptr1->next->data[comparisonIndex2]);
         bool condition = Desc ? i2 < ni2 : i2 > ni2;
         if(ni1 <= finish && condition){
            swap(ptr1, ptr1->next);
            swapped = true;
         }
         int ta = atoi(ptr1->data[1]);
         int te = atoi(ptr1->data[2]);
         int idle = (finish < ta) ? (ta - finish) : 0;
         finish += te + idle;
         ptr1 = ptr1->next;
      }
      lptr = ptr1;
   }while(swapped);
}


struct Queue *createQueueFromLinkedList(struct node *head){
   struct node *tmp = head;
   struct Queue *queue = createQueue();
   while(tmp){
      enQueue(queue, tmp);
      tmp = tmp->next;
   }
   return queue;
}


// A utility function to create a new linked list node.
struct node* newNode(struct node *dataNode){
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->data[0] = strdup(dataNode->data[0]);
    temp->data[1] = strdup(dataNode->data[1]);
    temp->data[2] = strdup(dataNode->data[2]);
    temp->data[3] = strdup(dataNode->data[3]);
    temp->data[4] = strdup(dataNode->data[4]);
    temp->next = NULL;
    return temp;
}

// A utility function to create an empty queue
struct Queue *createQueue(){
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->end = NULL;
    return q;
}


// The function to add a key k to q
void enQueue(struct Queue *q, struct node *element){
   // Create a new LL node
   struct node *temp = newNode(element);
   // If queue is empty, then new node is front and end both
   if (q->end == NULL) {
      q->front = q->end = temp;
      return;
   }
   if(atoi(q->front->data[1]) > atoi(temp->data[1])){
      temp->next = q->front;
      q->front = temp;
      return;
   }
   if(atoi(q->end->data[1]) <= atoi(temp->data[1])){
      q->end->next = temp;
      q->end = temp;
      return;
   }
   struct node *tmp = q->front->next, *prev = q->front;
   while(tmp && (atoi(tmp->data[1]) <= atoi(temp->data[1])) ){
      tmp = tmp->next;
      prev = prev->next;
   }
   temp->next= prev->next;
   prev->next = temp;
}
 
// Function to remove element from given queue q
void deQueue(struct Queue* q){
   // If queue is empty, return NULL.
   if (q->front == NULL)
      return;
   // Store previous front and move front one node ahead
   struct node* temp = q->front;
   //struct node *ret = q->front;
   q->front = q->front->next;

   // If front becomes NULL, then change end also as NULL
   if (q->front == NULL)
      q->end = NULL;
   free(temp);
}


//Function to create a new node with process information
struct ProcessInfo* newProcessInfo(char *processName, int turnaroundTime, int waitingTime , int startTime, int endTime, float rotation_moy,
    float attent_moy) {
    struct ProcessInfo* info = (struct ProcessInfo*)malloc(sizeof(struct ProcessInfo));
    info->processName = processName;
    info->turnaroundTime = turnaroundTime;
    info->waitingTime = waitingTime;
    info->startTime = startTime; 
    info->endTime = endTime; 
    info->rotation_moy=0; 
    info->attent_moy=0; 
    return info;
}


 int countNodes(struct node *head){ 
   int count = 0; struct node *tmp = head; 
   while(tmp){ count++; tmp = tmp->next; } return count; 
 
 }