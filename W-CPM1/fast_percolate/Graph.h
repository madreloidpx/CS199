//Graph.h
#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

template<class VertexType>
class Graph
{
public:
    set <VertexType> vertexNames;//a container of contain all vertex name;
    vector<pair<VertexType,VertexType> > linklist;
    map<int,VertexType> node_index_to_name;//a map of nodes about index and its name;
    map<VertexType,int> node_name_to_index;
    vector <int> degrees;
    vector<vector<int> > neighbors;
public:
    int vertex_count;//the number of vertex in the graph;
    int edge_count;

public:
    Graph();

    long int vcount(){return this->vertex_count;}
    long int ecount(){return this->edge_count;}

    int degree_of_node(int v){return degrees.at(v);}

    string name_of_node_asString(int v){return node_index_to_name[v];}

    void add_neighbor(int i,int j,vector<vector<int> > &N)
    {
        //j add as a neighbor of i
        N[i].push_back(j);
    }

    bool isConnect(int i, int j)
    {
        if (find(neighbors[i].begin(),neighbors[i].end(),j) != neighbors[i].end())
            return true;
        else
            return false;
    }
    void loadingGraph(string fileName);
    void Creat_Neighborschart( vector<vector<int> > &N );
    void SortNeighborschart( vector<vector<int> > &N );

    ~Graph();
};

template<class VertexType>
Graph<VertexType>::Graph()
{
    vertex_count=0;
    edge_count=0;
}

template<class VertexType>
Graph<VertexType>::~Graph()
{

}

template<class VertexType>
void Graph<VertexType>::loadingGraph(string fileName)
{
    std::ifstream myfile(fileName.c_str());
    std::string line;
    VertexType source, dest, t;

    if (myfile.is_open())
    {
        while (!myfile.eof())
        {
            getline(myfile, line);   // Read a line from input into a string.
            if (!line.empty())
            {
                std::istringstream is(line);  // Extract data using a stringstream.
                if ((is >> source) && (is >> dest));
                else
                {
                   std::cerr<<"Error reading "<<fileName<<".txt"<< std::endl;
                }

                linklist.push_back(make_pair(source,dest));

//                //if the network is directed, we need add another edge
                //linklist.push_back(make_pair(dest,source));

                vertexNames.insert(source);
                vertexNames.insert(dest);
            }
        }
        myfile.close();
    }

    vertex_count=vertexNames.size();
    edge_count=linklist.size()/2;

    //next,we get the map of node index and node name
    int index=0;
    for (typename set<VertexType>::iterator it=vertexNames.begin();it!=vertexNames.end();it++)
    {
        node_index_to_name[index]=*it;

        node_name_to_index[*it]=index++;
    }

    //Create Graph neighbors chart
    vector<vector<int> > N(vertex_count);
    Creat_Neighborschart( N );
    //after creat neighbors chart,we should sort the chart
    SortNeighborschart( N );

    //calculate the degrees of the each vertex in the graph
    for (int i=0;i<neighbors.size();i++)
        degrees.push_back(neighbors[i].size());
}

template<class VertexType>
void Graph<VertexType>::Creat_Neighborschart( vector<vector<int> > &N )
{
    for(typename vector<pair<VertexType,VertexType> >::iterator it=linklist.begin();it!=linklist.end();it++)
    {
        pair<VertexType,VertexType> edge=*it;

        //find the index of vertex which name is edge.first and edge.second;
        add_neighbor(node_name_to_index[edge.first],node_name_to_index[edge.second],N);
    }
}
template<class VertexType>
void Graph<VertexType>::SortNeighborschart( vector<vector<int> > &N )
{
    for(vector<vector<int> >::iterator it=N.begin();it!=N.end();it++)
    {
        vector<int> v=*it;
        sort(v.begin(),v.end());
        neighbors.push_back(v);
    }
}
extern Graph<string> theGlobalgraph;

#endif


