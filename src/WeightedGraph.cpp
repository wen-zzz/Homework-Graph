#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

// 定義邊的結構，包含目標頂點與權重
struct Edge {
    int to;
    int weight;
};

class WeightedGraph 
{
public:
    WeightedGraph(int vertices) : n(vertices), e(0) {adj.resize(n);}
    virtual ~WeightedGraph() {}
    bool IsEmpty() const { return n == 0; }
    int NumberOfVertices() const { return n; }
    int NumberOfEdges() const { return e; }

    virtual void InsertEdge(int u, int v, int w) 
    {
        adj[u].push_back({ v, w });
        adj[v].push_back({ u, w }); 
        e++;
    }
    virtual void DFS();
    virtual void DFS(const int v);
    virtual void BFS(int v);
    virtual void DfnLow(const int x);
    void DfnLow(const int u, const int v);
    virtual void Biconnected();
    virtual void Biconnected(const int u, const int v);

protected:
    int n; 
    int e; 
    vector<vector<Edge>> adj; 
    bool* visited;
    int num;
    int* dfn;
    int* low;
    stack<pair<int, int>> s;
};
void WeightedGraph::DFS() 
{
    visited = new bool[n];
    fill(visited, visited + n, false);
    DFS(0);
    cout << endl;
    delete[] visited;
}

void WeightedGraph::DFS(const int v) 
{
    visited[v] = true;
    cout << v << " ";
    for (auto& edge : adj[v]) 
    {
        if (!visited[edge.to]) 
        {
            DFS(edge.to);
        }
    }
}
void WeightedGraph::BFS(int v) 
{
    visited = new bool[n];
    fill(visited, visited + n, false);
    visited[v] = true;
    queue<int> q;
    q.push(v);
    while (!q.empty()) 
    {
        v = q.front();
        q.pop();
        cout << v << " ";
        for (auto& edge : adj[v]) 
        {
            if (!visited[edge.to]) 
            {
                q.push(edge.to);
                visited[edge.to] = true;
            }
        }
    }
    cout << endl;
    delete[] visited;
}

void WeightedGraph::DfnLow(const int x) 
{
    num = 1;
    dfn = new int[n];
    low = new int[n];
    fill(dfn, dfn + n, 0);
    fill(low, low + n, 0);
    DfnLow(x, -1);
    cout << "Vertex\tDFN\tLOW\n";
    for (int i = 0; i < n; i++) cout << i << "\t" << dfn[i] << "\t" << low[i] << "\n";
    delete[] dfn;
    delete[] low;
}

void WeightedGraph::DfnLow(const int u, const int v) 
{
    dfn[u] = low[u] = num++;
    for (auto& edge : adj[u]) 
    {
        int w = edge.to; 
        if (dfn[w] == 0) 
        {
            DfnLow(w, u);
            low[u] = min(low[u], low[w]);
        }
        else if (w != v) 
        {
            low[u] = min(low[u], dfn[w]);
        }
    }
}


void WeightedGraph::Biconnected() 
{
    num = 1;
    dfn = new int[n];
    low = new int[n];
    fill(dfn, dfn + n, 0);
    fill(low, low + n, 0);  
    while (!s.empty()) s.pop();
    Biconnected(0, -1);
    delete[] dfn;
    delete[] low;
}

void WeightedGraph::Biconnected(const int u, const int v) 
{
    dfn[u] = low[u] = num++;
    for (auto& edge : adj[u]) 
    {
        int w = edge.to; 
        if ((v != w) && (dfn[w] < dfn[u])) 
        {
            s.push(make_pair(u, w));
        }

        if (dfn[w] == 0) 
        { 
            Biconnected(w, u);
            low[u] = min(low[u], low[w]);

            
            if (low[w] >= dfn[u]) 
            {
                cout << "New Biconnected Component: " << endl;
                int x, y;
                do {
                    x = s.top().first;
                    y = s.top().second;
                    s.pop();
                    cout << "  Edge: (" << x << ", " << y << ")" << endl;
                } while (x != u || y != w); 
            }
        }
        else if (w != v) { // back edge
            low[u] = min(low[u], dfn[w]);
        }
    }
}

int main() 
{
    WeightedGraph wg(4);

    wg.InsertEdge(0, 1, 10);
    wg.InsertEdge(1, 2, 20);
    wg.InsertEdge(2, 3, 30);
    wg.InsertEdge(3, 0, 40);

    cout << "Weighted DFS" << endl;
    wg.DFS();

    cout << "\nWeighted BFS" << endl;
    wg.BFS(0);

    cout << "\nDfnLow values" << endl;
    wg.DfnLow(0);

    cout << "\nBiconnected Components" << endl;
    wg.Biconnected();

    return 0;
}