/* ============================================================
 *  实验：栈和队列的操作
 *  功能：
 *    1. 顺序栈的基本操作（创建、入栈、出栈、取栈顶、显示、清空）
 *    2. 循环队列的基本操作（创建、入队、出队、取队头、显示、清空）
 *    3. 用栈实现十进制数 → 二/八/十六进制转换
 *    4. 用栈实现算术表达式求值（算法 3.22）
 *  编译：gcc -o exp exp.c -lm
 * ============================================================ */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STACK_INIT_SIZE 100
#define STACKINCREMENT  10
#define MAXQSIZE        100

 /* 清空输入缓冲区，增强健壮性 */
static void clearInput(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* ============================================================
 *  一、顺序栈（整型）
 * ============================================================ */
typedef struct {
    int* base;          /* 栈底指针 */
    int* top;           /* 栈顶指针 */
    int  stacksize;     /* 当前已分配的存储容量 */
} SqStack;

/* 初始化（创建）栈 */
int InitStack(SqStack* S)
{
    S->base = (int*)malloc(STACK_INIT_SIZE * sizeof(int));
    if (!S->base) return 0;
    S->top = S->base;
    S->stacksize = STACK_INIT_SIZE;
    return 1;
}

/* 销毁栈 */
void DestroyStack(SqStack* S)
{
    if (S->base) free(S->base);
    S->base = S->top = NULL;
    S->stacksize = 0;
}

/* 清空栈 */
void ClearStack(SqStack* S)
{
    if (S->base) S->top = S->base;
}

int StackEmpty(SqStack S) { return S.top == S.base; }
int StackLength(SqStack S) { return (int)(S.top - S.base); }

/* 压栈：栈满时自动扩充 */
int Push(SqStack* S, int e)
{
    if (S->top - S->base >= S->stacksize) {          /* 栈满，追加空间 */
        int* newbase = (int*)realloc(S->base,
            (S->stacksize + STACKINCREMENT) * sizeof(int));
        if (!newbase) return 0;
        S->base = newbase;
        S->top = S->base + S->stacksize;
        S->stacksize += STACKINCREMENT;
    }
    *S->top++ = e;
    return 1;
}

/* 弹栈 */
int Pop(SqStack* S, int* e)
{
    if (S->top == S->base) return 0;                 /* 栈空 */
    *e = *--S->top;
    return 1;
}

/* 取栈顶元素（不删除） */
int GetTop(SqStack S, int* e)
{
    if (S.top == S.base) return 0;
    *e = *(S.top - 1);
    return 1;
}

/* ============================================================
 *  二、循环队列（整型）
 * ============================================================ */
typedef struct {
    int* base;
    int  front;         /* 头指针，指向队头元素 */
    int  rear;          /* 尾指针，指向队尾元素的下一个位置 */
} SqQueue;

int InitQueue(SqQueue* Q)
{
    Q->base = (int*)malloc(MAXQSIZE * sizeof(int));
    if (!Q->base) return 0;
    Q->front = Q->rear = 0;
    return 1;
}

void DestroyQueue(SqQueue* Q)
{
    if (Q->base) free(Q->base);
    Q->base = NULL;
    Q->front = Q->rear = 0;
}

void ClearQueue(SqQueue* Q) { Q->front = Q->rear = 0; }

int QueueEmpty(SqQueue Q) { return Q.front == Q.rear; }
int QueueLength(SqQueue Q)
{
    return (Q.rear - Q.front + MAXQSIZE) % MAXQSIZE;
}

/* 入队 */
int EnQueue(SqQueue* Q, int e)
{
    if ((Q->rear + 1) % MAXQSIZE == Q->front) return 0;   /* 队满 */
    Q->base[Q->rear] = e;
    Q->rear = (Q->rear + 1) % MAXQSIZE;
    return 1;
}

/* 出队 */
int DeQueue(SqQueue* Q, int* e)
{
    if (Q->front == Q->rear) return 0;                    /* 队空 */
    *e = Q->base[Q->front];
    Q->front = (Q->front + 1) % MAXQSIZE;
    return 1;
}

/* 取队头元素 */
int GetHead(SqQueue Q, int* e)
{
    if (Q.front == Q.rear) return 0;
    *e = Q.base[Q.front];
    return 1;
}

/* ============================================================
 *  三、表达式求值专用的两个栈（字符栈 / 实数栈）
 * ============================================================ */
typedef struct { char* base, * top; int stacksize; } CharStack;
typedef struct { double* base, * top; int stacksize; } NumStack;

static int CharStackInit(CharStack* S)
{
    S->base = (char*)malloc(STACK_INIT_SIZE * sizeof(char));
    if (!S->base) return 0;
    S->top = S->base; S->stacksize = STACK_INIT_SIZE;
    return 1;
}
static void CharStackDestroy(CharStack* S)
{
    if (S->base) free(S->base);
    S->base = S->top = NULL; S->stacksize = 0;
}
static int CharStackPush(CharStack* S, char e)
{
    if (S->top - S->base >= S->stacksize) {
        char* nb = (char*)realloc(S->base,
            (S->stacksize + STACKINCREMENT) * sizeof(char));
        if (!nb) return 0;
        S->base = nb; S->top = S->base + S->stacksize;
        S->stacksize += STACKINCREMENT;
    }
    *S->top++ = e; return 1;
}
static int CharStackPop(CharStack* S, char* e)
{
    if (S->top == S->base) return 0;
    *e = *--S->top; return 1;
}
static int CharStackGetTop(CharStack S, char* e)
{
    if (S.top == S.base) return 0;
    *e = *(S.top - 1); return 1;
}

static int NumStackInit(NumStack* S)
{
    S->base = (double*)malloc(STACK_INIT_SIZE * sizeof(double));
    if (!S->base) return 0;
    S->top = S->base; S->stacksize = STACK_INIT_SIZE;
    return 1;
}
static void NumStackDestroy(NumStack* S)
{
    if (S->base) free(S->base);
    S->base = S->top = NULL; S->stacksize = 0;
}
static int NumStackPush(NumStack* S, double e)
{
    if (S->top - S->base >= S->stacksize) {
        double* nb = (double*)realloc(S->base,
            (S->stacksize + STACKINCREMENT) * sizeof(double));
        if (!nb) return 0;
        S->base = nb; S->top = S->base + S->stacksize;
        S->stacksize += STACKINCREMENT;
    }
    *S->top++ = e; return 1;
}
static int NumStackPop(NumStack* S, double* e)
{
    if (S->top == S->base) return 0;
    *e = *--S->top; return 1;
}
static int NumStackLength(NumStack S) { return (int)(S.top - S.base); }

/* ============================================================
 *  四、算法 3.22 中调用的三个函数
 * ============================================================ */

 /* (1) In(c)：判断 c 是否为运算符 */
int In(char c)
{
    return c != '\0' && strchr("+-*/()#", c) != NULL;
}

/* (2) Precede(t1, t2)：判断运算符 t1 和 t2 的优先级
 *     返回 '>'：t1 优先级高于 t2，应先算 t1
 *          '<'：t1 优先级低于 t2，t2 入栈
 *          '='：脱括号（'(' 遇 ')'）或结束（'#' 遇 '#'）
 *          'E'：非法组合（如括号不匹配）
 */
char Precede(char t1, char t2)
{
    static const char ops[] = "+-*/()#";
    static const char tbl[7][7] = {
        /*         +    -    *    /    (    )    #   */
        /* + */  {'>', '>', '<', '<', '<', '>', '>'},
        /* - */  {'>', '>', '<', '<', '<', '>', '>'},
        /* * */  {'>', '>', '>', '>', '<', '>', '>'},
        /* / */  {'>', '>', '>', '>', '<', '>', '>'},
        /* ( */  {'<', '<', '<', '<', '<', '=', 'E'},
        /* ) */  {'>', '>', '>', '>', 'E', 'E', '>'},
        /* # */  {'<', '<', '<', '<', '<', 'E', '='}
    };
    const char* p1 = strchr(ops, t1);
    const char* p2 = strchr(ops, t2);
    if (!p1 || !p2) return 'E';
    return tbl[p1 - ops][p2 - ops];
}

/* (3) Operate(a, theta, b)：对 a 和 b 进行二元运算 theta */
double Operate(double a, char theta, double b)
{
    switch (theta) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return a / b;
    default: return 0.0;
    }
}

