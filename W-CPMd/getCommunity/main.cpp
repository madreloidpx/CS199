#include <iostream>
#include "graph.h"

using namespace std;

int main(){
    Node n1, n2;
    Node* n = &n1;
    n->name = "A";
    n->weight = 0.3;
    n->next = &n2;
    n = n->next;
    n->name = "B";
    n->weight = 0.4;
    n->next = NULL;
    cout << n1.name;
    cout << n2.name;
    return 0;
}