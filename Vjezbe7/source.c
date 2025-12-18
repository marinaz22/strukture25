#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 127

// struktura direktorija
typedef struct directory* dirPosition;
typedef struct directory {
    char name[MAX];
    dirPosition next;       // sljedeæi direktorij u istoj razini
    dirPosition subDirPos;  // prvi poddirektorij
} directory;

// struktura stacka
typedef struct stack* stackPosition;
typedef struct stack {
    dirPosition dirLevel;
    stackPosition next;
} stack;

void push(stackPosition stack, dirPosition dir);
dirPosition pop(stackPosition stack);
dirPosition mkDir(dirPosition current, char* name);
dirPosition cdDir(dirPosition current, stackPosition headStack, char* name);
dirPosition cdBack(stackPosition headStack);
void Dir(dirPosition current);
void menu(dirPosition root, stackPosition headStack);

int main() {
    directory root = { "C:", NULL, NULL };
    stack headStack = { NULL, NULL };

    push(&headStack, &root);
    menu(&root, &headStack);

    return 0;
}

void push(stackPosition stack, dirPosition dir) {
    stackPosition newElem = malloc(sizeof(stack));
    if (!newElem) {
        printf("Greška pri alokaciji memorije za stack!\n");
        exit(1);
    }
    newElem->dirLevel = dir;
    newElem->next = stack->next;
    stack->next = newElem;
}

dirPosition pop(stackPosition stack) {
    if (stack->next == NULL) {
        return NULL;
    }
    stackPosition topElem = stack->next;
    dirPosition d = topElem->dirLevel;
    stack->next = topElem->next;
    free(topElem);
    return d;
}

dirPosition mkDir(dirPosition current, char* name) {
    dirPosition newDir = malloc(sizeof(directory));
    if (!newDir) {
        printf("Greška pri alokaciji memorije!\n");
        exit(1);
    }
    strcpy(newDir->name, name);
    newDir->subDirPos = NULL;

    // dodaj na poèetak liste poddirektorija
    newDir->next = current->subDirPos;
    current->subDirPos = newDir;

    return newDir;
}

dirPosition cdDir(dirPosition current, stackPosition headStack, char* name) {
    dirPosition temp = current->subDirPos;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            push(headStack, temp);
            return temp;
        }
        temp = temp->next;
    }
    printf("Direktorij '%s' ne postoji!\n", name);
    return current;
}

dirPosition cdBack(stackPosition headStack) {
    pop(headStack); // skini trenutni
    if (headStack->next != NULL)
        return headStack->next->dirLevel;
    return NULL;
}

void Dir(dirPosition current) {
    dirPosition temp = current->subDirPos;
    if (temp == NULL) {
        printf("Direktorij je prazan.\n");
        return;
    }
    while (temp != NULL) {
        printf("%s\n", temp->name);
        temp = temp->next;
    }
}

void menu(dirPosition root, stackPosition headStack) {
    int choice = 0;
    char name[MAX];
    dirPosition current = root;

    while (choice != 5) {
        printf("\nTrenutni direktorij: %s\n", current->name);
        printf("1 - md\n");
        printf("2 - cd dir\n");
        printf("3 - cd..\n");
        printf("4 - dir\n");
        printf("5 - exit\n");
        printf("Izbor: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Ime direktorija: ");
            scanf("%s", name);
            mkDir(current, name);
            break;
        case 2:
            printf("Unesite ime direktorija: ");
            scanf("%s", name);
            current = cdDir(current, headStack, name);
            break;
        case 3:
            current = cdBack(headStack);
            break;
        case 4:
            Dir(current);
            break;
        case 5:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Neispravan izbor!\n");
            break;
        }
    }
}
