# include <stdio.h>
# include <iostream>
# include <fstream>
# include <sstream>
# include <ctime>
# include <string>
# include <vector>
# include <set>
# include <list>
# include <utility>
# include <algorithm>
# include <math.h>
#include <numeric>
#include <string>

#include "Graph.h"
#include "SubGraph.h"
#include "Percolate.h"
#include "pajek.h"

using namespace std;

//Definite a structure，in order to Sort Descending for the centralit later
typedef struct DesSort
{
    double elem1;
    double elem2;
}DesSort;

int desorder(const pair<int,int> &x, const pair<int,int> &y)
{
    //return x.second > y.second;
    int p1 = x.second;
    int p2 = y.second;

    return p1 > p2;
}

int main(/*int argc , char * argv[]*/)
{
    clock_t start,finish;
    double duration;

	void Calculate_extendcentrality( vector<vector<double> > &extend_centrality);
    vector <vector<int> > file2TrueCommunity(const string & TrueCommunity_filename);
    vector <vector<int> > file2TrueCommunity2(const string & TrueCommunity_filename);
	double CalculateNMI(vector<vector<int> >TrueCommunity , vector<vector<int> > myCommunity);
	double CalculateEQ(vector<vector<int> >myCommunity,vector<vector<int> >tmp_node_to_neighbors,int vertex_num,int edges_num);
	//function declaring
    double CalculateQov(vector<vector<int> > myCommunity);
    void pajek(string file, vector<vector<int> > myCommunity);

	/*
		//main有参数时：
		//Determine the number of parameters
		if (argc != 5)
		{
			cout<<"Incorrect number of parameters! Usage: inputFileName, similarity, TrueCommunityFileName outputFileName"<<endl;
			exit(0);
		}



		string edgeslist_filename = argv[1];
		double similarity_value=atof(argv[2]);
	//    double similarity_value;
	//    if (similarity_value <= 0)
	//    {
	//        cout<<"The similarity threshold must be positive,please input again!"<<endl;
	//        exit(0);
	//    }
		string TrueCommunity_filename = argv[3];
		string outputFileName = argv[4];
	*/


	/*
	string inputFileName1[30] = {"mu_sgsc_0.1.dat","mu_sgsc_0.15.dat","mu_sgsc_0.2.dat","mu_sgsc_0.25.dat",
									"mu_sgsc_0.3.dat","mu_sgsc_0.35.dat","mu_sgsc_0.4.dat","mu_sgsc_0.45.dat",
									"mu_sgsc_0.5.dat","mu_sgsc_0.55.dat","mu_sgsc_0.6.dat","mu_sgsc_0.65.dat",
									"mu_sgsc_0.7.dat","mu_sgsc_0.75.dat","mu_sgsc_0.8.dat",
									"mu_sglc_0.1.dat","mu_sglc_0.15.dat","mu_sglc_0.2.dat","mu_sglc_0.25.dat",
									"mu_sglc_0.3.dat","mu_sglc_0.35.dat","mu_sglc_0.4.dat","mu_sglc_0.45.dat",
									"mu_sglc_0.5.dat","mu_sglc_0.55.dat","mu_sglc_0.6.dat","mu_sglc_0.65.dat",
									"mu_sglc_0.7.dat","mu_sglc_0.75.dat","mu_sglc_0.8.dat"
									};

	string TrueFileName1[30] = {"mu_sgsc_0.1_true.dat","mu_sgsc_0.15_true.dat","mu_sgsc_0.2_true.dat","mu_sgsc_0.25_true.dat",
									"mu_sgsc_0.3_true.dat","mu_sgsc_0.35_true.dat","mu_sgsc_0.4_true.dat","mu_sgsc_0.45_true.dat",
									"mu_sgsc_0.5_true.dat","mu_sgsc_0.55_true.dat","mu_sgsc_0.6_true.dat","mu_sgsc_0.65_true.dat",
									"mu_sgsc_0.7_true.dat","mu_sgsc_0.75_true.dat","mu_sgsc_0.8_true.dat",
									"mu_sglc_0.1_true.dat","mu_sglc_0.15_true.dat","mu_sglc_0.2_true.dat","mu_sglc_0.25_true.dat",
									"mu_sglc_0.3_true.dat","mu_sglc_0.35_true.dat","mu_sglc_0.4_true.dat","mu_sglc_0.45_true.dat",
									"mu_sglc_0.5_true.dat","mu_sglc_0.55_true.dat","mu_sglc_0.6_true.dat","mu_sglc_0.65_true.dat",
									"mu_sglc_0.7_true.dat","mu_sglc_0.75_true.dat","mu_sglc_0.8_true.dat"
									};

	ofstream outNMI;
	outNMI.open("result_NMI.txt");
	*/
	ofstream outQov;
	outQov.open("result_Qov.txt");
	string inputFileName1[1]={"network.dat"};
	string TrueFileName1[1]={"community.dat"};

	string edgeslist_filename, TrueCommunity_filename, outputFileName;
	double similarity_value;
	for (int i = 0; i < 1; i++)
	{
		//double NMI_max = 0;
		for (similarity_value = 0.1; similarity_value <= 1; similarity_value += 0.1)
		{
			edgeslist_filename = inputFileName1[i];
			TrueCommunity_filename = TrueFileName1[i];


		theGlobalgraph.degrees.clear();theGlobalgraph.linklist.clear();theGlobalgraph.vertexNames.clear();theGlobalgraph.neighbors.clear();

	//    double similarity_value = i;


		theGlobalgraph.loadingGraph(edgeslist_filename);

		cout<<"vertex_num:"<<theGlobalgraph.vcount()<<endl;
		cout<<"edges_num:"<<theGlobalgraph.ecount()<<endl;
		cout<<"similarity value:"<<similarity_value<<endl;

		//start = clock();

		//calculate the extend centrality of each vertex which defined by us
		vector <vector <double> > extend_centrality;
		
		Calculate_extendcentrality(extend_centrality);
		//cout<<"Here?"<<endl;
		//create graph success，next find the density subgraph in the graph
		subgraph *myDensitySubgraph=new subgraph(extend_centrality);

		cout<<endl<<"finding density subgraph,please wait..."<<endl;
		myDensitySubgraph->GetSubgraph();
		cout<<"DensitySubgraph.size():"<<myDensitySubgraph->GetSubgraphSize()<<endl;

		cout<<endl<<"percolating the subgraph..."<<endl;
		Percolate *percolate=new Percolate(myDensitySubgraph->DensitySubgraph,similarity_value,outputFileName);
		percolate->find_community();
		cout<<"community.size():"<<percolate->GetcommunitySize()<<endl;

		//finish = clock();
		//duration = (double)(finish-start)/CLOCKS_PER_SEC;
		//cout<<"Time Used："<<duration<<endl;


	
	//	//Calculate NMI
		cout<<endl<<"Calculating NMI..."<<endl;
		double NMI1,NMI2,NMI;
		vector <vector <int> > TrueCommunity;
		TrueCommunity = file2TrueCommunity(TrueCommunity_filename);
		//TrueCommunity = file2TrueCommunity2(TrueCommunity_filename);

		//vector<vector<int> > myCommunity = percolate->community;

		vector <vector<int> > myCommunity;
		for (signed int i = 0 ; i < percolate->GetcommunitySize() ; i++)
		{
			vector <int> v = percolate->community[i];
			vector <int> com;
			for (unsigned int j = 0 ; j < v.size() ; j++)
			{
				com.push_back(atoi(theGlobalgraph.node_index_to_name[v[j]].c_str())); //if string
				//com.push_back(theGlobalgraph.node_index_to_name[v[j]]); if int
			}
			sort(com.begin(),com.end());
			myCommunity.push_back(com);
		}

		cout<<"myCommunity.size():"<<myCommunity.size()<<endl;

		cout<<"vertex_num:"<<theGlobalgraph.vcount()<<"   "<<"edge_num:"<<theGlobalgraph.ecount()<<endl;

		cout<<"TrueCommunity.size():"<<TrueCommunity.size()<<endl;

		NMI1 = CalculateNMI(TrueCommunity , myCommunity);
		NMI2 = CalculateNMI(myCommunity , TrueCommunity);
		NMI =1-(NMI1+NMI2)/2;
		cout<<"NMI："<<NMI<<endl;
	


	//	cout<<"正在计算EQ值..."<<endl;
	//	cout<<"myCommunity.size():"<<percolate->GetcommunitySize()<<endl;
	//	double EQ=CalculateEQ(percolate->community,theGlobalgraph.neighbors,theGlobalgraph.vcount(),theGlobalgraph.ecount());
	//	cout<<"网络的EQ值为："<<EQ<<endl;


		//compute the overlap modularity
		//cout<<endl<<"正在计算Qov的值..."<<endl;
		//double Qov=CalculateQov(percolate->community);
		//cout<<Qov<<endl;

		//outQov<<Qov<<endl;
		}
	}


	//    out<<Qov<<endl;

	//    vector<vector<int> > myCommunity = percolate->community;
	//    map<int,set<int> > m;
	//    set<int> overlap;
	//    for (vector<vector<int> >::iterator vit = myCommunity.begin(); vit != myCommunity.end(); vit++)
	//    {
	//        vector <int> v = *vit;
	//        for (vector<int>::iterator vvit = v.begin(); vvit != v.end(); vvit++)
	//        {
	//            m[*vvit].insert(vit - myCommunity.begin());
	//        }
	//    }
	//    for (map<int,set<int> >::iterator mit = m.begin(); mit != m.end(); mit++)
	//    {
	//        set<int> v = mit->second;
	//        if (v.size()>1)
	//        {
	//
	//            overlap.insert(mit->first);
	//        }
	//    }
	//    vector<pair<int,int> > TopOverlap;
	//    for (map<int,set<int> >::const_iterator mit = m.begin(); mit != m.end(); mit++)
	//    {
	//        set<int> v = mit->second;
	//        if (v.size() > 1)
	//        {
	//            TopOverlap.push_back(make_pair(mit->first,v.size()));
	//        }
	//
	//    }
	//    sort(TopOverlap.begin(),TopOverlap.end(),desorder);
	//
	//    cout<<"TopOverlap.size():"<<TopOverlap.size()<<endl;
	//
	//    ofstream result_overlap;
	//    result_overlap.open("result_overlap.txt");
	//    int ii=1;
	//    for (vector<pair<int,int> >::const_iterator vt = TopOverlap.begin(); vt != TopOverlap.end(); vt++)
	//    {
	//        pair<int,int> p = *vt;
	//
	//        //cout<<"("<<theGlobalgraph.node_index_to_name[p.first]<<","<<p.second<<") ";
	//        set<int> s = m[p.first];
	//        result_overlap<<ii++<<"."<<theGlobalgraph.node_index_to_name[p.first]<<"("<<s.size()<<"):";
	//        for (set<int>::const_iterator sit = s.begin(); sit != s.end(); sit++)
	//        {
	//            result_overlap<<" "<<*sit+1<<" ";
	//        }
	//        result_overlap<<endl;
	//    }


	//    cout<<endl<<endl<<"Do you want to draw the results? "<<endl;
	//    string inputstring;
	//    cin>>inputstring;
	//    while (strcmp(inputstring.c_str(),"Y")==0)
	//    {
	//        pajek(edgeslist_filename.c_str(),percolate->community);
	//
	//        cout<<endl<<"Do you want to draw the LLPA or OCG algorithms results? "<<endl;
	//        cin>>inputstring;
	//    }

	//}
	return 0;

}


