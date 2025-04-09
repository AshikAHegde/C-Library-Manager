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