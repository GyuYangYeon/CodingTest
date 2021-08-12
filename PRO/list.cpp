#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define MAX_NODE 10000

enum { ADD_HEAD = 1, ADD_TAIL, ADD_NUM, FIND, REMOVE, PRINT, PRINT_REVERSE, END = 99 };

///////////////////////////////////////////////////////////////////////////////////////
#define MAX_NODE 10000

struct Node {
	int data;
	Node* prev;
	Node* next;
};

Node node[MAX_NODE];
int nodeCnt;

struct List {
	Node Head;
	Node Tail;

	static void Link(Node* front, Node* back) {
		front->next = back;
		back->prev = front;
	}

	static void erase(Node* n) {
		Link(n->prev, n->next);
	}

	void init() {
		Link(&Head, &Tail);
	}

	void insert_tail(Node* n) {
		Link(Tail.prev, n);
		Link(n, &Tail);
	}

	void insert_head(Node* n) {
		Link(n, Head.next);
		Link(&Head, n);
	}

	void insert_middle(Node* nth, Node* n) {
		Link(nth->prev, n);
		Link(n, nth);
	}

	bool isEmpty() {
		return (Head.next == &Tail);
	}

	void slice(List * n){
		Link(Tail.prev, n->Head.next);
		Link(n->Tail.prev, &Tail);
		n->init();
	}
};
List lst[3];
Node* getNode(int data) {
	node[nodeCnt].data = data;
	node[nodeCnt].prev = nullptr;
	node[nodeCnt].next = nullptr;
	return &node[nodeCnt++];
}

void init()
{
	lst[0].init();
	nodeCnt = 0;
}

void addNode2Head(int data)
{
	Node* n = getNode(data);
	lst[0].insert_head(n);

}

void addNode2Tail(int data)
{
	Node* n = getNode(data);
	lst[0].insert_tail(n);

}

void addNode2Num(int data, int num)
{
	Node* search = &lst[0].Head;
	for (int i = 0; i < num; i++) {
		search = search->next;
	}
	Node* n = getNode(data);
	lst[0].insert_middle(search, n);

}

int findNode(int data)
{

	Node* search = &lst[0].Head;
	int res = 0;
	while (lst[0].Head.next != &lst[0].Tail) {
		if (search->data == data) {
			break;
		}
		res++;
		search = search->next;
	}

	return res;
}

void removeNode(int data)
{
	Node* search = lst[0].Head.next;
	while (search->next != &lst[0].Tail) {

		if (search->data == data) {
			lst[0].erase(search);
			break;
		}
		search = search->next;
	}
}

int getList(int output[MAX_NODE])
{
	int res = 0;
	for (Node* n = &lst[0].Head; n != &lst[0].Tail; n = n->next){
		if (res == 0) {
			res++;
			continue;
		}
		output[res++ - 1] = n->data;
	}

	if (--res < 0)
		res = 0;

	return res;
}

int getReversedList(int output[MAX_NODE])
{
	int res = 0;
	for (Node* n = &lst[0].Tail; n != &lst[0].Head; n = n->prev) {
		if (res == 0) {
			res++;
			continue;
		}
		output[res++ - 1] = n->data;
	}

	if (--res < 0)
		res = 0;

	return res;
}

///////////////////////////////////////////////////////////////////////////////////////

static void run() {
	while (1) {
		int cmd, data, num, cnt, i = 0;
		int output[MAX_NODE] = { 0 };

		scanf("%d", &cmd);
		switch (cmd) {
		case ADD_HEAD: // 1
			scanf("%d", &data);
			addNode2Head(data);
			break;
		case ADD_TAIL: // 2
			scanf("%d", &data);
			addNode2Tail(data);
			break;
		case ADD_NUM: // 3
			scanf("%d %d", &data, &num);
			addNode2Num(data, num);
			break;
		case FIND: // 4
			scanf("%d", &data);
			num = findNode(data);
			printf("%d\n", num);
			break;
		case REMOVE: // 5
			scanf("%d", &data);
			removeNode(data);
			break;
		case PRINT: // 6
			cnt = getList(output);
			while (cnt--)
				printf("%d ", output[i++]);
			printf("\n");
			break;
		case PRINT_REVERSE: // 7
			cnt = getReversedList(output);
			while (cnt--)
				printf("%d ", output[i++]);
			printf("\n");
			break;
		case END:
			return;
		}
	}
}

int main(void) {

	init();
	addNode2Head(5);
	addNode2Head(4);
	addNode2Head(3);
	addNode2Head(2);
	addNode2Head(1);

	lst[1].init();
	lst[1].slice(&lst[0]);

	addNode2Head(10);
	addNode2Head(9);
	addNode2Head(8);
	addNode2Head(7);
	addNode2Head(6);
	lst[1].slice(&lst[0]);



	setbuf(stdout, NULL);
	freopen("dll_input.txt", "r", stdin);

	int T;
	scanf("%d", &T);
	for (int t = 1; t <= T; t++) {
		printf("#%d\n", t);
		init();
		run();
		printf("\n");
	}

	return 0;
}