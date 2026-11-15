#include <stdio.h>
#include <stdlib.h>

#define MAXV 100

/*==============================================================
 *  一、邻接矩阵存储结构
 *==============================================================*/
typedef struct {
    char vexs[MAXV];               /* 顶点表 */
    int  arcs[MAXV][MAXV];         /* 邻接矩阵，arcs[i][j]=1 表示有边 */
    int  vexNum, arcNum;
} AMGraph;

static int AM_Locate(const AMGraph* g, char v)
{
    int i;
    for (i = 0; i < g->vexNum; ++i)
        if (g->vexs[i] == v) return i;
    return -1;
}

/* 由顶点集合与边集合建立无向图 */
void AM_Create(AMGraph* g, const char* vs, int n,
    const char (*edges)[2], int m)
{
    int i, j, k;
    g->vexNum = n;
    g->arcNum = 0;
    for (i = 0; i < n; ++i) g->vexs[i] = vs[i];
    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j)
            g->arcs[i][j] = 0;

    for (k = 0; k < m; ++k) {
        i = AM_Locate(g, edges[k][0]);
        j = AM_Locate(g, edges[k][1]);
        if (i < 0 || j < 0 || i == j) continue;
        if (g->arcs[i][j] == 0) {                 /* 防重边 */
            g->arcs[i][j] = g->arcs[j][i] = 1;    /* 无向图对称 */
            ++g->arcNum;
        }
    }
}

void AM_Print(const AMGraph* g)
{
    int i, j;
    printf("     ");
    for (i = 0; i < g->vexNum; ++i) printf("%4c", g->vexs[i]);
    printf("\n");
    for (i = 0; i < g->vexNum; ++i) {
        printf("%4c ", g->vexs[i]);
        for (j = 0; j < g->vexNum; ++j) printf("%4d", g->arcs[i][j]);
        printf("\n");
    }
}

/*------------------- 邻接矩阵 DFS（递归） -------------------*/
static void AM_DFS_Rec(const AMGraph* g, int v,
    int visited[], char out[], int* pn)
{
    int w;
    visited[v] = 1;
    out[(*pn)++] = g->vexs[v];
    for (w = 0; w < g->vexNum; ++w)                /* 按顶点序号扫描 */
        if (g->arcs[v][w] && !visited[w])
            AM_DFS_Rec(g, w, visited, out, pn);
}

int AM_DFS(const AMGraph* g, char start, char out[])
{
    int visited[MAXV] = { 0 };
    int i, s, n = 0;
    s = AM_Locate(g, start);
    if (s < 0) return 0;
    AM_DFS_Rec(g, s, visited, out, &n);
    for (i = 0; i < g->vexNum; ++i)                /* 处理非连通图 */
        if (!visited[i]) AM_DFS_Rec(g, i, visited, out, &n);
    return n;
}

/*------------------- 邻接矩阵 BFS（队列） -------------------*/
int AM_BFS(const AMGraph* g, char start, char out[])
{
    int visited[MAXV] = { 0 };
    int queue[MAXV];
    int front = 0, rear = 0, n = 0;
    int s, v, w;
    s = AM_Locate(g, start);
    if (s < 0) return 0;

    visited[s] = 1;
    queue[rear++] = s;
    while (front < rear) {
        v = queue[front++];
        out[n++] = g->vexs[v];
        for (w = 0; w < g->vexNum; ++w)
            if (g->arcs[v][w] && !visited[w]) {
                visited[w] = 1;                    /* 入队时即标记 */
                queue[rear++] = w;
            }
    }
    return n;
}

/*==============================================================
 *  二、邻接表存储结构
 *==============================================================*/
typedef struct ArcNode {
    int adjvex;                       /* 邻接点在顶点表中的下标 */
    struct ArcNode* next;
} ArcNode;

typedef struct {
    char data;
    ArcNode* first;                   /* 第一条边 */
} VNode;

typedef struct {
    VNode vertices[MAXV];
    int vexNum, arcNum;
} ALGraph;

static int AL_Locate(const ALGraph* g, char v)
{
    int i;
    for (i = 0; i < g->vexNum; ++i)
        if (g->vertices[i].data == v) return i;
    return -1;
}

/* 按邻接点序号升序插入，使遍历顺序与邻接矩阵一致 */
static void AL_AddArc(ALGraph* g, int i, int j)
{
    ArcNode** pp = &g->vertices[i].first;
    ArcNode* node;
    while (*pp && (*pp)->adjvex < j) pp = &(*pp)->next;
    if (*pp && (*pp)->adjvex == j) return;             /* 已存在 */
    node = (ArcNode*)malloc(sizeof(ArcNode));
    node->adjvex = j;
    node->next = *pp;
    *pp = node;
}

