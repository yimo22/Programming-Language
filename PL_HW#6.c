#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
typedef enum e
{
  ERROR,  // 0
  NUMBER, // 1
  ID,     // 2
  PRINT,  // 3
  PLUS,   // 4
  STAR,   // 5
  LP,     // 6
  RP,     // 7
  RET,    // 8
  EQUAL,  // 9
  END     // 10
} TOKEN;
int expr(void);
typedef struct Variable
{
  char name;
  int value;
} Variable;
Variable var[MAX];

int len_var = 0;
int num;
int nextToken;
char ch = ' ';

TOKEN get_next_token()
{

  char string[32];
  int i = 0;

  while ((ch == ' ') || (ch == '\t'))
    ch = getchar(); // 토큰앞의 빈칸을 무시

  if (ch == '+')
  {
    ch = getchar();
    nextToken = PLUS;
    return (PLUS);
  }

  if (ch == '*')
  {
    ch = getchar();
    nextToken = STAR;
    return (STAR);
  }

  if (ch == '(')
  {
    ch = getchar();
    nextToken = LP;
    return (LP);
  }

  if (ch == ')')
  {
    ch = getchar();
    nextToken = RP;
    return (RP);
  }

  if (ch == '\n')
  {
    //ch = getchar();
    nextToken = RET;
    return (RET);
  }
  if (ch == '=')
  {
    ch = getchar();
    nextToken = EQUAL;
    return (EQUAL);
  }

  if (ch == EOF)
  {
    ch = getchar();
    nextToken = END;
    return (END);
  }

  if ((ch >= '0') && (ch <= '9'))
  {

    num = 0;

    do
    {
      num = num * 10 + ch - '0';
      ch = getchar();
    } while ((ch >= '0') && (ch <= '9'));

    nextToken = NUMBER;
    return (NUMBER);
  }

  if ((ch >= 'a') && (ch <= 'z'))
  { //이름의 길이가 여러글자인 경우도 고려

    do
    {

      string[i++] = ch;
      ch = getchar();
    } // 이름을 저장

    while ((ch >= 'a') && (ch <= 'z'));

    string[i] = 0;

    if (strcmp(string, "print") == 0)
    {
      nextToken = PRINT;
      return (PRINT); // 이름이 print 인지 검사
    }
    if (strlen(string) == 1)
    {
      num = string[0];
      nextToken = ID;
      return (ID);
    }

    nextToken = ERROR;
    return (ERROR);
  }
}
void setValue(char find_var, int value)
{
  int i;
  for (i = 0; i < len_var; i++)
  {
    if (var[i].name == find_var)
    {
      var[i].value = value;
    }
  }
}
int getValue(char find_var)
{
  int i;
  for (i = 0; i < len_var; i++)
  {
    if (var[i].name == find_var)
    {
      return var[i].value;
    }
  }
  printf("error - no such varaible\n");
  exit(1);
}
int isVar(char find_var)
{
  int i;
  for (i = 0; i < len_var; i++)
  {
    if (var[i].name == find_var)
    {
      return 1;
    }
  }
  return 0;
}
int factor()
{
  int result;
  if (nextToken == ID)
  {
    if (isVar(num))
    {
      result = getValue(num);
      get_next_token();
      return result;
    }
    else
    {
      printf("error - no such varaible\n");
      exit(1);
    }
  }
  if (nextToken == NUMBER)
  {
    result = num;
    get_next_token();
    return result;
  }
  else if (nextToken == LP)
  {
    get_next_token();
    result = expr();
    if (nextToken == RP)
    {
      get_next_token();
      return result;
    }
    else
    {
      printf("error - factor:RP\n");
      exit(1);
    }
  }
  else
  {
    printf("error - factor:None\n");
    exit(1);
  }
}
int term()
{
  int result;
  result = factor();
  while (nextToken == STAR)
  {
    get_next_token();
    result *= factor();
  }
  return result;
}
int expr()
{
  int result;
  result = term();
  while (nextToken == PLUS)
  {
    get_next_token();
    result += term();
  }
  return result;
}
int main()
{
  while (1)
  {
    ch = ' ';
    printf(">> ");
    get_next_token();
    if (nextToken == PRINT)
    {
      get_next_token();
      printf("\t>> %d\n", expr());

      continue;
    }

    if (nextToken == ID)
    {
      int pre_num = num;
      get_next_token();
      if (nextToken == EQUAL)
      {
        // ID = expr()
        get_next_token();
        if (isVar(pre_num))
        {
          setValue(pre_num, expr());
        }
        else
        {
          var[len_var].name = pre_num;
          var[len_var].value = expr();
          len_var++;
        }

        continue;
      }
    }
    else
    {
      printf("error - no expected command\n");
      continue;
    }
  }
  return 0;
}