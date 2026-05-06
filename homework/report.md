# 41343132
# 41343136

作業二

## 解題說明

實作Array Adjacency Lists表示法，並做出有權重及無權重的無向圖
還有在表示法下做出:
   - DFS
   - BSF
   - Connected
   - Biconnected Components


## 程式實作

Graph.cpp
```cpp
﻿#include <iostream>
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

```

WeightedGraph.cpp
```cpp
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

```

## 效能分析
沒做出的部分，為理論值。
1. 空間複雜度

|表示法|複雜度|
|--|--|
|Adjacency Matrix|$O(V^2)$|
|Linked Adjacency Lists|$O(V + E)$|
|Array Adjacency Lists|$O(V + E)$|



2.時間複雜度

|演算法|matrix|lists|
|---|---|---|
|DFS / BFS|$O(V^2)$|$O(V+E)$|
|Connected Components|$O(V^2)$|$O(V + E)$|
|Spanning Trees|$O(V^2)$|$O(V+E)$|
|Biconnected Comp|$O(V^2)$|$O(V + E)$|
|Kruskal|$O(E \log E)$|$O(E \log E)$|
|Prim|$O(V^2)$|$O(E \log V)$|
|Nonnegative Edge Costs|$O(V^2)$|$O(E \log V)$| 
|General Weights|$O(V^3)$|$O(V \times E)$|
|All-Pairs Shortest Paths|$O(V^3)$|$O(V^3)$|
|AOV|$O(V^2)$|$O(V+E)$|
|AOE|$O(V^2)$|$O(V+E)$|

## 4. 測試與驗證

測試案例一（無權重圖）：建立了一個 5 個頂點的圖，包含環狀結構（0-1-2-0）與路徑（1-3-4）。  

驗證結果：成功跑出 BFS 走訪順序，並透過 Components() 找出連通分量。

測試案例二（有權重圖）：建立了一個 4 個頂點的完全循環圖，每條邊賦予了 10, 20, 30, 40 的權重。  

驗證結果：透過 DfnLow 計算出各頂點的 DFN 與 LOW 值，進而找出雙連通分量。


## 5. 申論及開發報告

這次作業做了許多東西，就比如Array Adjacency Lists表示法，還有運用於其底下的許多演算法。
去稍微查找資料了解到，在不同情況下有不同的使用方式及方法。
- Matrix適合密集圖。
- List適合稀疏圖。
- BFS跟DFS是圖的基礎核心演算法。
- Prim跟Kruskal在網路設計等問題中很重要。
- 最短路徑問題在導航、網路傳輸中廣泛應用。
- AOV跟AOE則應用於排程與專案管理。

動態陣列串列的選用：
- 在Graph與WeightedGraph中，用vector<vector<T>>作為Array Adjacency Lists的實作方式。這種結構比起傳統的鏈結串列Linked List，在存取相鄰節點時具有更好的快取局部性Cache Locality，且能利用vector的動態擴展特性，在不確定頂點數量的情況下保持靈活性。

無向圖的對稱處理：
- 為了正確實作無向圖，在InsertEdge函式中確保了每一條邊$(u, v)$都會同時在adj[u]與adj[v]中建立記錄。這在後續執行DfnLow運算時至關重要，否則圖形會變成有向圖，導致連通分量判斷錯誤。
