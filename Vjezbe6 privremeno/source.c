#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#define MAX 127
#define DATE_MAX 11

//struktura za artikle
typedef struct items* ItemPosition;
typedef struct items {
	char name[MAX];		//ime proizvoda
	int num;			//kolicina
	float price;		//cijena
	ItemPosition nextItem;		//pointer na sljedeci clan (sljedeci proizvod) 
} Items;

//struktura za racune 
typedef struct receipt* ReceiptPosition;
typedef struct receipt {
	char date[DATE_MAX];				//datum racuna 
	Items* list_items;	//lista proizvoda (Head) dummy
	ReceiptPosition nextDate;			//pointer na sljedeci clan (sljedeci datum)
} Receipt;


//funkcija za stvaranje novog proizvoda
ItemPosition createItem(const char* name, int num, float price) {
	ItemPosition newItem = (ItemPosition)malloc(sizeof(Items));		//d.a.
	if (!newItem)
		printf("Greska pri alociranju novog artikla.\n");
	strcpy(newItem->name, name);	//inic.
	newItem->num = num;
	newItem->price = price;
	newItem->nextItem = NULL;		//postavljanje na NULL pointer

	return newItem;
}

//dodavanje artikla u listu artikala na kraj
int addItem(ItemPosition Head, const char* name, int num, float price) {
	ItemPosition newItem = createItem(name, num, price);

	//dodavanje na kraj
	ItemPosition temp = Head;
	while (temp->nextItem != NULL) {
		temp = temp->nextItem;
	}
	temp->nextItem = newItem;
	return EXIT_SUCCESS;
}

//stvaranje novog racuna 
ReceiptPosition createReceipt(const char* date) {
	ReceiptPosition newReceipt = (ReceiptPosition)malloc(sizeof(Receipt));		//d.a.
	if (!newReceipt)
		printf("Greska pri alociranju novog racuna.\n");

	strcpy(newReceipt->date, date);		//inic.

	//stvorimo dummy da se povezu artikli
	newReceipt->list_items = malloc(sizeof(Items));
	newReceipt->list_items->nextItem = NULL;

	newReceipt->nextDate = NULL;		//postavljanje pointera na NULL
	return newReceipt;
}

//dodavanje racuna u listu na kraj 
ReceiptPosition addReceipt(ReceiptPosition Head, const char* date) {
	ReceiptPosition newReceipt = createReceipt(date);

	//dodavanje na kraj
	ReceiptPosition temp = Head;
	while (temp->nextDate != NULL) {
		temp = temp->nextDate;
	}
	temp->nextDate = newReceipt;
	return newReceipt;
}

//funkcija za otvaranje datoteke koja load-a racune
Receipt* loadReceipt(const char* file) {
	FILE* dat = fopen(file, "r");
	if (!dat) {
		printf("Nije moguce otvoriti datoteku.\n");
		return NULL; 
	}

	//stvaramo HEAD za listu racuna (struktura receipts)
	Receipt* Head = malloc(sizeof(Receipt));
	Head->nextDate = NULL;
	Head->list_items = NULL;
	Head->date[0] = '\0';

	char pathbuffer[MAX];
	//ulazimo u datoteku racuni.txt i sve dok je pathbuffer = 1 odnosno dok ima linija znaci da ima racuna
	while (fscanf(dat, "%s", pathbuffer) == 1) {

		//opet otvaramo datoteka ovog puta onu koja se nalazi unutar racuni.txt
		FILE* receiptFile = fopen(pathbuffer, "r");
		if (!receiptFile) {
			printf("Nije moguce otvoriti datoteku unutar datoteke.\n");
			continue;
		}
		//unutar racuna je potrebno spremiti datum iz prve linije u listu racuni i spremiti ostale linije u listu artikli
		char date[DATE_MAX];
		if (fscanf(receiptFile, "%s", date) != 1) {
			fclose(receiptFile);  
			continue;
		}
		//dodajemo novi racun (unutar funkcije dodavanja racuna je automatski funkcija stvaranja)
		ReceiptPosition newReceipt = addReceipt(Head, date);

		char name_buffer[MAX];
		int num_buffer;
		float price_buffer;

		while (fscanf(receiptFile, "%s %d %f", name_buffer, &num_buffer, &price_buffer) == 3) {
			//dodajemo novi artikl (unutar funkcije dodavanja je funkc. stvaranja)
			addItem(newReceipt->list_items, name_buffer, num_buffer, price_buffer);
		}

		fclose(receiptFile);
	}
	fclose(dat);

	return Head;
}

void printReceipts(Receipt* head) {
	ReceiptPosition rec = head->nextDate; 
	while (rec) { 
		printf("Datum: %s\n", rec->date);
		ItemPosition it = rec->list_items->nextItem; 
		while (it) {
			printf("  %s %d %.2f\n", it->name, it->num, it->price);
			it = it->nextItem;
		}
		rec = rec->nextDate; 
		printf("\n");
	}
}

//FUNKCIJE ZA SORTIRANJE

