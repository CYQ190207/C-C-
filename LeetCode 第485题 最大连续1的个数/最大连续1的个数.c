#include<stdio.h>
int findMaxnumber(int* num,int numsize)
{
	int cur_count = 0;
	int max_count = 0;
	for (int i = 0;i < numsize;i++)
	{
		if (num[i] == 1)
		{
			cur_count++;
			if (cur_count > max_count)
			{
				max_count = cur_count;
			}
		}
		else
		{
			cur_count = 0;
		}
	}
	return max_count;
}
int main()
{
	int num[10] = { 1,1,0,1,1,1,0,1,1,1 };
	int numsize = 10;
	int result = findMaxnumber(num, numsize);
	printf("Maximum consecutive 1s: %d\n", result);
	return 0;
}
