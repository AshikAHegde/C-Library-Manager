#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BOOKS 100
#define MAX_ISSUED 100
#define MAX_NAME_LEN 50
#define MAX_TITLE_LEN 100
#define MAX_AUTHOR_LEN 100

struct Book
{
	int sr_no1;
	int id;
	char title[MAX_TITLE_LEN];
	char author[MAX_AUTHOR_LEN];
	int quantity;
	int allocatedCount;
};

struct IssuedBook
{
	int sr_no2, id;
	char userName[MAX_NAME_LEN];
	int issueDate;
	int returnDate;
};

struct Book library[MAX_BOOKS];
struct IssuedBook issuedBooks[MAX_ISSUED];
int bookCount = 0;
int issuedCount = 0;

void toUpperCase(char *str);
void addBook();			   
void issueBook();		   
void returnBook();		   
void displayAllBooks();	   
void displayIssuedBooks(); 
void saveData();		   
void loadData();		   
void deleteBook();		   

// Definations Starts

void toUpperCase(char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		str[i] = toupper(str[i]);
	}
}


void addBook()
{
	if (bookCount >= MAX_BOOKS)
	{
		printf("Library is full!\n");
		return;
	}

	struct Book newBook;
	newBook.sr_no1 = bookCount + 1; // serial no.

	printf("Enter book ID: ");
	scanf("%d", &newBook.id);
	fflush(stdin);

	printf("Enter book title: ");
	fgets(newBook.title, MAX_TITLE_LEN, stdin);
	newBook.title[strcspn(newBook.title, "\n")] = '\0'; // Remove newline
	toUpperCase(newBook.title);							// Convert to uppercase

	printf("Enter author name: ");
	fgets(newBook.author, MAX_AUTHOR_LEN, stdin);
	newBook.author[strcspn(newBook.author, "\n")] = 0; // Remove newline
	toUpperCase(newBook.author);					   // Convert to uppercase

	printf("Enter quantity: ");
	scanf("%d", &newBook.quantity);
	fflush(stdin);
	newBook.allocatedCount = 0;
	fflush(stdin);

	library[bookCount++] = newBook;
	printf("Book added successfully!\n");
}

