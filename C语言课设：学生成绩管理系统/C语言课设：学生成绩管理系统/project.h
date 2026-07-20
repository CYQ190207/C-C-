#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#ifndef PROJECT_H
#define PROJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100     //最大存储学生信息的数量

// 学生结构体
typedef struct
{
    int id;             //学号
    char name[20];      //姓名
    float score;//得分
    int rank;//排名
} Student;

// 全局变量外部声明
extern Student stu[MAX];       
extern int count;

// 所有函数声明
void load();
void menu();
void save();
char* level(float score);
void addStudent();
void showStudent();
void searchStudent();
void updateStudent();
void deleteStudent();
void sortStudent();
void statistics();

#endif