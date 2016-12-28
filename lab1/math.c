#include "stdio.h"
#include "conio.h"

void sum()
{
  float n, s = 0, t = 1.5;
  for(n = 1; n <= 10; n += 5.5)
  {
    s += t*n;
  }
  printf("sum =%f\n", s);
}

main()
{
  int a,b,num1,num2,temp;
  sum();
  printf("please input two numbers:\n");
  scanf("%d,%d",&num1,&num2);
  if(num1 < num2) /*交换两个数，使大数放在num1上*/
  {
    temp=num1;
    num1=num2;
    num2=temp;
  }
  a=num1;b=num2;
  while(b!=0)/*利用辗除法，直到b为0为止*/
  {
    temp=a%b;
    a=b;
    b=temp;
  }
  printf("gongyueshu:%d\n",a);
  printf("gongbeishu:%d\n",num1*num2/a);
  getch();
}