void Calculate_extendcentrality(vector<vector<double> > &extend_centrality)
{
	void vector_sort(vector<double>num , vector<double>centrality,vector<vector<double> > &extend_centrality);

	int vertex_num=theGlobalgraph.vcount();
	int neighbors_num=0;
	vector <double> centrality(vertex_num,0.0);
	vector <double> num;
	vector <double> edges_num_of_node(vertex_num,0.0);
	vector <vector<int> > tmp_node_to_neighbors_copy(theGlobalgraph.neighbors);

	vector <int> :: const_iterator find_iterator;
	int index;
	
	for(int i=0;i<vertex_num;i++)
	{
	    
		vector <int> neighbors = tmp_node_to_neighbors_copy[i];
	    neighbors_num=neighbors.size();
		
	    num.push_back(i);
	    edges_num_of_node[i]=(edges_num_of_node[i]+neighbors_num);
		
	    for (int j = 0 ; j < neighbors_num ; j++)
        {
            
			int nei1 = neighbors[j];
            edges_num_of_node[nei1] = edges_num_of_node[nei1] + 1;
			
            for (int k = j+1 ; k < neighbors_num ; k++)
            {
                //cout<<"Here???? "<<k<<endl;
				int nei2 = neighbors[k];
                find_iterator = find(tmp_node_to_neighbors_copy[nei1].begin(),tmp_node_to_neighbors_copy[nei1].end(),nei2);
                if (find_iterator != tmp_node_to_neighbors_copy[nei1].end())
                {
                    edges_num_of_node[i]=(edges_num_of_node[i]+1);
                    edges_num_of_node[nei1]=(edges_num_of_node[nei1]+1);
                    edges_num_of_node[nei2]=(edges_num_of_node[nei2]+1);
                }
            }
            find_iterator = find(tmp_node_to_neighbors_copy[nei1].begin(),tmp_node_to_neighbors_copy[nei1].end(),i);
            index = (int)(find_iterator-tmp_node_to_neighbors_copy[nei1].begin());
            //tmp_node_to_neighbors_copy[nei1].erase(tmp_node_to_neighbors_copy[nei1].begin()+index);
			//cout<<"Here??? "<<j<<endl;
        }
		//cout<<"Here?? "<<i<<endl;
        centrality[i] = edges_num_of_node[i]/(theGlobalgraph.neighbors[i].size()+1);
	}

	//descending sort the centrality(this step is to facilitate the subsequent finding the density subgraph)
	vector_sort(num,centrality,extend_centrality);

}//Calculate_extendcentrality();

