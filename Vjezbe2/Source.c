#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 30

typedef struct People* Position;
typedef struct People {
	char firstName[MAX];
	char lastName[MAX];
	int birthYear;
	Position Next;
} People;

Position createNewPerson(char*, char*, int);        //dinamicka alokacija i inicijalizacija podataka
Position addToBeg(char*, char*, int, Position);         //dodavanje elemenata na pocetak liste
int printList(Position);    //printanje liste
Position addToEnd(char*, char*, int, Position);     //dodavanje elemenata na kraj liste  
Position findElement(char*, Position);  //pretraga elementa liste po prezimenu
Position deleteElement(char*, Position);    //brisanje elementa liste po prezimenu

int main() {
	People Head = { .firstName = "", .lastName = "", .birthYear = 0, .Next = NULL };        //stvaranje pocetnog cvora

	addToBeg("Roko", "Gaspic", 2000, &Head);
	addToBeg("Korina", "Cular", 2002, &Head);
	addToBeg("Luka", "Lampic", 2005, &Head);
	addToEnd("Borna", "Knez", 2006, &Head);
	addToEnd("Lara", "Culina", 2005, &Head);
	printList(&Head);

	Position found = findElement("Culina", &Head);      //pokazivac na element unutar liste
	if (found != NULL) {		//ako je prezime pronadeno
		printf("\nU listi je pronadeno prezime %s\n", found->lastName);
	}
	else
		printf("\nU listi nije pronadeno prezime.\n");

	deleteElement("Knez", &Head);
	printList(&Head);
}

Position createNewPerson(char* firstName, char* lastName, int birthYear) {
	Position newPerson = (Position)malloc(sizeof(People));		//dinamicka alokacija za newPerson

	strcpy(newPerson->firstName, firstName);	//inicijalizacija
	strcpy(newPerson->lastName, lastName);
	newPerson->birthYear = birthYear;
	newPerson->Next = NULL;

	return newPerson;
}

Position addToBeg(char* firstName, char* lastName, int birthYear, Position Person) {
	Position newPerson = createNewPerson(firstName, lastName, birthYear);

	newPerson->Next = Person->Next;
	Person->Next = newPerson;

	return newPerson;
}

int printList(Position Person) {
	Person = Person->Next;
	while (Person != NULL) {
		printf("%s %s %d\n", Person->firstName, Person->lastName, Person->birthYear);
		Person = Person->Next;
	}
	return 0;
}

Position addToEnd(char* firstName, char* lastName, int birthYear, Position Person) {
	Position newPerson = createNewPerson(firstName, lastName, birthYear);

	Position Current = Person;		//pointeru Trenutno pridodajemo &Head
	while (Current->Next != NULL) {	//dok Trenurno ne bude pokazivao na NULL odnosno kraj liste
		Current = Current->Next;	//pomicemo Trenutno na sljedci clan liste
	}
	Current->Next = newPerson;		//kada Trenutno bude pointer na NULL odnosno kraj, dodajemo novi element

	return newPerson;
}

Position findElement(char* lastName, Position Person) {
	Position Current = Person->Next;	//preskacemo Head element, Trenutno pokazuje na prvi pravi element

	while (Current != NULL) {		//dok Trenutno ne bude kraj
		if (strcmp(Current->lastName, lastName) == 0) {		//usporeduj Trenutno prezime elementa sa poslanim prezimenom
			return Current;		//ako je usporedba 0, odnosno tocna, vrati Trenutno 
		}
		Current = Current->Next;	//nastavi na sljedeci clan
	}
	return EXIT_FAILURE;	//ako prode kroz sve clanove bez pronalaska prezimena
}

Position deleteElement(char* lastName, Position Person) {
	Position Previous = Person;		//. pokazivac na prethodni clan, u pocetku &Head
	Position temp = Previous->Next;	//temp pokazuje na prvi stvrani clan

	while (temp != NULL && strcmp(temp->lastName, lastName) != 0) {		//dok temp ne bude kraj(ne dodemo) i dok se prezime bude odgovaralo poslanom
		Previous = temp;	//pomicemo prethodni na trenutni
		temp = temp->Next;	//a trenutni pomiecmo na sljedeci clan
	}

	if (temp != NULL) {		//ako temp nije kraj na izalsku iz while petlje tj ako smo pronasli element
		Previous->Next = temp->Next;	//preskacemo temp u listi
		free(temp);	//brisemo memoriju temp
		return 0;
	}
	return EXIT_FAILURE;	//ako nismo pronasli prezime koje smo trazili
}
