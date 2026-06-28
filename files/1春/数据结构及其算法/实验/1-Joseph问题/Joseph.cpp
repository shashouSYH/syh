#include <iostream>
using namespace std;

typedef struct LNode{
    int id;
    int password;
    LNode *next;
} LNode, *LinkList;

void Joseph(int n, int m, int passwords[]){
    if (n <= 0) return;
    LNode *head = new LNode;
    head->id = 1;
    head->password = passwords[0];
    LNode *prev = head;

    for (int i=2; i<=n; i++){
        LNode *newNode = new LNode;
        newNode->id = i;
        newNode->password = passwords[i-1];
        prev->next = newNode;
        prev = newNode;
    }
    prev->next = head;

    LNode *curr = head;
    LNode *pre = prev;

    cout << "出列顺序: ";
    while (curr->next != curr){
        for (int i=1; i<m; i++){
            pre = curr;
            curr = curr->next;
        }
        cout << curr->id << ", ";
        m = curr->password;
        pre->next = curr->next;
        LNode *temp = curr;
        curr = curr->next;
        delete temp;
    }
    cout << curr->id <<endl;
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
    int m;
    cout << "输入初始m: ";
    cin >> m;
    Joseph(n, m, passwords);
    return 0;
}