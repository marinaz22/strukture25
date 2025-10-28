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
Position addBehind(char*, char*,  int, char*, Position);
Position addBefore(char*, char*, int, char*, Position); 
int sortList(Position); 
int writeList(Position);
int readList(Position);
 

int main() {
	People Head = { .firstName = "", .lastName = "", .birthYear = 0, .Next = NULL };        //stvaranje pocetnog cvora

	addToBeg("Roko", "Kljakovic", 2000, &Head);
	addToBeg("Korina", "Cular", 2002, &Head);
	addToBeg("Luka", "Lampshire", 2000, &Head);
	addToEnd("Marko", "Banovic", 2006, &Head);
	addToEnd("Lara", "Culina", 2005, &Head);
	
	Position found = findElement("Culina", &Head);      //pokazivac na element unutar liste
	if (found != NULL) {		//ako je prezime pronadeno
		printf("U listi je pronadeno prezime %s.\n", found->lastName);
	}
	else
		printf("U listi nije pronadeno prezime.\n");
	puts("");  

	deleteElement("Banovic", &Head); 

	addBehind("Nina", "Kotarac", 2005, "Lampshire", &Head);  
	addBefore("Antonija", "Lukac", 2006, "Kljakovic", &Head);   

	sortList(&Head);
	//printf("Prije upisivanja liste u datoteku:\n");
	writeList(&Head);
	//printList(&Head);

	printf("Nakon citanja liste iz datoteke:\n");
	readList(&Head);    
	printList(&Head);

	return 0;
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

Position addBehind(char* firstName, char* lastName, int birthYear, char* behindLastName, Position Person) {  
	Position temp = Person;		//temp pokazuje na head

	while (temp != NULL) {		//sve dok temp ne bude NULL
		if (strcmp(temp->lastName, behindLastName) == 0) {		//usporedujemo prezime na koje pokazuje temp sa odredenim tempom	
			Position newPerson = createNewPerson(firstName, lastName, birthYear);		//alociramo i inicijaliziramo novi element 
			newPerson->Next = temp->Next;		//dodavanje novog elementa
			temp->Next = newPerson;  
		}
		temp = temp->Next;		//ako nezadovolja if petlju temp prelazi na sljedeci element
	}
	return EXIT_FAILURE; 
}

Position addBefore(char* firstName, char* lastName, int birthYear, char* beforeLastName, Position Person) {
	Position prev = Person;		//prev pokazuje na head
	Position temp = Person->Next;		//temp pokazuje na prvi pravi element

	while (temp != NULL) {		//dok temp ne bude null
		if (strcmp(temp->lastName, beforeLastName) == 0) {		
			Position newPerson = createNewPerson(firstName, lastName, birthYear);
			prev->Next = newPerson;
			newPerson->Next = temp;
		}
		prev = prev->Next;
		temp = temp->Next; 
	}
	return EXIT_FAILURE;
}

int sortList(Position Head) {
	Position temp = NULL;
	Position sorted = NULL;
	Position Q, P;

	while (Head->Next != sorted) 
	{
		Q = Head; 
		P = Q->Next;  
		while (P->Next != sorted){    
			if (strcmp(P->lastName, P->Next->lastName) > 0) { 
				temp = P->Next; 
				//zamijena elemenata   
				P->Next = temp->Next; 
				temp->Next = P;
				Q->Next = temp;
				//resetiranje pokazivaca na sljedeci
				P = temp; 
			}
			Q = P;  
			P = P->Next; 
		}
		sorted = P;  
	}
	return 0;
}

int writeList(Position Head) {
	FILE* dat = fopen("datoteka.txt", "w");		//otvaranje datoteke za upisivanje
	if (dat == NULL) {		//provjera je li se datoteka otvorila
		printf("Datoteka se ne moze otvoriti.\n");
	}
	Position temp = Head->Next;		//temp je prvi pravi element
	
	if (temp == NULL) {			//provjera je li lista prazna
		printf("Lista je prazna");
	}

	while (temp != NULL) {		//dok temp ne bude NULL
		fprintf(dat, "%s %s %d\n", temp->firstName, temp->lastName, temp->birthYear);		//upisuj vrijednosti 
		temp = temp->Next;		//predi na sljedeci element
	}
	fclose(dat);		//!!!! ZATVARANJE DATOTEKE !!!!

	return 0;
}

int readList(Position Head) {
	//brisanje svih elemenata liste
	Position temp = NULL;
	while (Head->Next != NULL) {
		temp = Head->Next;
		Head->Next = temp->Next;
		free(temp);
	}

	//iscitavanje iz datoteke
	FILE* dat = fopen("datoteka.txt", "r");

	if (dat == NULL) {		//provjera
		printf("Datoteka se ne moze otvoriti.\n");
		return EXIT_FAILURE;  
	}
	char firstName[MAX], lastName[MAX];
	int birthYear; 

	while(fscanf(dat, "%s %s %d", firstName, lastName, &birthYear) == 3) {     
		Position newPerson = createNewPerson(firstName, lastName, birthYear);    
		Position P = Head;  

		while (P->Next != NULL) {
			P = P->Next;  
		}

		P->Next = newPerson; 
	}
	fclose(dat); 
	return 0;
}