#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 10

//struktura za stablo
typedef struct tree* treePosition;
typedef struct tree{
	int value;
	treePosition left;
	treePosition right; 
}tree;

treePosition createNode(int value);
treePosition insertValue(treePosition root, int value);
int addAndReplace(treePosition root);
void inorderToFile(treePosition root, FILE* fp);
void writeTreeToFile(treePosition root, const char* filename);

int main() {
	treePosition root = NULL;
	int i, choice, value;
	int defined[N] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };

	srand((unsigned)time(NULL));
	printf("Odaberite.\n");
	printf("\t 1. Zadani niz brojeva:\n");
	printf("\t 2. Random niz brojeva:\n");
	scanf("%d", &choice);
 
	switch (choice) {
		case 1:
			for (i = 0; i < N; i++)
				root = insertValue(root, defined[i]); 
			break;

		case 2:
			for (i = 0; i < N; i++) {
				value = rand() % 81 + 10;
				root = insertValue(root, value);
			}
			break;
		default:
			printf("Neispravan odabir.\n");
			return -1;
			break;
	}

	writeTreeToFile(root, "inorder.txt");

	return 0;
}

treePosition createNode(int value) {
	treePosition newNode = (treePosition)malloc(sizeof(tree));
	if (!newNode) return NULL;
	newNode->value = value;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

treePosition insertValue(treePosition root, int value) {
	if (root == NULL) {
		return createNode(value);
	}

	if (value > root->value) 
		root->left = insertValue(root->left, value);
	
	else if (value < root->value)
		root->right = insertValue(root->right, value);

	return root;
}

int addAndReplace(treePosition root) {
	if (root == NULL)
		return 0;
	int prevValue = root->value;

	root->value = addAndReplace(root->left) + addAndReplace(root->right);

	return prevValue + root->value;
}

void inorderToFile(treePosition root, FILE* fp) {
	if (root == NULL)
		return;

	inorderToFile(root->left, fp);
	fprintf(fp, "%d ", root->value);
	inorderToFile(root->right, fp);
}


void writeTreeToFile(treePosition root, char* filename) {
	FILE* fp = fopen(filename, "w");
	if (!fp) {
		printf("Greska pri otvaranju datoteke!\n");
		return;
	}

	fprintf(fp, "Inorder nakon insert:\n");
	inorderToFile(root, fp);
	fprintf(fp, "\n");

	addAndReplace(root);

	fprintf(fp, "Inorder nakon replace:\n");
	inorderToFile(root, fp);
	fprintf(fp, "\n");

	fclose(fp);
}
