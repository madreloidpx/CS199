#include<iostream>
#include<ostream>
#include<vector>
#include "Graph.h"

void pajek(string inputFileName, vector<vector<int> >myCommunity)
{
    //.clu
    ofstream clufile;
    string file=inputFileName.substr(0,inputFileName.size()-4)+".clu";
    clufile.open(file.c_str());


    vector<int> inputcommunitynum;
    cout<<endl<<"请输入想要画出的社团编号(输入-1代表画出所有社团):"<<endl;
    int input;
    cin>>input;

    if (input == -1)
        for (int i = 0; i < myCommunity.size(); i++)
            inputcommunitynum.push_back(i+1);
    else
        while(input >= 0)
        {
            inputcommunitynum.push_back(input);

            cin>>input;
        }


//    set<size_t> allcom;
//    for (size_t x=0;x<myCommunity.size();x++)
//    {
//        for (size_t y=0;y<myCommunity[x].size();y++)
//        {
//            allcom.insert(myCommunity[x][y]);
//        }
//    }


    cout<<endl<<endl;
    set<int> allcom;
    vector<vector<int> > myC;
    for (int x=0;x<inputcommunitynum.size();x++)
    {
        myC.push_back(myCommunity[inputcommunitynum[x]-1]);

        cout<<"第"<<inputcommunitynum[x]<<"个社团:( "<<myCommunity[inputcommunitynum[x]-1].size()<<"个节点 )"<<endl;
        for (int y=0;y<myCommunity[inputcommunitynum[x]-1].size();y++)
        {
            cout<<theGlobalgraph.node_index_to_name[myCommunity[inputcommunitynum[x]-1][y]]<<" ";
            allcom.insert(myCommunity[inputcommunitynum[x]-1][y]);
        }
        cout<<endl;
    }

    clufile<<"*Vertices "<<allcom.size()<<endl;
    for (set<int>::const_iterator it=allcom.begin();it!=allcom.end();it++)
    {
        int node=*it;
        int flag=0,communitynum=0;
        for (int x=0;x<myC.size();x++)
        {
            vector<int> v=myC[x];
            if (find(v.begin(),v.end(),node)!=v.end())
            {
                flag=flag+1;
                communitynum=x+1;
            }
        }
        if (flag==1)
            clufile<<communitynum<<endl;
        else if(flag>=2)
            clufile<<myC.size()+1<<endl;

    }

    clufile.close();

    //.net
    ofstream netfile;
    file=inputFileName.substr(0,inputFileName.size()-4)+".net";
    netfile.open(file.c_str());
    netfile<<"*Vertices "<<allcom.size()<<endl;

    int y = 1;
    for (set<int>::iterator x=allcom.begin(); x != allcom.end(); x++)
    {
//        char c[256];
//        itoa(x,c,10);
//        netfile<<x<<" "<<c<<endl;
        netfile<<y++<<" "<<theGlobalgraph.node_index_to_name[*x]<<endl;

    }
    netfile<<"*Arcs"<<endl;
    netfile<<"*Edges"<<endl;
    int index1=0,index2=0;
    for (set<int>::const_iterator x=allcom.begin();x!=allcom.end();x++)
    {
        index1++;
        index2=0;
        for (set<int>::const_iterator y=allcom.begin();y!=allcom.end();y++)
        {
            index2++;
            if(theGlobalgraph.isConnect(*x,*y))
                netfile<<index1<<" "<<index2<<endl;
        }
    }
    netfile.close();

}
