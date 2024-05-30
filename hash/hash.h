#pragma once
#include<iostream>
#include<vector>
#include<fstream>
#include<string.h>
#include <algorithm>
#include <math.h>
#include <cstdlib>
#include <assert.h>
#include<functional>
#include<string>
#include<sstream>
#include<iterator>
#include<set>
#include <sys/resource.h>
// #define VNUM 2000000
using namespace std;

class Vertex_Value
{
    public:
    int vertex;
    float value;
    public:
    Vertex_Value(int x,float y);
    Vertex_Value();
    bool operator ==(Vertex_Value) const;
    bool operator <(Vertex_Value) const;

    Vertex_Value operator=(Vertex_Value) const;
};
class node
{
    public:
    int vertex;
    vector<Vertex_Value> vec;
    node()
    {}
    node(const node &n)
    {
        vertex=n.vertex;
        vec=n.vec;
    }
};
class G
{
    public:
    int vernum,arcnum;
    node *adj;
    int *deg;
    int maxdeg;
    // void init_deg(); 
    // G()
    // {

    // };
    // G(const G &c)
    // {
    //     vernum=c.vernum;
    //     arcnum=c.arcnum;
    //     maxdeg=c.maxdeg;
    //     adj=new node[vernum];
    //     deg=new int[vernum];
    //     for(int i=0;i<vernum;i++)
    //     {
    //         node tmp(c.adj[i]);
    //         adj[i]=tmp;
    //         deg[i]=c.deg[i];
    //     }
    // };
    ~G()
    {
        delete[] adj;
        delete[] deg;
    };
};


class maximal_eta_clique
{
    public:
    //int deg[VNUM];
    float eta;
    
    vector<int> LB;
    vector<int> topcore,colors;
    vector<bool> subset, checknodes;
	vector<int> counts, clset;
    public:
    void read_graph(const char *str,G &g);
    void maximaletaclique(vector<int> R,int rsize,float q,vector<Vertex_Value> C,int csize,vector<Vertex_Value> X,int xsize,vector<int> P,float eta, G &g,int &count);
    void hash_algorithm(set<size_t> &clique_hash);
    int maxC(vector<int> R);
    vector<int> adjVertex(int m, G &g);
    float FindValue(int u, int v,  G &g);
    float clq(vector<int> R, float q, Vertex_Value D, int m, G &g);
    bool isbelongto(int m, vector<int> S1);
    void removeedge( G &g,int u,int v);
    vector<Vertex_Value> Generateset(int m, float q, vector<Vertex_Value> C, G &g,bool cox);
    vector<Vertex_Value> update(vector<Vertex_Value> &C, vector<int> &Q);
    void UncertainMC(vector<int> R, float q, vector<Vertex_Value> C, vector<Vertex_Value>& X,G &g);

};
class Include_Edge_Clique:public maximal_eta_clique
{
    public:
    int u,v;
    float q;
    unsigned long long iterations=0;
    double begin1_clock,ending1_clock,elp1_clock=0;
    double begin2_clock,ending2_clock,elp2_clock=0;
    public:
    void baohanbian( G &g,vector<vector<int> > &clique);
    vector<int> publicadj(int u,int v);
    vector<int> maximaletaclique(vector<int> R,float q,vector<Vertex_Value> C,vector<Vertex_Value> X,vector<int> P,float eta, G &g,vector<vector<int>> &clique); 
    void qubian(G &g,int u,int v,int &change_size);
    void jiabian(G &g,int u,int v,float q,int &change_size,set<size_t> &clique_hash);
    void gailvzengjia(G &g,int u,int v,float q,int &change_size,set<size_t> &clique_hash);
    void baohanbian(G &g,vector<vector<int> > &clique,float q);
    void gailvjianxiao(G &g,int u,int v,float q,int &change_size);
    void print_information();
    long getMemoryUse();

};