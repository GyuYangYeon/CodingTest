#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT 1
#define CMD_BUY 2
#define CMD_SELL 3
#define CMD_CANCEL 4
#define CMD_BEST_PROFIT 5

extern void init();
extern int buy(int mNumber, int mStock, int mQuantity, int mPrice);
extern int sell(int mNumber, int mStock, int mQuantity, int mPrice);
extern void cancel(int mNumber);
extern int bestProfit(int mStock);


bool DEBUG_MODE = 0;

/////////////////////////////////////////////////////////////////////////
#include <queue>
#include <cstring>
using namespace std;

#define MAX_CALL 200001
#define MAX_BUY 100001
#define MAX_SELL 100001
#define MAX_STOCK 6

template<typename T, int sz>
struct POOL {
    T Arr[sz];
    int idx;

    POOL() {
        idx = 0;
    }

    T* getitem() {
        return &Arr[idx++];
    }
};

struct NODE {
    int mNumber;
    int mQunatity;
    int mPrice;

    NODE() {
        mNumber = mQunatity = mPrice = 0;
    }

    void update(int number, int quantity, int price) {
        mNumber = number;
        mQunatity = quantity;
        mPrice = price;
    }

    void Deal(NODE* node) {
        int quantity = this->mQunatity < node->mQunatity ? this->mQunatity : node->mQunatity;
        this->mQunatity -= quantity;
        node->mQunatity -= quantity;

        if (DEBUG_MODE) {
            printf("[%d, %d] : %d개 체결 ", this->mNumber, node->mNumber, quantity);
        }
    }
};

struct buycmp {
    bool operator()(NODE*a, NODE*b) {
        if (a->mPrice < b->mPrice)
            return 1;
        if (a->mPrice > b->mPrice)
            return 0;
        return (a->mNumber > b->mNumber);
    }
};
struct sellcmp {
    bool operator()(NODE* a, NODE* b) {
        if (a->mPrice > b->mPrice)
            return 1;
        if (a->mPrice < b->mPrice)
            return 0;
        return (a->mNumber > b->mNumber);
    }
};

struct INFO {
    int mVal;
    int mTime;

    void updateVal(int val, int time) {
        mVal = val;
        mTime = time;
    }
};

struct BESTDEAL {
    INFO mMax;
    INFO mMin;
    int mMaxGap;
    int mTime;

    BESTDEAL() {
        this->mTime = this->mMaxGap = 0;
        this->mMax.updateVal(0, this->mTime);
        this->mMin.updateVal(1000001, this->mTime);
    }

    void tracking(int Price) {
        this->mTime++;
        if (this->mMax.mVal < Price) {
            this->mMax.updateVal(Price, this->mTime);
        }

        int tmpProit = this->mMax.mVal - this->mMin.mVal;
        if (tmpProit > this->mMaxGap) {
            this->mMaxGap = tmpProit;
        }

        if (this->mMin.mVal > Price) {
            this->mMin.updateVal(Price, this->mTime);
            this->mMax.updateVal(0, this->mTime);
        }
    }
};

POOL<NODE, MAX_BUY> poolBuy;
POOL<NODE, MAX_SELL> poolSell;
priority_queue<NODE*, vector<NODE*>, buycmp> pqBuy[MAX_STOCK];
priority_queue<NODE*, vector<NODE*>, sellcmp> pqSell[MAX_STOCK];
BESTDEAL bestDeal[MAX_STOCK];
bool cancled[MAX_CALL];

void init()
{
    poolBuy.idx = poolSell.idx = 0;
    for(int i=1; i< MAX_STOCK; i++){
        while (!pqBuy[i].empty()) {
            pqBuy[i].pop();
        }
        while (!pqSell[i].empty()) {
            pqSell[i].pop();
        }
    }
    for (int i = 1; i <= 5; i++) {
        bestDeal[i] = BESTDEAL();
    }
    memset(cancled, 0, sizeof(cancled));
}

int buy(int mNumber, int mStock, int mQuantity, int mPrice)
{

    NODE* buy = poolBuy.getitem();
    buy->update(mNumber, mQuantity, mPrice);

    while (!pqSell[mStock].empty()) {
        NODE* sell = pqSell[mStock].top();

        if (sell->mPrice > buy->mPrice)
            break;

        //in case of quantity ==0, pop and skip it.
        if ((cancled[sell->mNumber] == 1) || sell->mQunatity == 0) {
            pqSell[mStock].pop();
            continue;
        }

        //deal with sell
        buy->Deal(sell);
        bestDeal[mStock].tracking(sell->mPrice);

        if (DEBUG_MODE) {
            printf("가격 %d\n", sell->mPrice);
        }

        if (buy->mQunatity == 0)
            break;

    }

    if (buy->mQunatity != 0)
        pqBuy[mStock].push(buy);

    return buy->mQunatity;
}

