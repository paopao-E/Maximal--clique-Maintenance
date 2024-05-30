#include "hash.h"
Vertex_Value::Vertex_Value(int x, float y)
{
    vertex = x;
    value = y;
}
Vertex_Value::Vertex_Value(void)
{
}

bool Vertex_Value::operator ==(const Vertex_Value B) const
{
    if ((vertex == B.vertex) && (value == B.value))
        return true;
    else
        return false;
}
bool Vertex_Value::operator <(const Vertex_Value B) const
{
    if(vertex <B.vertex)
        return true;
    else
        return false;
}

Vertex_Value Vertex_Value::operator =(const Vertex_Value B) const
{
    Vertex_Value m;
    m.vertex=B.vertex;
    m.value=B.value;
    return m;
}

void maximal_eta_clique::read_graph(const char *str, G &g)
{
    
    ifstream infile(str);
    cout << "开始读入文件" << endl;
    infile >> g.vernum >> g.arcnum;
    printf("顶点数=%d,边数=%d\n", g.vernum, g.arcnum);
    g.adj = new node[g.vernum];
    g.deg = new int[g.vernum];
    memset(g.deg, 0, g.vernum * sizeof(int));
    // cout<<"开始建立邻接表"<<endl;
    for (int i = 0; i < g.arcnum; i++)
    {
        int head, tail,head0, tail0;
        float value;
        infile >> head0 >> tail0 >> value;
        head=head0-1;
        tail=tail0-1;
        if (head < tail)
        {
            g.deg[head]++;
            g.deg[tail]++;
        }
        g.adj[head].vertex = head;
        Vertex_Value m;
        m.vertex = tail;
        m.value = value;
        g.adj[head].vec.push_back(m);
        g.adj[tail].vertex = tail;
        Vertex_Value m1;
        m1.vertex = head;
        m1.value = value;
        g.adj[tail].vec.push_back(m1);
    }

    cout << "end of read graph" << endl;
    cout << "#vertices = " << g.vernum << endl;
    cout << "#edges = " << g.arcnum << endl;
}

void maximal_eta_clique::hash_algorithm(set<size_t> &clique_hash)
{
    int count=0;
    string s;
    
    ifstream infile("./enron_test.txt");
    infile>>count;
    hash<string>szHash;
    getline(infile,s);
    for(int i=0;i<count;i++)
    {
        getline(infile,s);
        // cout<<s<<endl;
        clique_hash.insert(szHash(s));
    }
}
int maximal_eta_clique::maxC(vector<int> R) 
{
    if (R.empty())
    {
        return 0;
    }
    int max = -1;
    int i;
    for (i = 0; i < R.size(); i++)
    {
        if (max < R[i])
        {
            max = R[i];
        }
    }
    return max;
}
vector<int> maximal_eta_clique::adjVertex(int m, G &g) 
{
    vector<int> S;
    int i;

    for (i = 0; i < g.adj[m].vec.size(); i++)
    {
        S.push_back(g.adj[m].vec[i].vertex);
    }
    return S;
}
float maximal_eta_clique::FindValue(int u, int v,G &g) 
{
    int i;
    for (i = 0; i < g.adj[u].vec.size(); i++)
    {
        int b=g.adj[u].vec[i].vertex;
        if (g.adj[u].vec[i].vertex == v)
        {
            float a = g.adj[u].vec[i].value;
            return g.adj[u].vec[i].value;
        }
    }
}
float maximal_eta_clique::clq(vector<int> R, float q, Vertex_Value D, int m, G &g) 
{
    float temp;
    temp = FindValue(D.vertex, m, g);
    return q * D.value * temp;
}
bool maximal_eta_clique::isbelongto(int m, vector<int> S1) 
{
    for (int i = 0; i < S1.size(); i++)
    {
        if (m == S1[i])
        {
            return true;
        }
    }
    return false;
}