bool compare(DesSort a , DesSort b)
{
    return a.elem2 > b.elem2;
}

void vector_sort(vector<double> num , vector<double> centrality , vector<vector<double> > &extend_centrality)
{
    int vertex_num=theGlobalgraph.vcount();
    vector <DesSort> array1(vertex_num);
    for(int i=0;i<vertex_num;i++)
    {
        array1[i].elem1 = num[i];
        array1[i].elem2 = centrality[i];
    }

    stable_sort(array1.begin(),array1.end(),compare);

    vector <double> sam1;
    vector <double> sam2;

    for (int i=0;i<vertex_num;i++)
    {
        sam1.push_back(array1[i].elem1);
        sam2.push_back(array1[i].elem2);
    }
    extend_centrality.push_back(sam1);
	extend_centrality.push_back(sam2);
}


double CalculateNMI(vector<vector<int> > TrueCommunity , vector<vector<int> > myCommunity)
{
    double nmi;
    double vertex_num=theGlobalgraph.vcount();

    //下面开始计算NMI值
    int myCommunity_num = myCommunity.size();
    int TrueCommunity_num = TrueCommunity.size();


    //vector <vector <double> > Com11(myCommunity_num),Com10(myCommunity_num),Com01(myCommunity_num),Com00(myCommunity_num);
    vector <vector <double> > P11(myCommunity_num),P10(myCommunity_num),P01(myCommunity_num),P00(myCommunity_num);
    vector <int> intersect;
    int intersect_num;
    for(int i = 0 ; i < myCommunity_num ; i++)
    {
        vector <int> my = myCommunity[i];
        for(int j = 0 ; j < TrueCommunity_num ; j++)
        {
            vector <int> True = TrueCommunity[j];
            set_intersection(my.begin(),my.end(),
                             True.begin(),True.end(),back_inserter(intersect));


            intersect_num = intersect.size();

            P11[i].push_back((double)intersect_num/vertex_num);
            P10[i].push_back((double)(my.size()-intersect_num)/vertex_num);
            P01[i].push_back((double)(True.size()-intersect_num)/vertex_num);
            P00[i].push_back((double)(vertex_num-my.size()-True.size()+intersect_num)/vertex_num);


            intersect.clear();
        }
    }

    vector <double> min_A;
    double Hk_l,h11,h10,h01,h00,Hkl;
    double templ=0;

    for (int i = 0 ; i < myCommunity_num ; i++)
    {
        double min_k = 1000;
        Hk_l = 0;
        for (int j = 0 ; j < TrueCommunity_num ; j++)
        {

            Hkl = 0; h11 = 0; h10 = 0; h01 = 0; h00 = 0;
            if (P11[i][j]!=0)
            {
                h11 = -P11[i][j] * (log(P11[i][j])/log(2));
                Hkl = Hkl + h11;
            }
            if (P10[i][j]!=0)
            {
                h10 = -P10[i][j] * (log(P10[i][j])/log(2));
                Hkl = Hkl + h10;
            }
            if (P01[i][j]!=0)
            {
                h01 = -P01[i][j] * (log(P01[i][j])/log(2));
                Hkl = Hkl + h01;
            }
            if (P00[i][j]!=0)
            {
                h00 = -P00[i][j] * (log(P00[i][j])/log(2));
                Hkl = Hkl + h00;
            }

            double Py1 = P11[i][j] + P01[i][j];
            double Py0 = P10[i][j] + P00[i][j];
            double Hl = -Py1 * (log(Py1)/log(2)) + (-Py0 * (log(Py0)/log(2)));

            Hk_l = Hkl - Hl;

            if(Hk_l <= min_k && (h00 + h11 > h01 + h10))
            {
                min_k = Hk_l;
                templ = j;
            }

        }

        if (min_k == 1000)
        {
            min_k = 1;
        }
        else
        {

            double Px1 = P11[i][templ] + P10[i][templ];
            double Px0 = P01[i][templ] + P00[i][templ];
            double Hk = -Px1 * (log(Px1)/log(2)) + (-Px0 * (log(Px0)/log(2)));
            min_k = min_k/Hk;
        }
        min_A.push_back(min_k);
    }

    nmi = accumulate(min_A.begin(),min_A.end(),0.0)/myCommunity_num;

    return nmi;
}

