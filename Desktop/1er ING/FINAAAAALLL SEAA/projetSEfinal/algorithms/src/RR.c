#include "main.h"

struct node *sortByTaPreemptive(struct Queue *queue, int quantum)
{
   struct node *Res = NULL;
   int count = 0;
   struct node *current = queue->front;
   while (current != NULL)
   {
      count++;
      current = current->next;
   }
   int finish = atoi(queue->front->data[1]);
   int exec;
   int totalTurnaroundTime = 0;
   struct node *tmp;
   struct node *process = (struct node *)malloc(sizeof(struct node));
   printf("\n********************** Round Robin Turnaround ***********************\n\n");
   while (queue->front)
   { // tant que l queue mafraghch
      process->data[0] = strdup(queue->front->data[0]);
      process->data[1] = strdup(queue->front->data[1]);
      process->data[2] = strdup(queue->front->data[2]);
      process->data[3] = strdup(queue->front->data[3]);
      process->data[4] = strdup(queue->front->data[4]);

      deQueue(queue);

      struct node *Node = (struct node *)malloc(sizeof(struct node));

      if (atoi(process->data[2]) <= quantum)
      {
         Node = newNode(process);
         if (Res == NULL)
         {
            Res = Node;
            tmp = Res;
         }
         else
         {
            tmp->next = Node;
            tmp = tmp->next;
         }
         finish += atoi(process->data[2]);
         printf("Turnaround time for process %s: %d\n", process->data[0], finish - atoi(process->data[4]));
         totalTurnaroundTime += finish - atoi(process->data[4]);
      }
      else
      {
         Node = newNode(process);
         sprintf(Node->data[2], "%d", quantum);
         if (Res == NULL)
         {
            Res = Node;
            tmp = Res;
         }
         else
         {
            tmp->next = Node;
            tmp = tmp->next;
         }

         int ta = atoi(process->data[1]);
         int idle = finish < ta ? (ta - finish) : 0;
         exec = atoi(process->data[2]) - quantum;
         sprintf(process->data[2], "%d", exec);
         finish += quantum + idle;
         sprintf(process->data[1], "%d", finish);
         enQueue(queue, process);
      }
   }
   double averageTurnaroundTime = (double)totalTurnaroundTime / (count);
   
   return Res;
}

void RR(char configFile[], int q)
{
   struct node *processesList = getProcessesListFromFile(configFile);
   printProcessTable(processesList);
   sortByTA(processesList); // Asc Sort List Based On TA (which is on index 1 in DATA array)
   struct Queue *queue = createQueueFromLinkedList(processesList);
   struct node *tmp = sortByTaPreemptive(queue, q);
   addIdleNodes(tmp, "Round Robin");
   printGanttChart(tmp, "Round Robin");
}
int main(int argc, char *argv[])
{
   int Q;
   if (argc == 1)
      printf("Usage: %s <config>\n", argv[0]);
   else
   {
      do
      {
         printf("Enter a Quantum : ");
         scanf("%d", &Q);
         if (Q >= 1)
         {
            RR(argv[1], Q);
         }
         else
         {
            printf("A quantum must be greater than or equal to 1!\n");
         }
      } while (Q < 1);
   }
   
   return 0;
}