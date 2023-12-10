#include "main.h"  
void insertInPosition(struct node *head, struct node *newNode, int finish){
   struct node *tmp = head->next;
   struct node *prev = head;
   if(head == NULL){
      head = newNode;
      return;
   }
   while(tmp &&  
          atoi(newNode->data[2]) >= atoi(tmp->data[2]) &&
          atoi(tmp->data[1]) <= finish){
      prev = prev->next;
      tmp = tmp->next;
   }
   newNode->next = tmp;
   prev->next = newNode;
}


void SRTFPreemptive(struct node *head){
   double totalTurnaroundTime = 0.0;
   double totalWaitingTime = 0.0;
   int nbrP = 0;
   struct node *tmp = head;
   int finish = atoi(tmp->data[1]); 
   while(tmp){  
      nbrP ++;
      struct node *tmp2 = tmp->next;
      while(tmp2){ 
         int ta = atoi(tmp->data[1]); 
         int idle = ta > finish ? ta-finish : 0;
         int diffta = atoi(tmp2->data[1]) - (finish+idle);
         diffta = diffta < 0 ? 0 : diffta;
         bool condition = atoi(tmp2->data[2]) < atoi(tmp->data[2])-diffta;
         if(diffta == 0 && condition)
            swap(tmp, tmp2);
         else if(condition){
            struct node *remainderNode = newNode(tmp);
            sprintf(remainderNode->data[2], "%d", (atoi(tmp->data[2])-diffta));
            sprintf(tmp->data[2], "%d", diffta);
            swap(tmp->next, tmp2);
            insertInPosition(tmp, remainderNode, finish+idle);
            break;
         }
         tmp2 = tmp2->next;
      }
      int ta = atoi(tmp->data[1]);
      int idle = ta > finish ? ta-finish : 0;
      finish += atoi(tmp->data[2]) + idle;

      totalTurnaroundTime += finish - atoi(tmp->data[1]);
      int waitingTime = finish - atoi(tmp->data[1]) - atoi(tmp->data[2]);
      totalWaitingTime += waitingTime;


      tmp = tmp->next;
   }

   double averageTurnaroundTime = totalTurnaroundTime /nbrP;

   double averageWaitingTime = totalWaitingTime / nbrP;
   printf("Average Turnaround Time: %.2lf\n", averageTurnaroundTime);

   printf("Average Waiting Time: %.2lf\n", averageWaitingTime);

   printf("%s,%f,%f\n", "SRT", averageTurnaroundTime, averageWaitingTime);
   FILE *file = fopen("results.txt", "a");
   if (file == NULL) {
      perror("Error opening file");
      return;
   }

   fprintf(file, "%s,%f,%f\n", "SRT", averageTurnaroundTime, averageWaitingTime);

   fclose(file);

}


void SRT(char configFile[]){
   struct node *processesList = getProcessesListFromFile(configFile);
   printProcessTable(processesList);
   bubbleSortByTwoIndexes(processesList, 1, 2, false); 
   SRTFPreemptive(processesList);
   addIdleNodes(processesList,"SRT");
   printGanttChart(processesList, "SRT");
   
   
}


int main(int argc, char *argv[]) {
  
   if(argc == 1)
      printf("Usage: %s <config>\n", argv[0]);
   else
      SRT(argv[1]);
   return 0;
   
   FILE *file = fopen("results.txt", "w");
   if (file == NULL) {
      perror("Error opening file");
      return 1;
   }

   fprintf(file, "Algorithm,Average Turnaround Time,Average Waiting Time\n");

   // Close thefile
   fclose(file);
}
