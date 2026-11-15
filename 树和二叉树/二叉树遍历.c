
#define _CRT_SECURE_NO_WARNINGS
/*
 * 题1：二叉树中序遍历、后序遍历的递归算法和非递归算法
 * 建树方式：按先序序列输入，空指针用 '#' 表示
 * 例如输入  ABC##DE#G##F###  （教材图6.x 中的经典二叉树）
 */
#include <stdio.h>
#include <stdlib.h>

typedef char TElemType;

typedef struct BiTNode {
    TElemType data;
    struct BiTNode* lchild, * rchild;
} BiTNode, * BiTree;

/* ---------- 顺序栈（元素为二叉树结点指针） ---------- */
#define STACK_SIZE 100
typedef struct {
    BiTree base[STACK_SIZE];
    int top;
} SqStack;

void InitStack(SqStack* S) { S->top = 0; }
int  StackEmpty(const SqStack* S) { return S->top == 0; }
void Push(SqStack* S, BiTree e) {
    if (S->top == STACK_SIZE) {
        printf("栈溢出！\n");
        exit(1);
    }
    S->base[S->top++] = e;
}
BiTree Pop(SqStack* S) { return S->base[--S->top]; }
BiTree GetTop(const SqStack* S) { return S->base[S->top - 1]; }

/* ---------- 建树 / 销毁 ---------- */
void CreateBiTree(BiTree* T) {
    char ch;
    if (scanf(" %c", &ch) != 1 || ch == '#') {
        *T = NULL;
        return;
    }
    *T = (BiTree)malloc(sizeof(BiTNode));
    if (!*T) exit(1);
    (*T)->data = ch;
    CreateBiTree(&(*T)->lchild);
    CreateBiTree(&(*T)->rchild);
}

void DestroyBiTree(BiTree T) {
    if (T) {
        DestroyBiTree(T->lchild);
        DestroyBiTree(T->rchild);
        free(T);
    }
}

void Visit(TElemType e) { printf("%c ", e); }

/* ---------- 中序遍历：递归 ---------- */
void InOrder_Rec(BiTree T) {
    if (T) {
        InOrder_Rec(T->lchild);
        Visit(T->data);
        InOrder_Rec(T->rchild);
    }
}

/* ---------- 中序遍历：非递归 ----------
 * 思路：一路向左入栈；走到尽头后出栈访问，再转向右子树 */
void InOrder_NonRec(BiTree T) {
    SqStack S;
    BiTree p = T;
    InitStack(&S);
    while (p || !StackEmpty(&S)) {
        if (p) {
            Push(&S, p);
            p = p->lchild;
        }
        else {
            p = Pop(&S);
            Visit(p->data);
            p = p->rchild;
        }
    }
}

/* ---------- 后序遍历：递归 ---------- */
void PostOrder_Rec(BiTree T) {
    if (T) {
        PostOrder_Rec(T->lchild);
        PostOrder_Rec(T->rchild);
        Visit(T->data);
    }
}

/* ---------- 后序遍历：非递归 ----------
 * 思路：一路向左入栈；看栈顶结点——
 *   若其右子树存在且尚未访问过，则转向右子树；
 *   否则出栈并访问，同时用 last 记录刚访问过的结点 */
void PostOrder_NonRec(BiTree T) {
    SqStack S;
    BiTree p = T, top, last = NULL;
    InitStack(&S);
    while (p || !StackEmpty(&S)) {
        if (p) {
            Push(&S, p);
            p = p->lchild;
        }
        else {
            top = GetTop(&S);
            if (top->rchild && top->rchild != last) {
                p = top->rchild;          /* 右子树还没访问 */
            }
            else {
                Pop(&S);
                Visit(top->data);
                last = top;
                p = NULL;                 /* 访问完继续看新的栈顶 */
            }
        }
    }
}

int main() 
{
    BiTree T = NULL;

    printf("请按先序序列输入二叉树（空结点用 # 表示），如 ABC##DE#G##F###：\n");
    CreateBiTree(&T);

    printf("\n中序遍历（递归）  ： ");  InOrder_Rec(T);       printf("\n");
    printf("中序遍历（非递归）： ");    InOrder_NonRec(T);    printf("\n");
    printf("后序遍历（递归）  ： ");    PostOrder_Rec(T);     printf("\n");
    printf("后序遍历（非递归）： ");    PostOrder_NonRec(T);  printf("\n");

    DestroyBiTree(T);
    return 0;
}