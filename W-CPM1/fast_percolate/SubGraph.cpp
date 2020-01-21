//SubGraph.cpp
#include <stdio.h>
#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <assert.h>
#include <math.h>
#include <algorithm>
#include <time.h>
#include "SubGraph.h"

using namespace std;

typedef struct similarity_sort
{
    double similarity;
    vector<int> intersect;
}similarity_sort;

subgraph::subgraph(vector <vector <double> > extend_centrality_copy)
{
    this->extend_centrality.swap(extend_centrality_copy);
}

void subgraph::GetSubgraph()
{
    vector <double> vertices_num=this->extend_centrality.at(0);
    vector <double> centrality_copy=this->extend_centrality.at(1);

    vector <int> IsVisited(theGlobalgraph.vcount());

    vector <double> :: const_iterator maxdegree_iterator;
    vector <double> :: const_iterator maxsimilarity_iterator;
    vector <int> :: const_iterator intersect_iterator;

    int max_id;
    int maxsimilarity_index;
    int intersect_index;

    vector <int> intersect;
    int nei_id;

    set <int> unDelete_in_num;//unDelete_in_num defined as£ºthe nodes which not deleted from the vertices_num
    vector<int> unDelete;

    while(!centrality_copy.empty())
    {
        max_id = vertices_num.at(0);

        //get the all neighbors of the node max_id
        vector <int> maxdegree_of_node_neighbors=theGlobalgraph.neighbors.at(max_id);
        vector <int> maxdegree_of_node_neighbors_copy(maxdegree_of_node_neighbors);
//        for(vector <int> :: const_iterator i=maxdegree_of_node_neighbors.begin();i!=maxdegree_of_node_neighbors.end();i++)
//            maxdegree_of_node_neighbors_copy.push_back(*i);

        //calculate the similarity between max_id and it's all neighbors,and save in the vectory Similarity
        vector<pair<double,vector<int> > > Similarity_intersect;
        Similarity_intersect=this->Calculate_Similarity(max_id,maxdegree_of_node_neighbors_copy);

        vector <double> Similarity;
        pair <double , vector<int> > constpair;
        for(unsigned int i=0; i< Similarity_intersect.size(); i++)
        {
            constpair=Similarity_intersect.at(i);
            Similarity.push_back(constpair.first);
        }

        //return the index position of the max value in Similarity
        maxsimilarity_iterator=max_element(Similarity.begin(),Similarity.end());
        maxsimilarity_index=int(maxsimilarity_iterator-Similarity.begin());

        while(!Similarity.empty())
        {
            //find density subgraph
            nei_id=maxdegree_of_node_neighbors_copy.at(maxsimilarity_index);

            pair<double,vector<int> > mypair;
            mypair=Similarity_intersect.at(maxsimilarity_index);
            intersect=mypair.second;

            Similarity.erase(Similarity.begin()+maxsimilarity_index);
            Similarity_intersect.erase(Similarity_intersect.begin()+maxsimilarity_index);
            maxdegree_of_node_neighbors_copy.erase(maxdegree_of_node_neighbors_copy.begin()+maxsimilarity_index);

            for(unsigned int i=0; i<intersect.size(); i++)
            {
                intersect_iterator=find(maxdegree_of_node_neighbors_copy.begin(),maxdegree_of_node_neighbors_copy.end(),intersect.at(i));
                if(intersect_iterator!=maxdegree_of_node_neighbors_copy.end())
                {
                    intersect_index=int(intersect_iterator-maxdegree_of_node_neighbors_copy.begin());

                    //assert(intersect_index>=0);

                    Similarity.erase(Similarity.begin()+intersect_index);
                    Similarity_intersect.erase(Similarity_intersect.begin()+intersect_index);
                    maxdegree_of_node_neighbors_copy.erase(maxdegree_of_node_neighbors_copy.begin()+intersect_index);
                }
            }

            intersect.push_back(max_id);
            intersect.push_back(nei_id);

            if(intersect.size()>2)
            {
                sort(intersect.begin(),intersect.begin()+intersect.size());

                this->DensitySubgraph.push_back(intersect);

                for(vector <int> ::const_iterator i= intersect.begin(); i!= intersect.end(); i++)
                {
                    if (IsVisited[*i] == 0)
                    {
                        unDelete_in_num.insert(*i);
                        IsVisited[*i] = 1;
                    }
                }
            }
            maxsimilarity_iterator=max_element(Similarity.begin(),Similarity.end());

            maxsimilarity_index=int(maxsimilarity_iterator-Similarity.begin());

            intersect.clear();

        }//first while

        unDelete_in_num.insert(max_id);
        IsVisited[max_id] = 1;

        for(set <int> :: const_iterator i=unDelete_in_num.begin();i!=unDelete_in_num.end();i++)
            unDelete.push_back(*i);

        //delete the unDelete_in_num nodes from the vertices_num,select the center node next time don't consider these nodes;
        int unDelete_vertex;
        vector <double> :: const_iterator unDelete_iterator;
        int unDelete_index;
        for(unsigned int i=0;i<unDelete.size();i++)
        {
            unDelete_vertex=unDelete[i];
            unDelete_iterator=find(vertices_num.begin(),vertices_num.end(),unDelete_vertex);

            if(unDelete_iterator!=vertices_num.end())
            {
                unDelete_index=int(unDelete_iterator-vertices_num.begin());

                vertices_num.erase(vertices_num.begin()+unDelete_index);
                centrality_copy.erase(centrality_copy.begin()+unDelete_index);
            }

        }

        unDelete.clear();
        unDelete_in_num.clear();

    }
}

//calculate the similarity between max_id and it's all neighbors, save in the vectory and return
vector <pair<double,vector<int> > > subgraph :: Calculate_Similarity(int max_id,vector <int> neighbors)
{
    int neighbors_num=neighbors.size();

    vector <pair<double,vector<int> > > similarity_intersect;

    vector <int> nei;

    vector <int> intersect;
    int intersect_num;
    double simi;

    for(vector <int> :: const_iterator i = neighbors.begin(); i!= neighbors.end(); i++)
    {
        //get the all neighbors of the node *i
        nei=theGlobalgraph.neighbors.at(*i);

        //calculate the intersection between the neighbors and the nei
        set_intersection(neighbors.begin() , neighbors.end() ,
                           nei.begin() , nei.end() , back_inserter(intersect));

        intersect_num=intersect.size();

        simi=(intersect_num+2)/sqrt(nei.size()*neighbors_num);

        similarity_intersect.push_back(make_pair(simi,intersect));

        //clear the intersect;
        intersect.clear();
    }

    return similarity_intersect;
}

