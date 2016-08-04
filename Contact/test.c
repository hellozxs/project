#include "contact.h"

void fun()
{
FILE *pf = fopen("contact.dat", "wb");
int i = 0;
if (pf == NULL)
{
perror("contact.dat for write");
system("pause");
exit(EXIT_FAILURE);
}
fclose(pf);
}
void Test()
{
//fun();
Con con;
init(&con);
int input = 1;
while (input)
{
menu();
printf("Please enter function:>  ");
scanf("%d", &input);
switch (input)
{
case EXIT:
ExitContact(&con);
system("pause");
return ;
case ADD:
add(&con);
break;
case DEL:
del(&con);
break;
case FIND:
find(&con);
break;
case MOD:
mod(&con);
break;
case SHOW:
show(&con);
break;
case EMPTY:
empty(&con);
break;
case SORT:
sort(&con);
break;
default:
printf("\n\nEnter error!\n");

}
system("cls");
}
}
int main()
{
Test();
system("pause");
return 0;
}
