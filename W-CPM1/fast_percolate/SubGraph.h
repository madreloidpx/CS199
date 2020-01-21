//SubGraph.h
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <utility>

#include "Graph.h"

using namespace std;

class subgraph
{
private:
    vector <vector<double> > extend_centrality;

public:
    vector <vector <int> > DensitySubgraph; //storage the density subgraph

    subgraph(vector <vector<double> > extend_centrality);

    void GetSubgraph();

    vector <pair<double,vector<int> > > Calculate_Similarity(int max_id,vector <int> neighbors);

    int GetSubgraphSize(){return DensitySubgraph.size();}

};
