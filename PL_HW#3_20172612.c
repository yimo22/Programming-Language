/*
    Interpreter in C
    - Four rule Calculation 
    coded by yimo22
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
enum TokenCode
{
  // Token Code
  LETTER = 0,  // 문자열
  DIGIT,       // 숫자
  IDENT,       // 식별자
  ADD_OP,      // '+'
  SUB_OP,      // '-'
  MULT_OP,     // '*'
  DIV_OP,      // '/'
  LEFT_PAREN,  // '('
  RIGHT_PAREN, // ')'
  INT_LIT,     // 상수
  UNKNOWN = 99,
  MAX
};

bool IsError = false;

int charClass;
char input[MAX];
int len_input = 0;
char lexeme[100];
char nextChar = '\0';
int lexLen = 0;
int InputPos = 0;
int nextToken;

typedef struct Variable
{
  char name;
  int value;
} Variable;
Variable var[MAX];
int len_var = 0;

void express();

void EraseStrSpace(char *str)
{
  // str의 i번째 index 부터 공백을 지움
  for (int index = 0; index < strlen(str); index++)
  {
    if (str[index] == ' ')
    {
      for (int j = index; j < strlen(str); j++)
      {
        str[j] = str[j + 1];
      }
    }
  }
}

// addChar - add nextChar to lexeme
void addChar()
{
  if (lexLen <= 98)
  {
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = 0;
  }
  else
  {
    printf("Error - lexeme is too long \n");
  }
}
int lookup(char ch)
{
  switch (ch)
  {
  case '(':
    addChar();
    nextToken = LEFT_PAREN;
    break;
  case ')':
    addChar();
    nextToken = RIGHT_PAREN;
    break;
  case '+':
    addChar();
    nextToken = ADD_OP;
    break;
  case '-':
    addChar();
    nextToken = SUB_OP;
    break;
  case '*':
    addChar();
    nextToken = MULT_OP;
    break;
  case '/':
    addChar();
    nextToken = DIV_OP;
    break;

  default:
    break;
  }

  return nextToken;
}
// getChar() : 다음 문자의 type 확인
void getChar()
{
  nextChar = input[InputPos++];

  if (isalpha(nextChar))
  {
    charClass = LETTER;
  }
  else if (isdigit(nextChar))
    charClass = DIGIT;
  else
    charClass = UNKNOWN;
}
int lex()
{
  lexLen = 0;
  while (isspace(nextChar))
  {
    getChar();
  }

  switch (charClass)
  {
  case LETTER:
    addChar();
    getChar();
    while (charClass == LETTER)
    {
      addChar();
      getChar();
    }
    nextToken = IDENT;
    break;
  case DIGIT:
    addChar();
    getChar();
    while (charClass == DIGIT)
    {
      addChar();
      getChar();
    }
    nextToken = INT_LIT;
    break;
  case UNKNOWN:
    lookup(nextChar);
    getChar();
    break;
  default:
    break;
  }

  return nextToken;
}
int factor()
{
  lex();
  /* Determine */
  if (nextToken == IDENT || nextToken == INT_LIT)
  {
    // 식별자일 경우, 그 값을 return
    if (nextToken == IDENT)
    {
      // 해당 변수값 return
      for (int i = 0; i < len_var; i++)
      {
        if (var[i].name == lexeme[0])
        {
          return var[i].value;
        }
      }
      // 해당 변수가 없을 경우 Error 출력
      printf("Error - Variable doesn't Exist\n");
      IsError = true;
    }
    else
    {
      return atoi(lexeme);
    }
  }
  else
  {
    if (nextToken == LEFT_PAREN)
    {
      express();
      return atoi(lexeme);
    }
    if (nextToken == RIGHT_PAREN)
      lex();
    else
    {
      printf("Error - factor()\n");
    }
  }
}
void term()
{
  /* Parse the first factor */
  int front = factor();
  // getNext Token
  lex();
  int back;
  if (nextToken == MULT_OP || nextToken == DIV_OP)
  {
    while (nextToken == MULT_OP || nextToken == DIV_OP)
    {
      int Operation = nextToken;
      back = factor();
      lex();
      switch (Operation)
      {
      case MULT_OP:
        sprintf(lexeme, "%d", front * back);
        front = atoi(lexeme);
        break;
      case DIV_OP:
        sprintf(lexeme, "%d", front / back);
        front = atoi(lexeme);
        break;
      default:
        break;
      }
    }
  }
  else
  {
    sprintf(lexeme, "%d", front);
  }
}
void express()
{

  term();
  int front = atoi(lexeme);
  int back;
  while (nextToken == ADD_OP || nextToken == SUB_OP)
  {
    int Operation = nextToken;
    term();
    back = atoi(lexeme);
    switch (Operation)
    {
    case ADD_OP:
      sprintf(lexeme, "%d", front + back);
      front = atoi(lexeme);
      break;
    case SUB_OP:
      sprintf(lexeme, "%d", front - back);
      front = atoi(lexeme);
      break;
    default:
      break;
    }
  }
}
void InterPreter()
{
  while (1)
  {
    printf(" >> ");

    // Get input end with \0
    fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0';
    InputPos = 0;
    IsError = false;
    getChar();
    // Get next token
    lex();
    if (!strcmp(lexeme, "print"))
    {
      // case - Print
      express();
      if (!IsError)
        printf("Result : %d\n", atoi(lexeme));
    }
    else if (strlen(lexeme) == 1)
    {
      // case - IDENT
      // 공백 제거
      EraseStrSpace(input);
      if (input[InputPos - 1] == '=')
      {
        bool flag = false;
        // 이미 있는 변수 일경우
        for (int i = 0; i < len_var; i++)
        {
          if (var[i].name == lexeme[0])
          {
            getChar();
            express();
            var[i].value = atoi(lexeme);
            flag = true;
          }
        }
        if (!flag)
        { // 새로운 변수 일경우
          int test = len_var;
          var[len_var].name = lexeme[0];
          getChar();
          express();
          var[len_var++].value = atoi(lexeme);
        }
      }
      else
      {
        printf("Error - Unexpected Input\n");
      }
    }
    else
    {
      // strlen != 1 && str != print
      // print Error
      printf("Error - the length of variable must be 1\n");
    }
  }
}
int main()
{
  InterPreter();
  return 0;
}