int sell(int mNumber, int mStock, int mQuantity, int mPrice)
{
    NODE* sell = poolBuy.getitem();
    sell->update(mNumber, mQuantity, mPrice);

    while (!pqBuy[mStock].empty()) {
        NODE* buy = pqBuy[mStock].top();

        if (buy->mPrice < sell->mPrice)
            break;

        //in case of quantity ==0, pop and skip it.
        if ((cancled[buy->mNumber] == 1) || buy->mQunatity == 0) {
            pqBuy[mStock].pop();
            continue;
        }

        //deal with sell
        sell->Deal(buy);
        bestDeal[mStock].tracking(buy->mPrice);


        if (DEBUG_MODE) {
            printf("가격 %d\n", buy->mPrice);
        }

        if (sell->mQunatity == 0)
            break;

    }

    if (sell->mQunatity != 0)
        pqSell[mStock].push(sell);

    return sell->mQunatity;
}

void cancel(int mNumber)
{
    cancled[mNumber] = 1;
}

int bestProfit(int mStock)
{
    return bestDeal[mStock].mMaxGap;
}

/////////////////////////////////////////////////////////////////////////

void print_debug() {

    for(int i=1; i<MAX_STOCK; i++){
        if (!pqBuy[i].empty()) {
            printf("stock %d -> pqBuy : mNum %d mPrice %d mQuantity %d\n\n", i, pqBuy[i].top()->mNumber, pqBuy[i].top()->mPrice, pqBuy[i].top()->mQunatity);
        }

        if (!pqSell[i].empty()) {
            printf("stock %d -> pqSell : mNum %d mPrice %d mQuantity %d\n\n", i, pqSell[i].top()->mNumber, pqSell[i].top()->mPrice, pqSell[i].top()->mQunatity);
        }

        for (int i = 1; i <= 5; i++) {
            printf("mstock : %d , max %d, min %d, bestprofit : %d\n", i, bestDeal[i].mMax.mVal, bestDeal[i].mMin.mVal, bestDeal[i].mMaxGap);
        }
    }
    printf("\n");
}

static bool run()
{
    int numQuery;

    int mNumber, mStock, mQuantity, mPrice;

    int userAns, ans;

    bool isCorrect = false;

    scanf("%d", &numQuery);

    for (int i = 0; i < numQuery; ++i)
    {
        if (numQuery == 276)
        {
            int a = 1;
        }

        if (numQuery == 276 && i == 102)
        {
            int a = 1;
        }
        int cmd;
        scanf("%d", &cmd);
        
        switch (cmd)
        {
        case CMD_INIT:
            init();
            isCorrect = true;
            break;
        case CMD_BUY:
            scanf("%d %d %d %d", &mNumber, &mStock, &mQuantity, &mPrice);
            userAns = buy(mNumber, mStock, mQuantity, mPrice);
            scanf("%d", &ans);
            if (userAns != ans)
            {
                isCorrect = false;
            }
            break;
        case CMD_SELL:
            scanf("%d %d %d %d", &mNumber, &mStock, &mQuantity, &mPrice);
            userAns = sell(mNumber, mStock, mQuantity, mPrice);
            scanf("%d", &ans);
            if (userAns != ans)
            {
                isCorrect = false;
            }
            break;
        case CMD_CANCEL:
            scanf("%d", &mNumber);
            cancel(mNumber);
            break;
        case CMD_BEST_PROFIT:
            scanf("%d", &mStock);
            userAns = bestProfit(mStock);
            scanf("%d", &ans);
            if (userAns != ans)
            {
                isCorrect = false;
            }
            break;
        default:
            isCorrect = false;
            break;
        }
    }

    return isCorrect;
}

int main()
{
    setbuf(stdout, NULL);
    freopen("sample_25_input.txt", "r", stdin);

    int T, MARK;
    scanf("%d %d", &T, &MARK);

    for (int tc = 1; tc <= T; tc++)
    {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }

    return 0;
}