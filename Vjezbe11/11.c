#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256
#define TABLE_SIZE 11

typedef struct cityTree* CityPosition;
typedef struct countryList* CountryPosition;

typedef struct cityTree {
    char name[MAX];
    int population;
    CityPosition left;
    CityPosition right;
} City;

typedef struct countryList {
    char name[MAX];
    CountryPosition next;
    CityPosition cityRoot;
} Country;

int hashFunction(char*);
CountryPosition createCountry(char*);
CityPosition createCity(char*, int);
void insertCountry(CountryPosition*, char*, char*);
CountryPosition insertCountrySorted(CountryPosition, CountryPosition);
CityPosition insertCity(CityPosition, char*, int);
void readFromFile(char*, CountryPosition*);
void printTable(CountryPosition*);
void printCities(CityPosition);
void searchCities(CityPosition, int);


int main() {
    CountryPosition hashTable[TABLE_SIZE] = { NULL };

    readFromFile("drzave.txt", hashTable);

    printTable(hashTable);

    char countryName[MAX];
    int minPop;

    printf("\nUnesite ime drzave: ");
    scanf("%s", countryName);
    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &minPop);

    int index = hashFunction(countryName);
    CountryPosition curr = hashTable[index];

    while (curr && strcmp(curr->name, countryName) != 0)
        curr = curr->next;

    if (!curr)
        printf("Drzava ne postoji.\n");
    else
        searchCities(curr->cityRoot, minPop);

    return 0;
}

int hashFunction(char* name) {
    int sum = 0;
    for (int i = 0; i < 5 && name[i] != '\0'; i++)
        sum += (int)name[i];
    return sum % TABLE_SIZE;
}

CountryPosition createCountry(char* name) {
    CountryPosition newCountry = (CountryPosition)malloc(sizeof(Country));
    if (!newCountry) return NULL;

    strcpy(newCountry->name, name);
    newCountry->next = NULL;
    newCountry->cityRoot = NULL;

    return newCountry;
}

CityPosition createCity(char* name, int pop) {
    CityPosition newCity = (CityPosition)malloc(sizeof(City));
    if (!newCity) return NULL;

    strcpy(newCity->name, name);
    newCity->population = pop;
    newCity->left = newCity->right = NULL;

    return newCity;
}

void insertCountry(CountryPosition* table, char* countryName, char* cityFile) {
    int index = hashFunction(countryName);

    CountryPosition newCountry = createCountry(countryName);
    table[index] = insertCountrySorted(table[index], newCountry);

    FILE* file = fopen(cityFile, "r");
    if (!file) return;

    char city[MAX];
    int pop;

    while (fscanf(file, "%s %d", city, &pop) == 2)
        newCountry->cityRoot = insertCity(newCountry->cityRoot, city, pop);

    fclose(file);
}

CountryPosition insertCountrySorted(CountryPosition head, CountryPosition newCountry) {
    if (!head || strcmp(newCountry->name, head->name) < 0) {
        newCountry->next = head;
        return newCountry;
    }

    CountryPosition curr = head;
    while (curr->next && strcmp(curr->next->name, newCountry->name) < 0)
        curr = curr->next;

    newCountry->next = curr->next;
    curr->next = newCountry;

    return head;
}

CityPosition insertCity(CityPosition root, char* name, int pop) {
    if (!root)
        return createCity(name, pop);

    if (pop > root->population ||
        (pop == root->population && strcmp(name, root->name) < 0))
        root->right = insertCity(root->right, name, pop);
    else
        root->left = insertCity(root->left, name, pop);

    return root;
}

void readFromFile(char* fileName, CountryPosition* table) {
    FILE* file = fopen(fileName, "r");
    if (!file) {
        printf("Ne mogu otvoriti %s\n", fileName);
        return;
    }

    char country[MAX], cityFile[MAX];

    while (fscanf(file, "%s %s", country, cityFile) == 2)
        insertCountry(table, country, cityFile);

    fclose(file);
}

void printTable(CountryPosition* table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("\n[%d]", i);
        CountryPosition curr = table[i];

        while (curr) {
            printf("\n  Drzava: %s", curr->name);
            printCities(curr->cityRoot);
            curr = curr->next;
        }
    }
}

void printCities(CityPosition root) {
    if (!root) return;

    printCities(root->right);
    printf("\n    %s (%d)", root->name, root->population);
    printCities(root->left);
}

void searchCities(CityPosition root, int minPop) {
    if (!root) return;

    searchCities(root->right, minPop);

    if (root->population >= minPop)
        printf("    %s (%d)\n", root->name, root->population);

    searchCities(root->left, minPop);
}
