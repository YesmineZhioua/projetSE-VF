#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

const char ALGORITHMS_FOLDER[] = "./algorithms/build/";

struct algorithm
{
    int n;
    char name[500];
    char location[500];
};

int getNumberOfAlgorithms(DIR *algorithms)
{
    int nb = 0;
    struct dirent *dir; //une structure utilisée pour représenter les entrées de répertoire
//  a comme propriété l file name wel type mte3ou

    while ((dir = readdir(algorithms)) != NULL) //readdir pour lire le contenu d'une directory
    {
        if (dir->d_type != DT_DIR) //ken l type taa dir mahouch directory wa9tha ++nb_file
            nb++;
    }
    return nb;
}

struct algorithm *getListOfSchedulingAlgorithms(int numberOfAlgorithms)
{
    DIR *algorithms;
    struct dirent *dir;
    algorithms = opendir(ALGORITHMS_FOLDER); // pointer to files in src
    printf("Choose a scheduling algorithm : \n");
    struct algorithm *algorithmChoices;
    
    // réserver la taille du algorithmChoices qui est un tableau d'algorithm
    algorithmChoices = (struct algorithm *)malloc(numberOfAlgorithms * sizeof(struct algorithm));
    if (algorithms)
    {
        int i = 1;
        while ((dir = readdir(algorithms)) != NULL)
        {
            if (dir->d_type == DT_DIR)
                continue;
            struct algorithm algo;
            algo.n = i;
            strcpy(algo.name, dir->d_name); //copier une chaîne de caractères 
            char location[500] = "";
            strcat(location, ALGORITHMS_FOLDER); //concaténation ./algorithms/build/
            strcat(location, dir->d_name);
            strcpy(algo.location, location);
            algorithmChoices[i] = algo;
            printf("\t%d - %s\n", i, dir->d_name); // printing the menu
            i++;
        }
        closedir(algorithms);
    }
    return algorithmChoices;
}

int getUserChoice(int nbOfAlgorithms)
{
    int choice;
    while (1)
    {
        printf("Enter your choice : ");
        scanf("%d", &choice);
        if (choice > nbOfAlgorithms || choice < 1)
            printf("Choose a number from the menu!\n");
        else 
            break;
    }
    return choice;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Please enter processes file as an argument.\n");
        exit(0);
    }
    DIR *algorithmsFiles = opendir(ALGORITHMS_FOLDER); // pointer on ./build directory
    int nbOfAlgorithms = getNumberOfAlgorithms(algorithmsFiles);
    struct algorithm *algorithmChoices = getListOfSchedulingAlgorithms(nbOfAlgorithms);
    int choice = getUserChoice(nbOfAlgorithms);
    char command[500] = "";
    strcat(command, algorithmChoices[choice].location); //./algorithms/build/RR 
    char configFile[100] = " ";
    strcat(configFile, argv[1]); // processes.txt
    strcat(command, configFile); // ./algorithms/build/RR  processes.txt
    system(command); //execution
}