#ifndef _CONTACT_H__
#define _CINTACT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_CON 2
#define MAX_NAME 20
#define MAX_SEX 5
#define MAX_TEL 12
#define MAX_ADDR  30

enum
{
EXIT,
ADD,
DEL,
FIND,
MOD,
SHOW,
EMPTY,
SORT
};

typedef struct Per
{
char name[MAX_NAME];
char sex[MAX_SEX];
int age;
char tel[MAX_TEL];
char addr[MAX_ADDR];

}Per;

typedef struct Con
{
//Per per[MAX_CON];
int size;
int max_con;
Per *per;

}Con;

void menu();
void init();
void add();
void head();
void show();
void del();
void find();
void mod();
void empty();
void sort();
void WriteContact();
void  ReadContact();
void ExitContact();

#endif
