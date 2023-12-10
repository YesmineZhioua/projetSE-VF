#include "main.h"

void fifo(char *processesFile){
   struct node *processesList = getProcessesListFromFile(processesFile);
   printProcessTable(processesList);
   sortByTA(processesList); // Sort List comparing ascendante Based On TA
   addIdleNodes(processesList, "FIFO");
   printGanttChart(processesList, "FIFO");
}  

int main(int argc, char *argv[]) {
   fifo(argv[1]);
   return 0;
}
