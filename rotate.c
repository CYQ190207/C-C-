#include <stdio.h>
#include <stdlib.h>

// 方法一：额外数组
void rotate1(int* nums, int numsSize, int k) {
    if (numsSize == 0) return;
    k = k % numsSize;
    if (k == 0) return;
    
    int* new_nums = (int*)malloc(numsSize * sizeof(int));
    
    for (int i = 0; i < numsSize; i++) {
        new_nums[(i + k) % numsSize] = nums[i];
    }
    
    for (int i = 0; i < numsSize; i++) {
        nums[i] = new_nums[i];
    }
    
    free(new_nums);
}

// 方法二：环状替换
void rotate2(int* nums, int numsSize, int k) {
    if (numsSize == 0) return;
    k = k % numsSize;
    if (k == 0) return;
    
    int count = 0;
    
    for (int start = 0; count < numsSize; start++) {
        int current = start;
        int prev = nums[start];
        
        do {
            int next_idx = (current + k) % numsSize;
            int temp = nums[next_idx];
            nums[next_idx] = prev;
            prev = temp;
            current = next_idx;
            count++;
        } while (current != start);
    }
}

// 方法三：反转数组（最优）
void reverse(int* nums, int start, int end) {
    while (start < end) {
        int temp = nums[start];
        nums[start] = nums[end];
        nums[end] = temp;
        start++;
        end--;
    }
}

void rotate3(int* nums, int numsSize, int k) {
    if (numsSize == 0) return;
    k = k % numsSize;
    if (k == 0) return;
    
    reverse(nums, 0, numsSize - 1);
    reverse(nums, 0, k - 1);
    reverse(nums, k, numsSize - 1);
}

// 辅助函数：打印数组
void printArray(int* nums, int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", nums[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

// 辅助函数：复制数组
void copyArray(int* dest, int* src, int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

int main() {
    // 测试用例1
    int nums1[] = {1, 2, 3, 4, 5, 6, 7};
    int nums1_size = sizeof(nums1) / sizeof(nums1[0]);
    int k1 = 3;
    
    printf("测试用例1: 原数组: ");
    printArray(nums1, nums1_size);
    printf("k = %d\n", k1);
    
    // 测试方法一
    int nums1_copy1[nums1_size];
    copyArray(nums1_copy1, nums1, nums1_size);
    rotate1(nums1_copy1, nums1_size, k1);
    printf("方法一: ");
    printArray(nums1_copy1, nums1_size);
    
    // 测试方法二
    int nums1_copy2[nums1_size];
    copyArray(nums1_copy2, nums1, nums1_size);
    rotate2(nums1_copy2, nums1_size, k1);
    printf("方法二: ");
    printArray(nums1_copy2, nums1_size);
    
    // 测试方法三
    int nums1_copy3[nums1_size];
    copyArray(nums1_copy3, nums1, nums1_size);
    rotate3(nums1_copy3, nums1_size, k1);
    printf("方法三: ");
    printArray(nums1_copy3, nums1_size);
    
    printf("\n");
    
    // 测试用例2
    int nums2[] = {-1, -100, 3, 99};
    int nums2_size = sizeof(nums2) / sizeof(nums2[0]);
    int k2 = 2;
    
    printf("测试用例2: 原数组: ");
    printArray(nums2, nums2_size);
    printf("k = %d\n", k2);
    
    // 测试方法一
    int nums2_copy1[nums2_size];
    copyArray(nums2_copy1, nums2, nums2_size);
    rotate1(nums2_copy1, nums2_size, k2);
    printf("方法一: ");
    printArray(nums2_copy1, nums2_size);
    
    // 测试方法二
    int nums2_copy2[nums2_size];
    copyArray(nums2_copy2, nums2, nums2_size);
    rotate2(nums2_copy2, nums2_size, k2);
    printf("方法二: ");
    printArray(nums2_copy2, nums2_size);
    
    // 测试方法三
    int nums2_copy3[nums2_size];
    copyArray(nums2_copy3, nums2, nums2_size);
    rotate3(nums2_copy3, nums2_size, k2);
    printf("方法三: ");
    printArray(nums2_copy3, nums2_size);
    
    return 0;
}