int isLeapYear(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
int daysInMonth(int month, int year)
{
	switch (month)
	{
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	case 2:
		return isLeapYear(year) ? 29 : 28;
	default:
		return 31;
	}
}

void issueBook()
{
	if (issuedCount >= MAX_ISSUED)
	{
		printf("All books are currently issued!\n");
		return;
	}

	int id, issueDate, returnDate;
	char userName[MAX_NAME_LEN];

	printf("Enter book ID: ");
	scanf("%d", &id);
	fflush(stdin);

	int bookIndex = -1;
	for (int i = 0; i < bookCount; i++)
	{
		if (library[i].id == id)
		{
			bookIndex = i;
			break;
		}
	}

	if (bookIndex == -1 || library[bookIndex].quantity <= 0)
	{
		printf("Book not available!\n");
		return;
	}

	printf("Enter your name: ");
	fgets(userName, MAX_NAME_LEN, stdin);
	userName[strcspn(userName, "\n")] = 0; // Remove newline
	toUpperCase(userName);				   // Convert to uppercase

	printf("Enter issue valid date (yyyymmdd): ");
	scanf("%d", &issueDate);
	fflush(stdin);

	// Automatically set return date to 10 days after issue date
	int issueYear = issueDate / 10000;
	int issueMonth = (issueDate / 100) % 100;
	if (issueMonth > 12 || issueMonth < 1)
	{
		printf("Invalid Month Entered.");
		return;
	}
	int issueDay = issueDate % 100;
	int maxday = daysInMonth(issueMonth, issueYear);
	if (issueDay > maxday || issueDay < 1)
	{
		printf("Invalid Day Entered.");
		return;
	}

	int returnYear = issueYear;
	int returnMonth = issueMonth;
	int returnDay = issueDay + 10;

	int maxd = daysInMonth(returnMonth, returnYear);
	while (returnDay > maxd)
	{
		returnDay -= maxd;
		returnMonth++;
		if (returnMonth > 12)
		{
			returnMonth = 1;
			returnYear++;
		}
	}
	returnDate = returnYear * 10000 + returnMonth * 100 + returnDay;

	issuedBooks[issuedCount].sr_no2 = issuedCount + 1;
	issuedBooks[issuedCount].id = id;
	strcpy(issuedBooks[issuedCount].userName, userName);
	issuedBooks[issuedCount].issueDate = issueDate;
	issuedBooks[issuedCount].returnDate = returnDate;
	issuedCount++;
	library[bookIndex].allocatedCount++;
	library[bookIndex].quantity--;
	printf("Book issued successfully!\n");
}

void returnBook()
{
	int id, today;
	char userName[MAX_NAME_LEN];

	printf("Enter book ID: ");
	scanf("%d", &id);
	fflush(stdin);

	printf("Enter your name same as the name while given at issuing the book: ");
	fgets(userName, MAX_NAME_LEN, stdin);
	userName[strcspn(userName, "\n")] = 0; // Remove newline
	toUpperCase(userName);				   // Convert to uppercase

	printf("Enter today's date (yyyymmdd): ");
	scanf("%d", &today);
	fflush(stdin);
	int bookIndex = -1;
	for (int j = 0; j < bookCount; j++)
	{
		if (library[j].id == id)
		{
			bookIndex = j;
			break;
		}
	}

	for (int i = 0; i < issuedCount; i++)
	{
		if (issuedBooks[i].id==id && strcmp(issuedBooks[i].userName, userName) == 0)
		{
			if (issuedBooks[i].issueDate > today)
			{
				printf("Invalid date\n");
				return;
			}
			else if (issuedBooks[i].returnDate >= today)
			{
				library[bookIndex].allocatedCount--;
				library[bookIndex].quantity++;
				printf("Book returned successfully!\n");
				// Remove the issued book record
				for (int j = i; j < issuedCount - 1; j++)
				{
					issuedBooks[j] = issuedBooks[j + 1];
					issuedBooks[j].sr_no2--; // Update serial number
				}
				issuedCount--;
				return;
			}
			else
			{
				printf("You are late in returning the book!\n");
				return;
			}
		}
	}
	printf("Book not found in issued records or user mismatch!\n");
}
void displayAllBooks()
{
	printf("All Books in Library:\n");
	for (int i = 0; i < bookCount; i++)
	{
		printf("%d. ID: %d, Title: %s, Author: %s, Quantity: %d, Allocated: %d\n", library[i].sr_no1, library[i].id, library[i].title, library[i].author, library[i].quantity, library[i].allocatedCount);
	}
}

void displayIssuedBooks()
{
	printf("Issued Books:\n");
	for (int i = 0; i < issuedCount; i++)
	{
		printf("%d. Book ID: %d, User: %s, Issue Date: %d, Return Date: %d\n", issuedBooks[i].sr_no2, issuedBooks[i].id, issuedBooks[i].userName, issuedBooks[i].issueDate, issuedBooks[i].returnDate);
	}
}

void saveData()
{
	FILE *libFile = fopen("library.txt", "w");
	FILE *allocFile = fopen("books_allocation_info.txt", "w");

	if (libFile == NULL || allocFile == NULL)
	{
		printf("Error opening files for saving!\n");
		return;
	}

	// Saving library book information
	for (int i = 0; i < bookCount; i++)
	{
		fprintf(libFile, "%d. ID: %d, Title: %s, Author: %s, Quantity: %d,Allocated: %d\n", library[i].sr_no1, library[i].id, library[i].title, library[i].author, library[i].quantity, library[i].allocatedCount);
	}

	// Saving issued book information
	for (int i = 0; i < issuedCount; i++)
	{
		fprintf(allocFile, "%d. Book ID: %d, User: %s, Issue Date: %d, Return Date: %d\n", issuedBooks[i].sr_no2, issuedBooks[i].id, issuedBooks[i].userName, issuedBooks[i].issueDate, issuedBooks[i].returnDate);
	}

	fclose(libFile);
	fclose(allocFile);
	printf("Data saved successfully!\n");
}

void loadData()
{
	FILE *libFile = fopen("library.txt", "r");
	FILE *allocFile = fopen("books_allocation_info.txt", "r");

	if (libFile != NULL)
	{
		while (fscanf(libFile, "%d. ID: %d, Title: %99[^,], Author: %99[^,], Quantity: %d, Allocated: %d\n", &library[bookCount].sr_no1, &library[bookCount].id, &library[bookCount].title, library[bookCount].author, &library[bookCount].quantity, &library[bookCount].allocatedCount) != EOF)
		{
			bookCount++;
		}
		fclose(libFile);
	}

	if (allocFile != NULL)
	{
		while (fscanf(allocFile, "%d. Book ID: %d, User: %49[^,], Issue Date: %d, Return Date: %d\n", &issuedBooks[issuedCount].sr_no2, &issuedBooks[issuedCount].id, &issuedBooks[issuedCount].userName, &issuedBooks[issuedCount].issueDate, &issuedBooks[issuedCount].returnDate) != EOF)
		{
			issuedCount++;
		}
		fclose(allocFile);
	}
}

void deleteBook()
{
	int id;
	printf("Enter book ID to delete: ");
	scanf("%d", &id);
	fflush(stdin);
	for (int i = 0; i < issuedCount; i++)
	{
		if (issuedBooks[i].id == id)
		{
			printf("Book is currently issued! Cannot delete.\n");
			return;
		}
	}
	int bookIndex = -1;
	for (int i = 0; i < bookCount; i++)
	{
		if (library[i].id == id)
		{
			bookIndex = i;
			break;
		}
	}

	if (bookIndex == -1)
	{
		printf("Invalid book ID!\n");
		return;
	}

	// Shift books to fill the gap
	for (int i = bookIndex; i < bookCount - 1; i++)
	{
		library[i] = library[i + 1];
		library[i].sr_no1--; // Update serial number
	}
	bookCount--;

	printf("Book deleted and serial numbers reassigned successfully!\n");
	saveData(); // Save changes to library.txt
}

int main()
{
	loadData();
	int choice;

	do
	{
		printf("\nLibrary Management System\n");
		printf("1. Add Book\n");
		printf("2. Issue Book\n");
		printf("3. Return Book\n");
		printf("4. Display All Books\n");
		printf("5. Display Issued Books\n");
		printf("6. Delete Book\n");
		printf("7. Save and Exit\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);
		fflush(stdin);

		switch (choice)
		{
		case 1:
			addBook();
			break;
		case 2:
			issueBook();
			break;
		case 3:
			returnBook();
			break;
		case 4:
			displayAllBooks();
			break;
		case 5:
			displayIssuedBooks();
			break;
		case 6:
			deleteBook();
			break;
		case 7:
			saveData();
			break;
		default:
			printf("Invalid choice! Please try again.\n");
		}
	} while (choice != 7);

	return 0;
}