/*
double CalculateNMI(vector<vector<int> > TrueCommunity , vector<vector<int> > myCommunity)
{
    double nmi;
    double vertex_num=theGlobalgraph.vcount();

    //下面开始计算NMI值
    int myCommunity_num = myCommunity.size();
    int TrueCommunity_num = TrueCommunity.size();

/*
    //vector <vector <double> > Com11(myCommunity_num),Com10(myCommunity_num),Com01(myCommunity_num),Com00(myCommunity_num);
    vector <vector <double> > P11(myCommunity_num),P10(myCommunity_num),P01(myCommunity_num),P00(myCommunity_num);
    vector <int> intersect;
    int intersect_num;
    for(int i = 0 ; i < myCommunity_num ; i++)
    {
        vector <int> my = myCommunity[i];
        for(int j = 0 ; j < TrueCommunity_num ; j++)
        {
            vector <int> True = TrueCommunity[j];
            set_intersection(my.begin(),my.end(),
                             True.begin(),True.end(),back_inserter(intersect));


            intersect_num = intersect.size();

            P11[i].push_back((double)intersect_num/vertex_num);
            P10[i].push_back((double)(my.size()-intersect_num)/vertex_num);
            P01[i].push_back((double)(True.size()-intersect_num)/vertex_num);
            P00[i].push_back((double)(vertex_num-my.size()-True.size()+intersect_num)/vertex_num);


            intersect.clear();
        }
    }

    vector <double> min_A;
    double Hk_l,h11,h10,h01,h00,Hkl;
    double templ=0;

    double P11,P10,P01,P00;
    vector <int> intersect;
    double intersect_num;
    for (int i = 0 ; i < myCommunity_num ; i++)
    {
        vector <int> my = myCommunity[i];
        double min_k = 1000;
        Hk_l = 0;
        for (int j = 0 ; j < TrueCommunity_num ; j++)
        {
            intersect.clear();

            vector <int> True = TrueCommunity[j];

            set_intersection(my.begin(),my.end(),
                True.begin(),True.end(),back_inserter(intersect));
            intersect_num = intersect.size();

            P11=((double)intersect_num/vertex_num);
            P10=((double)(my.size()-intersect_num)/vertex_num);
            P01=((double)(True.size()-intersect_num)/vertex_num);
            P00=((double)(vertex_num-my.size()-True.size()+intersect_num)/vertex_num);

            Hkl = 0; h11 = 0; h10 = 0; h01 = 0; h00 = 0;
            if (P11>0.000001 ||P11<-0.000001)
            {
                h11 = -P11 * (log(P11)/log(2));
                Hkl = Hkl + h11;
            }
            if (P10>0.000001 ||P10<-0.000001)
            {
                h10 = -P10 * (log(P10)/log(2));
                Hkl = Hkl + h10;
            }
            if (P01>0.000001 ||P01<-0.000001)
            {
                h01 = -P01 * (log(P01)/log(2));
                Hkl = Hkl + h01;
            }
            if (P00>0.000001 ||P00<-0.000001)
            {
                h00 = -P00 * (log(P00)/log(2));
                Hkl = Hkl + h00;
            }

            double Py1 = P11 + P01;
            double Py0 = P10 + P00;
            double Hl = -Py1 * (log(Py1)/log(2)) + (-Py0 * (log(Py0)/log(2)));

            Hk_l = Hkl - Hl;

            if(Hk_l <= min_k && (h00 + h11 > h01 + h10))
            {
                min_k = Hk_l;
                templ = j;
            }

        }

        if (min_k == 1000)
        {
            min_k = 1;
        }
        else
        {

            intersect.clear();

            vector <int> True = TrueCommunity[templ];
            set_intersection(my.begin(),my.end(),
                True.begin(),True.end(),back_inserter(intersect));
            intersect_num = intersect.size();

            P11=((double)intersect_num/vertex_num);
            P10=((double)(my.size()-intersect_num)/vertex_num);
            P01=((double)(True.size()-intersect_num)/vertex_num);
            P00=((double)(vertex_num-my.size()-True.size()+intersect_num)/vertex_num);

            double Px1 = P11 + P10;
            double Px0 = P01 + P00;
            double Hk = -Px1 * (log(Px1)/log(2)) + (-Px0 * (log(Px0)/log(2)));
            min_k = min_k/Hk;
        }
        min_A.push_back(min_k);
    }

    nmi = accumulate(min_A.begin(),min_A.end(),0.0)/myCommunity_num;

    return nmi;
}
*/