//sortiranje racuna po datumu
int sortReceipts(Receipt* Head) {
	if (!Head || !Head->nextDate) {
		printf("Lista je prazna.\n");
		return EXIT_FAILURE;
	}

	ReceiptPosition end = NULL;

	//vanjska petlja, svaki put pomièemo end za jedan element
	while (Head->nextDate != end) {
		ReceiptPosition prev = Head;
		ReceiptPosition curr = Head->nextDate;
		ReceiptPosition next = curr->nextDate;

		while (next != end) {			//unutarnja petlja kroz listu do enda
			if (strcmp(curr->date, next->date) > 0) {
				//zamjena
				curr->nextDate = next->nextDate;
				next->nextDate = curr;
				prev->nextDate = next;
				//nakon zamjene, prev ide na next
				prev = next;
				next = curr->nextDate;
			}
			else {
				//nema zamjene, pomicemo pokazivaèe
				prev = curr;
				curr = next;
				next = next->nextDate;
			}
		}
		end = curr; //zadnji element je na svom mjestu
	}

	return EXIT_SUCCESS;
}

//sortiranje po nazivu artikla
int sortItems(Items* Head) {
	if (!Head || !Head->nextItem) {
		printf("Lista artikala je prazna.\n");
		return EXIT_FAILURE;
	}

	ItemPosition end = NULL;

	//vanjska petlja, svaki put pomièemo end za jedan element
	while (Head->nextItem != end) {
		ItemPosition prev = Head;
		ItemPosition curr = Head->nextItem;
		ItemPosition next = curr->nextItem;

		//unutarnja petlja, prolazimo do end-a
		while (next != end) {
			if (strcmp(curr->name, next->name) > 0) {
				//zamjena 
				curr->nextItem = next->nextItem;
				next->nextItem = curr;
				prev->nextItem = next;

				//nakon zamjene, prev minjamo
				prev = next;
				next = curr->nextItem;
			}
			else {
				//nema zamjene, samo pomakni pokazivaèe
				prev = curr;
				curr = next;
				next = next->nextItem;
			}
		}
		end = curr; //zadnji element na svom mistu
	}

	return EXIT_SUCCESS;
}

int sortItemsByNum(Items* Head) {
	if (!Head || !Head->nextItem) {
		printf("Lista artikala je prazna.\n");
		return EXIT_FAILURE;
	}

	ItemPosition end = NULL;

	//vanjska petlja, svaki put pomièemo end za jedan element
	while (Head->nextItem != end) {
		ItemPosition prev = Head;
		ItemPosition curr = Head->nextItem;
		ItemPosition next = curr->nextItem;

		//unutarnja petlja, prolazimo do end-a
		while (next != end) {
			if (curr->num < next->num) {
				//zamjena 
				curr->nextItem = next->nextItem;
				next->nextItem = curr;
				prev->nextItem = next;

				//nakon zamjene, prev minjamo
				prev = next;
				next = curr->nextItem;
			}
			else {
				//nema zamjene, samo pomakni pokazivaèe
				prev = curr;
				curr = next;
				next = next->nextItem;
			}
		}
		end = curr; //zadnji element na svom mistu
	}

	return EXIT_SUCCESS;
}

int isDateInRange(const char* date, const char* start, const char* end) {
	if (strcmp(date, start) >= 0 && strcmp(date, end) <= 0)
		return 1;
	else return 0;
}

void itemQuery(Receipt* Head, const char* itemName, const char* startDate, const char* endDate) {
	int totalItems = 0;
	float sumItems = 0.0;

	ReceiptPosition rec = Head->nextDate;		//krecemo od prvog racuna
	while (rec) {
		if (isDateInRange(rec->date, startDate, endDate) == 1) {
			ItemPosition it = rec->list_items->nextItem;		//krenimo od prvog artikla
			while (it) {
				if (strcmp(it->name, itemName)==0) {
					totalItems += it->num;
					sumItems += it->price * it->num;
				}
				it = it->nextItem;
			}
		}
		rec = rec->nextDate;
	}
	printf("\nUpit za artikl: %s\n", itemName);
	printf("Razdoblje: %s do %s\n", startDate, endDate);
	printf("Ukupna kolicina: %d\n", totalItems);
	printf("Ukupno potroseno: %.2f\n", sumItems);
}

void sortMenu(Receipt* list) {
	int choice;		//broj koji odaberu

	printf("Odaberite vrstu sortiranja:\n");
	printf("  1 - Sortiranje racuna po datumu.\n");
	printf("  2 - Sortiranje artikala po nazivu.\n");
	printf("  3 - Sortiranje artikala po kolcini.\n");
	printf("  4 - Sortiranje artikala po cijeni.\n");
	scanf("%d", &choice);

	switch (choice) {
	case 1:	
		sortReceipts(list);
		printf("Racuni su poredani po datumu.\n");
		break;
	case 2: {
		ReceiptPosition rec = list->nextDate;
		while (rec) {
			sortItems(rec->list_items);
			rec = rec->nextDate;
		}
		printf("Racuni su poredani po nazivu artikla.\n");
		break;
	}
	case 3: {
		ReceiptPosition rec = list->nextDate;
		while (rec) {
			sortItemsByNum(rec->list_items);
			rec = rec->nextDate;
		}
		printf("Racuni su poredani po kolicini artikala.\n");
		break;
	}
	default:
		printf("Nepoznata opcija.\n");
		break;
	}
}

int main() {
	Receipt* list = loadReceipt("racuni.txt");

	sortMenu(list);   // korisnik bira sortiranje

	printReceipts(list);
	puts("");

	char artikl[MAX];
	char start[DATE_MAX], end[DATE_MAX];

	printf("Unesite naziv artikla s prvim velikim slovom: ");
	scanf("%s", artikl);
	printf("Unesite pocetni datum (YYYY-MM-DD): ");
	scanf("%s", start);
	printf("Unesite krajnji datum (YYYY-MM-DD): ");
	scanf("%s", end);

	itemQuery(list, artikl, start, end); 

	return 0;
}
