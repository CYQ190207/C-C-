from typing import List

class Solution:
    def rotate(self, nums: List[int], k: int) -> None:
        """
        Do not return anything, modify nums in-place instead.
        """
        pass

class Solution1:
    """方法一：额外数组"""
    def rotate(self, nums: List[int], k: int) -> None:
        n = len(nums)
        k = k % n
        new_nums = [0] * n
        
        # 将每个元素放到新位置
        for i in range(n):
            new_nums[(i + k) % n] = nums[i]
        
        # 将新数组复制回原数组
        for i in range(n):
            nums[i] = new_nums[i]

class Solution2:
    """方法二：环状替换"""
    def rotate(self, nums: List[int], k: int) -> None:
        n = len(nums)
        k = k % n
        count = 0  # 已处理的元素个数
        
        for start in range(n):
            if count >= n:
                break
                
            current = start
            prev = nums[start]
            
            while True:
                next_idx = (current + k) % n
                temp = nums[next_idx]
                nums[next_idx] = prev
                prev = temp
                current = next_idx
                count += 1
                
                # 如果回到了起点，退出循环
                if current == start:
                    break

class Solution3:
    """方法三：反转数组"""
    def rotate(self, nums: List[int], k: int) -> None:
        n = len(nums)
        k = k % n
        
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
    # 测试用例
    test_cases = [
        ([1, 2, 3, 4, 5, 6, 7], 3),
        ([-1, -100, 3, 99], 2),
        ([1, 2], 3),
        ([1], 0),
    ]
    
    for nums, k in test_cases:
        print(f"\n原数组: {nums}, k = {k}")
        
        # 测试方法一
        nums_copy1 = nums.copy()
        Solution1().rotate(nums_copy1, k)
        print(f"方法一: {nums_copy1}")
        
        # 测试方法二
        nums_copy2 = nums.copy()
        Solution2().rotate(nums_copy2, k)
        print(f"方法二: {nums_copy2}")
        
        # 测试方法三
        nums_copy3 = nums.copy()
        Solution3().rotate(nums_copy3, k)
        print(f"方法三: {nums_copy3}")