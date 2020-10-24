#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define ABS(a) (a)<0?(-a):(a)

#define MAX_N 10000
int front;
int rear;
char queue[MAX_N];
#define VAL(a) ((a)>='0')&&((a)<='9')
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
int queueDequeue(int *value)
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
int queueSize(){
	int temp = front - rear;
	return ABS(temp);
}

#define MAX_NODE_NUM 1000
#define MAX_CHILD_NUM 2
#define LEFT 0
#define RIGTH 1
typedef struct
{
	int parent;
	char Value;
	int child[MAX_CHILD_NUM];
} TreeNode;
TreeNode tree[MAX_NODE_NUM];
int nodeNum;
int edgeNum;
int root;
void initTree(int val)
{
	int i;
	int j;
	for (i = 0; i <= val; i++)
	{
		tree[i].parent = -1;
		for (j = 0; j < MAX_CHILD_NUM; j++)
		{
			tree[i].child[j] = -1;
		}
	}
}
void addChild(int parent, int child)
{
	int i;
	for (i = 0; i < MAX_CHILD_NUM; i++)
	{
		if (tree[parent].child[i] == -1)
		{
			break;
		}
	}
	tree[parent].child[i] = child;
	tree[child].parent = parent;
}
int getRoot(int val)
{
	int i;
	int j;
	for (i = 1; i <= val; i++)
	{
		if (tree[i].parent == -1)
		{
			return i;
		}
	}
	return -1;
}
void parsing(char input[]){
	//문장별로 파싱 시작
	int index = 0;
	int chtoint = 0;
	int pNode = 0;
	int cNode_L = 0;
	int cNode_R = 0;

	//부모노드
	for (index = 0; input[index] != ' '; index++){
		chtoint *= 10;
		chtoint += input[index] - '0';
	}
	pNode = chtoint;

	//연산자 혹은 숫자
	while (input[index] == ' ')
		index++;
	tree[pNode].Value = input[index++];

	//만약 더이상 입력 값이 없다면 skip
	if (input[index] == 0){
		return;
	}

	//왼쪽 자식 노드
	while (input[index] == ' ')
		index++;
	chtoint = 0;
	for (index; (input[index] != ' ')&&(input[index]!= 0); index++){
		chtoint *= 10;
		chtoint += input[index] - '0';
	}
	cNode_L = chtoint;

	//만약 더이상 입력 값이 없다면 skip
	if (input[index] == 0){
		tree[pNode].child[LEFT] = cNode_L;
		tree[cNode_L].parent = pNode;
		return;
	}

	//오른쪽 자식 노드
	while (input[index] == ' ')
		index++;
	chtoint = 0;
	for (index; (input[index] != 0); index++){
		chtoint *= 10;
		chtoint += input[index] - '0';
	}
	cNode_R = chtoint;


	//파싱된 값 Update
	tree[pNode].child[LEFT] = cNode_L;
	tree[pNode].child[RIGTH] = cNode_R;
	tree[cNode_L].parent = pNode;
	tree[cNode_R].parent = pNode;
}

int Res = 1;
void BackOrder(int root)
{
	if (!Res) return;
	int i;
	int child;
	if (tree[root].child[LEFT] > 0){
		BackOrder(tree[root].child[LEFT]);
	}
	if (tree[root].child[RIGTH] > 0){
		BackOrder(tree[root].child[RIGTH]);
	}

	//숫자일 경우 그냥 파싱
	if (VAL(tree[root].Value)){
		queueEnqueue(tree[root].Value);
	}
	//연산자일 경우
	else{
		//누적된 숫자가 없거나 1개일때 연산자가 올경우 계산 불가
		if (queueSize() <= 1){
			Res = 0;
			return;
		}
		//누적된 숫자가 2개 이상일 경우
		int tmp;
		queueDequeue(&tmp);
	}
}

int main(void)
{
	for (int q = 1; q <= 10; q++){
		int Node = 0;
		scanf(" %d", &Node);
		queueInit();
		initTree(Node);
		for (int i = 0; i < Node; i++){
			char input[14] = { 0 };
			scanf(" %[^\n]s", input);
			parsing(input);

			int kkkk;
		}
		int lootNode = getRoot(Node);
		BackOrder(lootNode);

		if (Res){
			if (queueSize() != 1) Res = 0;
			if (!VAL(queue[front])) Res = 0;
		}

		printf("#%d %d\n", q, Res);
		Res = 1;
	}
	return 0;
}