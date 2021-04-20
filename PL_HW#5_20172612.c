#include <stdio.h>
int main()
{
  int array[42], *p;
  int year=0,i=0;
  printf("Enter Year : >> ");
  scanf("%d", &year);
  
  int x = ((((((year-1))*365) +( (year-1) / 4)) -( (year-1) / 100) )+ ((year-1) / 400))% 7;//해당 연도의 1월1일의 요일 계산  
  p = array;

  for (i = 0; i < 42; i++){
    if( i < x){ // 12월 날짜 입력
      *p = ( 32 - x ) + i;
      p++;
      continue;
      }
    else if( i==x){ // 1월 1일 일경우
      *p = 1;
      p++;
      continue;
    }
    else if ( *(p-1) < 31 ){ // 1월 날짜 입력
      *p = *(p-1) + 1;
    }
    else // 2월로 넘어갈경우
    {
      if(*(p-1) == 31)
        *p = 1;
      else
        *p = *p +1;
    }
    p++;
    
  }

  p = array;
  printf("MON\tTUE\tWED\tTHR\tFRI\tSAT\tSUN\n");
  for (i = 0; i < 42; i++)
  {
    if(i%7 == 0)
      printf("\n");
    printf("%d\t", *p);
    p++;
  }
  
  return 0;
}