void AL_Create(ALGraph* g, const char* vs, int n,
    const char (*edges)[2], int m)
{
    int i, j, k;
    g->vexNum = n;
    g->arcNum = 0;
    for (i = 0; i < n; ++i) {
        g->vertices[i].data = vs[i];
        g->vertices[i].first = NULL;
    }
    for (k = 0; k < m; ++k) {
        ArcNode* p;
        int existed = 0;
        i = AL_Locate(g, edges[k][0]);
        j = AL_Locate(g, edges[k][1]);
        if (i < 0 || j < 0 || i == j) continue;
        for (p = g->vertices[i].first; p; p = p->next)
            if (p->adjvex == j) { existed = 1; break; }
        if (!existed) {
            AL_AddArc(g, i, j);
            AL_AddArc(g, j, i);                        /* 无向图双向插入 */
            ++g->arcNum;
        }
    }
}

/* 必须显式释放所有边结点 */
void AL_Destroy(ALGraph* g)
{
    int i;
    for (i = 0; i < g->vexNum; ++i) {
        ArcNode* p = g->vertices[i].first;
        while (p) {
            ArcNode* q = p->next;
            free(p);
            p = q;
        }
        g->vertices[i].first = NULL;
    }
    g->vexNum = g->arcNum = 0;
}

void AL_Print(const ALGraph* g)
{
    int i;
    for (i = 0; i < g->vexNum; ++i) {
        ArcNode* p;
        printf("%3c |", g->vertices[i].data);
        for (p = g->vertices[i].first; p; p = p->next)
            printf(" -> %c", g->vertices[p->adjvex].data);
        printf("\n");
    }
}

/*------------------- 邻接表 DFS（递归） -------------------*/
static void AL_DFS_Rec(const ALGraph* g, int v,
    int visited[], char out[], int* pn)
{
    ArcNode* p;
    visited[v] = 1;
    out[(*pn)++] = g->vertices[v].data;
    for (p = g->vertices[v].first; p; p = p->next)
        if (!visited[p->adjvex])
            AL_DFS_Rec(g, p->adjvex, visited, out, pn);
}

int AL_DFS(const ALGraph* g, char start, char out[])
{
    int visited[MAXV] = { 0 };
    int i, s, n = 0;
    s = AL_Locate(g, start);
    if (s < 0) return 0;
    AL_DFS_Rec(g, s, visited, out, &n);
    for (i = 0; i < g->vexNum; ++i)
        if (!visited[i]) AL_DFS_Rec(g, i, visited, out, &n);
    return n;
}

/*------------------- 邻接表 BFS（队列） -------------------*/
int AL_BFS(const ALGraph* g, char start, char out[])
{
    int visited[MAXV] = { 0 };
    int queue[MAXV];
    int front = 0, rear = 0, n = 0;
    int s, v;
    ArcNode* p;
    s = AL_Locate(g, start);
    if (s < 0) return 0;

    visited[s] = 1;
    queue[rear++] = s;
    while (front < rear) {
        v = queue[front++];
        out[n++] = g->vertices[v].data;
        for (p = g->vertices[v].first; p; p = p->next)
            if (!visited[p->adjvex]) {
                visited[p->adjvex] = 1;                /* 入队时标记 */
                queue[rear++] = p->adjvex;
            }
    }
    return n;
}

/*==============================================================
 *  测试
 *==============================================================*/
static void Show(const char* title, const char out[], int n)
{
    int i;
    printf("%s", title);
    for (i = 0; i < n; ++i)
        printf("%c%s", out[i], i + 1 == n ? "" : " -> ");
    printf("\n");
}

int main(void)
{
    /*      A ---- B
     *      | \    |
     *      |  \   |
     *      C ---- D ---- E                         */
    const char vs[] = { 'A', 'B', 'C', 'D', 'E' };
    const char edges[][2] = {
        {'A','B'}, {'A','C'}, {'A','D'},
        {'B','D'}, {'C','D'}, {'D','E'}
    };
    int nv = (int)(sizeof(vs) / sizeof(vs[0]));
    int ne = (int)(sizeof(edges) / sizeof(edges[0]));
    char out[MAXV];
    int n;

    AMGraph am;
    ALGraph al;

    /* ---------- 邻接矩阵 ---------- */
    AM_Create(&am, vs, nv, edges, ne);
    printf("========== 邻接矩阵 ==========\n");
    AM_Print(&am);
    printf("顶点数: %d  边数: %d\n", am.vexNum, am.arcNum);

    n = AM_DFS(&am, 'A', out); Show("矩阵 DFS (从A出发): ", out, n);
    n = AM_BFS(&am, 'A', out); Show("矩阵 BFS (从A出发): ", out, n);

    /* ---------- 邻接表 ---------- */
    AL_Create(&al, vs, nv, edges, ne);
    printf("\n========== 邻接表 ==========\n");
    AL_Print(&al);
    printf("顶点数: %d  边数: %d\n", al.vexNum, al.arcNum);

    n = AL_DFS(&al, 'A', out); Show("邻接表 DFS (从A出发): ", out, n);
    n = AL_BFS(&al, 'A', out); Show("邻接表 BFS (从A出发): ", out, n);

    AL_Destroy(&al);                     /* 释放邻接表申请的边结点 */
    return 0;
}