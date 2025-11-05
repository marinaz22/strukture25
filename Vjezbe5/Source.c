#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 235

typedef struct postfix * Position; 
typedef struct postfix {
	double number;
	Position Next; 
} postfix;

Position push(Position, int);			//dodaje broj na vrh stoga
double pop(Position);			//skida broj s vrha stoga
int isOperator(char);			//provjerava koja je operacija
double doOperation(double, double, char);		//izvodi matematicku operaciju

int main() {

	postfix Head = {.number = 0, .Next = NULL};		//pocetni cvor
	FILE* dat = fopen("datoteka.txt", "r");			//otvaranje datoteke
	if (!dat) {
		printf("Datoteka nije otvorena");
		return EXIT_FAILURE;  
	}
	char buffer[MAX];
	double number;
	char operator;

	while (fscanf(dat, "%s", buffer) == 1) {		//ucitava se jedan po jedan znak
		if (sscanf(buffer, "%lf", &number) == 1) {	//ako se moze pretvoriti u broj
			push(&Head, number);	//pretvoreni broj dodaj na pocetak stoga
		}
		else if (sscanf(buffer, " %c", &operator) == 1 && isOperator(operator)) {		//ako nije broj, ali je operator 
			double a = pop(&Head);		//skini prvi broj sa stoga
			double b = pop(&Head);		//skini drugi broj sa stoga
			double result = doOperation(a, b, operator);		//funkciji koja racuna rezultat posalji oba broja i operator
			push(&Head, result);	//dodaj rezultat na pocetak stoga
		}
		else {
			printf("Nepoznat trash unos %s\n", buffer);
		}
	}

	fclose(dat);	//!!!ZATVORI DATOTEKU!!!

	double result = pop(&Head);		//makni rezultat sa stoga (tj konacni clan)
	printf("Rezultat izraza: %.2lf\n", result);

	return 0;
}

Position push(Position Head, int num) {		//obicna funkcija dodaj na pocetak
	Position newElement = (Position)malloc(sizeof(postfix));
	if (newElement == NULL) {
		printf("Greska pri alokaciji!\n");
		return EXIT_FAILURE;  
	}
	newElement->number = num;
	newElement->Next = NULL;  

	newElement->Next = Head->Next;
	Head->Next = newElement; 

	return newElement;  
}
double pop(Position Head) {		//funkcija brisi sa pocetka
	if (Head->Next == NULL) {
		printf("Stog je prazan!\n");
		exit(1);
	}

	Position temp = Head->Next;
	double number = temp->number;
	Head->Next = temp->Next; 
	free(temp);

	return number;
}

int isOperator(char c) {		//provjerava je li znak operator
	return (c == '+' || c == '-' || c == '*' || c == '/');
}

double doOperation(double a, double b, char operator) {
	switch (operator) {
		case '+': return b + a;
		case '-': return b - a;
		case '*': return b * a;
		case '/': return b / a;
			if (a == 0) { 
				printf("Greska pri racunanju.\n");
				return EXIT_FAILURE;  
			}
		default:
			printf("Nepoznat operator.\n"); 
			return EXIT_FAILURE;
	}
}