void maximal_eta_clique::removeedge( G &g,int u,int v)
{

    int j=0;
    vector<Vertex_Value>m;
    for(int i=0;i<g.adj[u].vec.size();i++)
    {
        m.push_back(g.adj[u].vec[i]);
    }
    int c=m.size();
    g.adj[u].vec.clear();
    for(int i=0;i<c;i++)
    {
        if(m[i].vertex!=v)
            g.adj[u].vec.push_back(m[i]);   
    }
    int k=0;
    vector<Vertex_Value>m1;
    for(int i=0;i<g.adj[v].vec.size();i++)
    {
        m1.push_back(g.adj[v].vec[i]);
    }
    int c1=m1.size();
    g.adj[v].vec.clear();
    for(int i=0;i<c1;i++)
    {
        if(m1[i].vertex!=u)
            g.adj[v].vec.push_back(m1[i]);   
    }
    m.clear();
    m1.clear();

}

vector<Vertex_Value> maximal_eta_clique::Generateset(int m, float q, vector<Vertex_Value> C, G &g, bool cox) //产生候选集
{
    vector<int> S2 = adjVertex(m, g);
    vector<Vertex_Value> Cn;
    vector<int> S1;
    vector<int>S3;
    int i;
    for (i = 0; i < C.size(); i++)
    {
        S1.push_back(C[i].vertex);
    }
    sort(S1.begin(),S1.end());
    sort(S2.begin(),S2.end());
    set_intersection(S1.begin(),S1.end(),S2.begin(),S2.end(),back_inserter(S3));
    // S1 = mixede(S1, S2);
    int j = 0;
    for (i = 0, j; i < C.size(); i++)
    {
        if (cox || C[i].vertex > m)
            if (isbelongto(C[i].vertex, S3))
            {
                float a = FindValue(C[i].vertex, m, g);
                a = q * C[i].value * FindValue(C[i].vertex, m, g);
                if (q * C[i].value * FindValue(C[i].vertex, m, g) >= eta)
                {
                    Cn.push_back(C[i]);
                    Cn[j++].value = C[i].value * FindValue(C[i].vertex, m, g);
                }
            }
    }
    return Cn;
}
vector<Vertex_Value> maximal_eta_clique::update(vector<Vertex_Value> &C, vector<int> &Q)
{
    for (int i = 0; i < C.size(); i++)
    {
        Vertex_Value m = C[i];
        // int m=C[i].vertex;
        for (int j = 0; j < Q.size(); j++)
        {
            if (m.vertex == Q[j])
            {
                for (vector<Vertex_Value>::iterator iter = C.begin(); iter != C.end(); iter++)
                { //从vector中删除指定的某一个元素
                    if (*iter == C[i])
                    {
                        C.erase(iter);
                        break;
                    }
                }
            }
            
        }
    }
    return C;
}


void maximal_eta_clique::UncertainMC(vector<int> R, float q, vector<Vertex_Value> C, vector<Vertex_Value> &X,  G &g)
{
    // int i;
    return;
}

