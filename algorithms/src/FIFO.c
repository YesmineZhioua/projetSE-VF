#include "main.h"

void fifo(char *processesFile){
   struct node *processesList = getProcessesListFromFile(processesFile);
   printProcessTable(processesList);
   sortByTA(processesList); 
   addIdleNodes(processesList, "FIFO");
   printGanttChart(processesList, "FIFO");
   printGanttChartFile(processesList, "FIFO" ,"./grantt.txt");

}  

int main(int argc, char *argv[]) {
   fifo(argv[1]);
   return 0;
}