/* ============================================================
 *  五、算术表达式求值（算法 3.22 的完整实现）
 *  成功返回 1，result 中为结果；失败返回 0，errmsg 中为错误信息
 * ============================================================ */
int EvaluateExpression(const char* expr, double* result, char* errmsg)
{
    CharStack OPTR;                 /* 运算符栈 */
    NumStack  OPND;                 /* 操作数栈 */
    char buf[600];
    int  i = 0, len;

    CharStackInit(&OPTR);
    NumStackInit(&OPND);
    CharStackPush(&OPTR, '#');      /* 栈底先放一个 '#' */

    /* 拷贝表达式、去掉首尾空白，并在末尾追加结束符 '#' */
    {
        const char* p = expr;
        while (*p == ' ' || *p == '\t') p++;
        snprintf(buf, sizeof(buf), "%s", p);
        len = (int)strlen(buf);
        while (len > 0 &&
            (buf[len - 1] == '\n' || buf[len - 1] == '\r' ||
                buf[len - 1] == ' ' || buf[len - 1] == '\t'))
            buf[--len] = '\0';
        buf[len] = '#';
        buf[len + 1] = '\0';
    }
    if (buf[0] == '#') { strcpy(errmsg, "表达式为空"); goto fail; }

    while (1) {
        char c = buf[i];

        if (c == ' ' || c == '\t') { i++; continue; }

        /* ---- 情况 1：读到数字，解析完整操作数并入操作数栈 ---- */
        if (isdigit((unsigned char)c) || c == '.') {
            double num = 0.0, frac = 0.1;
            int hasDot = 0, ndigit = 0;
            while (isdigit((unsigned char)buf[i]) || buf[i] == '.') {
                if (buf[i] == '.') {
                    if (hasDot) { strcpy(errmsg, "数字格式错误"); goto fail; }
                    hasDot = 1;
                }
                else {
                    ndigit++;
                    if (!hasDot) num = num * 10 + (buf[i] - '0');
                    else { num += (buf[i] - '0') * frac; frac /= 10; }
                }
                i++;
            }
            if (ndigit == 0) { strcpy(errmsg, "数字格式错误"); goto fail; }
            NumStackPush(&OPND, num);
            continue;
        }

        /* ---- 情况 2：非运算符的非法字符 ---- */
        if (!In(c)) {
            strcpy(errmsg, "表达式中含有非法字符");
            goto fail;
        }

        /* ---- 情况 3：结束判定（当前是 '#' 且栈顶也是 '#'） ---- */
        {
            char top;
            CharStackGetTop(OPTR, &top);
            if (c == '#' && top == '#') break;
        }

        /* ---- 情况 4：比较栈顶运算符与当前运算符的优先级 ---- */
        {
            char top, rel;
            CharStackGetTop(OPTR, &top);
            rel = Precede(top, c);

            if (rel == '<') {                 /* 当前运算符优先级高，入栈 */
                CharStackPush(&OPTR, c);
                i++;
            }
            else if (rel == '=') {          /* 脱括号 '(' 与 ')' 相遇 */
                char tmp;
                CharStackPop(&OPTR, &tmp);
                i++;
            }
            else if (rel == '>') {          /* 先算栈顶运算符 */
                char   theta;
                double a, b;
                CharStackPop(&OPTR, &theta);
                if (!NumStackPop(&OPND, &b) || !NumStackPop(&OPND, &a)) {
                    strcpy(errmsg, "表达式错误：操作数不足");
                    goto fail;
                }
                if (theta == '/' && b == 0.0) {
                    strcpy(errmsg, "表达式错误：除数为 0");
                    goto fail;
                }
                NumStackPush(&OPND, Operate(a, theta, b));
                /* 注意：此处不移动 i，继续用新的栈顶运算符与 c 比较 */
            }
            else {                          /* 'E'：括号不匹配等 */
                strcpy(errmsg, "表达式错误：括号不匹配");
                goto fail;
            }
        }
    }

    /* ---- 结束：操作数栈中应恰好剩一个数 ---- */
    if (NumStackLength(OPND) != 1) {
        strcpy(errmsg, "表达式错误：操作数与运算符不匹配");
        goto fail;
    }
    NumStackPop(&OPND, result);
    CharStackDestroy(&OPTR);
    NumStackDestroy(&OPND);
    return 1;

fail:
    CharStackDestroy(&OPTR);
    NumStackDestroy(&OPND);
    return 0;
}

