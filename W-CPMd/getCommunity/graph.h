//https://www.softwaretestinghelp.com/graph-implementation-cpp/#Types_of_Graphs_8211_Directed_And_Undirected_Graph

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <iostream>
#include "string"

using namespace std;

struct Node {
    string name;
    float weight;
    Node* next;

};

struct Edge {
    string start, end;
    float weight;
};

class Graph {
    private:
    int N; //number of nodes in the graph
    Node* createNode(string name, int weight, Node* head){
        Node* newNode = new Node;
        newNode -> name = name;
        newNode -> weight = weight;
        newNode -> next = head;
        return newNode;
    }
    Node nodeSearch(Node* head, int n, string node){
        for(int i = 0; i < n; i++){
            if(head[i].name == node)
                return head[i];
        }
    }

    public:
    Node** head;
    Graph(Edge edges[], int n, int N){
        head = new Node*[N]();
        this -> N = N;
        for(int i = 0; i < N; i++)
            head[i] = nullptr;
        for(int i = 0; i < n; i++){
            string start = edges[i].start;
            string end = edges[i].end; 
            Node startNode = nodeSearch(*head, n, start);
            float weight = edges[i].weight;
            //Node* newNode = createNode(end, weight, startNode);
        }
    }
};
#endif