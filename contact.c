#include "contact.h"

//通讯录菜单
void menu()
{
  printf("\n\t\t*****************   CONTACT   ******************\n");
  printf("\t\t|                                              |\n");
  printf("\t\t|    0 : exit contact        1 : add contact   |\n");
  printf("\t\t|                                              |\n");
  printf("\t\t|    2 : del contact         3 : find contact  |\n");
  printf("\t\t|                                              |\n");
  printf("\t\t|    4 : mod contact         5 : show contact  |\n");
  printf("\t\t|                                              |\n");
  printf("\t\t|    6 : empty contact       7 : sort contact  |\n");
  printf("\t\t|                                              |\n");
  printf("\t\t ----------------------------------------------\n");
}

//显示联系人标头
void head()
{
  printf("\n\t      *********************************************************\n");
  printf("\t%10s\t%5s\t%5s\t%12s\t%15s\n", "name", "sex", "age", "telephone", "address");
}

void check_contact(Con *con)
{
  if (con->max_con == con->size)
  {
    con->per = (Per*)realloc(con->per, (MAX_CON + 2)*sizeof(Per));
    con->max_con += 2;
  }
}
void _add(Con *con,Per *per)
{
  check_contact(con);
  con->per[con->size] = *per;
  con->size++;
}
//读联系人
void ReadContact(Con*con)
{
  FILE *pf = fopen("contact.dat", "rb");
  Per per;
  if (pf == NULL)
  {
    perror("contact.dat for read");
    system("pause");
    exit(EXIT_FAILURE);
  }

  while (fread(&per, sizeof(per), 1, pf))
  {
    _add(con, &per);
  }

  fclose(pf);
}

//写联系人

void WriteContact(Con *con)
{
  FILE *pf = fopen("contact.dat", "wb");
  int i = 0;
  if (pf == NULL)
  {
    perror("contact.dat for write");
    system("pause");
    exit(EXIT_FAILURE);
  }
  while (i<con->size)
  {
    fwrite(&con->per[i],sizeof(con->per[i]),1,pf);
    i++;
  }
  fclose(pf);
}
//初始化联系人
void init(Con *con)
{
  con->size = 0;
  con->max_con = MAX_CON;
  con->per = (Per*)malloc(sizeof(Per)*MAX_CON);
  memset(con->per, 0, sizeof(Per)*MAX_CON);
  if (con->per == NULL)
  {
    printf("out of memory !\n");
    system("pause");
    exit(EXIT_FAILURE);
  }
  ReadContact(con);
}

//添加联系人
void add(Con *con)
{
  int i = 0;
  check_contact(con);
  char tmp_name[MAX_NAME] = " ";
  printf("\nPlease enter name:>");
  scanf("%s", tmp_name);
  for (i = 0; i < con->size; i++)
  {
    if (strcmp(con->per[i].name, tmp_name) == 0)
    break;
  }

  if (i<con->size)
  {
    printf("\nThe contact was existence !\n\n");
    system("pause");
    return;
  }
  else
    strcpy(con->per[con->size].name, tmp_name);

  printf("\nPlease enter sex:>");
  scanf("%s", con->per[con->size].sex);
  printf("\nPlease enter age:>");
  scanf("%d", &(con->per[con->size].age));
  printf("\nPlease enter telephone:>");
  scanf("%s", con->per[con->size].tel);
  printf("\nPlease enter address:>");
  scanf("%s", con->per[con->size].addr);
  printf("\nAdd contact successful !\n\n");
  con->size++;
  system("pause");

}
//显示联系人
void show(Con *con)
{
  if (con->size == 0)
  {
    printf("\nContact is empty\n\n");
    system("pause");
    return;
  }
  head();
  int i = 0;
  for (i = 0; i < con->size; i++)
  {
    printf("\n\t%10s\t%5s\t%5d\t%12s\t%15s\n",
    con->per[i].name,
    con->per[i].sex,
    con->per[i].age,
    con->per[i].tel,
    con->per[i].addr);
  }
  system("pause");
}

//删除联系人
void del(Con *con)
{
  if (con->size == 0)
  {
    printf("\nContact is empty!\n\n");
    system("pause");
    return;
  }
  show(con);
  printf("\nPlease enter tne contact name to delete:>");
  int i = 0;
  char del_name[MAX_NAME];
  scanf("%s", del_name);
  for (i = 0; i < con->size; i++)
  {
    if (strcmp(del_name, con->per[i].name) == 0)
    {
      memset(&(con->per[i]), 0, sizeof(con->per[i]));
      con->size--;
      int count = con->size-i;
      int j = 0;
      for (j = 1; j <= count; j++)
      {
        memmove(&(con->per[i]), &(con->per[i + j]), sizeof(con->per[i]));
      }
      printf("\ndelete successful\n\n");
      system("pause");
      return;
    }
  }
  if (i == con->size)
  {
    printf("\nThe contact name to delete not found!\n\n");
    system("pause");
    return;
  }
}

//查找联系人
void  find(const Con *con)
{
char find_name[MAX_NAME];
printf("\nPlease enter the contact name to find:>");
scanf("%s", find_name);
int i = 0;
for (i = 0; i < con->size; i++)
{
if (strcmp(con->per[i].name, find_name) == 0)
{
head();
printf("\n\t%10s\t%5s\t%5d\t%12s\t%15s\n\n",
con->per[i].name,
con->per[i].sex,
con->per[i].age,
con->per[i].tel,
con->per[i].addr);
system("pause");
return;
}
}
if (i == con->size)
{
printf("\nThe contact name not found!\n\n");
}
system("pause");
return;
}

//修改联系人
void mod(Con *con)
{
char tmp_name[MAX_NAME];
printf("\nPlease enter contact name to modify :>");
scanf("%s", tmp_name);
int i = 0;
for (i = 0; i < con->size; i++)
{
if (strcmp(tmp_name, con->per[i].name) == 0)
{
printf("\nPlease enter sex:>");
scanf("%s", con->per[i].sex);
printf("\nPlease enter age:>");
scanf("%d", &(con->per[i].age));
printf("\nPlease enter telephone:>");
scanf("%s", con->per[i].tel);
printf("\nPlease enter address:>");
scanf("%s", con->per[i].addr);
printf("\nModify contact successful !\n\n");
system("pause");
return;
}
}
if (i == con->size)
{
printf("\nThe contact name not found !\n\n");
system("pause");
}
}

//清空联系人
void empty(Con *con)
{
con->size = 0;
printf("The contact has been cleared !\n\n");
system("pause");
}

//排序联系人
void sort(Con *con)
{
void my_memswap();
int i = 0, j = 0;
for (j = 0; j < con->size - 1; j++)
{
for (i = 0; i < con->size - 1 - j; i++)
{
if (strcmp(con->per[i].name, con->per[i + 1].name)>0)
{
Per tmp = con->per[i];
con->per[i] = con->per[i + 1];
con->per[i + 1] = tmp;
//my_memswap(&(con->per[i]), &(con->per[i + 1]), sizeof(con->per[i]));
}
}
}
show(con);
}

//退出
void ExitContact(Con * con)
{
char ch = 0;
printf("Do you want to save contact ?   y  or n :>");
getchar();              //接受菜单中选择0之后按的回车键
scanf("%c", &ch);
if (ch == 'n')
{

exit(EXIT_SUCCESS);
}
else
{
WriteContact(con);
printf("The  contact has been saved !\n ");
system("pause");
exit(EXIT_SUCCESS);
}
}
