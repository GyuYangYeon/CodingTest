//2020.10.24 [H2]작업순서

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define NODE_MAX 1005
#define MAX_N 1005



int front;
int rear;
int queue[MAX_N];
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
int queueEnqueue(int value)
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
int queueDequeue(int* value)
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



//트리 구조체 선언
typedef struct _Tree{



	int parent[NODE_MAX];
	int child[NODE_MAX];
	int child_num;
	int parent_cnt;
	int parent_max;

	_Tree() {
		child_num = 0;
		parent_cnt = 0;
		parent_max = 0;
	}

}TREE;



TREE Tree[NODE_MAX];



void find_leafs(int node) {



	for (int i = 1; i <= node; i++)
	{
		if (Tree[i].child_num == 0) {
			queueEnqueue(i);
		}
	}



}



char visited[NODE_MAX];
void visit_map_init(int size) {
	for (int i = 1; i <= size; i++) {
		visited[i] = 0;
	}
}



void DFS(int point) {


	//방문한적이 있으면 리턴
	if (visited[point]) return;
	visited[point] = 1;

	int parent_cnt = Tree[point].parent_max;

	for (int i = 0; i < parent_cnt; i++) {

		//방문한적이 있는 노드면 스킵
		if (visited[Tree[point].parent[i]]) continue;

		//방문하지 않았으면 방문시작
		DFS(Tree[point].parent[i]);

	}
	printf("%d ", point);
}

void Tree_init(int size){

	for (int i = 1; i <= size; i++) {
		for (int k = 0; k < Tree[i].child_num; k++)
		{
			Tree[i].child[k] = 0;
		}

		for (int k = 0; k < Tree[i].parent_max; k++)
		{
			Tree[i].parent[k] = 0;
		}

		Tree[i].child_num = 0;
		Tree[i].parent_cnt = 0;
		Tree[i].parent_max = 0;
	}
}

int main() {



	for (int TC = 1; TC <= 10; TC++) {
		int Node = 0;
		int Edge = 0;



		scanf("%d %d", &Node, &Edge);



		queueInit();
		visit_map_init(Node);
		Tree_init(Node);

		for (int n = 0; n < Edge; n++) {
			int start, destiny;
			scanf("%d %d", &start, &destiny);

			//부모의 자식 노드 넣어주기
			Tree[start].child[Tree[start].child_num++] = destiny;

			//자식의 부모노드가 이미 들어와 있는게 있으면
			//인덱스 찾고 부모 노드값 넣기
			if (Tree[destiny].parent[0] != 0) {
				Tree[destiny].parent_max++;

				int index = 0;
				for (int i = 1; i <= Node; i++) {
					if (Tree[destiny].parent[i] == 0) {
						index = i;
						break;
					}
				}
				Tree[destiny].parent[index] = start;
			}
			//자식의 부모노드 Pool이 비워져 있을경우
			//그냥 부모노드의 값 넣기
			else {
				Tree[destiny].parent[0] = start;
				Tree[destiny].parent_max++;
			}
		}



		find_leafs(Node);


		printf("#%d ", TC);
		while (!queueIsEmpty())
		{
			int a;
			queueDequeue(&a);
			DFS(a);
		}
		printf("\n");


		int a = 0;
	}



	return 0;
}