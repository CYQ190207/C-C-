#define _CRT_SECURE_NO_WARNINGS 1
#include "project.h"

int main()
{
    int choice;
    load();
    while (1)
    {
        menu();//操作界面
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            addStudent();//添加新学生信息到数组
            break;
        case 2:
            showStudent();//遍历打印全部学生信息
            break;
        case 3:
            searchStudent();//根据学号查询单个学生详细信息
            break;
        case 4:
            updateStudent();//根据学号修改指定学生的成绩
            break;
        case 5:
            deleteStudent();//根据学号删除对应学生数据
            break;
        case 6:
            sortStudent();//按成绩降序冒泡排序，生成排名并打印排序结果
            break;
        case 7:
            statistics();//班级成绩统计：总分、平均分、最高最低分、及格人数及格率
            break;
        case 8:
            save();//保存学生成绩的数据
            break;
        case 0:
            save();//保存学生成绩的数据
            printf("\n感谢使用学生成绩管理系统！\n");
            exit(0);
        default:
            printf("输入错误，请重新输入！\n");
        }
    }
    return 0;
}