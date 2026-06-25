#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#ifndef PROJECT_H
#define PROJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// 学生结构体
typedef struct
{
    int id;
    char name[20];
    float score;
    int rank;
} Student;

// 全局变量外部声明
extern Student stu[MAX];
extern int count;

// 所有函数声明
void load();
void save();
char* level(float score);
void menu();
void addStudent();
void showStudent();
void searchStudent();
void updateStudent();
void deleteStudent();
void sortStudent();
void statistics();

#endif