//Percolate.cpp
#include "Percolate.h"
#include "Graph.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <math.h>

using namespace std;

Percolate :: Percolate(vector<vector<int> > sub_graph , double similarity_value , string &outputFileName)
{
    this->subgraph.swap(sub_graph);
    this->subgraph_num=this->subgraph.size();
    this->similarity_value=similarity_value;
    this->outputFileName.swap(outputFileName);
}

void Percolate :: find_community()
{
    //This is effectively a map from nodes to cliques the node is in.
    vector < set < int> * > nodesToSubgraph;
    nodesToSubgraph.resize(theGlobalgraph.vcount(), NULL);

    for(int i = 0; i < subgraph_num; ++i)
    {
		for( vector<int> :: const_iterator current_node = subgraph[i].begin();
		                  current_node != subgraph[i].end(); ++current_node)
        {
            if (nodesToSubgraph[*current_node] == NULL)
            {
                //XXX this would leak memory, if the application didnt terminate after one run.
                nodesToSubgraph[*current_node] = new set<int>;
            }
            nodesToSubgraph[*current_node]->insert(i);
		}
	}

    map <int, int> subgraphToComponents;

    int currentComponent = 0;

    for (unsigned int i = 0; i < subgraph.size(); ++i)
    {
        if (subgraphToComponents.find(i) == subgraphToComponents.end())
        {
            ++currentComponent;
            subgraphToComponents[i] = currentComponent;


            set <int> frontier;
            frontier.insert(i);

            while(! frontier.empty())
            {
                int currentSubgraph = *(frontier.begin());
                frontier.erase(frontier.begin());

                set<int> neighbors_of_currentSubgraph;

                //for each node in the current subgraph
		        for( vector<int> :: const_iterator current_node = subgraph[currentSubgraph].begin();
		                               current_node != subgraph[currentSubgraph].end(); ++current_node)

                {
                    //for this specific node, in the current subgraph, for each of the other cliques
                    //that the node is in.
                    for (set<int> :: const_iterator other_subgraph =  (nodesToSubgraph[*current_node])->begin();
                                other_subgraph != (nodesToSubgraph[*current_node])->end(); ++other_subgraph)
                    {

                        if(*other_subgraph!=currentSubgraph)
                        {
                            neighbors_of_currentSubgraph.insert(*other_subgraph);
                        }
                    }
                }

                //for each other subgraph that overlaps with the current subgraph
                for (set<int>::const_iterator nei_currentSubgraph = neighbors_of_currentSubgraph.begin();
                        nei_currentSubgraph != neighbors_of_currentSubgraph.end(); ++nei_currentSubgraph)
                {

                    int neighborSubgraph = *nei_currentSubgraph;
                    vector <int> intersect;

                    set_intersection(subgraph.at(currentSubgraph).begin(),subgraph.at(currentSubgraph).end(),
                                       subgraph.at(neighborSubgraph).begin(),subgraph.at(neighborSubgraph).end(),
                                        back_inserter(intersect));

                    vector <int> inter1;
                    vector <int> inter2;

                    set_difference(subgraph[currentSubgraph].begin(),subgraph[currentSubgraph].end(),
                                     intersect.begin(),intersect.end(),back_inserter(inter1));

                    set_difference(subgraph[neighborSubgraph].begin(),subgraph[neighborSubgraph].end(),
                                     intersect.begin(),intersect.end(),back_inserter(inter2));

                    double intersect_edges_num=0;
                    for(vector<int>::const_iterator i= inter1.begin(); i!= inter1.end(); i++)
                    {
                        int node1=*i;
                        for(vector<int>::const_iterator j= inter2.begin(); j!= inter2.end(); j++)
                        {
                            int node2=*j;
                            if(find(theGlobalgraph.neighbors[node1].begin(),theGlobalgraph.neighbors[node1].end(),node2)
                                                         != theGlobalgraph.neighbors[node1].end())
                            {
                                intersect_edges_num+=1;
                            }
                        }
                    }

                    double intersect_vertex_num=intersect.size();

                    double similarity = (intersect_edges_num+intersect_vertex_num)/min(subgraph[currentSubgraph].size(),
                                                                                       subgraph[neighborSubgraph].size());


                    //if the overlap size is greater than value of similarity
                    if ( similarity >= similarity_value)
                    {
                        //percolates
                        subgraphToComponents[neighborSubgraph] = currentComponent;
                        //add it to the frontier
                        frontier.insert(neighborSubgraph);

                        //remove from nodesToCliques map
                        for (vector <int> :: const_iterator otherSubgraphNodes = subgraph[neighborSubgraph].begin();
                                          otherSubgraphNodes != subgraph[neighborSubgraph].end(); ++otherSubgraphNodes)
                        {
                            (nodesToSubgraph[*otherSubgraphNodes])->erase(neighborSubgraph);
                        }
                    }
                }

            }

        }


    }

    //Making the list of nodes in each component, from the directory of which subgraph are in the same component.
    map <int, set < int > > componentsToNodes;
    for (map <int, int>::iterator subgraphToComponentsItr = subgraphToComponents.begin();
               subgraphToComponentsItr != subgraphToComponents.end(); ++subgraphToComponentsItr)
    {
        int theSubgraph = (*subgraphToComponentsItr).first;
        int theComponent = (*subgraphToComponentsItr).second;

        for ( vector <int>::iterator subgraphItr = subgraph[theSubgraph].begin();
                        subgraphItr != subgraph[theSubgraph].end(); ++subgraphItr)
        {
            componentsToNodes[theComponent].insert( *subgraphItr);
        }
    }


    //Output
    ofstream myfile;
    myfile.open(this->outputFileName.c_str());
    int max_communitynum=0;

    for (map <int , set <int> > ::iterator componentItr = componentsToNodes.begin();
                            componentItr != componentsToNodes.end(); ++componentItr)
    {
        int component = (*componentItr).first;

        vector<int> v;

        for ( set <int>::iterator nodeItr = (*componentItr).second.begin();
                        nodeItr != (*componentItr).second.end(); ++nodeItr)
        {
            //node_community << ordered_node_names[*nodeItr] <<" " << component << endl;
            //node_community << theGlobalgraph.name_of_node_asString(*nodeItr) <<" " << component << endl;

            //myfile << (*nodeItr) << " "<<component<<endl;
            //myfile << theGlobalgraph.name_of_node_asString(*nodeItr) << " "<<component<<endl;

            v.push_back(*nodeItr);
        }

        if (v.size()>max_communitynum)
            max_communitynum=v.size();

        this->community.push_back(v);

        //myfile << endl;
    }
    //myfile<<"一共有"<<community.size()<<"个社团 "<<" 最大的社团节点数为："<<max_communitynum<<endl;


    for (int i = 0; i < community.size(); i++)
    {
        myfile<<i+1<<": ";
        for (int j = 0; j < community[i].size(); j++)
            myfile<<theGlobalgraph.name_of_node_asString(community[i][j])<<" ";
        myfile<<endl;
    }

    myfile.close();


    this->community_num = this->community.size();

}

