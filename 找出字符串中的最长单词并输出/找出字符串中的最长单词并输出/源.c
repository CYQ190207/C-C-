#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
void search_and_print_max_len_word(char*str)
{
	char* p = str;
	int max_len = 0;
	int cur_len = 0;
	char* max_start = p;
	char* cur_start = p;
	while (*p != '\0')
	{
		// 每次读取单词前，先跳过空格
		while (*p == ' ' && *p != '\0')
		{
			p++;
		}
		if (*p == '\0') break;

		cur_start = p;
		cur_len = 0;
		while (*p != ' ' && *p != '\0')
		{
			cur_len++;
			p++;
		}
		if (cur_len > max_len)
		{
			max_start = cur_start;
			max_len = cur_len;
		}
	}
	char* out = max_start;
	while (*out != ' ' && *out != '\0')
	{
		putchar(*out);
		out++;
	}
}
int main()
{
	char str[100] = { 0 };
	gets(str);
	search_and_print_max_len_word(str);
	return 0;
}