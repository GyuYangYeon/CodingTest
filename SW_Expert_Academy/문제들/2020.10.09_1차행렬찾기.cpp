//20년도 하반기 신입 S/W 직무교육 사전학습 -1차 행렬찾기
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

typedef struct __TT{
	int x;
	int y;
	int sum;

}TEST;
#define MAX_REC 50
TEST Rec[MAX_REC];

typedef struct __XYPoint{
	int x;
	int y;
}XYPOINT;

//큐 FIFO
#define MAX_N 1000

int front;
int rear;
XYPOINT queue[MAX_N];
void queueInit(void)
{
	front = 0;
	rear = 0;
}
int queueIsEmpty(void)
{
	return (front == rear);
}
int queueIsFull(void)
{
	if ((rear + 1) % MAX_N == front)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int queueEnqueue(XYPOINT value)
{
	if (queueIsFull())
	{
		printf("queue is full!");
		return 0;
	}
	queue[rear] = value;
	rear++;
	if (rear == MAX_N)
	{
		rear = 0;
	}

	return 1;
}
int queueDequeue(XYPOINT *value)
{
	if (queueIsEmpty())
	{
		printf("queue is empty!");
		return 0;
	}
	*value = queue[front];
	front++;
	if (front == MAX_N)
	{
		front = 0;
	}
	return 1;
}

//맵 입력 및 소트 함수들
#define MAX_SIZE 105
char map[MAX_SIZE][MAX_SIZE] = { 0, };
char visited[MAX_SIZE][MAX_SIZE] = { 0, };
int wx[] = {1,0};
int wy[] = {0,1};

void Mapinit(int Size){
	for (int y = 0; y < Size; y++){
		for (int x = 0; x < Size; x++){
			map[y][x] = 0;
			visited[y][x] = 0;
		}
	}
}
void ArrayRecinit(){
	for (int i = 0; i < 50; i++){
		Rec[i].x = 0;
		Rec[i].y = 0;
		Rec[i].sum = 0;
	}
}
void quickSort_sum(int first, int last, TEST* pTarget)
{
	int pivot;
	int i;
	int j;
	TEST temp;

	if (first < last)
	{
		pivot = first;
		i = first;
		j = last;

		while (i < j)
		{
			while (pTarget[i].sum <= pTarget[pivot].sum && i < last)
			{
				i++;
			}
			while (pTarget[j].sum > pTarget[pivot].sum)
			{
				j--;
			}
			if (i < j)
			{
				temp = pTarget[i];
				pTarget[i] = pTarget[j];
				pTarget[j] = temp;
			}
		}

		temp = pTarget[pivot];
		pTarget[pivot] = pTarget[j];
		pTarget[j] = temp;

		quickSort_sum(first, j - 1, pTarget);
		quickSort_sum(j + 1, last, pTarget);
	}
}
void quickSort_y(int first, int last, TEST* pTarget)
{
	int pivot;
	int i;
	int j;
	TEST temp;

	if (first < last)
	{
		pivot = first;
		i = first;
		j = last;

		while (i < j)
		{
			while (pTarget[i].x <= pTarget[pivot].x && i < last)
			{
				i++;
			}
			while (pTarget[j].x > pTarget[pivot].x)
			{
				j--;
			}
			if (i < j)
			{
				temp = pTarget[i];
				pTarget[i] = pTarget[j];
				pTarget[j] = temp;
			}
		}

		temp = pTarget[pivot];
		pTarget[pivot] = pTarget[j];
		pTarget[j] = temp;

		quickSort_y(first, j - 1, pTarget);
		quickSort_y(j + 1, last, pTarget);
	}
}
void fsearch_same_val(int Len, TEST* pTarget){
	int cnt = 1;	
	int start = 0;
	int end = 0;

	for (int i = 0; i <= (Len - 1); i++){
		if (pTarget[i].sum == pTarget[i + 1].sum){
			cnt++;
		}
		if ((cnt != 1) && (pTarget[i].sum != pTarget[i + 1].sum)){
			start = i - (cnt - 1);
			end = start + (cnt-1);

			quickSort_y(start, end, pTarget);
			cnt = 1;
		}
		//마지막 항일경우
		if ((cnt != 1) && (i == (Len - 1))){
			start = i - (cnt-2);
			end = start + (cnt-1);

			quickSort_y(start, end, pTarget);
			cnt = 1;
		}

	}
}
int main(){

	int TC = 0;
	scanf("%d", &TC);

	for (int i = 1; i <= TC; i++){

		int Size = 0;
		scanf("%d", &Size);

		Mapinit(Size);
		ArrayRecinit();

		for (int y = 0; y < Size; y++){
			for (int x = 0; x < Size; x++)
			{
				scanf("%d", &map[y][x]);
			}
		}

		queueInit();
		int index = 0;
		for (int y = 0; y < Size; y++){
			for (int x = 0; x < Size; x++)
			{
				
				int direction = 0;
				
				int garo = 1;
				int sero = 1;

				//0이 아니고 방문한 적이 없다면
				if ((map[y][x] != 0) && (!visited[y][x])){
					XYPOINT tmp;
					tmp.x = x;
					tmp.y = y;
					visited[y][x] = 1;
					queueEnqueue(tmp);


					//0아닌 지점 오른쪽 아래쪽으로 탐색 시작
					while (!queueIsEmpty())
					{
						XYPOINT Cur;
						queueDequeue(&Cur);
						
						int nx = Cur.x + wx[direction];
						int ny = Cur.y + wy[direction];

						//판때기 안에 있고 방문한적이 없고 값이 들어 있을 경우만 통과
						if ((nx < 0) || (nx >= Size) || (ny < 0) || (ny >= Size)){
							if (direction == 0){
								direction++;
								queueEnqueue(Cur);
							}
							continue;
						}
						if (visited[ny][nx])
							continue;
						if ((map[ny][nx] == 0)){
							if (direction == 0){
								direction++;
								queueEnqueue(Cur);
							}
							continue;
						}

						XYPOINT Nxt;
						Nxt.x = nx;
						Nxt.y = ny;
						visited[ny][nx] = 1;
						if (direction == 0)
							garo++;
						else if (direction == 1)
							sero++;
						queueEnqueue(Nxt);

					}
					Rec[index].x = garo;
					Rec[index].y = sero;
					Rec[index].sum = garo*sero;
					index++;

					for (int o = y; o < y+sero; o++){
						for (int p = x; p < x+garo; p++){
							if (!visited[o][p]) visited[o][p] = 1;
						}
					}
				}
			}
		}
		//마지막 루프를 빠져나오면서 증가된 index 번호 감소 후 정렬 수행
		index--;
		quickSort_sum(0, (index), Rec);
		fsearch_same_val(index, Rec);

		printf("#%d %d ", i, index+1);
		for (int kk = 0; kk <= index; kk++){
			printf("%d %d ", Rec[kk].x, Rec[kk].y);
		}
		printf("\n");
	}
	
	
	
	
	

	return 0;
}