vector<int> Include_Edge_Clique::maximaletaclique(vector<int> R, float q, vector<Vertex_Value> C, vector<Vertex_Value> X, vector<int> P, float eta,  G &g, vector<vector<int>> &clique)
{
    iterations++;
    if (C.empty() && X.empty())
    {
        //cout << "产生一个极大clique" << endl;
        sort(R.begin(),R.end());
        clique.push_back(R);
        // for (int i = 0; i < R.size(); i++)
        // {
        //     cout << R[i] << " ";
        // }
        // cout << endl;
        return P;
    }


    vector<int> Q;
    Q.clear();
  
    
    //枢轴算法
    for(auto el:C)
    {
    
        if(find(Q.begin(), Q.end(), el.vertex) == Q.end())
        {
            Vertex_Value pivot;
            vector<int> Pn;
            vector<int> Rn;
            pivot.vertex = el.vertex;
            pivot.value = el.value;
            R.push_back(pivot.vertex);
            Pn = R;
            float qn = q * pivot.value;
            vector<Vertex_Value> Cn = Generateset(pivot.vertex, qn, C, g, true);
            vector<Vertex_Value> Xn = Generateset(pivot.vertex, qn, X, g, true);
            Pn=maximaletaclique(R, qn, Cn, Xn, Pn, eta, g, clique);
            
            R.pop_back();
            if (Q.size() < Pn.size())

                Q = Pn;
            if (P.size() < Pn.size())
                P = Pn;
            X.push_back(pivot);

            for(int k=0;k<C.size();k++) 
            {
                if(C[k]==pivot)
                {
                    C[k].vertex=C[C.size()-1].vertex;
                    C[k].value=C[C.size()-1].value;
                }
            }
            C.pop_back();
        }
        else{
            continue;
        }
    }


    //不是基于枢轴的方法
    // while (C.size() != 0)
    // {
    //     int maxdegree = 0, pos = 0;
    //     Vertex_Value pivot;
    //     vector<int> Pn;
    //     vector<int> Rn;
    //     for (int i = 0; i < C.size(); i++)
    //     {
    //         if (g.deg[C[i].vertex] > maxdegree)
    //         {
    //             maxdegree = g.deg[C[i].vertex];
    //             pos = i;
    //         }
    //     }
    //     pivot.vertex = C[pos].vertex;
    //     pivot.value = C[pos].value;

    //     R.push_back(pivot.vertex);
    //     Pn = R;
    //     float qn = q * pivot.value;
    //     vector<Vertex_Value> Cn = Generateset(pivot.vertex, qn, C, g, true);
    //     vector<Vertex_Value> Xn = Generateset(pivot.vertex, qn, X, g, true);
    //     maximaletaclique(R, qn, Cn, Xn, Pn, eta, g, clique);

    //     R.pop_back();
    //     // if (Q.size() < Pn.size())
    //     //     Q = Pn;
    //     // if (P.size() < Pn.size())
    //     //     P = Pn;
    //     X.push_back(pivot);

    //     for(int i=0;i<C.size();i++)
    //     {
    //         if(C[i]==pivot)
    //         {
    //             C[i].vertex=C[C.size()-1].vertex;
    //             C[i].value=C[C.size()-1].value;

    //         }
    //     }
    //     C.pop_back();       
        
    // }    
    return P;    
}
// }
void Include_Edge_Clique::baohanbian( G &g, vector<vector<int> > &clique)
{
    vector<int> R;
    vector<int> P;
    vector<int> Q;
    vector<Vertex_Value> Y;
    vector<Vertex_Value> X;
    q = FindValue(u, v, g);
    //将u，v加入到R中
    R.push_back(u);
    R.push_back(v);
    //找到u，v的公共邻居
    //这一步其实就是找两个集合的交集。
    sort(g.adj[u].vec.begin(),g.adj[u].vec.end());
    
    sort(g.adj[v].vec.begin(),g.adj[v].vec.end());
    

    set_intersection(g.adj[u].vec.begin(),g.adj[u].vec.end(),g.adj[v].vec.begin(),g.adj[v].vec.end(),inserter(Y,Y.begin()));
    
   
    vector<Vertex_Value> C = Generateset(v, q, Y, g, true);
    maximaletaclique(R, q, C, X, P, eta, g, clique);
}

