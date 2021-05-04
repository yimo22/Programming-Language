/*
가우스-조던 소거법
*/
#include <stdio.h>
#include <stdlib.h>
#define PROCESS 1

void Input(double **matrix, int size) //입력
{
  printf("input Elements:\n");
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size + 1; j++)
      scanf("%lf", &matrix[i][j]);
}

void swap_row(double **matrix, int size, int num1, int num2)
{
  for (int i = 0; i < size + 1; i++) //행을 바꿔준다
  {
    int temp = matrix[num1][i];
    matrix[num1][i] = matrix[num2][i];
    matrix[num2][i] = temp;
  }
}

void print_matrix(double **matrix, int size) //출력
{
  printf("==================matrix===================\n");
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size + 1; j++)
      printf("%.2f ", matrix[i][j]);
    printf("\n");
  }
}
void gauss_jordan(double **matrix, int size) //가우스 조던 소거법
{
  for (int i = 0; i < size - 1; i++)
  {
    if (matrix[i][i] == 0)
    {
      for (int j = i + 1; j < size; j++)
      {
        if (matrix[j][i] != 0)
        {
          swap_row(matrix, size, i, j); //행을 바꿈
#ifdef PROCESS
          print_matrix(matrix, size);
#endif
          break;
        }
      }
      //다시 실행
      i--;
    }
    //
    else //R(i)의 첫번째 계수가 0이 아닐 경우
    {
      double divide = matrix[i][i];
      for (int j = 0; j < size + 1; j++)
      {
        matrix[i][j] *= (1 / divide);
      }
#ifdef PROCESS
      print_matrix(matrix, size);
#endif
      for (int j = i + 1; j < size; j++)
      {
        double multiply = matrix[j][i]; //multiply가 필요한 이유: i번째 계수를 1로 만들기 위해
        for (int k = 0; k < size + 1; k++)
        {
          matrix[j][k] -= (matrix[i][k] * multiply);
        }
      }
#ifdef PROCESS
      print_matrix(matrix, size);
#endif
    }
  }
  double temp = matrix[size - 1][size - 1]; //마지막 행의 i번째 계수를 저장해놓습니다.
  for (int i = 0; i < size + 1; i++)
    matrix[size - 1][i] *= (1 / temp); //마지막 행에 i번째 계수의 역수만큼 곱함.
#ifdef PROCESS
  print_matrix(matrix, size);
#endif

  //여기서부터 가우스 조던 소거법
  for (int i = size - 1; i > 0; i--) //거꾸로연산
  {
    for (int j = i - 1; j >= 0; j--)
    {
      double multiply = matrix[j][i];
      for (int k = i; k < size + 1; k++)
        matrix[j][k] -= (matrix[i][k] * multiply);
#ifdef PROCESS
      print_matrix(matrix, size);
#endif
    }
  }
}

int main(void)
{
  int size;
  double **matrix;
  printf("Matrix size >> ");
  scanf("%d", &size);
  if (!(size >= 2 && size <= 10))
  {
    printf("Error - Unexpected input\n");
    exit(1);
  }
  matrix = (double **)malloc(sizeof(double *) * size);

  for (int i = 0; i < size; i++)
    matrix[i] = (double *)malloc(sizeof(double) * (size + 1));

  Input(matrix, size);
  printf("\n");

#ifdef PROCESS
  print_matrix(matrix, size);
#endif
  gauss_jordan(matrix, size);

  printf("\n==================ANSW=======================\n");
  print_matrix(matrix, size);

  return 0;
}
