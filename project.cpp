#include<iostream>
#include<vector>
#include<queue>
#include<string.h>
using namespace std;
const int N=6005,M=1e6+5,Inf=1e8;
int n,m,s,t,ans,pig[N],num[N],d[N],cur[N],maxflow;
int head[N],Next[M],ver[M],edge[M],tot=1;
vector<int> p[N];//用不定长数组存客户有的保险柜钥匙
inline void add(int x,int y,int z)//链式前向星(邻接表)存图 
{
    ver[++tot]=y,edge[tot]=z;
    Next[tot]=head[x],head[x]=tot;
}
inline bool bfs()
{
    memset(d,0,sizeof(d));
    queue<int> q;//q数组每次重新定义，就不用一一pop了 
    d[s]=1,q.push(s);
    while(!q.empty())
    {
    int x=q.front();q.pop();
    for(int i=head[x];i;i=Next[i])
    {
        int y=ver[i];
        if(edge[i]==0 || d[y]!=0)
            continue;//容量不为 0且没被访问过 
        d[y]=d[x]+1;
        q.push(y);
    }
    }
    if(d[t]==0) return false;
    return true;
}
int dfs(int x,int incf)
{
    if(x==t)
        return incf;
    for(int &i=cur[x];i;i=Next[i])
    {
        int y=ver[i],z=edge[i];
        if(z==0 || d[y]!=d[x]+1)
            continue;
        int dist=dfs(y,min(incf,z));
        if(dist>0)
        {
            edge[i]-=dist;
            edge[i^1]+=dist;
            return dist;
        }
    }
    return 0;
}
int main()
{
    scanf("%d %d",&m,&n);
    s=n+1,t=s+1;
    for(int i=1;i<=m;i++)
        scanf("%d",&pig[i]);
    for(int i=1,x,y;i<=n;i++)
    {
        scanf("%d",&x);
        for(int j=1;j<=x;j++)
        {
            scanf("%d",&y);
            p[y].push_back(i);//用不定长数组存顾客有的钥匙
        }
        scanf("%d",&x);//这个是顾客的需求量 
        add(i,t,x),add(t,i,0);//顾客点到汇点建边 
    }
    for(int i=1;i<=m;i++)
    {
        int l=p[i].size();
        for(int j=0;j<l;j++)
        {
            if(j==0)//第i个保险柜的第一个开启者
            {
                num[p[i][j]]+=pig[i];//容量累加 
                continue;
            }
            add(p[i][j-1],p[i][j],Inf);//开同一个保险柜的顾客按顺序连接无穷边
            add(p[i][j],p[i][j-1],0);
        }
    }
    for(int i=1;i<=n;i++)
        if(num[i]!=0)//以累加好的结果为容量给开某个保险柜的第一个顾客建边
            add(s,i,num[i]),add(i,s,0);
    while(bfs())//Dinic求解 
    {
        for(int i=1;i<=t;i++)
            cur[i]=head[i];
        while(int di=dfs(s,Inf))
            maxflow+=di;
    }
    printf("%d",maxflow);//答案即最大流 
    return 0;
}