/* ============================================================
 *  六、各功能菜单
 * ============================================================ */

 /* ---------- 6.1 栈操作菜单 ---------- */
void StackMenu(void)
{
    SqStack S;
    int inited = 0, choice;

    while (1) {
        printf("\n----------- 栈的基本操作 -----------\n");
        printf("  1. 创建(初始化)栈     2. 入栈(Push)\n");
        printf("  3. 出栈(Pop)          4. 取栈顶元素\n");
        printf("  5. 显示栈中元素       6. 清空栈\n");
        printf("  0. 返回主菜单\n");
        printf("------------------------------------\n");
        printf("请选择: ");
        if (scanf("%d", &choice) != 1) { clearInput(); printf("输入非法!\n"); continue; }
        clearInput();

        if (choice == 0) break;

        switch (choice) {
        case 1:
            if (inited) { printf("栈已经创建过了。\n"); break; }
            if (InitStack(&S)) { inited = 1; printf("栈创建成功!\n"); }
            else printf("创建失败(内存不足)!\n");
            break;

        case 2:
            if (!inited) { printf("请先创建栈!\n"); break; }
            {
                int e;
                printf("请输入要入栈的整数: ");
                if (scanf("%d", &e) != 1) { clearInput(); printf("输入非法!\n"); break; }
                clearInput();
                if (Push(&S, e)) printf("%d 已入栈，当前栈长度 %d。\n", e, StackLength(S));
                else             printf("入栈失败!\n");
            }
            break;

        case 3:
            if (!inited) { printf("请先创建栈!\n"); break; }
            {
                int e;
                if (Pop(&S, &e)) printf("出栈元素: %d\n", e);
                else             printf("栈空，无法出栈!\n");
            }
            break;

        case 4:
            if (!inited) { printf("请先创建栈!\n"); break; }
            {
                int e;
                if (GetTop(S, &e)) printf("栈顶元素: %d\n", e);
                else               printf("栈空!\n");
            }
            break;

        case 5:
            if (!inited) { printf("请先创建栈!\n"); break; }
            if (StackEmpty(S)) { printf("栈为空!\n"); break; }
            printf("栈中元素(从栈底到栈顶): ");
            for (int* p = S.base; p < S.top; p++) printf("%d ", *p);
            printf("\n栈长度: %d\n", StackLength(S));
            break;

        case 6:
            if (!inited) { printf("请先创建栈!\n"); break; }
            ClearStack(&S);
            printf("栈已清空!\n");
            break;

        default: printf("无效选择，请重新输入!\n");
        }
    }

    if (inited) DestroyStack(&S);
}

