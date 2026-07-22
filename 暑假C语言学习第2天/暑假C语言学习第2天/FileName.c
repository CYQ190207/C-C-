#include<stdio.h>
int main()
{
	/*int n;
	scanf("%d",&n);
	printf("%d",n);*/

	//printf("%c\n",'Q');     //打印字符Q
	//printf("%c\n", '\x51'); //16进制
	//printf("%c\n", '\121'); //8进制
	//printf("%c\n", 81);     //ASCll码

	      
	//printf("%zd\n", sizeof(char));
	//printf("%zd\n", sizeof(_Bool));
	//printf("%zd\n", sizeof(short));
	//printf("%zd\n", sizeof(int));
	//printf("%zd\n", sizeof(long));
	//printf("%zd\n", sizeof(long long));
	//printf("%zd\n", sizeof(float));
	//printf("%zd\n", sizeof(double));
	//printf("%zd\n", sizeof(long double));//在vs上是8个字节，在gcc上测试是16


	short s = 2;
	int b = 10;
	printf("%d\n", sizeof(s = b + 1));
	printf("s = %d\n", s);




	 

	return 0;
} 
