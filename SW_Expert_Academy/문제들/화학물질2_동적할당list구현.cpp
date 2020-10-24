//[VH1]화학물질2 - list 구현
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

//리스트 구현
typedef struct _NODE{
	int ldata;
	int rdata;
	struct _NODE *next;
}NODE;
NODE* Head = (NODE*)malloc(sizeof(NODE));
NODE* Tail = NULL;
void Listinit(){
	Head->next = NULL;
	Tail = NULL;
}
void addList_front(NODE* HEAD, int Left, int Right){

	NODE* TMP = (NODE*)malloc(sizeof(NODE));
	TMP->ldata = Left;
	TMP->rdata = Right;

	TMP->next = HEAD->next;
	HEAD->next = TMP;
	if (TMP->next == NULL){
		Tail = Head->next;
	}
}
void addList_rear(int Left, int Right){

	NODE* TMP = (NODE*)malloc(sizeof(NODE));
	TMP->ldata = Left;
	TMP->rdata = Right;

	if (Tail == NULL){
		Head->next = TMP;
		TMP->next = NULL;
		Tail = TMP;
	}
	else{
		Tail->next = TMP;
		TMP->next = NULL;
		Tail = TMP;
	}
}
void freeList(){
	NODE* Cur = Head->next;
	while (Cur != NULL){
		NODE* TMP = Cur->next;
		free(Cur);
		Cur = TMP;
	}

}
//문제 풀이
typedef struct __TT{
	int y;
	int x;
	int sum;

}TEST;
#define MAX_REC 50
TEST Rec[MAX_REC];
typedef struct __XYPoint{
	int y;
	int x;
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
int wx[] = { 1, 0 };
int wy[] = { 0, 1 };

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

int main(){

	//테스트 케이스 입력받기
	int TC = 0;
	scanf("%d", &TC);

	for (int i = 1; i <= TC; i++){

		//맵 사이즈 입력받기
		int Size = 0;
		scanf("%d", &Size);

		//맵, 탐지 영역, 큐, 리스트 초기화
		Mapinit(Size);
		ArrayRecinit();
		queueInit();
		Listinit();

		//맵 사이즈에 따른 입력 받기
		for (int y = 0; y < Size; y++){
			for (int x = 0; x < Size; x++)
			{
				scanf("%d", &map[y][x]);
			}
		}

		//0이 아닌 Rec 탐지해서 가로, 세로 크기 계산하기 -> BFS 우하 탐색만 실시
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

					//가로 세로 크기에 따른 Visited 방문 표시 해주기
					for (int o = y; o < y + sero; o++){
						for (int p = x; p < x + garo; p++){
							if (!visited[o][p]) visited[o][p] = 1;
						}
					}
				}
			}
		}
		//마지막 루프를 빠져나오면서 증가된 index 번호 감소 후 정렬 수행
		index--;



		//오른쪽으로 배열 탐색 실시, 처음 값 넣어주고 탐색 실시 예정
		int std = 0;
		addList_rear(Rec[std].y, Rec[std].x);
		char node_flag[1000] = { 1, 0, };
		int flag = 1;
		while (flag){
			for (int search = 1; search <= index; search++)
			{
				if (node_flag[search]){
					if (search == index){
						flag = 0;
						break;
					}
					continue;
				}
				if (Rec[std].x == Rec[search].y){
					addList_rear(Rec[search].y, Rec[search].x);
					node_flag[search] = 1;
					std = search;
					if (search == index){
						flag = 0;
						break;
					}
					search = 1;
				}

				if (search == index){
					flag = 0;
					break;
				}
			}
		}

		//왼쪽으로 탐색 실시
		flag = 1;
		std = 0;
		while (flag){
			for (int search = 1; search <= index; search++)
			{
				if (node_flag[search]) {
					if (search == index){
						flag = 0;
						break;
					}
					continue;
				}
				if (Rec[std].y == Rec[search].x){
					addList_front(Head, Rec[search].y, Rec[search].x);
					node_flag[search] = 1;
					std = search;
					if (search == index){
						flag = 0;
						break;
					}
					search = 1;
				}

				if (search == index){
					flag = 0;
					break;
				}
			}
		}

		printf("#%d ", i);
		NODE* Cur = Head->next;
		int res = 0;
		int ccnt = 0;

		//Rec이 2개 이상이여야함, index는 0부터 시작임으로 1이면 2개임
		if (index >= 1){
			int firstValue = 0;
			while (Cur != NULL)
			{
				if (ccnt == 0){
					firstValue = Cur->ldata;
					ccnt++;
				}
				else
				{
					res += firstValue*Cur->ldata*Cur->rdata;
				}
				Cur = Cur->next;

			}
		}

		printf("%d\n", res);
		freeList();
	}






	return 0;
}