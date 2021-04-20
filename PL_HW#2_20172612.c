#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
enum
{
    PLUS,   // 0
    MIN,    // 1
    STAR,   // 2
    DIV,    // 3
    NUMBER, // 4
    LP,     // 5
    RP,     // 6
    ERR     // 7
} token;
#define MAX 200
char arr[MAX];
int arr_pos = 0;
void term();
void factor();
void error();
void get_next_token()
{
    /* next token of input --> token */
    while (isspace(arr[arr_pos]))
    {
        // 공백 제거
        arr_pos++;
    }
    if (isdigit(arr[arr_pos]))
    {
        // INT_LIT
        arr_pos++;
        while (isdigit(arr[arr_pos]))
        {
            arr_pos++;
        }
        token = NUMBER;

        return;
    }
    else
    {
        switch (arr[arr_pos])
        {
        case '+':
            token = PLUS;
            break;
        case '-':
            token = MIN;
            break;
        case '*':
            token = STAR;
            break;
        case '/':
            token = DIV;
            break;
        case '(':
            token = LP;
            break;
        case ')':
            token = RP;
            break;
        default:
            token = ERR;
            break;
        }
        arr_pos++;
    }

}
void expr()
{
    term();
    while (token == PLUS || token == MIN)
    {
        get_next_token();
        term();
    }
}
void term()
{
    factor();
    while (token == STAR || token == DIV)
    {
        get_next_token();
        factor();
    }
}
void factor()
{
    if (token == NUMBER)
    {
        get_next_token();
    }
    else if (token == LP)
    {
        get_next_token();
        expr();
        if (token == RP)
        {
            get_next_token();
        }
        else
        {
            error();
        }
    }
    else
    {
        error();
    }
}
int main()
{
    printf("수식을 입력하세요. 수식이 아닐경우 Error 메시지가 출력됩니다.\n");
    while (1)
    {
        arr_pos = 0;
        printf("Enter the expression >> ");
        scanf("%s", arr);
        get_next_token();
        expr();
    }
}
void error()
{
    printf("Error - This is not expression \n");
}