vector <vector <int> > file2TrueCommunity(const string & TrueCommunity_filename)
{
    vector <vector<int> *> TrueCommunity;
    int community_num_max = -1;

    /*
    vector <int> example;
    vector <string> names;
    for (int i = 0 ; i < vertex_num ; i++)
    {
        example.push_back(i);
        names.push_back(ordered_node_names[i]);
    }*/

    //按行读文件
	ifstream read(TrueCommunity_filename.c_str() , ios_base :: in | ios_base :: binary);
    if(read.fail())
	{
		cout<<"读取"<<TrueCommunity_filename<<"失败，请确保该文件正确!"<<endl;
		exit(0);
	}

	string line;
	while(getline(read ,line ))
	{
	    if (!line.empty())
        {
            int splitnum = 0;//记录line中有多少个空格

            //每一行的最后一个是空格，应该把空格也去掉
            if(!line.empty() && line.at(line.length()-2) == ' ')
            {
                line.erase(line.length()-2,1);
            }

            //每一行的line可能是以'\n'结尾的，所以要把line中'\n'去掉
            if(!line.empty() && *line.rend() == '\n')
            {
                line.erase(line.length()-1,1);
            }

            //将字符串line按照中间的空格分隔开
            for(string :: iterator i = line.begin(); i!=line.end(); i++)
            {
                if(*i == ',' || *i == ' ' || *i == '\t' || *i == '|')
                {
                    *i = '\n';
                    splitnum = splitnum + 1;
                }
            }

            istringstream fields(line);
            string nodename;
            int node_num = 0;

            getline(fields , nodename);

            /*
            //找到节点名称为nodename所对应的节点编号
            vector <string> :: const_iterator name_iterator;
            int name_index;
            name_iterator = find(names.begin(),names.end(),nodename);
            name_index = int(name_iterator-names.begin());
            node_num = example[name_index];
            //再把example和names向量中的name_index位置值删除
            example.erase(example.begin()+name_index);
            names.erase(names.begin()+name_index);
            */
            node_num = atoi(nodename.c_str());

            for (int i = 0 ; i < splitnum ; i++)
            {
                string communitynum;
                getline(fields , communitynum);

                //把communitynum转化成数字
                int community_num = atoi(communitynum.c_str());
                if (community_num != 0)
                {
                    if (community_num > community_num_max)
                    {
                        community_num_max = community_num;
                        TrueCommunity.resize(community_num_max,NULL);
                    }

                    if(TrueCommunity[community_num-1] == NULL)
                    {
                        TrueCommunity[community_num-1] = new vector<int>;
                    }

                    TrueCommunity[community_num-1]->push_back(node_num);
                }
            }
        }
    }
    vector <vector <int> > True;

    for (unsigned int i=0;i<TrueCommunity.size();i++)
    {
        vector<int> * TrueCommunityBegin = TrueCommunity[i];
        vector <int> a;
        for (unsigned int j = 0 ; j<TrueCommunityBegin->size(); j++)
        {
            a.push_back(TrueCommunityBegin->at(j));
        }
        sort(a.begin(),a.end());
        True.push_back(a);
    }

    return True;
}

