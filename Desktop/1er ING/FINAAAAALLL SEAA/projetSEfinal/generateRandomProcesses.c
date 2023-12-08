#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Function to get a valid value within a specified range
int getValidValue(const char *prompt, int minValue, int maxValue) {
    int value;
    do {
        printf("%s", prompt);
        scanf("%d", &value);
        if (value < minValue || value > maxValue) {
            printf("Valeur invalide. La valeur doit être entre %d et %d.\n", minValue, maxValue);
        }
    } while (value < minValue || value > maxValue);

    return value;
}

int main() {
    int maxr = 10; // Default randint 10

    int debutArrivee = getValidValue("Entrez le début de l'intervalle pour les valeurs de temps d'arrivée : ", 0, INT_MAX);
    int finArrivee = getValidValue("Entrez le fin de l'intervalle pour les valeurs de temps d'arrivée : ", debutArrivee + 1, INT_MAX);
    int debutExecution = getValidValue("Entrez le début de l'intervalle pour les valeurs de temps d'exécution : ", 1, INT_MAX);
    int finExecution = getValidValue("Entrez le fin de l'intervalle pour les valeurs de temps d'exécution : ", debutExecution, INT_MAX);
    int debutPriorite = getValidValue("Entrez le début de l'intervalle pour les valeurs de priorité : ", 1, INT_MAX);
    int finPriorite = getValidValue("Entrez le fin de l'intervalle pour les valeurs de priorité : ", debutPriorite, INT_MAX);

    int n = getValidValue("Entrez le nombre n de processus à générer : ", 0, INT_MAX);

    FILE *file = fopen("processes.txt", "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= n; ++i) {
        fprintf(file, "P%d:%d:%d:%d\n", i, rand() % (finArrivee - debutArrivee + 1) + debutArrivee,
               rand() % (finExecution - debutExecution + 1) + debutExecution,
               rand() % (finPriorite - debutPriorite + 1) + debutPriorite);
    }

    fclose(file);

    printf("Les valeurs ont été écrites dans le fichier processes.txt.\n");

    return 0;
}
