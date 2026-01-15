#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 267

typedef struct cityTree* cityPosition;
typedef struct countryList* countryPosition;

typedef struct cityTree {
    char cityName[MAX];
    int population;
    cityPosition left;
    cityPosition right;
} city;

typedef struct countryList {
    char countryName[MAX];
    countryPosition countryNext;
    cityPosition cityRoot;
} country;


countryPosition createCountry(char*);
cityPosition createCity(char*, int);
countryPosition sortedCountryInsert(countryPosition, char*);
cityPosition sortedCityInsert(cityPosition, char*, int);
void readFromFile(char*, countryPosition);
void printCountries(countryPosition);
void printCities(cityPosition);
void searchCities(cityPosition, int);


int main() {
    country head = { "", NULL, NULL };

    readFromFile("drzave.txt", &head);
    printCountries(head.countryNext);

    char searchCountry[MAX];
    int minPop;

    printf("\nUnesite ime drzave: ");
    scanf(" %s", searchCountry);
    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &minPop);

    countryPosition curr = head.countryNext;
    while (curr && strcmp(curr->countryName, searchCountry) != 0)
        curr = curr->countryNext;

    if (!curr)
        printf("Drzava ne postoji.\n");
    else {
        printf("\nGradovi s vise od %d stanovnika:\n", minPop);
        searchCities(curr->cityRoot, minPop);
    }

    return 0;
}

countryPosition createCountry(char* name) {
    countryPosition newCountry = malloc(sizeof(country));
    if (!newCountry) {
        printf("Neuspjesna alokacija memorije (drzava).\n");
        return NULL;
    }

    strcpy(newCountry->countryName, name);
    newCountry->countryNext = NULL;
    newCountry->cityRoot = NULL;
    return newCountry;
}

cityPosition createCity(char* name, int pop) {
    cityPosition newCity = malloc(sizeof(city));
    if (!newCity) {
        printf("Neuspjesna alokacija memorije (grad).\n");
        return NULL;
    }

    strcpy(newCity->cityName, name);
    newCity->population = pop;
    newCity->left = newCity->right = NULL;
    return newCity;
}

countryPosition sortedCountryInsert(countryPosition head, char* name) {
    countryPosition newCountry = createCountry(name);
    if (!newCountry) return NULL;

    countryPosition prev = head;
    countryPosition curr = head->countryNext;

    while (curr && strcmp(curr->countryName, name) < 0) {
        prev = curr;
        curr = curr->countryNext;
    }

    newCountry->countryNext = curr;
    prev->countryNext = newCountry;

    return newCountry;
}

cityPosition sortedCityInsert(cityPosition root, char* name, int pop) {
    if (!root)
        return createCity(name, pop);

    if (pop < root->population)
        root->left = sortedCityInsert(root->left, name, pop);
    else if (pop > root->population)
        root->right = sortedCityInsert(root->right, name, pop);
    else {
        if (strcmp(name, root->cityName) < 0)
            root->left = sortedCityInsert(root->left, name, pop);
        else
            root->right = sortedCityInsert(root->right, name, pop);
    }

    return root;
}

void readFromFile(char* fileName, countryPosition head) {
    FILE* file = fopen(fileName, "r");
    if (!file) {
        printf("Ne mogu otvoriti %s\n", fileName);
        return;
    }

    char countryName[MAX];
    char countryFile[MAX];

    while (fscanf(file, "%s %s", countryName, countryFile) == 2) {
        countryPosition newCountry = sortedCountryInsert(head, countryName);

        FILE* cities = fopen(countryFile, "r");
        if (!cities) continue;

        char cityName[MAX];
        int pop;

        while (fscanf(cities, "%s %d", cityName, &pop) == 2)
            newCountry->cityRoot = sortedCityInsert(newCountry->cityRoot, cityName, pop);

        fclose(cities);
    }
    fclose(file);
}

void printCountries(countryPosition head) {
    while (head) {
        printf("\nDrzava: %s\n", head->countryName);
        printCities(head->cityRoot);
        head = head->countryNext;
    }
}

void printCities(cityPosition root) {
    if (!root) return;
    printCities(root->right);
    printf("  %s (%d)\n", root->cityName, root->population);
    printCities(root->left);
}

void searchCities(cityPosition root, int minPop) {
    if (!root) return;

    searchCities(root->right, minPop);
    if (root->population > minPop)
        printf("%s (%d)\n", root->cityName, root->population);
    searchCities(root->left, minPop);
}