double CalculateEQ(vector <vector <int> > myCommunity , vector <vector<int> > tmp_node_to_neighbors , int vertex_num , int edges_num)
{
    double EQ=0;
    vector <int> degree;
    set <int> community_vertex_set;
    for (unsigned int i=0;i<tmp_node_to_neighbors.size();i++)
    {
        degree.push_back(tmp_node_to_neighbors[i].size());
    }

    for (unsigned int i=0;i<myCommunity.size();i++)
    {
        for(unsigned int j=0;j<myCommunity[i].size();j++)
        {
            community_vertex_set.insert(myCommunity[i][j]);
        }
    }

    //在计算EQ之前，先获得一张表，这张表示2*n，n为在community中出现的节点的个数
    vector <vector<int> > Table;
    vector <int> v1,v2;
    for (set<int>::iterator i=community_vertex_set.begin();i!=community_vertex_set.end();i++)
    {
        v1.push_back(*i);
        v2.push_back(0);
    }
    Table.push_back(v1);
    Table.push_back(v2);

    vector <int> :: const_iterator is_iterator;
    int is_index;
    for (unsigned int i=0;i<myCommunity.size();i++)
    {
        for (unsigned int j=0;j<myCommunity[i].size();j++)
        {
            int node = myCommunity[i][j];
            is_iterator=find(Table[0].begin(),Table[0].end(),node);
            if(is_iterator!=Table[0].end())
            {
                is_index=int(is_iterator-Table[0].begin());

                Table[1][is_index] = Table[1][is_index] + 1;
            }
        }
    }

    for (unsigned int i=0;i<myCommunity.size();i++)
    {
        //cout<<"正在处理"<<i<<"个社团"<<endl;

        vector <int> com = myCommunity.at(i);

        int node1,node2;
        int contain_node_num1=0,contain_node_num2=0;

        for (unsigned int j=0;j<com.size();j++)
        {
            node1 = com[j];
            vector<int>::const_iterator itea1 = find(Table[0].begin(),Table[0].end(),node1);
            int index1;
            if (itea1!=Table[0].end())
            {
                index1=int(itea1-Table[0].begin());
                contain_node_num1=Table[1][index1];
            }
            for (unsigned int k=0;k<com.size();k++)
            {
                node2 = com[k];
                vector<int>::const_iterator itea2 = find(Table[0].begin(),Table[0].end(),node2);
                int index2;
                if (itea2!=Table[1].end())
                {
                    index2=int(itea2-Table[0].begin());
                    contain_node_num2=Table[1][index2];
                }

                double adj=0;
                vector<int>::const_iterator is_contain=find(tmp_node_to_neighbors[node1].begin(),tmp_node_to_neighbors[node1].end(),node2);
                if(is_contain!=tmp_node_to_neighbors[node1].end())
                {
                    adj=1;
                }

                EQ=EQ+(1/((double)contain_node_num1*contain_node_num2))*(adj-(double)degree[node1]*degree[node2]/(2*edges_num));

            }
        }
    }
    EQ=EQ/(2*edges_num);
    return EQ;
}

