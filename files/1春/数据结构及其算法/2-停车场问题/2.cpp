#include <iostream>
#include <string>

using namespace std;

struct Car{
    int plate;
    int time;
};

// 停车场顺序栈

#define STACK_INIT_SIZE 10

struct SqStack{
    Car *elem;
    int stacksize;
    int top;
};

void InitStack_sq(SqStack &S, int msize = STACK_INIT_SIZE){
    S.elem = new Car[msize];
    S.stacksize = msize;
    S.top = -1;
}

bool IsFull_sq(SqStack &S){
    return S.top == S.stacksize - 1;
}

bool IsEmpty_sq(SqStack &S){
    return S.top == -1;
}

bool Push_sq(SqStack &S, Car e){
    if (IsFull_sq(S)) return false;
    S.top++;
    S.elem[S.top] = e;
    return true;
}

bool Pop_sq(SqStack &S, Car &e){
    if (IsEmpty_sq(S)) return false;
    e = S.elem[S.top];
    S.top--;
    return true;
}

// 便道链队列

struct QNode{
    Car data;
    QNode *next;
};

struct LinkQueue{
    QNode *front;
    QNode *rear;
    int length;
};

void InitQueue(LinkQueue &Q){
    Q.front = Q.rear = new QNode;
    Q.front->next = NULL;
    Q.length = 0;
}

bool IsEmptyQueue(LinkQueue &Q){
    return Q.front == Q.rear;
}

void EnQueue(LinkQueue &Q, Car e){
    QNode *p = new QNode;
    p->data = e;
    p->next = NULL;
    Q.rear->next = p;
    Q.rear = p;
    Q.length++;
}

bool DeQueue(LinkQueue &Q, Car &e){
    if (IsEmptyQueue(Q)) return false;
    QNode *p = Q.front->next;
    e = p->data;
    Q.front->next = p->next;
    if (Q.rear == p) Q.rear = Q.front;
    delete p;
    Q.length--;
    return true;
}

int main(){
    int n = 3;
    int m = 4;
    int price = 2;
    SqStack Lot;
    SqStack Temp;
    LinkQueue Wait;
    InitStack_sq(Lot, n);
    InitStack_sq(Temp, n);
    InitQueue(Wait);
    int plate, time;
    char ch;

    cout << "请输入操作命令 (格式例如 A 101 5, P 0 0, E 0 0):\n";

    while (cin >> ch >> plate >> time){
        
        if (ch == 'E') break;

        else if (ch == 'A'){
            Car c = {plate, time};
            if (!IsFull_sq(Lot)){
                Push_sq(Lot, c);
                cout << "车辆 " << plate << " 到达，停在停车场第 " << Lot.top + 1 << " 个位置\n";
            } else if (Wait.length < m){
                EnQueue(Wait, c);
                cout << "车辆 " << plate << " 到达，停在便道第 " << Wait.length << " 个位置\n";
            } else {
                cout << "停车场和便道均已满，车辆 " << plate << " 无法进入\n";
            }
        }

        else if (ch == 'D'){
            bool found = false;
            Car tempCar;
            while (!IsEmpty_sq(Lot)){
                Pop_sq(Lot, tempCar);
                if (tempCar.plate == plate){
                    found = true;
                    int stayTime = time - tempCar.time;
                    int fee = stayTime * price;
                    cout << "车辆 " << plate << " 离开停车场。停留时间: " << stayTime << "，应缴费用: " << fee << "\n";
                    break;
                } else {
                    Push_sq(Temp, tempCar);
                }
            }

            while (!IsEmpty_sq(Temp)){
                Pop_sq(Temp, tempCar);
                Push_sq(Lot, tempCar);
            }

            if (found){
                if (!IsEmptyQueue(Wait)){
                    Car enterCar;
                    DeQueue(Wait, enterCar);
                    enterCar.time = time;
                    Push_sq(Lot, enterCar);
                    cout << "便道上的车辆 " << enterCar.plate << " 进入停车场，当前位置: " << Lot.top + 1 << "\n";
                }
            } else {
                QNode* pre = Wait.front;
                QNode* p = pre->next;
                while (p != NULL){
                    if (p->data.plate == plate){
                        found = true;
                        pre->next = p->next;
                        if (p == Wait.rear) Wait.rear = pre;
                        delete p;
                        Wait.length--;
                        cout << "车辆 " << plate << " 从便道离开。停留时间不计，应缴费用: 0\n";
                        break;
                    }
                    pre = p;
                    p = p->next;
                }
                if (!found){
                    cout << "未找到车辆 " << plate << "\n";
                }
            }
        }

        else if (ch == 'P'){
            if (IsEmpty_sq(Lot)){
                cout << "当前停车场为空\n";
            } else {
                cout << "停车场车辆 (从大门到最里端): ";
                for (int i = Lot.top; i >= 0; i--) {
                    cout << Lot.elem[i].plate << " ";
                }
                cout << "\n";
            }
        }

        else if (ch == 'W'){
            if (IsEmptyQueue(Wait)) {
                cout << "当前便道为空\n";
            } else {
                cout << "便道车辆 (从前到后): ";
                QNode* pNode = Wait.front->next;
                while (pNode) {
                    cout << pNode->data.plate << " ";
                    pNode = pNode->next;
                }
                cout << "\n";
            }
        }
    }
    return 0;
}