#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

/////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <vector>
#define MAX 20017
#define R register
using namespace std;

int totalfilesize = 0;
#define ADD(n, nn) do{\
	n->nxt = nn;\
	n->prv = n->nxt->prv;\
	\
	n->nxt->prv = n;\
	n->prv->nxt = n;\
}while(0)

#define DEL(n) do{\
	n->nxt->prv = n->prv;\
	n->prv->nxt = n->nxt;\
}while(0)

int fidx = 0;
struct FILEINFO
{
	int start, size, key;
	FILEINFO* nxt;
	FILEINFO* prv;
}nfile[MAX];
FILEINFO* fhead, * ftail;
FILEINFO* fgetitem(int mstart=0, int msize=0, int mkey=0) {
	nfile[fidx].start = mstart;
	nfile[fidx].size = msize;
	nfile[fidx].key = mkey;
	return &nfile[fidx++];
}

int eidx = 0;
struct EMPTYINFO {
	int start, size;
	EMPTYINFO* nxt;
	EMPTYINFO* prv;
}nempty[MAX];
EMPTYINFO* ehead, * etail;
EMPTYINFO* egetitem(int mstart=0, int msize=0) {
	nempty[eidx].start = mstart;
	nempty[eidx].size = msize;
	return &nempty[eidx++];
}

struct HASHTABLE {
	int key;
	FILEINFO* fp;
}Hash[MAX];

int inserthash(int key) {
	int h = key % MAX;
	while (Hash[h].key != -1) {
		h = (h + 1) % MAX;
	}

	return h;
}
void delhashkey(int key) {
	Hash[key].key = -1;
}

int gethashkey(int key) {
	int h = key % MAX;
	int cnt = MAX;
	while ((Hash[h].key != -1) && cnt--) {
		if (Hash[h].key == key)
			return h;
		h = (h + 1) % MAX;
	}
	return -1;
}

void init(int N) {
	//init file size
	totalfilesize = 0;

	//init list
	fidx = eidx = 0;
	fhead = fgetitem();
	ftail = fgetitem();
	ehead = egetitem();
	etail = egetitem();
	fhead->nxt = ftail;
	ftail->prv = fhead;
	ehead->nxt = etail;
	etail->prv = ehead;

	//init hash table
	for (R int i = 0; i < MAX; i++) {
		if(Hash[i].key!=-1)
			Hash[i].key = -1;
	}

	//Add empty info in initial state
	EMPTYINFO* newnode = egetitem(0, N);
	ADD(newnode, etail);

	return;
}

EMPTYINFO* searchEmptySpace(int fileSize) {
	for (R EMPTYINFO* snode = ehead->nxt; snode != etail; snode=snode->nxt) {
		if (snode->size >= fileSize) {
			return snode;
		}
	}
	return 0;
}

int access(int fileId, int fileSize) {
	int res_start = 0;

	//if the file is already registered,
	int h;
	if ((h=gethashkey(fileId))!=-1) {
		//delete the fileinfo and add in tail
		DEL(Hash[h].fp);
		ADD(Hash[h].fp, ftail);
		res_start = Hash[h].fp->start;
	}
	//if the file is not registered,
	else {
		EMPTYINFO* sNode = searchEmptySpace(fileSize);
		//if there is empty space,
		if (sNode != 0) {
			//FILEINFO 리스트에 업데이트 & empty 테이블 업데이트 & 토탈사이즈 업데이트 & 해쉬 테이블 등록
			FILEINFO* newnode = fgetitem(sNode->start, fileSize, fileId);
			res_start = newnode->start;
			ADD(newnode, ftail);

			sNode->start += fileSize;
			sNode->size -= fileSize;

			if (sNode->size == 0)
				DEL(sNode);

			totalfilesize += fileSize;

			int h = inserthash(fileId);
			Hash[h].key = fileId;
			Hash[h].fp = newnode;
		}
		else {
			//빈 공간 나올떄까지 sNode 삭제 수행
			while (sNode == 0) {
				
				//LRU 삭제 및 캐쉬사용량 업데이트
				FILEINFO* delnode = fhead->nxt;
				DEL(delnode);
				int h = gethashkey(delnode->key);
				delhashkey(h);

				totalfilesize -= delnode->size;

				//empty space 찾고 업데이트 수행 만약 다음 공간이 비어져 있을 경우 merge 수행
				//empty space에서 삽입할 공간 찾기&삭제된 노드 추가				
				R EMPTYINFO* searchNode;
				for (searchNode = ehead->nxt; searchNode != etail; searchNode = searchNode->nxt) {
					if (delnode->start < searchNode->start) {
						break;
					}
				}
				
				R EMPTYINFO* newenode = egetitem(delnode->start, delnode->size);
				ADD(newenode, searchNode);

				if((newenode->prv->start + newenode->prv->size == newenode->start)&&(newenode->prv != ehead)){
					newenode->prv->size += newenode->size;
					newenode = newenode->prv;
					DEL(newenode->nxt);
					
				}
				if ((newenode->start + newenode->size == newenode->nxt->start)&& (newenode->nxt!= etail)) {
					newenode->size += newenode->nxt->size;
					DEL(newenode->nxt);
				}


				/*if ((delnode->start + delnode->size == searchNode->start)) {
					newenode->size += searchNode->size;
					ADD(newenode, searchNode);
					DEL(searchNode);
				}

				//다음이 빈공간이 아니면 그냥 업데이트
				else {
					ADD(newenode, searchNode);
				}
				*/

				if (newenode->size >= fileSize) {
					sNode = newenode;
				}
			}

			//빈공간을 찾아서 sNode로 가지고 온 경우 임으로 업데이트 수행
			//FILEINFO 리스트에 업데이트 & empty 테이블 업데이트 & 토탈사이즈 업데이트 & 해쉬 테이블 등록
			FILEINFO* newnode = fgetitem(sNode->start, fileSize, fileId);
			ADD(newnode, ftail);
			res_start = newnode->start;

			sNode->start += fileSize;
			sNode->size -= fileSize;

			totalfilesize += fileSize;

			int h = inserthash(fileId);
			Hash[h].key = fileId;
			Hash[h].fp = newnode;
		}
	}

	return res_start;
}

int usage() {
	return totalfilesize;
}

/////////////////////////////////////////////////////////////////////////
#define CMD_INIT 1
#define CMD_ACCESS 2
#define CMD_USAGE 3

static bool run() {
	int q;
	scanf("%d", &q);

	int n, fileId, fileSize;
	int cmd, ans, ret = 0;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
		case CMD_INIT:
			scanf("%d", &n);
			init(n);
			okay = true;
			break;
		case CMD_ACCESS:			
			scanf("%d %d %d", &fileId, &fileSize, &ans);	
			ret = access(fileId, fileSize);
			if (ans != ret)
				okay = false;
			break;
		case CMD_USAGE:
			ret = usage();
			scanf("%d", &ans);
			if (ans != ret)
				okay = false;
			break;
		default:
			okay = false;
			break;
		}
	}
	return okay;
}

int main() {
	setbuf(stdout, NULL);
	freopen("sample_H2129_FILE_CASH.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++) {
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}
