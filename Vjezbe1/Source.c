/*
Napisati program koji prvo proèita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamièki alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i uèitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX 25

typedef struct {
	char ime[MAX];
	char prezime[MAX];
	int bodovi;
} Stud; 

int amountOfStudents();	//citanje redaka, brojanje studenata
Stud* arrangeStudents(int);	//dinamicka alokacija, ucitati informacije o studentima u strukture
int printStudents(Stud * studenti, int);	//ispis podataka

int main() {

	int brojStudenata = amountOfStudents();

	printStudents(arrangeStudents(brojStudenata), brojStudenata);
	
	return 0;
}

int amountOfStudents() {

	//otvaranje datoteke
	FILE*dat;
	dat = fopen("datoteka.txt", "r");

	//provjera je li datoteka otvorena
	if (dat == NULL) {
		printf("Greska pri otvaranju.\n");
		return 1;
	}

	int brojac = 0;

	//prebrojavanje broja studenata
	while (!feof(dat))
		if(fgetc(dat) == '\n') brojac++;	


	fclose(dat);	//zatvaranje datoteke

	return brojac;
}

Stud* arrangeStudents(int brojStudenata) {
	
	FILE* dat;
	dat = fopen("datoteka.txt", "r");

	if (dat == NULL) {
		printf("Greska pri otvaranju koda.\n");
		return 1;
	}

	//dinamicka alokacija
	Stud* studenti; 
	studenti = (Stud*)malloc(brojStudenata * sizeof(Stud)); 
	
	for (int i = 0; i < brojStudenata; i++) {
		fscanf(dat, "%s %s %d", studenti[i].prezime, studenti[i].ime, &studenti[i].bodovi); 
	}

	fclose(dat); 

	return studenti; 
}

int printStudents(Stud* studenti, int brojStudenata) {

	int maxBodovi;
	printf("Unesite maksimalni broj bodova na ispitu:\n");
	scanf("%d", &maxBodovi); 

	for (int i = 0; i < brojStudenata; i++) {
		float relatBrojBod = (float)studenti[i].bodovi / maxBodovi * 100;
		printf("Ime i prezime: %s %s\tApsolutan broj bodova: %d\tRelativan broj bodova: %f\n", studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, relatBrojBod); 
	}

	return 0;
}