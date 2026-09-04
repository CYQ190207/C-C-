from typing import List

class Solution:
    def rotate(self, nums: List[int], k: int) -> None:
        """
        Do not return anything, modify nums in-place instead.
        """
        n = len(nums)
        k = k % n  # 处理 k 大于数组长度的情况
        
        # 反转整个数组
        self.reverse(nums, 0, n - 1)
        # 反转前 k 个元素
        self.reverse(nums, 0, k - 1)
        # 反转后 n - k 个元素
        self.reverse(nums, k, n - 1)
    
    def reverse(self, nums: List[int], start: int, end: int) -> None:
        while start < end:
            nums[start], nums[end] = nums[end], nums[start]
            start += 1
            end -= 1

# 测试代码
if __name__ == "__main__":
    sol = Solution()
    
    # 测试用例 1
    nums1 = [1, 2, 3, 4, 5, 6, 7]
    k1 = 3
    sol.rotate(nums1, k1)
    print(f"测试用例 1: {nums1}")  # 应该输出 [5, 6, 7, 1, 2, 3, 4]
    
    # 测试用例 2
    nums2 = [-1, -100, 3, 99]
    k2 = 2
    sol.rotate(nums2, k2)
    print(f"测试用例 2: {nums2}")  # 应该输出 [3, 99, -1, -100]