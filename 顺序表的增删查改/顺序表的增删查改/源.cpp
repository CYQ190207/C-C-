#define _CRT_SECURE_NO_WARNINGS
#include"SqList.h"
//typedef 是为了方便类型转换
typedef int SqDateType;


//动态顺序表结构定义
typedef struct SequenceList
{
	SqDataType* arr;//存储数据的动态数组指针
	int size;//记录顺序表中已经存入的数据个数
	int capacity;//动态数组的容量空间大小
}SqList;

//动态顺序表的实现


//1.初始化 SqListInit — 准备一个空柜
void SqListInit(SqList* ps)
{
	assert(ps);   // 防止传入空指针（ps 必须有效）
	ps->arr = NULL;    // 柜子还没买，指针指向空
	ps->size = 0;  // 放了 0 件东西
	ps->capacity = 0;// 一共有 0 个格子
}

//2. 检查扩容 CheckCapacity — 格子不够就换大的
static void CheckCapacity(SqList* ps)
{
    assert(ps);

    // 如果 size >= capacity，说明格子满了（或还没买柜子）
    if (ps->size >= ps->capacity)
    {
        // 新容量：如果是第一次（0），就给4个；否则翻倍
        int newCap = (ps->capacity == 0) ? 4 : ps->capacity * 2;

        // 申请新内存，把旧内容搬过去
        SqDataType* tmp = (SqDataType*)realloc(ps->arr, newCap * sizeof(SqDataType));

        if (tmp == NULL)   // 申请失败
        {
            perror("realloc");
            exit(-1);
        }

        ps->arr = tmp;         // 指向新柜子
        ps->capacity = newCap; // 更新总格子数
    }
}    

//3. 插入 SqListInsert — 在指定位置放东西
void SqListInsert(SqList* ps, int i, SqDataType x)
{
    assert(ps);
    assert(i >= 0 && i <= ps->size);  // i 不能越界（可以插在末尾，所以是 <=）

    CheckCapacity(ps);   // 先检查格子够不够，不够就扩容

    // 第一步：把 [i, size) 的元素逐个往后挪一格，腾出位置
    for (int j = ps->size; j > i; j--)
    {
        ps->arr[j] = ps->arr[j - 1];
        // 比如 j=3: arr[3] = arr[2]，把 2 号位置的复制到 3 号位置
    }

    // 第二步：把 x 放进腾出来的位置 i
    ps->arr[i] = x;

    // 第三步：东西多了一件
    ps->size++;
}




//4. 删除 SqListDelete — 拿走指定位置的东西
SqDataType SqListDelete(SqList* ps, int i)
{
    assert(ps);
    assert(i >= 0 && i < ps->size);   // i 不能越界（必须是已有元素）

    SqDataType del = ps->arr[i];      // 第一步：先记住拿走的是什么

    // 第二步：把 [i+1, size) 的元素逐个往前挪一格，补上空位
    for (int j = i; j < ps->size - 1; j++)
    {
        ps->arr[j] = ps->arr[j + 1];
        // 比如 j=1: arr[1] = arr[2]，把 2 号位置的复制到 1 号位置
    }

    ps->size--;    // 第三步：东西少了一件

    return del;     // 把拿走的东西还给你
}

//5. 取值 GetElem — 直接按编号拿
SqDataType GetElem(SqList* ps, int i)
{
    assert(ps);
    assert(i >= 0 && i < ps->size);  // 必须是有效位置

    return ps->arr[i];   // 直接返回第 i 个格子的东西，O(1)
}

//6. 查找 LocateElem — 挨个翻找
int LocateElem(SqList* ps, SqDataType x)
{
    assert(ps);

    for (int i = 0; i < ps->size; i++)   // 从 0 号开始翻
    {
        if (ps->arr[i] == x)   // 找到了！
            return i;          // 返回编号
    }
    return -1;   // 翻遍了都没有，返回 -1 表示"没找到"
}

//9. 尾插 SqListPushBack — 往最后面加
void SqListPushBack(SqList* ps, SqDataType x)
{
    SqListInsert(ps, ps->size, x);
    //                   ↑ 在size位置插入，就是末尾
    // 末尾后面没有元素，所以for循环一次都不执行，最快
}

//10. 头插 SqListPushFront — 最前面塞一件
void SqListPushFront(SqList* ps, SqDataType x)
{
    SqListInsert(ps, 0, x);
    //              ↑ 在位置0插入，所有元素都要后移
}

//11. 尾删 SqListPopBack — 拿走最后一件
void SqListPopBack(SqList* ps)
{
    assert(ps);
    assert(ps->size > 0);   // 空表不能删
    ps->size--;             // 直接减少计数，最快！O(1)
}

//12. 头删 SqListPopFront — 拿走第一件
void SqListPopFront(SqList* ps)
{
    assert(ps);
    assert(ps->size > 0);   // 空表不能删
    SqListDelete(ps, 0);    // 删除位置0，所有元素前移
}

//13. 打印 SqListPrint
void SqListPrint(SqList* ps)
{
    assert(ps);
    for (int i = 0; i < ps->size; i++)   // 只打印有效元素
    {
        printf("%d ", ps->arr[i]);
    }
    printf("\n");   // 最后换行
}

//14. 销毁 SqListDestroy — 归还柜子
void SqListDestroy(SqList* ps)
{
    assert(ps);
    free(ps->arr);         // 释放柜子占用的内存
    ps->arr = NULL;        // 指针置空（防止野指针）
    ps->size = 0;          // 清零
    ps->capacity = 0;
}

