#include "main.h"  


void sjf(char configFile[]){
   struct node *processesList = getProcessesListFromFile(configFile);
   printProcessTable(processesList);
   bubbleSortByTwoIndexes(processesList, 1, 2, false); // Sort List by Ta & Te to get First process to run
   sortByTwoIndexes(processesList, 1, 2, false);
   addIdleNodes(processesList,"SJF");
   printGanttChart(processesList, "SJF");
}


int main(int argc, char *argv[]) {
   if(argc == 1)
      printf("Usage: %s <config>\n", argv[0]);
   else
      sjf(argv[1]);
   return 0;
}
