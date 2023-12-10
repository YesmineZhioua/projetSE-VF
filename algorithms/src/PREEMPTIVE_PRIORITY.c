#include "main.h"  


void insertInPosition(struct node *head, struct node *newNode, int finish){
   struct node *tmp = head->next;
   struct node *prev = head;
   if(head == NULL){
      head = newNode;
      return;
   }
   while(tmp &&
          atoi(newNode->data[3]) <= atoi(tmp->data[3]) &&
          atoi(tmp->data[1]) <= finish){
      prev = prev->next;
      tmp = tmp->next;
   }
   newNode->next = tmp;
   prev->next = newNode;
}

void calculateAverageRotationAndWaitingTimes(struct ProcessInfo *processesInfo, int size){
    float rotation_moy = 0;
    float attent_moy = 0;
    for(int i = 0; i < size; i++){
        rotation_moy += processesInfo[i].turnaroundTime;
        attent_moy += processesInfo[i].waitingTime;
    }
    rotation_moy /= size;
    attent_moy /= size;
   printf("Average Turnaround Time: %.2lf\n", rotation_moy);

   printf("Average Waiting Time: %.2lf\n", attent_moy);
   
   printf("%s,%f,%f\n", "PreemPriority", rotation_moy , attent_moy);
  
   FILE *file = fopen("results.txt", "a");
   if (file == NULL) {
      perror("Error opening file");
      return;
   }

  
   fprintf(file, "%s,%f,%f\n", "PrioritéPree", rotation_moy , attent_moy);

  
   fclose(file);

}

void PriorityPreemptive(struct node *head){
   struct node *tmp = head;
   int finish = atoi(tmp->data[1]);
     int currentTime = 0; 
   while(tmp){
      struct node *tmp2 = tmp->next;
      while(tmp2){
         int ta = atoi(tmp->data[1]);
         int idle = ta > finish ? ta - finish : 0;
         int diffta = atoi(tmp2->data[1]) - (finish+idle);
         diffta = diffta < 0 ? 0 : diffta;
         bool condition = atoi(tmp2->data[1]) <= (finish+idle+atoi(tmp->data[2])) &&  atoi(tmp2->data[3]) > atoi(tmp->data[3]);
         if(diffta == 0 && condition)
            swap(tmp, tmp2);
         else if(condition){
            if(atoi(tmp->data[2]) > diffta){
               struct node *remainderNode = newNode(tmp);
               sprintf(remainderNode->data[2], "%d", (atoi(tmp->data[2])-diffta));
               insertInPosition(tmp, remainderNode, finish+idle);
            }
            sprintf(tmp->data[2], "%d", diffta);
           
           
            swap(tmp->next, tmp2);
            break;
         }
         tmp2 = tmp2->next;
      }
      int ta = atoi(tmp->data[1]);
      int idle = ta > finish ? ta-finish : 0;
  
      
      finish += atoi(tmp->data[2]) + idle;
      currentTime += atoi(tmp->data[2]);  
      tmp = tmp->next;
   }
   
   
   
}
 bool processExists(struct ProcessInfo *processesInfo, int size, char *processName){ 
   for(int i = 0; i < size; i++){
       if(strcmp(processesInfo[i].processName, processName) == 0){ 
         return true; 
         }
      } return false; 
}
void updateProcessInfo(struct ProcessInfo *processesInfo, int size, char *processName, int wt, int tt, int st){ 
   for(int i = 0; i < size; i++){ 
      if(strcmp(processesInfo[i].processName, processName) == 0){ 
        processesInfo[i].waitingTime = wt + atoi(processesInfo[i].processName); 
          processesInfo[i].turnaroundTime = tt; processesInfo[i].startTime = st; break;
           }
            }
             }

 void calculateWaitingAndTurnaroundTimes(struct node *head, struct ProcessInfo *processesInfo){
    struct node *tmp = head;
     int finish = 0; 
     int index = 0;
      while(tmp){
          int ta = atoi(tmp->data[1]);
           int idle = ta > finish ? ta - finish : 0;
            finish += atoi(tmp->data[2]) + idle; 
            int st = finish - atoi(tmp->data[2]); 
             int wt = finish - ta - atoi(tmp->data[2]); 
             int tt = finish - ta; 
             char *processName = tmp->data[0]; if(processExists(processesInfo, index, processName)){ 
               updateProcessInfo(processesInfo, index, processName, wt, tt, st); 
                } else{ 
                   processesInfo[index].processName = processName;
                    processesInfo[index].turnaroundTime = tt;
                     processesInfo[index].waitingTime = wt; 
                     processesInfo[index].startTime = st; index++; 
                     } tmp = tmp->next; } return;
}

void printProcessInfoTable(struct ProcessInfo *processesInfo, int size){ 
   printf("Voici le tableau qui contient les informations des processus :\n\n");
    printf("| P | Temps d attente | Temps de rotation |\n"); 
    printf("|—|------------------|-------------------|\n"); 
    for(int i = 0; i < size; i++){ 
      printf("| %s | %d | %d |\n", processesInfo[i].processName, processesInfo[i].waitingTime, processesInfo[i].turnaroundTime); 
      } }

void PreemptivePriority(char configFile[]){
   struct node *processesList = getProcessesListFromFile(configFile);
   printProcessTable(processesList);
   bubbleSortByTwoIndexes(processesList, 1, 2, false); 
   PriorityPreemptive(processesList);
   addIdleNodes(processesList,"preemptivePriority");
   printGanttChart(processesList, "Pre-emptive Priority");
  int size = countNodes(processesList); 
  struct ProcessInfo processesInfo[size-1]; 
 calculateWaitingAndTurnaroundTimes(processesList, processesInfo); 
 printProcessInfoTable(processesInfo, size-1); 
  calculateAverageRotationAndWaitingTimes(processesInfo, size-1);

}


int main(int argc, char *argv[]) {
   if(argc == 1)
      printf("Usage: %s <config>\n", argv[0]);
   else
      PreemptivePriority(argv[1]);
     
   return 0;
   FILE *file = fopen("results.txt", "w");
   if (file == NULL) {
      perror("Error opening file");
      return 1;
   }

   fprintf(file, "Algorithm,rotation_moy,attent_moy\n");


   fclose(file);
}