/* ---------- 6.2 队列操作菜单 ---------- */
void QueueMenu(void)
{
    SqQueue Q;
    int inited = 0, choice;

    while (1) {
        printf("\n----------- 队列的基本操作 -----------\n");
        printf("  1. 创建(初始化)队列   2. 入队(EnQueue)\n");
        printf("  3. 出队(DeQueue)      4. 取队头元素\n");
        printf("  5. 显示队列元素       6. 清空队列\n");
        printf("  0. 返回主菜单\n");
        printf("--------------------------------------\n");
        printf("请选择: ");
        if (scanf("%d", &choice) != 1) { clearInput(); printf("输入非法!\n"); continue; }
        clearInput();

        if (choice == 0) break;

        switch (choice) {
        case 1:
            if (inited) { printf("队列已经创建过了。\n"); break; }
            if (InitQueue(&Q)) { inited = 1; printf("队列创建成功!\n"); }
            else printf("创建失败(内存不足)!\n");
            break;

        case 2:
            if (!inited) { printf("请先创建队列!\n"); break; }
            {
                int e;
                printf("请输入要入队的整数: ");
                if (scanf("%d", &e) != 1) { clearInput(); printf("输入非法!\n"); break; }
                clearInput();
                if (EnQueue(&Q, e)) printf("%d 已入队，当前队长 %d。\n", e, QueueLength(Q));
                else                printf("队满，入队失败!\n");
            }
            break;

        case 3:
            if (!inited) { printf("请先创建队列!\n"); break; }
            {
                int e;
                if (DeQueue(&Q, &e)) printf("出队元素: %d\n", e);
                else                 printf("队空，无法出队!\n");
            }
            break;

        case 4:
            if (!inited) { printf("请先创建队列!\n"); break; }
            {
                int e;
                if (GetHead(Q, &e)) printf("队头元素: %d\n", e);
                else                printf("队空!\n");
            }
            break;

        case 5:
            if (!inited) { printf("请先创建队列!\n"); break; }
            if (QueueEmpty(Q)) { printf("队列为空!\n"); break; }
            {
                int i = Q.front;
                printf("队列元素(从队头到队尾): ");
                while (i != Q.rear) {
                    printf("%d ", Q.base[i]);
                    i = (i + 1) % MAXQSIZE;
                }
                printf("\n队长: %d\n", QueueLength(Q));
            }
            break;

        case 6:
            if (!inited) { printf("请先创建队列!\n"); break; }
            ClearQueue(&Q);
            printf("队列已清空!\n");
            break;

        default: printf("无效选择，请重新输入!\n");
        }
    }

    if (inited) DestroyQueue(&Q);
}

