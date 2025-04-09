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
