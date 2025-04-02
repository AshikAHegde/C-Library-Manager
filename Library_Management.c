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
