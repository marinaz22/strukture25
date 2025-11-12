#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>

typedef struct poly* Position;
typedef struct poly {
	int coef;		//koeficijent clana
	int power;		//eksponent
	Position Next;
} poly;

int openFile(Position, const char*);		//otvara datoteku i ucitaje podatke
Position createNewElement (int, int);		//stvara novi element, din. alokacija i inicijalizacija
int Sort(Position, int, int);		//
Position addPoly(Position, Position);		//zbrajanje polinomma
Position multiPoly(Position, Position);		//mnozenje polinoma
int printPoly(Position);		//printanje polinoma u u x^n obliku
void freeList(Position);		//oslobadanje memorije


int main() {

	poly firstP = { .coef = 0, .power = 0, .Next = NULL };		//prvi polinom
	poly secondP = { .coef = 0, .power = 0, .Next = NULL };		//drugi polinom
			
	openFile(&firstP, "polinom1.txt");		//otvaranje datoteke s prvim polinomom
	openFile(&secondP, "polinom2.txt");		//otvaranje datotekse s drugim polinomom

	printf("Prvi polinom:\t");
	printPoly(firstP.Next);		//printanje prvog polinoma

	printf("Drugi polinom:\t");
	printPoly(secondP.Next);	//printanje drugog polinoma

	//zbrajanje polinoma
	Position sum = addPoly(firstP.Next, secondP.Next);		//suma dva polinoma, vraca novi sortirani polinom
	printf("\nZbroj:\t");
	printPoly(sum);	

	//mnozenje polinoma
	Position product = multiPoly(firstP.Next, secondP.Next);	//produkt dva polinoma
	printf("\nUmnozak:\t");
	printPoly(product);

	//oslobadanje memorije
	freeList(firstP.Next);  
	freeList(secondP.Next);  
	freeList(sum);
	freeList(product);

	return 0;
}

int openFile(Position P, const char* file) {		//saljemo prvi clan i ime datoteke koju otvaramo
	FILE* dat = fopen(file, "r");		
	if (dat == NULL) {
		printf("Datoteka je prazna");
		return EXIT_FAILURE;
	}
	//ocitavanje podataka k za koef, p za eksponent
	int k, p;
	while (fscanf(dat, "%d %d", &k, &p) == 2) {
		Sort(P, k, p);
	}

	fclose(dat);		//zatvaranje datoteke
	return 0;
}

Position createNewElement(int coef, int power) {
	Position newElement = (Position)malloc(sizeof(poly));		//dinamicka alokacija
	newElement->coef = coef;		//inic.
	newElement->power = power;		//inic.
	newElement->Next = NULL;		//postavljanje novog elementa na NULL

	return newElement;
}

int Sort(Position P, int coef, int power) {
	Position newElement = createNewElement(coef, power);	//kreira novi cvor

	Position prev = P;		//pokazuje na prethodni element 
	Position q = P->Next;	//pokazuje na prethodni element u listi

	while (q != NULL && q->power > power) {		//trazi mjesto na koje ce se umetnuti element
		prev = q;
		q = q->Next;
	}

	if (q != NULL && q->power == power) {	//ako postoji clan s istim eksponentom ne dodaje se novi cvor
		q->coef += coef;	//samo se zbrajaju koeficijenti
		free(newElement);	//netreba nam novi element jer smo zbrojili
		
		if (q->coef == 0) { // ako je koef 0 isto ukloni element
			prev->Next = q->Next;
			free(q);
		}
		return 0;
	}

	//ako ne postoji clan s istim eksponentom umecemo novi element
	newElement->Next = q;	
	prev->Next = newElement;

	return 0;
}

Position addPoly(Position firstP, Position secondP) {
	poly resultHead = { 0, 0, NULL };
	Position result = &resultHead;

	while (firstP != NULL && secondP != NULL) {		//petlja koja prolazi kroz oba polinoma dok oba imaju clanove
		if (firstP->power > secondP->power) {		//prvi slucaj je da nam je eksponent prvog polinoma veci
			Sort(result, firstP->coef, firstP->power);		//taj clan ide direktno u rezultat jer nam je polinom vec sortiran u sort-u
			firstP = firstP->Next;		//pomicemo se na sljedeci clan
		}
		else if (firstP->power < secondP->power) {		//eksponent drugog polinoma je veci od prvog
			Sort(result, secondP->coef, secondP->power);
			secondP = secondP->Next;	//pomicemo se na sljedeci clan
		}
		//ako su koeficijenti jednaki zbrjamo ih 
		else
		{
			int sumCoef = firstP->coef + secondP->coef;
			if (sumCoef != 0)
				Sort(result, sumCoef, firstP->power);
			firstP = firstP->Next;		//pomicemo oba pokazivaca
			secondP = secondP->Next;
		}
	}

	//dodajemo sve preostale clanove iz prvog polinoma
	while (firstP != NULL) {
		Sort(result, firstP->coef, firstP->power);
		firstP = firstP->Next;
	}

	//dodajemo sve preostale clanove iz drugog polinoma
	while (secondP != NULL) {
		Sort(result, secondP->coef, secondP->power);
		secondP = secondP->Next;
	}

	return resultHead.Next;		//prvi stvarni clan rezultat
}
 
Position multiPoly(Position firstP, Position secondP) {
	poly resultHead = { 0, 0, NULL };
	Position result = &resultHead;

	Position temp1 = firstP;
	while (temp1 != NULL) {		//idemo kroz sve clanove prvog polinoma
		Position temp2 = secondP;
		while (temp2 != NULL) {		//za svaki clan prvog polinoma prolazimo kroz sve clanove drugog polinoma
			int newCoef = temp1->coef * temp2->coef;	//umnoza koef
			int newPower = temp1->power + temp2->power;	//zbroj ekspo
			Sort(result, newCoef, newPower);	//sortiramo padajuci 
			temp2 = temp2->Next;	//sljedeci clan drugog polinoma
		}
		temp1 = temp1->Next;	//sljedeci clan prvog polinoma
	}
	return resultHead.Next;		//prvi starni clan razultata
}

int printPoly(Position first) {
	if (!first) {	//ako je lista prazna
		printf("0\n");
		return;
	}
	while (first) {	//kroz listu rezultata i ispisuje polinom 
		printf("%dx^%d", first->coef, first->power);

		if (first->Next && first->Next->coef > 0)
			printf(" + ");

		else if (first->Next)
			printf(" ");
		first = first->Next;
	}
	printf("\n");
	return 0; 
}

void freeList(Position P) {
	Position temp;
	while (P) {
		temp = P;
		P = P->Next;
		temp->Next = NULL;  
		free(temp);
	}
}