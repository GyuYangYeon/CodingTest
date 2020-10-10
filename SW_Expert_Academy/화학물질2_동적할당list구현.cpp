//[VH1]ȭ�й���2 - list ����
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

//����Ʈ ����
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
//���� Ǯ��
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

//ť FIFO
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

//�� �Է� �� ��Ʈ �Լ���
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

	//�׽�Ʈ ���̽� �Է¹ޱ�
	int TC = 0;
	scanf("%d", &TC);

	for (int i = 1; i <= TC; i++){

		//�� ������ �Է¹ޱ�
		int Size = 0;
		scanf("%d", &Size);

		//��, Ž�� ����, ť, ����Ʈ �ʱ�ȭ
		Mapinit(Size);
		ArrayRecinit();
		queueInit();
		Listinit();

		//�� ����� ���� �Է� �ޱ�
		for (int y = 0; y < Size; y++){
			for (int x = 0; x < Size; x++)
			{
				scanf("%d", &map[y][x]);
			}
		}

		//0�� �ƴ� Rec Ž���ؼ� ����, ���� ũ�� ����ϱ� -> BFS ���� Ž���� �ǽ�
		int index = 0;
		for (int y = 0; y < Size; y++){
			for (int x = 0; x < Size; x++)
			{

				int direction = 0;

				int garo = 1;
				int sero = 1;

				//0�� �ƴϰ� �湮�� ���� ���ٸ�
				if ((map[y][x] != 0) && (!visited[y][x])){
					XYPOINT tmp;
					tmp.x = x;
					tmp.y = y;
					visited[y][x] = 1;
					queueEnqueue(tmp);


					//0�ƴ� ���� ������ �Ʒ������� Ž�� ����
					while (!queueIsEmpty())
					{
						XYPOINT Cur;
						queueDequeue(&Cur);

						int nx = Cur.x + wx[direction];
						int ny = Cur.y + wy[direction];

						//�Ƕ��� �ȿ� �ְ� �湮������ ���� ���� ��� ���� ��츸 ���
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

					//���� ���� ũ�⿡ ���� Visited �湮 ǥ�� ���ֱ�
					for (int o = y; o < y + sero; o++){
						for (int p = x; p < x + garo; p++){
							if (!visited[o][p]) visited[o][p] = 1;
						}
					}
				}
			}
		}
		//������ ������ ���������鼭 ������ index ��ȣ ���� �� ���� ����
		index--;



		//���������� �迭 Ž�� �ǽ�, ó�� �� �־��ְ� Ž�� �ǽ� ����
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

		//�������� Ž�� �ǽ�
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

		//Rec�� 2�� �̻��̿�����, index�� 0���� ���������� 1�̸� 2����
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