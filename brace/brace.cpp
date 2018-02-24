#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <stack>

#define match(x) if (memcmp(s + key, x, len) == 0)
using namespace std;
char opt[] = ",+-*/";
char s[1000];
int varcnt;
stack<int> parm;
void filter(int l, int r, bool precmd, bool cutparm);

void keysolve(int key, int len)//执行函数
{
	++varcnt;
	match("read")
	{
		printf("set /p #%d=!#%d!&", varcnt, varcnt-1);
		return;
	}
	match("head")
	{
		printf("set #%d=&cls&@echo off&setlocal enabledelayedexpansion&rem ", varcnt);
		return;
	}
	match("pow")
	{
		int y = parm.top(); parm.pop();
		int x = parm.top(); parm.pop();
		printf("set /a #%d=1&(for /l %%%%z in (1,1,!#%d!) do set /a #%d*=#%d)&", varcnt, y,varcnt,x);
		return;
	}
	//解析错误
	printf("\nERROR\n");
	return;
}

void dfs(int l, int r)//解析表达式 
{
	int wrap = 0;//泡泡卷积层数 
	//向右结合从左扫，拆分表达式
	for (int j = r; j >= l; j -= 1)
	{
		//泡泡屏蔽 
		if (s[j] == '{') { wrap++; continue; }
		if (s[j] == '}') { wrap--; continue; }
		if (wrap) continue;
		//正常计算 
		for (int k = 0; k < (int)strlen(opt); k++)
			if (s[j] == opt[k])
			{
				int lvarcnt, rvarcnt;
				dfs(l, j - 1);lvarcnt = varcnt;
				dfs(j + 1, r);rvarcnt = varcnt;
				if (k) printf("set /a #%d=#%d%c#%d&", ++varcnt, lvarcnt, s[j], rvarcnt);
				return;
			}
	}
	//检测空表达式
	if (l - 1 == r)
	{
		printf("set #%d=&", ++varcnt);
		return;
	}
	//检测字符串
	if (s[l] == '"' && s[r] == '"')
	{
		printf("set #%d=", ++varcnt);
		for (int i = l + 1; i <= r - 1; i++) printf("%c", s[i]);
		printf("&");
		return;
	}
	//检测变量
	if (s[l] != '{' && s[r] != '}')
	{
		printf("set /a #%d=", ++varcnt);
		for (int i = l; i <= r; i++) printf("%c",s[i]);
		printf("&");
		return;
	}
	//检测函数
	if (s[l] != '{' && s[r] == '}')
	{
		for (int i = l; i <= r; i++)
		{
			if (s[i] == '{')
			{
				filter(i + 1, r - 1, 0, 1);
				keysolve(l, (i - 1) - l + 1);
				break;
			}
		}
		return;
	}
	//检测多括号并列
	if (s[l] == '{' && s[r] == '}')
	{
		filter(l, r, 1, 0);
		return;
	}
	//解析错误
	printf("\nERROR\n");
	return;
}

void filter(int l, int r, bool precmd, bool cutparm)//解析特别的表达式 
{
	string cmd;
	int op, ed, comma = l - 1;
	int wrap = 0;//泡泡卷积层数 
	//向右结合从左扫，拆分表达式
	for (int j = l; j <= r ; j += 1)
	{
		//泡泡屏蔽 
		if (s[j] == '{')
		{
			if (wrap == 0) op = j;
			wrap++;
			continue;
		}
		if (s[j] == '}')
		{
			wrap--;
			if (!cutparm&&wrap == 0)
			{
				ed = j;
				dfs(op + 1, ed - 1);
				cmd += "!#" + to_string(varcnt) + "!";
			}
			continue;
		}
		if (wrap) continue;
		if (!precmd&&!cutparm)
		{
			cmd += s[j];
			if (s[j] == '&')
			{
				cout << cmd;
				cmd = "";
			}
		}
		if (cutparm&&s[j] == ',')
		{
			dfs(comma + 1, j - 1);
			parm.push(varcnt);
			comma = j;
		}
	}
	//手动处理压栈最后一个参数，因为最后一个参数后没有逗号
	if (cutparm)
	{
		dfs(comma + 1, r);
		parm.push(varcnt);
	}
	//输出结果
	if (!cutparm)
	{
		if (precmd) printf("set #%d=", ++varcnt);
		cout << cmd;
		if (precmd) printf("&");
	}
	return;
}

int main(int argc, char *argv[])
{
	while (cin.getline(s, 1000))
	{
		varcnt = 0;
		filter(0, strlen(s) - 1, 0, 0);
		cout << endl;
	}
}