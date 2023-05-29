#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX0 12
#define MAX1 50
#define MAX2 100
#define MAX3 1000

typedef struct {
    char serialNum[MAX1];
    char model[MAX1];
    char maker[MAX1];
    char adap[MAX2];
    char startDate[MAX0];
    char lastDate[MAX0];
    char notes[MAX3];
} SORTER;

void die(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

void readLine(char *line, int max)
{
    fflush(stdin);
    fgets(line, max, stdin);
    line[strcspn(line, "\n")] = '\0';
}

void readDate(char *szDate, int max)
{
    struct tm date = {0};
    date.tm_isdst = -1;
    do
    {
        fflush(stdin);
        scanf("%d/%d/%d", &date.tm_mday, &date.tm_mon, &date.tm_year);
        date.tm_year -= 1900;
        date.tm_mon -= 1;
        if (-1 == mktime(&date))
            printf("ERREUR!\nSaisir une date valide: ");
    } while (-1 == mktime(&date));

    snprintf(szDate, max, "%d/%d/%d", date.tm_mday, date.tm_mon + 1, date.tm_year + 1900);
}

void readNote(char *notes, int max)
{
    char line[max];
    char *text = calloc(max, sizeof *text);
    do
    {
        fflush(stdin);
        fgets(line, max, stdin);
        if (line[0] != '\n')
            strncat(text, line, max);
    } while(line[0] != '\n');
    snprintf(notes, max, "%s", text);
}

void printInfo(void)
{
    printf(" _________________________________________________________________\n");
    printf("|                                                                 |\n");
    printf("|         Ce programme a été écrit par Oussama Med Teyib.         |\n");
    printf("|_________________________________________________________________|\n");
    printf("|                                                                 |\n");
    printf("| Le code source est disponible sur:                              |\n");
    printf("| https://github.com/OussamaTeyib/SortersDB                       |\n");
    printf("|_________________________________________________________________|\n");
    printf("\n");
}
    
int main(void)
{
    int choice;
    system("cls");
    printInfo();
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
    char fName[MAX_PATH];
    FILE *fp;
    switch (choice)
    {
        case 1: // adding
            printf("\n\nLe numéro de série: ");
            readLine(sorter.serialNum, MAX1);           
 
            snprintf(fName, MAX_PATH, "../sorters/%s.bin", sorter.serialNum);
            fp = fopen(fName, "wb");
            if (!fp)
                die("Imposible de créer un ficher!");

            printf("Saisir le modèle: ");
            readLine(sorter.model, MAX1); 

            printf("Saisir le nom du fabricant: ");
            readLine(sorter.maker, MAX1); 

            printf("Saisir le type d'adaptation: ");
            readLine(sorter.adap, MAX2); 

            printf("Saisir la date de mise en servic:!e (JJ/MM/AAAA): ");
            readDate(sorter.startDate, MAX0);            

            printf("Saisir la dernière date de service (JJ/MM/AAAA): ");
            readDate(sorter.lastDate, MAX0);  

            printf("Saisir vos remarques: ");
            readNote(sorter.notes, MAX3); 

            if (1 == fwrite(&sorter, sizeof sorter, 1, fp))
                printf("\nDonnées enregistrées\n");
            else
                die("\nERREUR: Données NON enregistrées!");

            fclose(fp);         
            break;

        case 2: // search
            char temp[MAX1];
            printf("\nSaisir le numéro de série: ");
            readLine(temp, MAX1); 

            snprintf(fName, MAX_PATH, "../sorters/%s.bin", temp);
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
            printf("Date de mise en service: %s\n", sorter.startDate);
            printf("Dernière date de service: %s\n", sorter.lastDate);
            printf("Remarques: \n %s\n", sorter.notes);
            
            fclose(fp);
            break;

        default:
            printf("hi\n"); // nothing to do!
    }
    int ch = getchar();
    return EXIT_SUCCESS;
 }