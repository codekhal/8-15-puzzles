#include<iostream>
#include<vector>
#include<math.h>
#include<set>
#include<queue>
#include<time.h>
#define N 4
using namespace std;
int grid[N][N]={{2,8,7,11},                                            
                 {5,0,4,15},
                 {13,9,14,3},
                 {1,10,6,12}};

struct Node                                                         
{
    int x0,y0,g,h,f,manhattan_h;
    int Grid[N][N];
    Node* parent=NULL;
    Node(int x=0,int y=0,int G=0,Node* node=NULL)
    {
        x0=x;
        y0=y;
        g=G;
        parent=node;
    }
}start,*process;
int f,i,j,ctr,siz,original_h,exchanged_h,maxi,maxj,linear_horizontal_conflicts,linear_vertical_conflicts;
struct compareNode                                                                  //Custom Comparison for Node min(f) and reprioritizing
{
    bool operator()(const Node* Node1,const Node* Node2)const
    {
        if(Node1->f!=Node2->f)
            return Node1->f<Node2->f;
        for(i=0;i<N;i++)
        {
            for(j=0;j<N;j++)
            {
                if(Node1->Grid[i][j]!=Node2->Grid[i][j])
                {
                    return Node1->Grid[i][j]<Node2->Grid[i][j];
                }
            }
        }
        return false;
    }
};
struct GridLess                                                                     //custom comparison for Node* type
{
    bool operator()(const Node *a,const Node *b) const
    {
        for(i=0;i<N;i++)
        {
           for(j=0;j<N;j++)
           {
               if(a->Grid[i][j]!=b->Grid[i][j])
               {
                    return a->Grid[i][j]<b->Grid[i][j];
               }
           }
        }
        return false;
    }
};
multiset<Node*,compareNode>open_list_dup;                                               //For O(1) search time min(f)  More nodes N=4
set<Node*,GridLess>closed_list,open_list;                                               //For O(log(n)) search time.
set<Node*,GridLess>::const_iterator open_list_cpy;
clock_t tStart,tEnd;
vector<Node*> neighbors;
void Neighbors(Node* temp_Node)                                                        //Uses Manhattan Heuristics and linear conflict
{
    siz=0;linear_horizontal_conflicts=0;linear_vertical_conflicts=0;
    if(temp_Node->x0+1<=N)                                                      //
        siz++;                                                                  //
    if(temp_Node->x0-1>0)                                                       //
        siz++;                                                                  //
    if(temp_Node->y0+1<=N)                                                      // Resize vector neighbors
        siz++;                                                                  //
    if(temp_Node->y0-1>0)                                                       //
        siz++;                                                                  //
    neighbors.resize(siz);                                                      //
    if(temp_Node->x0+1<=N)                                  //Space->Right, Move=Left;
    {
        neighbors[siz-1]=(new Node(temp_Node->x0+1,temp_Node->y0,temp_Node->g+1,temp_Node));
        for(i=0;i<N;i++)
        {
            maxi=-1;
            maxj=-1;
            for(j=0;j<N;j++)
            {
                if(i==temp_Node->y0-1&&j==temp_Node->x0-1)
                    neighbors.at(siz-1)->Grid[i][j]=temp_Node->Grid[i][j+1];
                else if(i==temp_Node->y0-1&&j==temp_Node->x0)
                    neighbors.at(siz-1)->Grid[i][j]=temp_Node->Grid[i][j-1];
                else
                    neighbors.at(siz-1)->Grid[i][j]=temp_Node->Grid[i][j];
                if((i==temp_Node->y0-1&&j==temp_Node->x0-1))
                {
                    if(temp_Node->Grid[i][j+1]!=0&&(temp_Node->Grid[i][j+1]-1)/N==i)
                        if(temp_Node->Grid[i][j+1]>maxi)
                            maxi=temp_Node->Grid[i][j+1];
                        else
                            linear_horizontal_conflicts++;
                }
                else if(i==temp_Node->y0-1&&j==temp_Node->x0)
                {
                    if(temp_Node->Grid[i][j-1]!=0&&(temp_Node->Grid[i][j-1]-1)/N==i)
                        if(temp_Node->Grid[i][j-1]>maxi)
                            maxi=temp_Node->Grid[i][j-1];
                         else
                            linear_horizontal_conflicts++;
                }
                else
                {
                    if(temp_Node->Grid[i][j]!=0&&(temp_Node->Grid[i][j]-1)/N==i)
                        if(temp_Node->Grid[i][j]>maxi)
                            maxi=temp_Node->Grid[i][j];
                        else
                            linear_horizontal_conflicts++;
                }
                if((j==temp_Node->y0-1&&i==temp_Node->x0-1))
                {
                    if(temp_Node->Grid[j][i+1]!=0&&(temp_Node->Grid[j][i+1]-1)%N==i)
                        if(temp_Node->Grid[j][i+1]>maxj)
                            maxj=temp_Node->Grid[j][i+1];
                        else
                            linear_vertical_conflicts++;
                }
                else if(j==temp_Node->y0-1&&i==temp_Node->x0)
                {
                    if(temp_Node->Grid[j][i-1]!=0&&(temp_Node->Grid[j][i-1]-1)%N==i)
                        if(temp_Node->Grid[j][i-1]>maxj)
                            maxj=temp_Node->Grid[j][i-1];
                        else
                            linear_vertical_conflicts++;
                }
                else
                    if(temp_Node->Grid[j][i]!=0&&(temp_Node->Grid[j][i]-1)%N==i)
                        if(temp_Node->Grid[j][i]>maxj)
                            maxj=temp_Node->Grid[j][i];
                        else
                            linear_vertical_conflicts++;
            }
        }
        original_h=abs(temp_Node->y0-1-(temp_Node->Grid[temp_Node->y0-1][temp_Node->x0]-1)/N)+abs(temp_Node->x0-(temp_Node->Grid[temp_Node->y0-1][temp_Node->x0]-1)%N);
        exchanged_h=abs(temp_Node->y0-1-(temp_Node->Grid[temp_Node->y0-1][temp_Node->x0]-1)/N)+abs(temp_Node->x0-1-(temp_Node->Grid[temp_Node->y0-1][temp_Node->x0]-1)%N);
        neighbors.at(siz-1)->manhattan_h=temp_Node->manhattan_h+exchanged_h-original_h;
        neighbors.at(siz-1)->h=neighbors.at(siz-1)->manhattan_h+2*linear_vertical_conflicts+2*linear_horizontal_conflicts;
        neighbors.at(siz-1)->f=neighbors.at(siz-1)->h+neighbors.at(siz-1)->g;
        siz--;
    }
    if(temp_Node->x0-1>0)                                  //Space->Left, Move=Right;
    {
        linear_horizontal_conflicts=0;linear_vertical_conflicts=0;
        neighbors[siz-1]=(new Node(temp_Node->x0-1,temp_Node->y0,temp_Node->g+1,temp_Node));
        for(i=0;i<N;i++)
        {
            maxi=-1;
            maxj=-1;
            for(j=0;j<N;j++)
            {
                if(i==temp_Node->y0-1&&j==temp_Node->x0-1)
                    neighbors.at(siz-1)->Grid[i][j]=temp_Node->Grid[i][j-1];
                else if(i==temp_Node->y0-1&&j==temp_Node->x0-2)
                    neighbors.at(siz-1)->Grid[i][j]=temp_Node->Grid[i][j+1];
                else
                    neighbors.at(siz-1)->Grid[i][j]=temp_Node->Grid[i][j];
                if((i==temp_Node->y0-1&&j==temp_Node->x0-1))
                {
                    if(temp_Node->Grid[i][j-1]!=0&&(temp_Node->Grid[i][j-1]-1)/N==i)
                        if(temp_Node->Grid[i][j-1]>maxi)
                            maxi=temp_Node->Grid[i][j-1];
                        else
                            linear_horizontal_conflicts++;
                }
                else if(i==temp_Node->y0-1&&j==temp_Node->x0-2)
                {
                    if(temp_Node->Grid[i][j+1]!=0&&(temp_Node->Grid[i][j+1]-1)/N==i)
                        if(temp_Node->Grid[i][j+1]>maxi)
                            maxi=temp_Node->Grid[i][j+1];
                         else
                            linear_horizontal_conflicts++;
                }
                else
                {
                    if(temp_Node->Grid[i][j]!=0&&(temp_Node->Grid[i][j]-1)/N==i)
                        if(temp_Node->Grid[i][j]>maxi)
                            maxi=temp_Node->Grid[i][j];
                        else
                            linear_horizontal_conflicts++;
                }
                if((j==temp_Node->y0-1&&i==temp_Node->x0-1))
                {
                    if(temp_Node->Grid[j][i-1]!=0&&(temp_Node->Grid[j][i-1]-1)%N==i)
                        if(temp_Node->Grid[j][i-1]>maxj)
                            maxj=temp_Node->Grid[j][i-1];
                        else
                            linear_vertical_conflicts++;
                }
                else if(j==temp_Node->y0-1&&i==temp_Node->x0-2)
                {
                    if(temp_Node->Grid[j][i+1]!=0&&(temp_Node->Grid[j][i+1]-1)%N==i)
                        if(temp_Node->Grid[j][i+1]>maxj)
                            maxj=temp_Node->Grid[j][i+1];
                        else
                            linear_vertical_conflicts++;
                }
                else
                    if(temp_Node->Grid[j][i]!=0&&(temp_Node->Grid[j][i]-1)%N==i)
                        if(temp_Node->Grid[j][i]>maxj)
                            maxj=temp_Node->Grid[j][i];
                        else
                            linear_vertical_conflicts++;
            }
        }
        original_h=abs(temp_Node->y0-1-(temp_Node->Grid[temp_Node->y0-1][temp_Node->x0-2]-1)/N)+abs(temp_Node->x0-2-(temp_Node->Grid[temp_Node->y0-1][temp_Node->x0-2]-1)%N);
        exchanged_h=abs(temp_Node->y0-1-(temp_Node->Grid[temp_Node->y0-1][temp_Node->x0-2]-1)/N)+abs(temp_Node->x0-1-(temp_Node->Grid[temp_Node->y0-1][temp_Node->x0-2]-1)%N);
        neighbors.at(siz-1)->manhattan_h=temp_Node->manhattan_h+exchanged_h-original_h;
        neighbors.at(siz-1)->h=neighbors.at(siz-1)->manhattan_h+2*linear_vertical_conflicts+2*linear_horizontal_conflicts;
        neighbors.at(siz-1)->f=neighbors.at(siz-1)->h+neighbors.at(siz-1)->g;
        siz--;
    }
    if(temp_Node->y0+1<=N)                                  //Space->Down Move=Up
    {
        linear_horizontal_conflicts=0;linear_vertical_conflicts=0;
        neighbors[siz-1]=(new Node(temp_Node->x0,temp_Node->y0+1,temp_Node->g+1,temp_Node));
        for(i=0;i<N;i++)
        {
            maxi=-1;
            maxj=-1;
            for(j=0;j<N;j++)
            {
                if(i==temp_Node->y0-1&&j==temp_Node->x0-1)
                    neighbors.at(siz-1)->Grid[i][j]=temp_Node->Grid[i+1][j];
                else if(i==temp_Node->y0&&j==temp_Node->x0-1)
                    neighbors.at(siz-1)->Grid[i][j]=temp_Node->Grid[i-1][j];
                else
                    neighbors.at(siz-1)->Grid[i][j]=temp_Node->Grid[i][j];
                if((i==temp_Node->y0-1&&j==temp_Node->x0-1))
                {
                    if(temp_Node->Grid[i+1][j]!=0&&(temp_Node->Grid[i+1][j]-1)/N==i)
                        if(temp_Node->Grid[i+1][j]>maxi)
                            maxi=temp_Node->Grid[i+1][j];
                        else
                            linear_horizontal_conflicts++;
                }
                else if(i==temp_Node->y0&&j==temp_Node->x0-1)
                {
                    if(temp_Node->Grid[i-1][j]!=0&&(temp_Node->Grid[i-1][j]-1)/N==i)
                        if(temp_Node->Grid[i-1][j]>maxi)
                            maxi=temp_Node->Grid[i-1][j];
                         else
                            linear_horizontal_conflicts++;
                }
                else
                {
                    if(temp_Node->Grid[i][j]!=0&&(temp_Node->Grid[i][j]-1)/N==i)
                        if(temp_Node->Grid[i][j]>maxi)
                            maxi=temp_Node->Grid[i][j];
                        else
                            linear_horizontal_conflicts++;
                }
                if((j==temp_Node->y0-1&&i==temp_Node->x0-1))
                {
                    if(temp_Node->Grid[j+1][i]!=0&&(temp_Node->Grid[j+1][i]-1)%N==i)
                        if(temp_Node->Grid[j+1][i]>maxj)
                            maxj=temp_Node->Grid[j+1][i];
                        else
                            linear_vertical_conflicts++;
                }
                else if(j==temp_Node->y0&&i==temp_Node->x0-1)
                {
                    if(temp_Node->Grid[j-1][i]!=0&&(temp_Node->Grid[j-1][i]-1)%N==i)
                        if(temp_Node->Grid[j-1][i]>maxj)
                            maxj=temp_Node->Grid[j-1][i];
                        else
                            linear_vertical_conflicts++;
                }
                else
                    if(temp_Node->Grid[j][i]!=0&&(temp_Node->Grid[j][i]-1)%N==i)
                        if(temp_Node->Grid[j][i]>maxj)
                            maxj=temp_Node->Grid[j][i];
                        else
                            linear_vertical_conflicts++;
            }
        }
        original_h=abs(temp_Node->y0-(temp_Node->Grid[temp_Node->y0][temp_Node->x0-1]-1)/N)+abs(temp_Node->x0-1-(temp_Node->Grid[temp_Node->y0][temp_Node->x0-1]-1)%N);
        exchanged_h=abs(temp_Node->y0-1-(temp_Node->Grid[temp_Node->y0][temp_Node->x0-1]-1)/N)+abs(temp_Node->x0-1-(temp_Node->Grid[temp_Node->y0][temp_Node->x0-1]-1)%N);
        neighbors.at(siz-1)->manhattan_h=temp_Node->manhattan_h+exchanged_h-original_h;
        neighbors.at(siz-1)->h=neighbors.at(siz-1)->manhattan_h+2*linear_vertical_conflicts+2*linear_horizontal_conflicts;
        neighbors.at(siz-1)->f=neighbors.at(siz-1)->h+neighbors.at(siz-1)->g;
        siz--;
    }
    if(temp_Node->y0-1>0)                                   //Space->Up  Move=Down
    {
        linear_horizontal_conflicts=0;linear_vertical_conflicts=0;
        neighbors[siz-1]=(new Node(temp_Node->x0,temp_Node->y0-1,temp_Node->g+1,temp_Node));
        for(i=0;i<N;i++)
        {
            maxi=-1;
            maxj=-1;
            for(j=0;j<N;j++)
            {
                if(i==temp_Node->y0-1&&j==temp_Node->x0-1)
                    neighbors.at(siz-1)->Grid[i][j]=temp_Node->Grid[i-1][j];
                else if(i==temp_Node->y0-2&&j==temp_Node->x0-1)
                    neighbors.at(siz-1)->Grid[i][j]=temp_Node->Grid[i+1][j];
                else
                    neighbors.at(siz-1)->Grid[i][j]=temp_Node->Grid[i][j];
                if((i==temp_Node->y0-1&&j==temp_Node->x0-1))
                {
                    if(temp_Node->Grid[i-1][j]!=0&&(temp_Node->Grid[i-1][j]-1)/N==i)
                        if(temp_Node->Grid[i-1][j]>maxi)
                            maxi=temp_Node->Grid[i-1][j];
                        else
                            linear_horizontal_conflicts++;
                }
                else if(i==temp_Node->y0-2&&j==temp_Node->x0-1)
                {
                    if(temp_Node->Grid[i+1][j]!=0&&(temp_Node->Grid[i+1][j]-1)/N==i)
                        if(temp_Node->Grid[i+1][j]>maxi)
                            maxi=temp_Node->Grid[i+1][j];
                         else
                            linear_horizontal_conflicts++;
                }
                else
                {
                    if(temp_Node->Grid[i][j]!=0&&(temp_Node->Grid[i][j]-1)/N==i)
                        if(temp_Node->Grid[i][j]>maxi)
                            maxi=temp_Node->Grid[i][j];
                        else
                            linear_horizontal_conflicts++;
                }
                if((j==temp_Node->y0-1&&i==temp_Node->x0-1))
                {
                    if(temp_Node->Grid[j-1][i]!=0&&(temp_Node->Grid[j-1][i]-1)%N==i)
                        if(temp_Node->Grid[j-1][i]>maxj)
                            maxj=temp_Node->Grid[j-1][i];
                        else
                            linear_vertical_conflicts++;
                }
                else if(j==temp_Node->y0-2&&i==temp_Node->x0-1)
                {
                    if(temp_Node->Grid[j+1][i]!=0&&(temp_Node->Grid[j+1][i]-1)%N==i)
                        if(temp_Node->Grid[j+1][i]>maxj)
                            maxj=temp_Node->Grid[j+1][i];
                        else
                            linear_vertical_conflicts++;
                }
                else
                    if(temp_Node->Grid[j][i]!=0&&(temp_Node->Grid[j][i]-1)%N==i)
                        if(temp_Node->Grid[j][i]>maxj)
                            maxj=temp_Node->Grid[j][i];
                        else
                            linear_vertical_conflicts++;

            }
        }
        original_h=abs(temp_Node->y0-2-(temp_Node->Grid[temp_Node->y0-2][temp_Node->x0-1]-1)/N)+abs(temp_Node->x0-1-(temp_Node->Grid[temp_Node->y0-2][temp_Node->x0-1]-1)%N);
        exchanged_h=abs(temp_Node->y0-1-(temp_Node->Grid[temp_Node->y0-2][temp_Node->x0-1]-1)/N)+abs(temp_Node->x0-1-(temp_Node->Grid[temp_Node->y0-2][temp_Node->x0-1]-1)%N);
        neighbors.at(siz-1)->manhattan_h=temp_Node->manhattan_h+exchanged_h-original_h;
        neighbors.at(siz-1)->h=neighbors.at(siz-1)->manhattan_h+2*linear_vertical_conflicts+2*linear_horizontal_conflicts;
        neighbors.at(siz-1)->f=neighbors.at(siz-1)->h+neighbors.at(siz-1)->g;
    }
}
void construct(Node* Node)                                                                       //Display Solution
{
    if(Node->parent!=NULL)
    {
        construct(Node->parent);
    }
    if(Node->parent!=NULL)
    {
        if(Node->parent->x0<Node->x0)
            cout<<"Left";
        else if(Node->parent->x0>Node->x0)
            cout<<"Right";
        else if(Node->parent->y0<Node->y0)
            cout<<"Up";
        else
            cout<<"Down";
        cout<<endl;
    }
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            cout<<Node->Grid[i][j]<<"\t";
        }
        cout<<endl<<endl;
    }
    system("pause");
    cout<<endl;
}
void astar()                                                                                             //Generalized Algorithm
{
    while(!open_list.empty())
    {
        process=*open_list_dup.begin();                                                             //Node with min(f)
        if(process->h==0)                                                                          //Node found
        {
            tEnd=clock()-tStart;
            return construct(process);
        }
        else
        {
            open_list_dup.erase(open_list_dup.begin());
            open_list.erase(process);                                                               //Remove from nodes to be checked
            closed_list.insert(process);                                                            //Insert in checked nodes
            Neighbors(process);                                                                     //Vector of next possible moves b/w 2-4
            for(auto temp_Node:neighbors)
            {
                if(closed_list.count(temp_Node)==0)
                {
                    open_list_cpy=open_list.find(temp_Node);                                        //Iterator to node in 'to be checked nodes' same as new node
                    if(open_list_cpy==open_list.end())                                              //If new node not already in 'to be checked nodes'
                    {
                        open_list.insert(temp_Node);                                                //Insert in both open list's
                        open_list_dup.insert(temp_Node);
                    }
                    else
                    {
                        if(temp_Node->g<(*open_list_cpy)->g)                                        //More efficient parent found(new node)
                        {
                            f=temp_Node->f;                                                         //Reprioritizing/refresh/update priority queue
                            temp_Node->f=(*open_list_cpy)->f;                                       //simulating set
                            open_list_dup.erase(temp_Node);                                         //
                            temp_Node->f=f;                                                         //
                            open_list_dup.insert(temp_Node);                                        //
                            (*open_list_cpy)->g=temp_Node->g;                                       //changing the f,g and parent values
                            (*open_list_cpy)->f=temp_Node->f;                                       //of original node.
                            (*open_list_cpy)->parent=temp_Node->parent;                             //
                        }
                    }
                }
            }
        }
    }
    cout<<"No Solution";                                                                                //All Nodes to be checked are exhausted
}
int main()
{
    linear_horizontal_conflicts=0;linear_vertical_conflicts=0;
    tStart=clock();
    neighbors.reserve(4);
    for(i=0;i<N;i++)                                                        //Start Recognition and filling 'start' members.
    {
        maxi=-1;
        maxj=-1;
        for(j=0;j<N;j++)
        {
            if(grid[i][j]!=0&&(grid[i][j]-1)/N==i)
                if(grid[i][j]>maxi)
                    maxi=grid[i][j];
                else
                    linear_horizontal_conflicts++;
            if(grid[j][i]!=0&&(grid[j][i]-1)%N==i)
                if(grid[j][i]>maxj)
                    maxj=grid[j][i];
                else
                    linear_vertical_conflicts++;
            if(grid[i][j]==0)
            {
                start.x0=j+1;
                start.y0=i+1;
            }
            else
                start.manhattan_h+=abs(i-(grid[i][j]-1)/N)+abs(j-(grid[i][j]-1)%N);
            start.Grid[i][j]=grid[i][j];
        }
    }
    start.h=start.manhattan_h+2*linear_horizontal_conflicts+2*linear_vertical_conflicts;
    start.g=0;                                                                                  //Initial 0th move
    start.f=start.g+start.h;                                                                    //f=g+h
    open_list.insert(&start);                                                                   //Insert start in both lists
    open_list_dup.insert(&start);
    astar();
    cout<<"Moves Taken:\t"<<process->g<<endl;                                                   //Run Algorithm
    cout<<"Time  Taken:\t"<<(double)tEnd/CLOCKS_PER_SEC<<endl;
}