double CalculateQov(vector<vector<int> > myCommunity)
{
    //function declaring
    double beta(map<int,set<int> >belongcoefficient, int node1, int node2, int c);
    double beta2(map<int,set<int> >belongcoefficient, int node1, int c);

    double sum=0.0;

    //First, compute the Oi
    set<int> nodeSet;
    map<int,set<int> > belongcoefficient;


//    cout<<"查看myCommunity的值："<<endl;
//    for (size_t i=0; i<myCommunity.size(); i++)
//    {
//        for (size_t j=0; j<myCommunity[i].size(); j++)
//            cout<<myCommunity[i][j]<<" ";
//        cout<<endl;
//    }

//    cout<<endl<<"myCommunity.size():"<<myCommunity.size()<<endl;

    for (vector<vector<int> >::const_iterator i=myCommunity.begin(); i!=myCommunity.end(); i++)
    {
        vector<int> com=*i;
        for (vector<int>::const_iterator it=com.begin(); it!=com.end(); it++)
        {
            nodeSet.insert(*it);
            belongcoefficient[*it].insert(i-myCommunity.begin());

        }
    }

    vector<vector<double> > singlecoefficient,averagecoefficient;
    for (size_t i=0; i<myCommunity.size(); i++)
    {
//        vector<double> v;
        vector<int> com=myCommunity.at(i);

        vector<double> f,avera;
        for (vector<int>::const_iterator it_com = com.begin(); it_com != com.end(); it_com++)
        {
            int comnode = *it_com;
            int comnode_index = int(it_com-com.begin());
            double tt = 1.0/belongcoefficient[comnode].size();
            double tem = 1.0/(1+exp(30-60*tt));

            f.push_back(tem);
        }

//        for (size_t j=0;j<com.size();j++)
//        {
//            vector<double> v1;
//
//            int node1=com[j];
//            for (size_t k=j;k<com.size();k++)
//            {
//                int node2=com[k];
////                v1.push_back(beta(belongcoefficient,node1,node2,i));
//                v1.push_back(F[j]*F[k]);
//            }
//            v.push_back(v1);
//
//        }
//        beta1.push_back(v);


        for (size_t j = 0; j < com.size(); j++)
        {
            int node1 = com[j];
            double sum1 = 0.0;
            for (size_t k = 0; k < com.size(); k++)
            {
                int node2 = com[k];
                sum1 = sum1 + f[j]*f[k];
            }
            avera.push_back(sum1/theGlobalgraph.vcount());
        }


        singlecoefficient.push_back(f);
        averagecoefficient.push_back(avera);
    }


    for (size_t i=0; i<myCommunity.size(); i++)

    {
//        if (i%10==0)
//            cout<<"正在计算第"<<i<<"个社团"<<endl;



        vector<double> v=singlecoefficient.at(i);//com这个社团内部每对节点对之间构成的coefficient of belonging;

        vector<double> average_cb = averagecoefficient.at(i);

        vector<int> com=myCommunity.at(i);

//        vector<double> average_cb;double sum1=0.0,beta_average;
//        //计算com中每个节点的平均coefficients of belonging to c
//        for (vector<int>::const_iterator com_it = com.begin(); com_it != com.end(); com_it++)
//        {
//            int index_node = int(com_it-com.begin());
//            int com_node = *com_it;
//
//            for (int x=0;x<index_node;x++)
//                sum1+=v[x][index_node-x];
//            for (size_t x=index_node;x<v[index_node].size();x++)
//                sum1+=v[index_node][x-index_node];
//            beta_average=sum1/theGlobalgraph.vcount();
//
//            average_cb.push_back(beta_average);
//
//            sum1 = 0.0;
//        }


//        cout<<"v的长度为："<<v.size()<<endl;


        for (vector<int>::const_iterator it1=com.begin(); it1!=com.end(); it1++)
        {
            int node1=*it1;
            int index1=int(it1-com.begin());

            for (vector<int>::const_iterator it2=com.begin(); it2!=com.end(); it2++)
            {
                int node2=*it2;
                int index2=int(it2-com.begin());
                //double beta1=beta(belongcoefficient,node1,node2,i-myCommunity.begin());

                double beta_1;
                beta_1 = v[index1]*v[index2];
//                if (index1<=index2)
//                    beta_1=v[index1][index2-index1];
//                else
//                    beta_1=v[index2][index1-index2];

//                double beta_2,sum1=0.0;
//                for (int x=0;x<index1;x++)
//                    sum1+=v[x][index1-x];
//                for (size_t x=index1;x<v[index1].size();x++)
//                    sum1+=v[index1][x-index1];
//                beta_2=sum1/theGlobalgraph.vcount();
//
//
//                double beta_3,sum2=0.0;
//                for (int x=0;x<index2;x++)
//                    sum2+=v[x][index2-x];
//                for (size_t x=index2;x<v[index2].size();x++)
//                    sum2+=v[index2][x-index2];
//                beta_3=sum2/theGlobalgraph.vcount();


                double beta_2 = average_cb[index1],beta_3 = average_cb[index2];

                double adj=0.0;
                if (find(theGlobalgraph.neighbors[node1].begin(),theGlobalgraph.neighbors[node1].end(),node2)!=theGlobalgraph.neighbors[node1].end())
                    adj=1.0;

                sum=sum+beta_1*adj-((beta_2*beta_3*theGlobalgraph.degree_of_node(node1)*theGlobalgraph.degree_of_node(node2))/(2*theGlobalgraph.ecount()));
            }
        }
    }
    return sum/(2*theGlobalgraph.ecount());

}