void Include_Edge_Clique::baohanbian( G &g,vector<vector<int> > &clique,float q)
{
   vector<int>R;
   vector<int>P;
   vector<int>Q;
   vector<Vertex_Value>Y; 
   vector<Vertex_Value>X;
//    int aa=0;
   R.push_back(u);
   if(q>eta) R.push_back(v);  
   
   node c=g.adj[u];
   vector<Vertex_Value>::iterator a=g.adj[u].vec.begin();
   vector<Vertex_Value>::iterator b=g.adj[u].vec.end();
   sort(g.adj[u].vec.begin(),g.adj[u].vec.end());  
   
    sort(g.adj[v].vec.begin(),g.adj[v].vec.end());

    set_intersection(g.adj[u].vec.begin(),g.adj[u].vec.end(),g.adj[v].vec.begin(),g.adj[v].vec.end(),inserter(Y,Y.begin()));

   vector<Vertex_Value>C=Generateset(v,q,Y,g,true);
   maximaletaclique(R,q,C,X,P,eta,g,clique);
}
void Include_Edge_Clique::qubian(G &g, int u, int v,int &change_size)
{
    
    vector<vector<int> > clique;
    vector<vector<int> > Del;
    vector<vector<int> > New;
    vector<vector<int> > new_clique;
    begin2_clock=clock();
    baohanbian(g, clique);
    
    Del = clique;
    ending2_clock=clock();
    elp2_clock+=(ending2_clock-begin2_clock)/CLOCKS_PER_SEC;
    // for (int i = 0; i < Del.size(); i++)
    // {
    //     cout << "消失的极大n-clique有" << endl;
    //     for (int j = 0; j < Del[i].size(); j++)
    //     {
    //         cout << Del[i][j]<<" ";
    //     }
    //     cout << endl;
    // }
    g.arcnum = g.arcnum - 1;
    
    begin1_clock=clock();
    g.deg[u]--;
    g.deg[v]--;
    removeedge(g,u,v);
       
    for (int i = 0; i < clique.size(); i++)
    {
       
        for (vector<int>::iterator iter = clique[i].begin(); iter != clique[i].end(); iter++)
        { //从vector中删除指定的某一个元素
            if (*iter == u)
            {
                clique[i].erase(iter);
                break;
            }
        }
        new_clique.push_back(clique[i]);
        clique[i].push_back(u);
        for (vector<int>::iterator iter = clique[i].begin(); iter != clique[i].end(); iter++)
        { //从vector中删除指定的某一个元素
            if (*iter == v)
            {
                clique[i].erase(iter);
                break;
            }
        }
        new_clique.push_back(clique[i]);
        clique[i].push_back(v);

        for (int j = 0; j < new_clique.size(); j++)
        {
            if(new_clique[j].size()==1) New.push_back(new_clique[j]);
            else
            {           
                vector<vector<int> > mxed;

                for (int m = 0; m < new_clique[j].size(); m++)
                {

                    vector<int> a;

                    for (int k = 0; k < g.adj[new_clique[j][m]].vec.size(); k++)
                    {
                        
                        a.push_back(g.adj[new_clique[j][m]].vec[k].vertex);
                        
                    }
                    sort(a.begin(),a.end());
                    mxed.push_back(a);
                }
                
                vector<int> mx;
                set_intersection(mxed[0].begin(),mxed[0].end(),mxed[1].begin(),mxed[1].end(),back_inserter(mx));
                for (int i = 2; i < mxed.size(); i++)
                {
                    vector<int>mx1;
                    set_intersection(mx.begin(),mx.end(),mxed[i].begin(),mxed[i].end(),back_inserter(mx1));
                    mx.clear();
                    mx=mx1;
                }
                int d = mx.size();
                if(mx.size()==0) New.push_back(new_clique[j]);
                else
                {
                    float b=1;
                    for(int i=0;i<new_clique[j].size();i++)
                    {
                        for(int k=i+1;k<new_clique[j].size();k++)
                        {
                            b=b*FindValue(new_clique[j][i],new_clique[j][k],g);
                        }
                    }
                        
                    bool a=true;

                    for(int m=0;m<mx.size();m++)
                    {
                        if(new_clique[j].size()==1)
                        {
                            a=false;
                            break;
                        }
                        else
                        {
                            float x=1;
                            for(int k=0;k<new_clique[j].size();k++)
                            {
                                x=x*FindValue(mx[m],new_clique[j][k],g);
                            }
                            if(x*b>eta)
                            {
                                a=false;
                                break;
                            } 
                        }

                    }
                
                    if(a)
                    New.push_back(new_clique[j]);
                }
            }
        }
        new_clique.clear();
    }

    // for (int i = 0; i < New.size(); i++)
    // {
    //     cout << "new clique have:" << endl;
    //     for (int j = 0; j < New[i].size(); j++)
    //     {
    //         cout << New[i][j]<<" ";
    //     }
    //     cout << endl;
    // }
    
    // cout<<"new clique have: "<<New.size()<<endl;
    // cout<<"del clique have: "<<Del.size()<<endl;
    change_size=change_size+New.size()+Del.size();

    ending1_clock=clock();
    elp1_clock+=(ending1_clock-begin1_clock)/CLOCKS_PER_SEC;

    clique.clear();
    Del.clear();
    New.clear();
    new_clique.clear();
    

}
void Include_Edge_Clique::jiabian(G &g, int u, int v, float q,int &change_size,set<size_t> &clique_hash)
{
    vector<vector<int> > clique;
    vector<vector<int> > Del;
    vector<vector<int> > New;
    vector<vector<int> > new_clique;
    
    begin1_clock=clock();
    g.arcnum = g.arcnum + 1;
    
    g.deg[u]++;
    g.deg[v]++;
    g.adj[u].vertex = u;
    Vertex_Value m;
    m.vertex = v;
    m.value = q;
    g.adj[u].vec.push_back(m);
    g.adj[v].vertex = v;
    Vertex_Value m1;
    m1.vertex = u;
    m1.value = q;
    g.adj[v].vec.push_back(m1);
    if (q >= eta)
    {
        baohanbian(g, clique);
    }
    New = clique;
    for(int i=0;i<New.size();i++)
    {
        stringstream ss;
        string str;
        copy(New[i].begin(),New[i].end(),ostream_iterator<int>(ss,""));
        str=ss.str();
        hash<string>sHash;
        clique_hash.insert(sHash(str));
    }
    ending1_clock=clock();
    elp1_clock+=(ending1_clock-begin1_clock)/CLOCKS_PER_SEC;
    // for (int i = 0; i < New.size(); i++)
    // {
    //     cout << "new clique have:" << endl;
    //     for (int j = 0; j < New[i].size(); j++)
    //     {
    //         cout << New[i][j]<<" ";
    //     }
    //     cout << endl;
    // }
    begin2_clock=clock();
    removeedge(g,u,v);
    
    
    for (int i = 0; i < New.size(); i++)
    {
        new_clique.clear();
        
        for (vector<int>::iterator iter = New[i].begin(); iter != New[i].end(); iter++)
        { //从vector中删除指定的某一个元素
            if (*iter == u)
            {
                New[i].erase(iter);
                break;
            }
        }
        new_clique.push_back(New[i]);
        New[i].push_back(u);
        for (vector<int>::iterator iter = New[i].begin(); iter != New[i].end(); iter++)
        { //从vector中删除指定的某一个元素
            if (*iter == v)
            {
                New[i].erase(iter);
                break;
            }
        }
        new_clique.push_back(New[i]);
        New[i].push_back(v);
        hash<string>sHash;
        for (int j = 0; j < new_clique.size(); j++)
        {
            sort(new_clique[j].begin(),new_clique[j].end());
            stringstream ss;
            string str;
            copy(new_clique[j].begin(),new_clique[j].end(),ostream_iterator<int>(ss,""));
            str=ss.str();
            auto it=clique_hash.find(sHash(str));
            if(it!=clique_hash.end())
            { 
                Del.push_back(new_clique[j]);
                clique_hash.erase(sHash(str));
            }
        }
        
    }

    ending2_clock=clock();
    
    
    // for (int i = 0; i < Del.size(); i++)
    // {
    //     cout << "消失的极大n-clique有" << endl;
    //     for (int j = 0; j < Del[i].size(); j++)
    //     {
    //         cout << Del[i][j]<<" ";
    //     }
    //     cout << endl;
    // }
    // cout<<"new clique have: "<<New.size()<<endl;
    // cout<<"del clique have: "<<Del.size()<<endl;
    change_size=change_size+New.size()+Del.size();

    
    elp2_clock+=(ending2_clock-begin2_clock)/CLOCKS_PER_SEC;

    g.adj[u].vertex = u;
    Vertex_Value a;
    a.vertex = v;
    a.value = q;
    g.adj[u].vec.push_back(a);
    g.adj[v].vertex = v;
    Vertex_Value b;
    b.vertex = u;
    b.value = q;
    g.adj[v].vec.push_back(b);
    
}
void Include_Edge_Clique::gailvzengjia( G &g, int u, int v, float q,int &change_size,set<size_t> &clique_hash)
{
    vector<vector<int> > S;
    vector<vector<int> > F;
    vector<int>Q;
    vector<vector<int> > Del;
    vector<vector<int> > New;
    vector<Vertex_Value> C;
    vector<Vertex_Value> X;
    vector<vector<int> > clique;
    vector<int> R;
    vector<int> P;
    set<int>new_vertex;
    
    begin1_clock=clock();
    float s = FindValue(u, v, g);
    if (q > s)
    {
        // baohanbian(g, S, s);
        baohanbian(g, F, q);
    }

    hash<string>sHash;
    for(int i=0;i<F.size();i++)
    {
        sort(F[i].begin(),F[i].end());
        stringstream ss;
        string str;
        copy(F[i].begin(),F[i].end(),ostream_iterator<int>(ss,""));
        str=ss.str();
        auto it=clique_hash.find(sHash(str));
        if(it==clique_hash.end())
        {
            New.push_back(F[i]);
            sort(F[i].begin(),F[i].end());
            stringstream ss;
            string str;
            copy(F[i].begin(),F[i].end(),ostream_iterator<int>(ss,""));
            str=ss.str();
            // hash<string>sHash;
            clique_hash.insert(sHash(str));
            for(int j=0;j<F[i].size();j++)
            {
                new_vertex.insert(F[i][j]);
            }
        }
    }

    ending1_clock=clock();
    elp1_clock+=(ending1_clock-begin1_clock)/CLOCKS_PER_SEC;
    // for (int i = 0; i < New.size(); i++)
    // {
    //     cout << "new clique have:" << endl;
    //     for (int j = 0; j < New[i].size(); j++)
    //     {
    //         cout << New[i][j]<<" ";
    //     }
    //     cout << endl;
    // }
    begin2_clock=clock();


    C.clear();
    for(auto y:new_vertex)
    {
        Vertex_Value temp;
        temp.vertex = y;
        temp.value = 1;
        C.push_back(temp);
    }
    
    X.clear();
    Q.clear();
    clique.clear();
    R.clear();
    P.clear();
    maximaletaclique(R, 1, C, X, P, eta, g, clique);
    hash<string>wHash;
    for (int j = 0; j < clique.size(); j++)
    {
        sort(clique[j].begin(),clique[j].end());
        stringstream ss;
        string str;
        copy(clique[j].begin(),clique[j].end(),ostream_iterator<int>(ss,""));
        str=ss.str();
        auto it=clique_hash.find(wHash(str));
        if(it!=clique_hash.end())
        {
            Del.push_back(clique[j]);
            clique_hash.erase(wHash(str));
        }
    }



//     for (int i = 0; i < New.size(); i++)
//     {
//         C.clear();
//         X.clear();
//         Q.clear();
//         clique.clear();
//         R.clear();
//         P.clear();
//         for (int j = 0; j < New[i].size(); j++)
//         {
//             Vertex_Value temp;
//             temp.vertex = New[i][j];
//             temp.value = 1;
//             C.push_back(temp);
//         }
//         maximaletaclique(R, 1, C, X, P, eta, g, clique);

//         hash<string>sHash;
//         for (int j = 0; j < clique.size(); j++)
//         {
//             sort(clique[j].begin(),clique[j].end());
//             stringstream ss;
//             string str;
//             copy(clique[j].begin(),clique[j].end(),ostream_iterator<int>(ss,""));
//             str=ss.str();
//             auto it=clique_hash.find(sHash(str));
//             if(it!=clique_hash.end())
//             {
//                 Del.push_back(clique[j]);
//                 clique_hash.erase(sHash(str));
//             }
//         }
//     }


//     for(int i=0;i<Del.size();i++)
//    {
//     sort(Del[i].begin(),Del[i].end());
//    }

//     sort(Del.begin(),Del.end());
//     Del.erase(unique(Del.begin(),Del.end()),Del.end());
    
    // for (int i = 0; i < Del.size(); i++)
    // {
    //     cout << "消失的极大n-clique有" << endl;
    //     for (int j = 0; j < Del[i].size(); j++)
    //     {
    //         cout << Del[i][j]<<" ";
    //     }
    //     cout << endl;
    // }
    // cout<<"new clique have: "<<New.size()<<endl;
    // cout<<"del clique have: "<<Del.size()<<endl;
    change_size=change_size+New.size()+Del.size();
    ending2_clock=clock();
    elp2_clock+=(ending2_clock-begin2_clock)/CLOCKS_PER_SEC;


    for (int i = 0; i < g.adj[u].vec.size(); i++)
    {
        if (g.adj[u].vec[i].vertex == v)
        {

            g.adj[u].vec[i].value = q;
        }
    }
    for (int i = 0; i < g.adj[v].vec.size(); i++)
    {
        if (g.adj[v].vec[i].vertex == u)
        {
            g.adj[v].vec[i].value = q;
        }
    }
}
void Include_Edge_Clique::gailvjianxiao(G &g, int u, int v, float q,int &change_size)
{
    vector<vector<int> > S;
    vector<vector<int> > F;
    vector<int>Q;
    vector<vector<int> > Del;
    vector<vector<int> > New;
    vector<Vertex_Value> C;
    vector<Vertex_Value> X;
    vector<vector<int> > clique;
    vector<int> R;
    vector<int> P;  
    begin2_clock=clock();
    float s = FindValue(u, v, g);
    if (q < s)
    {
        baohanbian(g, S, s);
        baohanbian(g, F, q);
    }
    for (int i = 0; i < g.adj[u].vec.size(); i++)
    {
        if (g.adj[u].vec[i].vertex == v)
            g.adj[u].vec[i].value = q;
    }
    for (int i = 0; i < g.adj[v].vec.size(); i++)
    {
        if (g.adj[v].vec[i].vertex == u)
            g.adj[v].vec[i].value = q;
    }
    for (int i = 0; i < S.size(); i++)
    {
        bool a = true;
        for (int j = 0; j < F.size(); j++)
        {
            if (F[j] == S[i])
                a = false;
        }
        if (a)
            Del.push_back(S[i]);
    }
    ending2_clock=clock();
    elp2_clock+=(ending2_clock-begin2_clock)/CLOCKS_PER_SEC;
    // for (int i = 0; i < Del.size(); i++)
    // {
    //     cout << "del clique have:" << endl;
    //     for (int j = 0; j < Del[i].size(); j++)
    //     {
    //         cout << Del[i][j]<<" ";
    //     }
    //     cout << endl;
    // }
    begin1_clock=clock();
    for (int i = 0; i < Del.size(); i++)
    {
        C.clear();
        X.clear();
        clique.clear();
        R.clear();
        P.clear();
        Q.clear();
        // q=FindValue(u,v,g);
        for (int j = 0; j < Del[i].size(); j++)
        {
            
            Vertex_Value temp;
            temp.vertex = Del[i][j];
            temp.value = 1;
            C.push_back(temp);
        }
        maximaletaclique(R, 1, C, X, P, eta, g, clique);
        for (int j = 0; j < clique.size(); j++)
        {
            vector<vector<int> > mxed;

            for (int m = 0; m < clique[j].size(); m++)
            {

                vector<int> a;

                for (int k = 0; k < g.adj[clique[j][m]].vec.size(); k++)
                {
                    if (g.adj[clique[j][m]].vec[k].vertex >= 0)
                        a.push_back(g.adj[clique[j][m]].vec[k].vertex);
                }
                sort(a.begin(),a.end());
                mxed.push_back(a);
            }
             vector<int> mx;
            set_intersection(mxed[0].begin(),mxed[0].end(),mxed[1].begin(),mxed[1].end(),back_inserter(mx));
            for (int i = 2; i < mxed.size(); i++)
            {
                vector<int>mx1;
                set_intersection(mx.begin(),mx.end(),mxed[i].begin(),mxed[i].end(),back_inserter(mx1));
                mx.clear();
                mx=mx1;
            }
            float b = 1;
            for (int i = 0; i < clique[j].size(); i++)
            {
                for(int k=i+1;k<clique[j].size();k++)
                {
                    b=b*FindValue(clique[j][i],clique[j][k],g);
                }
            }

            bool a = true;
          
            for (int m = 0; m < mx.size(); m++)
            {
                if(clique[j].size()==1)
                {
                    a=false;
                    break;
                }
                else
                {
                    float x = 1;
                    for (int k = 0; k < clique[j].size(); k++)
                    {
                        x = x * FindValue(mx[m], clique[j][k], g);
                    }
                    if (x * b > eta)
                    {
                        a = false;
                        break;
                    }
                }
                
            }
           

            if (a)
                New.push_back(clique[j]);
        }
    }
    for(int i=0;i<New.size();i++)
   {
    sort(New[i].begin(),New[i].end());
   }

    sort(New.begin(),New.end());
    New.erase(unique(New.begin(),New.end()),New.end());
    // for (int i = 0; i < New.size(); i++)
    // {
    //     cout << "new n-clique have:" << endl;
    //     for (int j = 0; j < New[i].size(); j++)
    //     {
    //         cout << New[i][j]<<" ";
    //     }
    //     cout << endl;
    // }
    
    // cout<<"new clique have: "<<New.size()<<endl;
    // cout<<"del clique have: "<<Del.size()<<endl;
    change_size=change_size+New.size()+Del.size();
    ending1_clock=clock();
    elp1_clock+=(ending1_clock-begin1_clock)/CLOCKS_PER_SEC;

}
void Include_Edge_Clique::print_information()
{
    cout<<"iterations "<<iterations<<endl;
    cout<<"新生成花费"<<elp1_clock<<endl;
    cout<<"消失花费"<<elp2_clock<<endl;
}

