#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int SLDataType;

typedef struct SListNode
{
	SLDataType data;
	struct SListNode* next;
} SListNode;

typedef struct SList
{
	SListNode* head;                  // 哨兵位头结点，不存有效数据
	int size;                         // 有效元素个数
} SList;

// 初始化单链表（创建哨兵位头结点）
void SListInit(SList* ps)
{
	assert(ps);
	ps->head = (SListNode*)malloc(sizeof(SListNode));
	if (ps->head == NULL)
	{
		printf("申请空间失败\n");
		exit(-1);
	}
	ps->head->next = NULL;
	ps->size = 0;
}

// 销毁单链表
void SListDestroy(SList* ps)
{
	assert(ps);
	SListNode* cur = ps->head;
	while (cur != NULL)
	{
		SListNode* next = cur->next;
		free(cur);
		cur = next;
	}
	ps->head = NULL;
	ps->size = 0;
}

// 在下标 i 处插入 x（i 从 0 开始，i == size 时相当于尾插）
void SListInsert(SList* ps, int i, SLDataType x)
{
	assert(ps);
	assert(i >= 0 && i <= ps->size);

	// 找到下标 i 的前一个结点；i == 0 时就是哨兵位头结点
	SListNode* prev = ps->head;
	for (int j = 0; j < i; j++)
		prev = prev->next;

	SListNode* newnode = (SListNode*)malloc(sizeof(SListNode));
	if (newnode == NULL)
	{
		printf("申请空间失败\n");
		exit(-1);
	}
	newnode->data = x;
	newnode->next = prev->next;       // 先连后面
	prev->next = newnode;             // 再连前面
	ps->size++;
}

// 删除下标 i 处的元素，返回被删除的值
int SListDelete(SList* ps, int i)
{
	assert(ps);
	assert(i >= 0 && i < ps->size);

	// 找到下标 i 的前一个结点
	SListNode* prev = ps->head;
	for (int j = 0; j < i; j++)
		prev = prev->next;

	SListNode* del = prev->next;      // 先保存，再断链
	int ret = del->data;
	prev->next = del->next;
	free(del);
	ps->size--;
	return ret;
}

// 按值查找：找到返回下标，找不到返回 -1
int SListFind(SList* ps, SLDataType x)
{
	assert(ps);
	SListNode* cur = ps->head->next;
	int i = 0;
	while (cur != NULL)
	{
		if (cur->data == x)
			return i;
		cur = cur->next;
		i++;
	}
	return -1;
}

// 按位查找：返回下标 pos 处的元素
SLDataType SListGet(SList* ps, int pos)
{
	assert(ps);
	assert(pos >= 0 && pos < ps->size);
	SListNode* cur = ps->head->next;
	for (int i = 0; i < pos; i++)
		cur = cur->next;
	return cur->data;
}

// 打印单链表
void SListPrint(SList* ps)
{
	assert(ps);
	SListNode* cur = ps->head->next;
	while (cur != NULL)
	{
		printf("%d ", cur->data);
		cur = cur->next;
	}
	printf("\n");
}

// 保存到文件：第一行写元素个数，后面每行一个元素
int SListSave(SList* ps, const char* filename)
{
	assert(ps);
	FILE* pf = fopen(filename, "w");
	if (pf == NULL)
	{
		perror("打开文件失败");
		return 0;
	}
	fprintf(pf, "%d\n", ps->size);
	for (SListNode* cur = ps->head->next; cur != NULL; cur = cur->next)
		fprintf(pf, "%d\n", cur->data);
	fclose(pf);
	return 1;
}

// 从文件加载：文件不存在就当作空表；用尾插
void SListLoad(SList* ps, const char* filename)
{
	assert(ps);
	FILE* pf = fopen(filename, "r");
	if (pf == NULL)
		return;
	int n, x;
	if (fscanf(pf, "%d", &n) == 1)
	{
		for (int i = 0; i < n && fscanf(pf, "%d", &x) == 1; i++)
			SListInsert(ps, ps->size, x);
	}
	fclose(pf);
}

// 菜单
void Menu()
{
	printf("\n===== 单链表菜单 =====\n");
	printf("1.尾插  \n2.头插  \n3.指定位置插入\n");
	printf("4.删除  \n5.按值查找  \n6.按位查找\n");
	printf("7.打印  \n0.保存并退出\n");
	printf("请选择：");
}

int main()
{
	SList s;
	SListInit(&s);
	SListLoad(&s, "data.txt");        // 启动时读取上次保存的数据
	int choice, pos, x;

	do
	{
		Menu();
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			printf("输入值：");
			scanf("%d", &x);
			SListInsert(&s, s.size, x);
			break;
		case 2:
			printf("输入值：");
			scanf("%d", &x);
			SListInsert(&s, 0, x);
			break;
		case 3:
			printf("输入下标和值：");
			scanf("%d%d", &pos, &x);
			SListInsert(&s, pos, x);
			break;
		case 4:
			printf("输入要删除的下标：");
			scanf("%d", &pos);
			printf("已删除：%d\n", SListDelete(&s, pos));
			break;
		case 5:
			printf("输入要查找的值：");
			scanf("%d", &x);
			pos = SListFind(&s, x);
			if (pos != -1)
				printf("下标是 %d\n", pos);
			else
				printf("没找到\n");
			break;
		case 6:
			printf("输入下标：");
			scanf("%d", &pos);
			printf("元素是 %d\n", SListGet(&s, pos));
			break;
		case 7:
			SListPrint(&s);
			break;
		}
	} while (choice != 0);

	SListSave(&s, "data.txt");        // 退出前保存
	SListDestroy(&s);
	return 0;
}