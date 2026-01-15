#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 267

typedef struct cityList* cityPosition;
typedef struct countryTree* countryPosition;

typedef struct cityList {
    char cityName[MAX];
    int population;
    cityPosition next;
} city;

typedef struct countryTree {
    char countryName[MAX];
    countryPosition left;
    countryPosition right;
    cityPosition cityHead;
} country;

countryPosition createCountry(char*);
cityPosition createCity(char*, int);
countryPosition insertCountry(countryPosition, char*);
cityPosition insertCitySorted(cityPosition, char*, int);
void readFromFile(char*, countryPosition*);
void printCountries(countryPosition);
void printCities(cityPosition);
void searchCities(countryPosition, char*, int);

int main() {
    countryPosition root = NULL;

    readFromFile("drzave.txt", &root);
    printCountries(root);

    char countryName[MAX];
    int minPop;

    printf("\nUnesite ime drzave: ");
    scanf(" %s", countryName);
    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &minPop);

    searchCities(root, countryName, minPop);

    return 0;
}

countryPosition createCountry(char* name) {
    countryPosition newCountry = malloc(sizeof(country));
    if (!newCountry) return NULL;

    strcpy(newCountry->countryName, name);
    newCountry->left = newCountry->right = NULL;

    /* dummy head za listu gradova */
    newCountry->cityHead = malloc(sizeof(city));
    newCountry->cityHead->next = NULL;

    return newCountry;
}

cityPosition createCity(char* name, int pop) {
    cityPosition newCity = malloc(sizeof(city));
    if (!newCity) return NULL;

    strcpy(newCity->cityName, name);
    newCity->population = pop;
    newCity->next = NULL;

    return newCity;
}

countryPosition insertCountry(countryPosition root, char* name) {
    if (!root)
        return createCountry(name);

    if (strcmp(name, root->countryName) < 0)
        root->left = insertCountry(root->left, name);
    else if (strcmp(name, root->countryName) > 0)
        root->right = insertCountry(root->right, name);

    return root;
}

cityPosition insertCitySorted(cityPosition head, char* name, int pop) {
    cityPosition newCity = createCity(name, pop);
    cityPosition prev = head;
    cityPosition curr = head->next;

    while (curr) {
        if (curr->population < pop)
            break;

        if (curr->population == pop && strcmp(curr->cityName, name) > 0)
            break;

        prev = curr;
        curr = curr->next;
    }


    newCity->next = curr;
    prev->next = newCity;
    return head;
}

void readFromFile(char* fileName, countryPosition* root) {
    FILE* file = fopen(fileName, "r");
    if (!file) return;

    char countryName[MAX], cityFile[MAX];

    while (fscanf(file, "%s %s", countryName, cityFile) == 2) {
        *root = insertCountry(*root, countryName);

        countryPosition curr = *root;
        while (strcmp(curr->countryName, countryName) != 0) {
            curr = strcmp(countryName, curr->countryName) < 0
                ? curr->left : curr->right;
        }

        FILE* cities = fopen(cityFile, "r");
        if (!cities) continue;

        char cityName[MAX];
        int pop;

        while (fscanf(cities, "%s %d", cityName, &pop) == 2)
            insertCitySorted(curr->cityHead, cityName, pop);

        fclose(cities);
    }
    fclose(file);
}

//ispis stabla drzava
void printCountries(countryPosition root) {
    if (!root) return;

    printCountries(root->left);
    printf("\nDrzava: %s\n", root->countryName);
    printCities(root->cityHead->next);
    printCountries(root->right);
}

//ispis liste gradova
void printCities(cityPosition head) {
    while (head) {
        printf("  %s (%d)\n", head->cityName, head->population);
        head = head->next;
    }
}

void searchCities(countryPosition root, char* countryName, int minPop) {
    if (!root) {
        printf("Drzava ne postoji.\n");
        return;
    }

    if (strcmp(countryName, root->countryName) < 0)
        searchCities(root->left, countryName, minPop);
    else if (strcmp(countryName, root->countryName) > 0)
        searchCities(root->right, countryName, minPop);
    else {
        cityPosition curr = root->cityHead->next;
        printf("\nGradovi s vise od %d stanovnika:\n", minPop);
        while (curr) {
            if (curr->population > minPop)
                printf("%s (%d)\n", curr->cityName, curr->population);
            curr = curr->next;
        }
    }
}
