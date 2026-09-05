#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

//功能：原地反转数组 nums 中 [left, right] 闭区间内的元素
//void reverse(int* nums, int left, int right)
//{
//	while (left < right)
//	{
//		int temp = nums[right];
//		nums[right] = nums[left];
//		nums[left] = temp;
//		left++;
//		right--;
//	}
//}
//void rotate(int* nums, int k, int numssize)
//{
//	k %= numssize;
//	reverse(nums, 0, numssize - 1);
//	reverse(nums, 0, k - 1);
//	reverse(nums, k, numssize - 1);
//}




//暴力法每次整体右移一位，重复 k 次
void rotate(int* nums, int k, int numssize)
{
	k %= numssize;
	while (k--)
	{
		int temp = nums[numssize - 1];
		for (int i = numssize-1; i >0; i--)
		{
			nums[i] = nums[i - 1];
		 }
		nums[0] = temp;
	}
}


int main()
{
	int nums[10] = { 0 };
	int numssize = 0;
	scanf("%d",&numssize);
	int k;
	scanf("%d", &k);
	for (int i = 0; i < numssize; i++)
	{
		scanf("%d",&nums[i]);
	}

	rotate(nums, k, numssize);
	for (int i = 0; i < numssize; i++)
	{
		printf("%d ", nums[i]);
	}

	return 0;
}