/* ---------- 6.3 进制转换（用栈实现） ---------- */
void Conversion(int n, int r)
{
    SqStack S;
    const char digits[] = "0123456789ABCDEF";
    unsigned int num;
    int sign = 0, e;

    InitStack(&S);

    if (n < 0) { sign = 1; num = (unsigned int)(-(long long)n); }
    else { sign = 0; num = (unsigned int)n; }

    if (num == 0) Push(&S, 0);
    while (num > 0) {
        Push(&S, (int)(num % r));       /* 余数依次入栈 */
        num /= r;
    }

    printf("十进制 %d 转换为 %d 进制的结果为: ", n, r);
    if (sign) printf("-");
    while (Pop(&S, &e)) printf("%c", digits[e]);   /* 倒序输出 */
    printf("\n");

    DestroyStack(&S);

    /* 结果追加保存到文件，便于长期保留 */
    FILE* fp = fopen("conversion_result.txt", "a");
    if (fp) {
        fprintf(fp, "十进制 %d -> %d 进制\n", n, r);
        fclose(fp);
    }
}

void ConversionMenu(void)
{
    int n, base, again = 1;

    printf("\n----------- 十进制数转换 -----------\n");
    while (again) {
        printf("请输入一个十进制整数: ");
        if (scanf("%d", &n) != 1) { clearInput(); printf("输入非法，请重新输入!\n"); continue; }
        clearInput();

        printf("请选择目标进制 (2 / 8 / 16): ");
        if (scanf("%d", &base) != 1) { clearInput(); printf("输入非法!\n"); }
        else {
            clearInput();
            if (base == 2 || base == 8 || base == 16)
                Conversion(n, base);
            else
                printf("只支持 2、8、16 进制!\n");
        }

        printf("继续转换吗? (1-继续 / 0-返回): ");
        if (scanf("%d", &again) != 1) { clearInput(); again = 0; }
        clearInput();
    }
}

/* ---------- 6.4 算术表达式求值菜单 ---------- */
void ExpressionMenu(void)
{
    char   expr[512], errmsg[128];
    double result;

    printf("\n=========== 算术表达式求值 ===========\n");
    printf("说明: 支持 + - * / 和圆括号，操作数建议 0~9。\n");
    printf("      输入空行返回主菜单。\n");

    while (1) {
        printf("\n请输入算术表达式: ");
        if (!fgets(expr, sizeof(expr), stdin)) return;

        {
            size_t len = strlen(expr);
            while (len > 0 && (expr[len - 1] == '\n' || expr[len - 1] == '\r'))
                expr[--len] = '\0';
            if (len == 0) return;            /* 空行退出 */
        }

        if (EvaluateExpression(expr, &result, errmsg)) {
            printf("结果: %s = %g\n", expr, result);
            /* 结果追加保存到文件 */
            FILE* fp = fopen("expression_result.txt", "a");
            if (fp) { fprintf(fp, "%s = %g\n", expr, result); fclose(fp); }
        }
        else {
            printf("求值失败: %s\n", errmsg);
        }
    }
}

/* ============================================================
 *  七、主函数
 * ============================================================ */
int main(void)
{
    int choice;

    while (1) {
        printf("\n");
        printf("================================================\n");
        printf("        数据结构实验 —— 栈和队列的操作\n");
        printf("================================================\n");
        printf("   1. 栈的基本操作\n");
        printf("   2. 队列的基本操作\n");
        printf("   3. 十进制数转换(二 / 八 / 十六进制)\n");
        printf("   4. 算术表达式求值\n");
        printf("   0. 退出\n");
        printf("================================================\n");
        printf("请选择: ");

        if (scanf("%d", &choice) != 1) {
            clearInput();
            printf("输入非法，请输入数字菜单项!\n");
            continue;
        }
        clearInput();

        switch (choice) {
        case 1: StackMenu();       break;
        case 2: QueueMenu();       break;
        case 3: ConversionMenu();  break;
        case 4: ExpressionMenu();  break;
        case 0: printf("程序结束，感谢使用!\n"); return 0;
        default: printf("无效选择，请重新输入!\n");
        }
    }
}