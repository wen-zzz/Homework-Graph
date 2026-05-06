#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

class Graph 
{
public:
    Graph(int vertices) : n(vertices), e(0) {adj.resize(n);}
    virtual ~Graph() {}
    bool IsEmpty() const { return n == 0; }
    int NumberOfVertices() const { return n; }
    int NumberOfEdges() const { return e; }

    
    virtual int Degree(int u) const { return adj[u].size(); }

    virtual void InsertEdge(int u, int v) 
    {
        adj[u].push_back(v);
        adj[v].push_back(u); 
        e++;
    }

    virtual void DFS();
    virtual void DFS(const int v);
    virtual void BFS(int v);
    virtual void Components();

    virtual void DfnLow(const int x);
    void DfnLow(const int u, const int v);

    virtual void Biconnected();
    virtual void Biconnected(const int u, const int v);

protected:
    int n;      
    int e;      
    vector<vector<int>> adj; 
    bool* visited;
    int num;
    int* dfn;
    int* low;
    stack<pair<int, int>> s; 

    
    void OutputNewComponent() 
    {
        cout << "End of Component" << endl;
    }
};

void Graph::DFS() 
{
    visited = new bool[n];
    fill(visited, visited + n, false);
    DFS(0);
    cout << endl;
    delete[] visited;
}

void Graph::DFS(const int v) 
{
    visited[v] = true;
    cout << v << " ";
    for (int w : adj[v]) 
    {
        if (!visited[w]) 
        {
            DFS(w);
        }
    }
}
void Graph::BFS(int v)
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
        for (int w : adj[v]) 
        {
            if (!visited[w]) 
            {
                q.push(w);
                visited[w] = true;
            }
        }
    }
    cout << endl;
    delete[] visited;
}
void Graph::Components()
{
    visited = new bool[n];
    fill(visited, visited + n, false);
    for (int i = 0; i < n; i++) 
    {
        if (!visited[i]) {
            DFS(i); 
            OutputNewComponent();
        }
    }
    delete[] visited;
}
void Graph::DfnLow(const int x) 
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

void Graph::DfnLow(const int u, const int v)
{
    dfn[u] = low[u] = num++;
    for (int w : adj[u]) 
    {
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
void Graph::Biconnected()
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

void Graph::Biconnected(const int u, const int v)
{
    dfn[u] = low[u] = num++;
    for (int w : adj[u]) {
        if ((v != w) && (dfn[w] < dfn[u])) 
        {
            s.push(make_pair(u, w));
        }

        if (dfn[w] == 0) { 
            Biconnected(w, u);
            low[u] = min(low[u], low[w]);
            if (low[w] >= dfn[u]) {
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
        else if (w != v) 
        {
            low[u] = min(low[u], dfn[w]);
        }
    }
}
int main() {
    Graph g(5);

    g.InsertEdge(0, 1);
    g.InsertEdge(1, 2);
    g.InsertEdge(2, 0);
    g.InsertEdge(1, 3);
    g.InsertEdge(3, 4);

    cout << "BFS from vertex 0" << endl;
    g.BFS(0);

    cout << "\nDFS Components" << endl;
    g.Components();

    cout << "\nDfnLow" << endl;
    g.DfnLow(0);

    cout << "\nBiconnected Components" << endl;
    g.Biconnected();

    return 0;
}