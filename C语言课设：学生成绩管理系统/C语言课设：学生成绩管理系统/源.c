#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct
{
    int id;
    char name[20];
    float score;
    int rank;
} Student;

Student stu[MAX];
int count = 0;

/*================= 文件读取 =================*/
void load()
{
    FILE* fp = fopen("student.txt", "r");

    if (fp == NULL)
        return;

    count = 0;

    while (fscanf(fp, "%d %s %f",
        &stu[count].id,
        stu[count].name,
        &stu[count].score) != EOF)
    {
        count++;
    }

    fclose(fp);
}

/*================= 文件保存 =================*/
void save()
{
    FILE* fp = fopen("student.txt", "w");

    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "%d %s %.2f\n",
            stu[i].id,
            stu[i].name,
            stu[i].score);
    }

    fclose(fp);

    printf("\n数据保存成功！\n");
}

/*================= 成绩等级 =================*/
char* level(float score)
{
    if (score >= 90)
        return "优秀";
    else if (score >= 80)
        return "良好";
    else if (score >= 70)
        return "中等";
    else if (score >= 60)
        return "及格";
    else
        return "不及格";
}

/*================= 主菜单 =================*/
void menu()
{
    printf("\n");
    printf("+==================================================+\n");
    printf("|                                                  |\n");
    printf("|            学 生 成 绩 管 理 系 统               |\n");
    printf("|                                                  |\n");
    printf("+==================================================+\n");
    printf("|  1. 添加学生信息                                 |\n");
    printf("|  2. 显示所有学生信息                             |\n");
    printf("|  3. 查询学生信息                                 |\n");
    printf("|  4. 修改学生成绩                                 |\n");
    printf("|  5. 删除学生信息                                 |\n");
    printf("|  6. 成绩排序与排名                               |\n");
    printf("|  7. 班级成绩统计                                 |\n");
    printf("|  8. 保存数据                                     |\n");
    printf("|  0. 退出系统                                     |\n");
    printf("+==================================================+\n");
    printf("请输入功能编号：");
}

/*================= 添加学生 =================*/
void addStudent()
{
    if (count >= MAX)
    {
        printf("学生数量已满！\n");
        return;
    }

    printf("\n请输入学号：");
    scanf("%d", &stu[count].id);

    printf("请输入姓名：");
    scanf("%s", stu[count].name);

    printf("请输入成绩：");
    scanf("%f", &stu[count].score);

    count++;

    printf("添加成功！\n");
}

/*================= 显示学生 =================*/
void showStudent()
{
    if (count == 0)
    {
        printf("暂无学生信息！\n");
        return;
    }

    printf("\n");
    printf("+================================================================+\n");
    printf("| 排名 | 学号    | 姓名        | 成绩      | 等级               |\n");
    printf("+----------------------------------------------------------------+\n");

    for (int i = 0; i < count; i++)
    {
        printf("| %-4d | %-7d | %-10s | %-8.2f | %-10s |\n",
            i + 1,
            stu[i].id,
            stu[i].name,
            stu[i].score,
            level(stu[i].score));
    }

    printf("+================================================================+\n");
}

/*================= 查询学生 =================*/
void searchStudent()
{
    int id;

    printf("输入学号：");
    scanf("%d", &id);

    for (int i = 0; i < count; i++)
    {
        if (stu[i].id == id)
        {
            printf("\n");
            printf("+================================+\n");
            printf("| 查询结果                       |\n");
            printf("+================================+\n");
            printf("学号：%d\n", stu[i].id);
            printf("姓名：%s\n", stu[i].name);
            printf("成绩：%.2f\n", stu[i].score);
            printf("等级：%s\n", level(stu[i].score));
            return;
        }
    }

    printf("未找到该学生！\n");
}

/*================= 修改成绩 =================*/
void updateStudent()
{
    int id;

    printf("输入要修改的学号：");
    scanf("%d", &id);

    for (int i = 0; i < count; i++)
    {
        if (stu[i].id == id)
        {
            printf("当前成绩：%.2f\n", stu[i].score);

            printf("输入新成绩：");
            scanf("%f", &stu[i].score);

            printf("修改成功！\n");
            return;
        }
    }

    printf("未找到该学生！\n");
}

/*================= 删除学生 =================*/
void deleteStudent()
{
    int id;

    printf("输入要删除的学号：");
    scanf("%d", &id);

    for (int i = 0; i < count; i++)
    {
        if (stu[i].id == id)
        {
            for (int j = i; j < count - 1; j++)
            {
                stu[j] = stu[j + 1];
            }

            count--;

            printf("删除成功！\n");
            return;
        }
    }

    printf("未找到该学生！\n");
}

/*================= 排序排名 =================*/
void sortStudent()
{
    Student temp;

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - 1 - i; j++)
        {
            if (stu[j].score < stu[j + 1].score)
            {
                temp = stu[j];
                stu[j] = stu[j + 1];
                stu[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < count; i++)
    {
        stu[i].rank = i + 1;
    }

    printf("\n排序完成！\n");

    printf("+===============================================================+\n");
    printf("| 排名 | 学号      | 姓名          | 成绩                        |\n");
    printf("+---------------------------------------------------------------+\n");

    for (int i = 0; i < count; i++)
    {
        printf("| %-4d | %-9d | %-12s | %-10.2f |\n",
            stu[i].rank,
            stu[i].id,
            stu[i].name,
            stu[i].score);
    }

    printf("+===============================================================+\n");
}

/*================= 成绩统计 =================*/
void statistics()
{
    if (count == 0)
    {
        printf("暂无数据！\n");
        return;
    }

    float sum = 0;
    float max = stu[0].score;
    float min = stu[0].score;

    int pass = 0;

    for (int i = 0; i < count; i++)
    {
        sum += stu[i].score;

        if (stu[i].score > max)
            max = stu[i].score;

        if (stu[i].score < min)
            min = stu[i].score;

        if (stu[i].score >= 60)
            pass++;
    }

    printf("\n");
    printf("+================================================+\n");
    printf("|                  成绩统计                      |\n");
    printf("+================================================+\n");
    printf("| 学生人数：%-36d |\n", count);
    printf("| 平均分：%-38.2f |\n", sum / count);
    printf("| 最高分：%-38.2f |\n", max);
    printf("| 最低分：%-38.2f |\n", min);
    printf("| 及格人数：%-34d |\n", pass);
    printf("| 及格率：%-36.2f%% |\n", (float)pass / count * 100);
    printf("+================================================+\n");
}

/*================= 主函数 =================*/
int main()
{
    int choice;

    load();

    while (1)
    {
        menu();

        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addStudent();
            break;

        case 2:
            showStudent();
            break;

        case 3:
            searchStudent();
            break;

        case 4:
            updateStudent();
            break;

        case 5:
            deleteStudent();
            break;

        case 6:
            sortStudent();
            break;

        case 7:
            statistics();
            break;

        case 8:
            save();
            break;

        case 0:
            save();
            printf("\n感谢使用学生成绩管理系统！\n");
            exit(0);

        default:
            printf("输入错误，请重新输入！\n");
        }
    }

    return 0;
}