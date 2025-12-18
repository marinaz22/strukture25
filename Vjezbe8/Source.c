#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct tree* treePos; 
typedef struct tree {
	int value;
	treePos left;
	treePos right;

}tree;

/*
typedef struct Red{ 
	treePos node;
	struct Red* next; 
}Red;
*/

treePos newElement(int);
treePos insertElement(treePos, int);
int inorder(treePos);
int preorder(treePos);
int postorder(treePos);
treePos findElem(treePos, int);
treePos findMin(treePos);
treePos deleteElem(treePos, int);
treePos printMenu(treePos);

int main() {
	treePos root = NULL;
	printMenu(root); 

	return 0;
}

treePos newElement(int elem) {
	treePos newEl = (treePos)malloc(sizeof(tree));
	newEl->value = elem;
	newEl->left = NULL;
	newEl->right = NULL;

	return newEl;
}

treePos insertElement(treePos root, int value) {
	//1. sluc
	if (root == NULL)
		return newElement(value);
	//2. sluc ako je manje
	if (value < root->value)
		root->left = insertElement(root->left, value);
	//3. sluc ako je vece
	if (value > root->value)
		root->right = insertElement(root->right, value);

	return root;
}

int inorder(treePos root) {
	if (root != NULL) {
		inorder(root->left);
		printf("%d", root->value);
		inorder(root->right);
	}

	return EXIT_SUCCESS;
}

int preorder(treePos root) {
	if (root != NULL) {
		printf("%d", root->value);
		preorder(root->left); 
		preorder(root->right);
	}
	return EXIT_SUCCESS;
}

int postorder(treePos root) {
	if (root != NULL) { 
		postorder(root->left);
		postorder(root->right);
		printf("%d", root->value);
	}
	return EXIT_SUCCESS;
}

treePos findElem(treePos root, int elem) {
	if (root == NULL)
		return NULL;
	if (elem < root->value)
		return findElem(root->left, elem);
	if (elem > root->value)
		return findElem(root->right, elem);

	return root;
}

treePos findMin(treePos root) {
	while (root->left != NULL)
		root = root->left;
	return root;
}

treePos deleteElem(treePos root, int elem) {
	if (root == NULL) {
		printf("Nema ga!\n");
		return root;
	}

	if (elem < root->value) {
		root->left = deleteElem(root->left, elem);
	}
	else if (elem > root->value) {
		root->right = deleteElem(root->right, elem);
	}
	else {
		if (root->left != NULL && root->right != NULL) {
			treePos temp = findMin(root->right);
			root->value = temp->value;
			root->right = deleteElem(root->right, temp->value);
		}
		else {
			treePos temp = root;
			if (root->left == NULL)
				root = root->right;
			else
				root = root->left;
			free(temp);
		}
	}
	return root;
}


/*
int levelorder() {
}
*/

treePos printMenu(treePos root) {
	int choice, val;
	do {
		printf("\n--- MENI ---\n");
		printf("1. Unesi element\n");
		printf("2. Ispis inorder\n");
		printf("3. Ispis preorder\n");
		printf("4. Ispis postorder\n");
		printf("5. Ispis level order\n");
		printf("6. Brisi element\n");
		printf("7. Pronadji element\n");
		printf("0. Izlaz\n");
		printf("Odabir: ");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			printf("Unesi broj: ");
			scanf("%d", &val);
			root = insertElement(root, val); 
			break;
		case 2:
			inorder(root);
			printf("\n");
			break;
		case 3:
			preorder(root);
			printf("\n");
			break;
		case 4:
			postorder(root);
			printf("\n");
			break;
		case 5:
			//levelOrder(root);
			printf("Ne\n");
			break;
		case 6:
			printf("Unesi broj za brisanje: ");
			scanf("%d", &val);
			root = deleteElem(root, val);
			break;
		case 7:
			printf("Unesi broj za pretragu: ");
			scanf("%d", &val);
			if (findElem(root, val)) 
				printf("Element %d postoji u stablu.\n", val);
			else
				printf("Element %d nije pronaðen.\n", val);
			break;
		}
	} while (choice != 0);

	return root;
}
