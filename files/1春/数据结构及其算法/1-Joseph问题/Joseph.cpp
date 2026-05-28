#include <iostream>

using namespace std;

typedef struct{
    int id;
    int password;
} ElemType;

typedef struct LNode{
    ElemType data;
    struct LNode *next;
} LNode, *LinkList;

void Joseph(int n, int m, int passwords[]){
    if (n <= 0) return;
    LinkList head = new LNode;
    head->data.id = 1;
    head->data.password = passwords[0];
    head->next = nullptr;
    LinkList prev = head;

    for (int i = 2; i <= n; i++){
        LinkList newNode = new LNode;
        newNode->data.id = i;
        newNode->data.password = passwords[i-1];
        prev->next = newNode;
        prev = newNode;
    }
    prev->next = head;

    LinkList curr = head;
    LinkList pre = prev;

    cout << "出列顺序: ";

    while (curr->next != curr){
        for (int i = 1; i < m; i++){
            pre = curr;
            curr = curr->next;
        }

        cout << curr->data.id << ", ";

        m = curr->data.password;

        pre->next = curr->next;
        LinkList temp = curr;
        curr = curr->next;
        delete temp;
    }

    cout << curr->data.id << endl;
    delete curr;
}

int main(){
    int n;
    cout << "输入n: ";
    cin >> n;
    cout << "输入数组: " << endl;
    int *passwords = new int[n];
    for (int i = 1; i <= n; i++){
        cin >> passwords[i-1];
    }
    int initial_m;
    cout << "输入初始m: ";
    cin >> initial_m;
    Joseph(n, initial_m, passwords);
    return 0;
}