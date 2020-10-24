#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>


int test_case, T, res, cmp[2], i, total_num, input_num;
int main()
{	
	scanf("%d", &T);

	for (test_case = 1; test_case <= T; ++test_case){
		scanf("%d", &total_num);
		scanf("%d", &res);

		for (i = 0; i<total_num - 1; i++){
			scanf("%d", &input_num);
			cmp[0] = res + input_num;
			cmp[1] = res * input_num;
			res = cmp[0]>cmp[1] ? cmp[0] : cmp[1];
		}
		printf("#%d %d\n", test_case, res);
	}
	return 0;
}
