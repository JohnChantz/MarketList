/*
		Γιάννης Χαντζηγούλας
		ΑΜ:21368
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAXCHAR 100																		//Αντικατασταση της τιμης 100 χαρακτηρων του πινακα με το ονομα MAXCHAR
#if defined (WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)		//Ελεγχος αν το λειτουεγικο συστημα στο οποιο τρεχει το προγραμμα ειναι windows η linux
#define TRUE 1																			//αν το λειτουργικο μας ειναι windows η μεταβλητη TRUE παιρνει τιμη 1
#else																					//αλλιως
#define TRUE 0																			//παιρνει τιμη 0
#endif																					//τελος ελεγχου

struct product{																			//Δηλωση του βασικου struct για τα προιοντα
	int code;																			//κωδικος προιοντος, παιρνει ακεραιες τιμες εκτος απο 0, γιατι το μηδεν χρησιμοποιειτε σε ελεγχους παρακατω
	char description[MAXCHAR];															//πινακας με την περιγραφη του προιοντος, δεν δεχετε whitespaces(space)
	int volume;																			//μεταβλητη για τον ογκο του προιοντος
	struct product *nextProduct;														//δεικτης σε ενα αλλο struct τυπου product για να υλοποιειτε η δομη της λιστας
};																						//τελος struct

typedef struct product product;
typedef struct product *productPtr;														//το productPtr ειναι ενας δεικτης σε ενα struct product


int displayMenu(void);																	//για εκτυπωση του μενου
void displayProducts(productPtr);														//για την εκτυπωση τις λιστας με τα προιοντα, εμφανιζει και τα descriptions διπλα απο καθε κωδικο προιοντος της λιιστας
void displayVolume(int, productPtr);													//ζηταει τον κωδικο ενος προιοντος και εκτυπωνει τον ογκο του
void addProduct(productPtr);															//προσθετει ενα newProduct στο τελος της λιστας, ζηταει code,volume,description
productPtr deleteProduct(int, productPtr);												//ζηταει κωδικο προιοντος και στη συνεχεια το διαγραφει απο την λιστα, αναδιατασονταςκαι τα προιοντα ωστε να ειναι σε σωστη σειρα
void changeVolume(int, productPtr);														//ζηταει τον κωδικο ενος προιοντος και αλλαζει τον ογκο αυτου το προιοντος
void writeData(productPtr);																//γραφει-αποθηκευει τα δεδομενα στο αρχειο stock.dat
productPtr readData(FILE*);																//διαβαζει ολα τα δεδομενα απο το αρχειο stock.dat και τα αποθηκευει σε struct product δομες


int main(int argc, char ** argv)
{
	productPtr firstProduct = (productPtr)malloc(sizeof(product));						//δημιουργια του πρωτου struct, λειτουργει και σαν το head της λιστας
	assert(firstProduct);																//ελεγχος
	firstProduct->nextProduct = NULL;													//δεν υπαρχει (ακομα) επομενο-2ο προιον
	firstProduct->code = 0;																//θετω τον κωδικο προιοντος σε 0 για ελεγχο που γινετε στις addProduct και displayProducts

	FILE* stream;																		//δημιουργεια stream για το αρχειο
	stream = fopen("stock.dat", "a+");													//ανοιγμα του αρχειου για αναγνωση και αν δεν υπαρχει το αρχειο το δημιουργει

	productPtr lastProduct = firstProduct;												//το προηγουμενο προιον 
	productPtr currentProduct = firstProduct;											//προσωρινο προιον στο οποιον γινετε φορτωση με την fread
	productPtr tempProduct = firstProduct;

	fseek(stream, 0, SEEK_END);															//δεικτης-θεσης αρχειου στο τελος
	int fileSize = ftell(stream);														//fileSize=θεση αρχειου στο τελος
	int dataNumber = fileSize / sizeof(product);										//dataNUmber=πληθος στοιχειων στο αρχειο
	fseek(stream, 0, SEEK_SET);															//δεικτης-θεσης αρχειου στην αρχη


	int i;
	for (i = 0; i<dataNumber; i++)
	{
		tempProduct = readData(stream);													//αναγνωση ολων των στοιχειων απο το αρχειο
		if (currentProduct == firstProduct)
		{
			firstProduct = tempProduct;													//το πρωτο στοιχειο του αρχειου παει στο firstProduct
			firstProduct->nextProduct = NULL;											//δεν υπαρχει δευτερο στοιχειο ακομα
			currentProduct = currentProduct->nextProduct;
			lastProduct = firstProduct;
		}
		else
		{
			productPtr newProduct = (productPtr)malloc(sizeof(product));				//φορτωμα των επομενων στοιχειων σε struct newProduct
			assert(newProduct);															//ελεγχος
			newProduct = tempProduct;													//το struct newProduct μπαινει το tempProduct που εχει φορτωθει απο την readData
			lastProduct->nextProduct = newProduct;										//τα παρον στοιχειο δειχνει στο επομενο στοιχειο που δημιουργειτε
			lastProduct = lastProduct->nextProduct;										//το lastProduct ειναι τωρα το τελευταιο προιον που εχει φορτωθει
			newProduct->nextProduct = NULL;

		}
	}

	printf("\nThis program allowes you to manage your products in your Store...\n");
	int choice = 0;
	int productCode;
	while (choice != -1)
	{
		choice = displayMenu();
		switch (choice)
		{
		case 1:
			displayProducts(firstProduct);
			break;
		case 2:
			displayProducts(firstProduct);
			if (firstProduct->code == 0)
			{
				printf("Displaying volume Calceled,due to empty list, please add some products first and try again...\n");
			}
			else
			{
				printf("Please insert the code of the product, which volume you wish to inspect: ");
				scanf("%d", &productCode);
				displayVolume(productCode, firstProduct);
			}
			break;
		case 3:
			addProduct(firstProduct);
			break;
		case 4:
			displayProducts(firstProduct);
			if (firstProduct->code == 0)
			{
				printf("Deleting Calceled,due to empty list, please add some products first and try again...\n");
			}
			else
			{
				printf("\nPlease insert the code of the product you wish to delete: ");
				scanf("%d", &productCode);
				firstProduct = deleteProduct(productCode, firstProduct);
			}
			break;
		case 5:
			displayProducts(firstProduct);
			if (firstProduct->code == 0)
			{
				printf("Changing volume Calceled,due to empty list, please add some products first and try again...\n");
			}
			else
			{
				printf("Please enter the code of the product, which you wish to edit its volume: ");
				scanf("%d", &productCode);
				changeVolume(productCode, firstProduct);
			}
			break;
		case 6:
			choice = -1;
			break;
		default:
			printf("Invalid choice, try again...\n");
			break;
		}
	}
	writeData(firstProduct);

	free(firstProduct);					//αφου εχουμε κανει αποθηκευση των δεδομενων στο αρχειο, κανουμε free το firstProduct και ετσι χαλαει η δομη της λιστας
										//και ελευθερονετε οτι δεσμευση μνημης εχει γινει, αφου χανεται η αληλουχια των δεικτων

	if (TRUE)							//αν απο defined TRUE=1
		system("PAUSE");				//κανε PAUSE σε περιβαλον windows
	return 0;
}

int displayMenu(void)
{
	int choice;

	printf("\n\n\t\t\tProgram Menu\n");
	int i = 1;
	printf("Choose one of the following function pressing the code number:\n");
	printf("%d)For displaying products press %d.\n", i, i);
	i++;
	printf("%d)For displaying volume of a product press %d.\n", i, i);
	i++;
	printf("%d)For adding products press %d.\n", i, i);
	i++;
	printf("%d)For deleting products press %d.\n", i, i);
	i++;
	printf("%d)For changing volume press %d.\n", i, i);
	i++;
	printf("%d)For exit press %d.\n", i, i);

	do
	{
		printf("\n\tGive your choice: ");
		scanf("%d", &choice);
		if (choice <= 0 || choice>6)
		{
			printf("Wrong choice...\nTry again.\n");
		}

	} while (choice <= 0 || choice>6);

	printf("\n\n");

	return choice;

}


void addProduct(productPtr firstProduct)
{
	if (firstProduct->nextProduct == NULL && firstProduct->code == 0)			//αν δεν υπαρχουν στοιχεια και το firstProduct ειναι αδειο βαλε δεδομενα σε αυτο
	{
		printf("Please enter the code for the 1st product: ");
		scanf("%d", &firstProduct->code);
		printf("Please enter the volume for the 1st product: ");
		scanf("%d", &firstProduct->volume);
		printf("Please enter a sort despription of the 1st product: ");
		scanf("%s", firstProduct->description);
		firstProduct->nextProduct = NULL;
		return;
	}

	productPtr newProduct = (productPtr)malloc(sizeof(product));				//δημιουργησε struct newProduct για αποθηκευση νεου προιοντος
	printf("Please enter the code for the current product: ");
	scanf("%d", &newProduct->code);
	printf("Please enter the volume for the current product: ");
	scanf("%d", &newProduct->volume);
	printf("Please enter a sort despription of the current product: ");
	scanf("%s", newProduct->description);
	newProduct->nextProduct = NULL;

	productPtr currentProduct = firstProduct;					//δεικτης στο παρον προιον
	productPtr lastProduct = firstProduct;						//δεικτης στο προιογουμενο προιον
	while (currentProduct != NULL)								//οσο δεν εχουμε φτασει στο τελος της λιστας
	{
		currentProduct = currentProduct->nextProduct;			//προχωρα τον δεικτη στο επομενο προιον
		if (currentProduct != NULL)								//αν φτασει στο τελος της λιστας
			lastProduct = currentProduct;						//βαλε στο το τελευταιο προιον στο lastProduct
	}

	lastProduct->nextProduct = newProduct;						//το επομενο του lastProduct ειναι το newProduct
}


void displayProducts(productPtr firstProduct)
{
	productPtr currentProduct = firstProduct;

	if (firstProduct->code == 0)
	{
		printf("List is empty!!!\n\n");
		return;
	}
	printf("The products stored in your store are the following:\n");
	int i = 1;
	while (currentProduct != NULL)
	{
		printf("%d) %d: %s.\n", i, currentProduct->code, currentProduct->description);
		i++;
		currentProduct = currentProduct->nextProduct;
	}
	printf("End of list...\n\n");
}


void displayVolume(int productCode, productPtr firstProduct)
{
	int found = -1;
	productPtr currentProduct = firstProduct;
	while (1)
	{
		if (currentProduct->code == productCode)
		{
			found = 1;
			printf("The current volume of the product with code->%d equals %d.\n", currentProduct->code, currentProduct->volume);
			return;
		}

		currentProduct = currentProduct->nextProduct;
		if (currentProduct == NULL)
			break;
	}

	if (found == -1)
		printf("The product you required does not exist...\n");
}


void changeVolume(int productCode, productPtr firstProduct)
{
	productPtr currentProduct = firstProduct;
	int choice;
	int found = -1;
	while (1)
	{
		if (currentProduct->code == productCode)
		{
			found = 1;
			printf("Product with code_name=%d found.\n\tThe current volume=%d.\nWould you like to edit the volume, Yes or No?\nPress 1 for Yes or 2 for No.\n", currentProduct->code, currentProduct->volume);
			printf("Choice: ");
			scanf("%d", &choice);
			switch (choice)
			{
			case 1:
				printf("Please enter new value for product volume: ");
				scanf("%d", &(currentProduct->volume));
				printf("Current volume of the product with code->%d is %d.\n", currentProduct->code, currentProduct->volume);
				return;
			case 2:
				printf("Changing Volume Canceled...\n");
				return;
			default:
				printf("Wrong choice...\nPlease try again.\n");
				break;
			}
		}

		currentProduct = currentProduct->nextProduct;
		if (currentProduct == NULL)
			break;


	}
	if (found == -1)
		printf("The product you required does not exist...\n");
}


productPtr deleteProduct(int productCode, productPtr firstProduct)
{
	productPtr currentProduct = firstProduct;
	productPtr lastProduct = firstProduct;
	int choice;
	int found = -1;
	while (1)
	{
		if (currentProduct->code == productCode)
		{
			found = 1;
			printf("Product with code_name=%d found.\n\tWould you like to delete that product, Yes or No?\nPress 1 for Yes or 2 for No.\n", currentProduct->code);
			printf("Choice: ");
			scanf("%d", &choice);
			switch (choice)
			{
			case 1:
				printf("Product with code_name=%d deleted...\n", currentProduct->code);
				if (currentProduct->code == firstProduct->code)					//αν το προιον ειναι το πρωτο
				{
					currentProduct = firstProduct->nextProduct;
					firstProduct->code = 0;										//μηδενισε τον κωδικο του
					firstProduct->volume = 0;									//μηδενισε τον ογκο
					strcpy(firstProduct->description, "");						//και βαλε κενο στο description
					if (currentProduct != NULL)
						return currentProduct;									//αν υπαρχει επομενο πριον απο το firstProduct κανε αυτο firstProduct, δεν κανουμε free(firstProduct) οπως παρακατω 
				}																//γιατι θα χαλασει η λιστα
				else if (currentProduct->nextProduct == NULL)
				{
					lastProduct->nextProduct = NULL;							//αν το προιον που θελουμε να διαγραψουμε ειναι το τελευταιο
					free(currentProduct);										//ελευθερωσε το δεικτη σε αυτο
				}
				else
				{
					lastProduct->nextProduct = currentProduct->nextProduct;		//αν το προιον ειναι καπου ενδιαμεσα, συνδεσε το προηγουμενο αυτου, με το επομενο του
					free(currentProduct);										//και ελευθερωσε τον δεικτη
				}

				return firstProduct;
			case 2:
				printf("Deleting Canceled...\n");
				return firstProduct;
			default:
				printf("Wrong choice...\nPlease try again.\n");
				break;
			}
		}

		lastProduct = currentProduct;
		currentProduct = currentProduct->nextProduct;
		if (currentProduct == NULL)
			break;


	}
	if (found == -1)
		printf("The product you required does not exist...\n");
}


void writeData(productPtr firstProduct)
{
	FILE* stream = fopen("stock.dat", "w");							//κανουμε ξεχωριστω fopen για να κανουμε fwrite για αν ειχαμε το a+ mode δεν θα εκανε ανανεωση των αλλαγων αλλα θα εγραφε το στοιχεια 
	productPtr currentProduct = firstProduct;						//κατω απο τα προηγουμενα
	while (currentProduct != NULL)
	{
		fwrite(currentProduct, sizeof(product), 1, stream);			//γραφουμε ενα ενα τα struct product στο αρχειο μεχρι και το τελευταιο, δηλαδη μεχρι currentProduct==NULL
		currentProduct = currentProduct->nextProduct;
		if (currentProduct == NULL)
			printf("Data stored succesfully, in a ~stock.dat~ file.\n");
	}
	fclose(stream);			//κλεισε το stream
}


productPtr readData(FILE* stream)
{
	productPtr tempProduct = (productPtr)malloc(sizeof(product));			//δημιουργεια ενος προσωρινου προιοντος για την φορτωσγ των στοιχειων

	fread(tempProduct, sizeof(product), 1, stream);							//αναγνωση ενα ενα τα στοιχεια
	return tempProduct;														//και επιστροφη αυτων στην main για προσθηκη στη λιστα
}