void get_parameters(int argc, char *argv[], float &eta)
{
    eta = atof(argv[3]);
}
long Include_Edge_Clique::getMemoryUse(){
    int who = RUSAGE_SELF;
    struct rusage usage;
    getrusage(who, &usage);
    return usage.ru_maxrss;
}

int main(int argc, char *argv[])
{
    char *argv2[] = {"hash", "./enron.txt", "./enron_1.txt", "0.2"};
    G g;
    float eta = 0, q = 0;
    int u = 0, v = 0;
    get_parameters(argc, argv2, eta);
    Include_Edge_Clique iec;
    iec.eta = eta;
    // clock_t beg_clock, end_clock, elapsed_time;
    double beg_clock, end_clock, elapsed_time;
    beg_clock = clock();
    iec.read_graph(argv2[1], g);
    end_clock = clock();
    elapsed_time = (end_clock - beg_clock) / CLOCKS_PER_SEC;
    cout << "Read graph: using " << elapsed_time << " seconds." << endl;

    set<size_t> clique_hash;
    iec.hash_algorithm(clique_hash);

    ifstream infile(argv2[2]);
    int counts = 0;
    int change_size=0;
    infile >> counts;
    cout << "there are " << counts << " operators need deal" << endl;
    beg_clock = clock();

    for (int i = 0; i < counts; i++)
    {
        int sign = -1;
        infile >> sign >> u >> v >> q;
        if (sign == 1) 
        {
            
            iec.u = u;
            iec.v = v;
            iec.qubian(g, u, v,change_size);
        }
        if (sign == 2) 
        {                       
            iec.u = u;
            iec.v = v;
            iec.q = q;
            iec.jiabian(g, u, v, q,change_size,clique_hash);           
        }
        if (sign == 3) 
        {
            iec.u = u;
            iec.v = v;
            iec.q = q;
            iec.gailvzengjia(g, u, v, q,change_size,clique_hash);
        }
        if (sign == 4) 
        {
            iec.u = u;
            iec.v = v;
            iec.q = q;
            iec.gailvjianxiao(g, u, v, q,change_size);
        }
    }
    end_clock = clock();
    elapsed_time = (end_clock - beg_clock) / CLOCKS_PER_SEC;
    cout << "maintence : using " << elapsed_time << " seconds." << endl;
    cout<<"chang size have: "<<change_size<<endl;
    iec.print_information();
    cout << "Memory usage : " << iec.getMemoryUse() / 1024 << "MB" << endl;
    return 0;
}
