//Percolate.h
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <utility>


using namespace std;

class Percolate
{

private:

    double similarity_value;

    vector <vector<int> > subgraph;

    int subgraph_num;

    int community_num;

public:
    vector <vector <int> > community;

    string outputFileName;

    Percolate(vector<vector<int> > subgraph,double similarity_value,string &outputFileName);

    void find_community();

    int GetcommunitySize(){return community_num;}

};







