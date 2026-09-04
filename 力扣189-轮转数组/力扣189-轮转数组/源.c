#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>






//旋转数组时间复杂度为 O(N2 ) ，空间复杂度为 O(1) 的解题思路 
//void rotate(int* nums, int k, int numsSize)
//{
//	k %= numsSize;
//	while (k--)
//	{
//		int temp = nums[numsSize - 1];
//		for (int i = numsSize - 1; i > 0; i--)
//		{
//			nums[i] = nums[i - 1];
//		}
//		nums[0] = temp;
//	}
//}




//三段反转法    时间 O (n)，空间 O (1)
void reverse(int* nums, int left, int right)
{
    while(left < right)
    {
        int t = nums[left];
        nums[left] = nums[right];
        nums[right] = t;
        left++;
        right--;
    }
}

void rotate(int* nums, int numsSize, int k) {
    k %= numsSize;
    reverse(nums, 0, numsSize - 1);     // 整个数组反转
    reverse(nums, 0, k - 1);            // 反转前k个
    reverse(nums, k, numsSize - 1);     // 反转后面剩余
}




int main()
{
	int nums[10] = { 0 };
	int numsSize = 0;
	scanf("%d", &numsSize);

	int k=0;
	scanf("%d", &k);
	for (int i = 0; i < numsSize; i++)
	{
		scanf("%d",&nums[i]);
	}

	rotate(nums, numsSize, k);


	for (int i = 0; i < numsSize; i++)
	{
		printf("%d ", nums[i]);
	}
	return 0;
}