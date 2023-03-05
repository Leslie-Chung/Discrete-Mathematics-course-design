#include <iostream>
#include <cstring>
#define M 202
using namespace std;
/*��������ţ���ô���Ƚ��߹����ŵ���ͨ��֧���꣬Ȼ�������֮��ص�ԭ�����ŵĵط���������һ����ͨ��֧������Ӧ�����߲����ŵ������ߣ���������Ҳû�й�ϵ����Ϊ�϶�Ҫ�߹�����·��ֻ����������Ȼ���������ͨ��֧����ˡ� 
û�����ŵĻ��������㷨 
�����ʵ�ǵ��������ŷ����·*/
struct stack
{
    int top,node[M];
}s;
int e[M][M],n;
void dfs(int x)
{
    int i;
    s.node[++s.top]=x;
    for(i=0;i<n;i++)
    {
        if(e[i][x]>0)
        {
            e[i][x]=e[x][i]=0; 
            dfs(i);
            break;
        }
    }
}
void fleury(int x)
{
    int i,flag;
    s.top=0; s.node[s.top]=x;
    while(s.top>=0)
    {
        flag=0;
        for(i=0;i<n;i++)
        {
            if(e[s.node[s.top]][i]>0)
            {
                flag=1;
                break;
            }
        }
        if(!flag) printf("%d ",s.node[s.top--]+1);
        else dfs(s.node[s.top--]);
    }
    puts("");
}
int main( )
{
    int i,j,u,v,m,degree,num=0,start=0;
    scanf("%d%d",&n,&m);
    memset(e,0,sizeof(e)); 
    for(i=0;i<m;i++)
    {
        scanf("%d%d",&u,&v);
        e[u-1][v-1]=e[v-1][u-1]=1;
    }
    for(i=0;i<n;i++)
    {
        degree=0;
        for(j=0;j<n;j++)
            degree+=e[i][j];
        if(degree%1)
        {
            start=i;
            num++;
        }
    }
    
    if(num==0||num==2) fleury(start);
    else printf("No Euler path\n");
    return 0;
} 
