#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX1 50
#define MAX2 100
#define MAX3 1000

typedef struct {
    char serialNum[MAX1];
    char model[MAX1];
    char maker[MAX1];
    char adap[MAX2];
    struct tm startDate;
    struct tm lastDate;
    char notes[MAX3];
} SORTER;

void die(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}
    
int main(void)
{
    int choice;
    system("cls");
    printf("Qu'est ce que vous voulez:\n");
    printf("	1. Ajouter une nouvelle machine\n");
    printf("	2. Rechercher une machine\n");
    printf("	0. Rien!\n");
    printf("Entrez votre choix: ");
    do
    {
        fflush(stdin);
        scanf("%d", &choice);
        if (choice < 0 || choice > 2)
            printf("ERREUR!\nEntrez un choix valide [0, 2]: ");
    } while (choice < 0 || choice > 2);

    SORTER sorter;
    char fName[MAX3];
    FILE *fp;
    switch (choice)
    {
        case 1: // adding
            printf("\nSaisir le numéro de série: ");
            fflush(stdin);
            fgets(sorter.serialNum, MAX1, stdin);
            sorter.serialNum[strcspn(sorter.serialNum, "\n")] = '\0';

            snprintf(fName, MAX3, "../sorters/%s.bin", sorter.serialNum);
            fp = fopen(fName, "wb");
            if (!fp)
                die("Imposible de créer un ficher!");

            printf("Saisir le modèle: ");
            fflush(stdin);
            fgets(sorter.model, MAX1, stdin);
            sorter.model[strcspn(sorter.model, "\n")] = '\0';

            printf("Saisir le nom du fabricant: ");
            fflush(stdin);
            fgets(sorter.maker, MAX1, stdin);
            sorter.maker[strcspn(sorter.maker, "\n")] = '\0';

            printf("Saisir le type d'adaptation: ");
            fflush(stdin);
            fgets(sorter.adap, MAX2, stdin);
            sorter.adap[strcspn(sorter.adap, "\n")] = '\0';

            sorter.startDate.tm_hour = sorter.startDate.tm_min = sorter.startDate.tm_sec = 0;
            sorter.startDate.tm_isdst = -1;
            printf("Saisir la date de mise en service (JJ/MM/AAAA): ");
            do
            {
                fflush(stdin);
                scanf("%d/%d/%d", &sorter.startDate.tm_mday, &sorter.startDate.tm_mon, &sorter.startDate.tm_year);
                sorter.startDate.tm_year -= 1900;
                sorter.startDate.tm_mon -= 1;
                if (-1 == mktime(&sorter.startDate))
                    printf("ERREUR!\nSaisir une date valide: ");
            } while (-1 == mktime(&sorter.startDate));

            sorter.lastDate.tm_hour = sorter.lastDate.tm_min = sorter.lastDate.tm_sec = 0;
            sorter.lastDate.tm_isdst = -1;
            printf("Saisir la dernière date de service (JJ/MM/AAAA): ");
            do
            {
                fflush(stdin);
                scanf("%d/%d/%d", &sorter.lastDate.tm_mday, &sorter.lastDate.tm_mon, &sorter.lastDate.tm_year);
                sorter.lastDate.tm_year -= 1900;
                sorter.lastDate.tm_mon -= 1;
                if (-1 == mktime(&sorter.lastDate))
                    printf("ERREUR!\nSaisir une date valide: ");
            } while (-1 == mktime(&sorter.lastDate));

            printf("Saisir vos remarques: ");
            fflush(stdin);
            fgets(sorter.notes, MAX3, stdin);
            sorter.notes[strcspn(sorter.notes, "\n")] = '\0';

            if (1 == fwrite(&sorter, sizeof sorter, 1, fp))
                printf("\nDonnées enregistrées\n");
            else
                die("\nERREUR: Données NON enregistrées!");

            fclose(fp);         
            break;

        case 2: // search
            char temp[MAX1];
            printf("\nSaisir le numéro de série: ");
            fflush(stdin);
            fgets(temp, MAX1, stdin);
            temp[strcspn(temp, "\n")] = '\0';

            snprintf(fName, MAX3, "../sorters/%s.bin", temp);
            fp = fopen(fName, "rb");
            if (!fp)
            {
                WIN32_FIND_DATAA findData;
                HANDLE hFind = FindFirstFileA("../sorters/*.bin", &findData);
                if (hFind == INVALID_HANDLE_VALUE)
                    die("\nIntrouvable. Aucune machine n'a encore été enregistrée!");

                fprintf(stderr, "\nLe numéro est INCORRECT!\n");
                printf("Voici les machines enregistrées:\n");
                do
                {
                    if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                        printf("- Machine N°%s\n", strtok(findData.cFileName, ".")); 
                } while (FindNextFileA(hFind, &findData));                
                die("\nVeuillez réessayer.");
            }

            if (1 != fread(&sorter, sizeof sorter, 1, fp) || strncmp(temp, sorter.serialNum, MAX1))
                die("Ce ficher est vide!");

            printf("\nNuméro de série: %s\n", sorter.serialNum);
            printf("Modèle: %s\n", sorter.model);
            printf("Fabricant: %s\n", sorter.maker);
            printf("Type d'adaptation: %s\n", sorter.adap);
            printf("Date de mise en service: %d/%d/%d\n", sorter.startDate.tm_mday, sorter.startDate.tm_mon + 1, sorter.startDate.tm_year + 1900);
            printf("Dernière date de service: %d/%d/%d\n", sorter.lastDate.tm_mday, sorter.lastDate.tm_mon + 1, sorter.lastDate.tm_year + 1900);
            printf("Remarques: \n%s\n", sorter.notes);
            
            fclose(fp);
            break;
        case 0:
            // nothing to do!
    }

    printf("\nFin!");
    int ch = getchar();
    return EXIT_SUCCESS;
 }