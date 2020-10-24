#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int booklist[200000] = { 0, };


int main(){
	int test_case;
	int T;
	int total_num;

	scanf("%d", &T);

	for (test_case = 1; test_case <= T; ++test_case)
	{
		int res = 1;
		scanf("%d", &total_num);
		int input = 0;
		for (int i = 0; i < total_num; i++)
		{
			scanf("%d", &input);
			booklist[input] = i;
		}
		for (int i = 1; i < total_num; i++)
			if (booklist[i]>booklist[i + 1])
				res++;
		printf("#%d %d\n", test_case, res);
	}
	return 0;
}
