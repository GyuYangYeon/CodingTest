#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
//////////////////////////////////////////////////////////////////////////
#include <cstring>
#include <list>

using namespace std;

const int MAX_HEAP = 11;
const int MAX_CUSTOMER = 25;
const int MAX_RESTAURANT = 7777;
#define MYABS(a) a<0?(-a):(a)
#define R register

struct CUSTOMER {
	int x, y;
	list<int> flist;
	
	int caldistance(int res_x, int res_y) {
		int cx = this->x - res_x;
		int cy = this->y - res_y;
		cx = MYABS(cx);
		cy = MYABS(cy);
		return cx + cy;
	}

	CUSTOMER() {
		this->x = this->y = 0;
		flist = list<int>();
	}

	void init() {
		this->x = this->y = 0;
		flist = list<int>();
	}
}customer[MAX_CUSTOMER];

#define ADD(n, nn)do{\
	n->nxt=nn;\
	n->prv=n->nxt->prv;\
\
	n->nxt->prv=n;\
	n->prv->nxt=n;\
}while(0);

#define DEL(n)do{\
	n->nxt->prv = n->prv;\
	n->prv->nxt = n->nxt;\
}while(0);

struct RESTAURANT {
	int pid;
	int x, y;
	RESTAURANT* nxt;
	RESTAURANT* prv;
	CUSTOMER cusOrder[MAX_CUSTOMER];

	RESTAURANT() {
		pid = x = y = 0;
		nxt = prv = 0;
		memset(cusOrder, 0, sizeof(cusOrder));
	}

	void init() {
		pid = x = y = 0;
		nxt = prv = 0;
		memset(cusOrder, 0, sizeof(cusOrder));
	}

}restaurant[MAX_RESTAURANT];
int restaurant_idx = 0;
RESTAURANT* rhead;
RESTAURANT* rtail;

RESTAURANT* getresnode() {
	return &restaurant[restaurant_idx++];
}

struct RES_HASHTABLE {
	int pid;
	RESTAURANT* nxt;

	RES_HASHTABLE() {
		pid = -1;
		nxt = 0;
	}

	void init() {
		pid = -1;
		nxt = 0;
	}
}hashtable[MAX_RESTAURANT];

int hashinsert(int pid) {
	int h = pid % MAX_RESTAURANT;
	while (hashtable[h].pid != -1) {
		h = (h + 1) % MAX_RESTAURANT;
	}
	return h;
}

void hashdel(int pid) {
	hashtable[pid].pid = -1;
}

int hashgetkey(int pid) {
	int h = pid % MAX_RESTAURANT;
	int cnt = MAX_RESTAURANT;

	while ((hashtable[h].pid != -1) && cnt--) {
		if (hashtable[h].pid = pid) {
			return h;
		}
		h = (h + 1) % MAX_RESTAURANT;
	}
	return h;
}

struct ORDER_SYSTEM {
	int pid;
	int distance;
	int delivery;
	int order[MAX_RESTAURANT];

	ORDER_SYSTEM(){
		pid = distance = delivery;
		memset(order, 0, sizeof(order));
	}

	void init() {
		pid = distance = delivery;
		memset(order, 0, sizeof(order));
	}
};

struct PQ {
	int idx;
	ORDER_SYSTEM order_system[MAX_CUSTOMER];
	int order[MAX_CUSTOMER];

	PQ() {
		idx = 0;
		memset(order_system, 0, sizeof(order_system));
		memset(order, 0, sizeof(order));
	}

	void init() {
		idx = 0;
		memset(order_system, 0, sizeof(order_system));
		memset(order, 0, sizeof(order));
	}

	bool compare(int a, int b) {
		if (order_system[a].delivery > order_system[b].delivery)
			return 1;
		else if ((order_system[a].delivery == order_system[b].delivery) && (order_system[a].distance < order_system[b].distance)) {
			return 1;
		}
		else if ((order_system[a].delivery == order_system[b].delivery) && (order_system[a].distance == order_system[b].distance) &&(order_system[a].pid < order_system[b].pid)) {
			return 1;
		}
		return 0;
	}

	void swap(ORDER_SYSTEM &a, ORDER_SYSTEM &b) {
		ORDER_SYSTEM tmp = a;
		a = b;
		b = tmp;

		int tmp_pid = order[a.pid];
		order[a.pid] = order[b.pid];
		order[b.pid] = tmp_pid;
	}

	void upheapify(int midx) {
		if (midx == 1)
			return;

		if (compare(midx / 2, midx)) {
			swap(order_system[midx / 2], order_system[midx]);
			upheapify(midx / 2);
		}
	}

	void push(ORDER_SYSTEM add) {
		order_system[++idx] = add;
		order[add.pid] = idx;
		upheapify(idx);
	}

	void downheapify(int midx) {
		if (midx * 2 > idx)
			return;

		//complete binary tree
		int tidx = 0;
		if (midx * 2 + 1 <= midx) {
			if (compare(midx * 2+1, midx * 2)) {
				tidx = midx * 2+1;
			}
			else {
				tidx = midx * 2;
			}
		}
		//only have one child
		else {
			tidx = midx * 2;
		}

		if (compare(tidx, midx)) {
			swap(order_system[tidx], order_system[midx]);
			downheapify(tidx);
		}
	}