double beta(map<int,set<int> >belongcoefficient, int node1, int node2, int c)
{
    //f(x)=60x-30
//    double x,f1,f2;
    double x1,x2,f1,f2,F;

//    for (set<int>::const_iterator i=belongcoefficient[node1].begin();i!=belongcoefficient[node1].end();i++)
//    {
//        cout<<*i<<" ";
//    }
//    cout<<endl;

    if (belongcoefficient[node1].find(c) == belongcoefficient[node1].end() ||
        belongcoefficient[node2].find(c) == belongcoefficient[node2].end())
        return 0.0;
    else
    {
        x1 = 1.0/belongcoefficient[node1].size();
        x2 = 1.0/belongcoefficient[node2].size();
    }
    f1 = 60 * x1 - 30; f2 = 60 * x2 - 30;

    F = 1.0/((1+exp(0-f1))*(1+exp(0-f2)));

    return F;

//    f1=60*x-30;
//    f1=0-f1;
//
//    if (belongcoefficient[node2].find(c)==belongcoefficient[node2].end())
//        x=0.0;
//    else
//        x=1.0/belongcoefficient[node2].size();
//    f2=60*x-30;
//    f2=0-f2;
//
//    double f=(1+exp(f1))*(1+exp(f2));
//
//    return 1.0/f;
}

//double beta2(map<int, set<int> >belongcoefficient, int node2, int c)
//{
//    double sum=0.0;
//    for (int i=0; i<theGlobalgraph.vcount(); i++)
//    {
//        double f=beta(belongcoefficient,node2,i,c);
//        sum+=f;
//    }
//    return sum/theGlobalgraph.vcount();
//}



vector <vector <int> > file2TrueCommunity2(const string & TrueCommunity_filename)
{
    vector<vector<int> > trueCommunity;

    //按行读文件
	ifstream read(TrueCommunity_filename.c_str() , ios_base :: in | ios_base :: binary);
    if(read.fail())
	{
		cout<<"读取"<<TrueCommunity_filename<<"失败，请确保该文件正确!"<<endl;
		exit(0);
	}

	string line;
	while(getline(read ,line ))
	{
	    if (!line.empty())
        {
            vector<int> vcom;

            int splitnum = 0;//记录line中有多少个空格

            //每一行的最后一个是空格，应该把空格也去掉
            if(!line.empty() && line.at(line.length()-2) == ' ')
            {
                line.erase(line.length()-2,1);
            }

            //每一行的line可能是以'\n'结尾的，所以要把line中'\n'去掉
            if(!line.empty() && *line.rend() == '\n')
            {
                line.erase(line.length()-1,1);
            }

            //将字符串line按照中间的空格分隔开
            for(string :: iterator i = line.begin(); i!=line.end(); i++)
            {
                if(*i == ',' || *i == ' ' || *i == '\t' || *i == '|')
                {
                    *i = '\n';
                    splitnum = splitnum + 1;
                }
            }

            istringstream fields(line);
            string nodename;
            int node_num = 0;

            for (int i = 0 ; i <= splitnum ; i++)
            {
                string communitynum;
                getline(fields , communitynum);

                vcom.push_back(theGlobalgraph.node_name_to_index[communitynum]);
            }

            if (vcom.size() > 2)
                trueCommunity.push_back(vcom);
        }
    }

    vector <vector <int> > True;

    for (unsigned int i=0;i<trueCommunity.size();i++)
    {
        vector <int> a = trueCommunity[i];

        sort(a.begin(),a.end());
        True.push_back(a);
    }

    return True;
}
