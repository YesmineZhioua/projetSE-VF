#include "main.h"

void priority(char configFile[]){
   struct node *processesList = getProcessesListFromFile(configFile);
   printProcessTable(processesList);
   bubbleSortByTwoIndexes(processesList, 1, 3, true); // Sort List by Ta & Priority to get First process to run
   sortByTwoIndexes(processesList, 1, 3, true);
   addIdleNodes(processesList, "PRIORITY");
   printGanttChart(processesList, "PRIORITY");
}  

int main(int argc, char *argv[]) {
   if(argc == 1)
      printf("Usage: %s <config>\n", argv[0]);
   else{
      priority(argv[1]);
   }
   return 0;
}