	void pop() {
		ORDER_SYSTEM res = order_system[1];
		order_system[1] = order_system[idx];

		order[order_system[1].pid] = 0;
		order[order_system[idx].pid] = 1;

		order_system[idx].pid = order_system[idx].delivery = order_system[idx].distance = 0;
		downheapify(1);
	}

	ORDER_SYSTEM top() {
		return order_system[1];
	}

	void updateHeapNode(int pid, int mdelivery)
	{
		order_system[order[pid]].delivery = mdelivery;

		upheapify(order[pid]);
		downheapify(order[pid]);
	}

}pq[MAX_CUSTOMER];

void init(int N, int px[], int py[])
{
	//고객 정보 초기화 수행
	for (int i = 0; i < N; i++) {
		customer[i].init();
		customer[i].x = px[i];
		customer[i].y = py[i];

		//HEAP 초기화
		pq[i].init();;
	}

	//list & hashtable 초기화
	for (R int i = 0; i < MAX_RESTAURANT; i++) {
		if (restaurant[i].pid != 0) {
			restaurant[i].init();
		}

		hashtable[i].init();
	}
	
	restaurant_idx = 0;
	rhead = getresnode();
	rtail = getresnode();
	rhead->nxt = rtail;
	rtail->prv = rhead;

	
}

void AddHeapResInfo(int pID, int x, int y, int uID) {
	
	ORDER_SYSTEM addinfo;
	addinfo.distance = customer[uID].caldistance(x, y);
	addinfo.pid = pID;
	addinfo.delivery = 0;

	//10보다 작거나 같을때는 그냥 push 수행
	if (pq[uID].idx <= 10) {
		pq[uID].push(addinfo);
	}
	//10클 경우에는 10번째보다 우선순위가 높을때 pop 후에 push를 수행
	else {
		int check = 0;
		if (pq->top().delivery < addinfo.delivery)
			check = 1;
		else if ((pq->top().delivery == addinfo.delivery) && (pq->top().distance > addinfo.distance)) {
			check = 1;
		}
		else if ((pq->top().delivery == addinfo.delivery) && (pq->top().distance == addinfo.distance) && (pq->top().pid > addinfo.pid)) {
			check = 1;
		}

		if (check) {
			pq[uID].pop();
			pq[uID].push(addinfo);
		}
	}
}

void addRestaurant(int pID, int x, int y)
{
	// 새로운 음식점 리스트 및 해쉬 테이블 추가
	RESTAURANT* newres = getresnode();
	newres->x = x;
	newres->y = y;
	newres->pid = pID;

	//리스트 추가
	ADD(newres, rtail);

	//해쉬 테이블 추가
	int h = hashinsert(pID);
	hashtable[h].nxt = newres;
	hashtable[h].pid=pID;

	//고객 리스트에 haash 정보 업데이트
	for (R int i = 0; i < MAX_CUSTOMER; i++) {
		AddHeapResInfo(pID, x, y, i);
	}	
}

void removeRestaurant(int pID)
{

}

void order(int uID, int pID)
{

}

void beFriend(int uID1, int uID2)
{

}

int recommend(int uID)
{
	return 0;
}
//////////////////////////////////////////////////////////////////////////
#define INIT 100
#define ADD_RESTAURANT 200
#define REMOVE_RESTAURANT 300
#define ORDER 400
#define BE_FRIEND 500
#define RECOMMEND 600

static int px[30];
static int py[30];

static bool run()
{
	int query_num;
	scanf("%d", &query_num);

	int n = 0;
	bool ok = false;

	for (int q = 0; q < query_num; q++)
	{
		int query;
		scanf("%d", &query);

		if (query == INIT)
		{
			scanf("%d", &n);
			for (int i = 0; i < n; i++)
				scanf("%d", &px[i]);
			for (int i = 0; i < n; i++)
				scanf("%d", &py[i]);

			init(n, px, py);

			ok = true;
		}
		else if (query == ADD_RESTAURANT)
		{
			int pID, x, y;
			scanf("%d%d%d", &pID, &x, &y);

			addRestaurant(pID, x, y);
		}
		else if (query == REMOVE_RESTAURANT)
		{
			int pID;
			scanf("%d", &pID);

			removeRestaurant(pID);
		}
		else if (query == ORDER)
		{
			int uID, pID;
			scanf("%d%d", &uID, &pID);

			order(uID, pID);
		}
		else if (query == BE_FRIEND)
		{
			int uID1, uID2;
			scanf("%d%d", &uID1, &uID2);

			beFriend(uID1, uID2);
		}
		else if (query == RECOMMEND)
		{
			int uID, ans;
			scanf("%d %d", &uID, &ans);
			int ret = recommend(uID);
			if (ret != ans)
			{
				ok = false;
			}
		}
	}

	return ok;
}

int main()
{
	setbuf(stdout, NULL);
	//freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d%d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}
