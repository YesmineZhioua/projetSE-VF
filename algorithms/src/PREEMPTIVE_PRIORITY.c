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
    // Initialiser les variables à zéro
    float rotation_moy = 0;
    float attent_moy = 0;
    // Calculer la somme des temps de rotation et d'attente
    for(int i = 0; i < size; i++){
        rotation_moy += processesInfo[i].turnaroundTime;
        attent_moy += processesInfo[i].waitingTime;
    }
    // Diviser par le nombre de processus
    rotation_moy /= size;
    attent_moy /= size;
    
   // Example in .c
   printf("%s,%f,%f\n", "SRT", rotation_moy , attent_moy);
      // Open the file in append mode
   FILE *file = fopen("results.txt", "a");
   if (file == NULL) {
      perror("Error opening file");
      return;
   }

   // Print the results to the file
   fprintf(file, "%s,%f,%f\n", "PrioritéPree", rotation_moy , attent_moy);

   // Close the file
   fclose(file);

    //printf("|    | %f | %f |\n",rotation_moy, attent_moy); 
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
          // on ajoute le temps d’exécution au temps d’attente 
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
            int st = finish - atoi(tmp->data[2]); // temps de début = temps de fin - temps d’exécution
             int wt = finish - ta - atoi(tmp->data[2]); // temps d’attente = temps de fin - temps d’arrivée - temps d’exécution 
             int tt = finish - ta; // temps de rotation = temps de fin - temps d’arrivée 
             char *processName = tmp->data[0]; if(processExists(processesInfo, index, processName)){ 
               // si le processus existe déjà dans le tableau 
               updateProcessInfo(processesInfo, index, processName, wt, tt, st); 
               // on met à jour les temps d’attente et de rotation
                } else{ 
                  // sinon, on ajoute une nouvelle ligne
                   processesInfo[index].processName = processName;
                    processesInfo[index].turnaroundTime = tt;
                     processesInfo[index].waitingTime = wt; 
                     processesInfo[index].startTime = st; index++; 
                     } tmp = tmp->next; } return;
                      // on ajoute une instruction return vide 
}

// Une fonction qui affiche le tableau des informations des processus 
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
   bubbleSortByTwoIndexes(processesList, 1, 2, false); // Sort List by Ta & Te to get First process to run
   PriorityPreemptive(processesList);
   addIdleNodes(processesList,"preemptivePriority");
   printGanttChart(processesList, "Pre-emptive Priority");
  int size = countNodes(processesList); 
  struct ProcessInfo processesInfo[size-1]; // Un tableau qui stocke les informations des processus 
 calculateWaitingAndTurnaroundTimes(processesList, processesInfo); // Une fonction qui calcule les temps d’attente et de rotation pour chaque processus
 printProcessInfoTable(processesInfo, size-1); // Une fonction qui affiche le tableau des informations des processus
  calculateAverageRotationAndWaitingTimes(processesInfo, size-1);

}


int main(int argc, char *argv[]) {
   if(argc == 1)
      printf("Usage: %s <config>\n", argv[0]);
   else
      PreemptivePriority(argv[1]);
     
   return 0;
   // Open the file in write mode
   FILE *file = fopen("results.txt", "w");
   if (file == NULL) {
      perror("Error opening file");
      return 1;
   }
   // Print the header to the file
   fprintf(file, "Algorithm,rotation_moy,attent_moy\n");

   // Close the file
   fclose(file);
}
