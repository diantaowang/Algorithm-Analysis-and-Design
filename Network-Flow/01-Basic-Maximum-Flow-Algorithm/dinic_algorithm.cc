#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const long long INF = -1;
const long long MAX = INT64_MAX;

struct node {
    int id; // the node's id 
    long long c;  // the capacity
    node(int id, long long c): id(id), c(c) {}
};

long long dfs(vector<vector<struct node> > &N,
        vector<vector<long long> > &G, int s, int t,
        long long b)
{
    if (N[s].empty()) {
        if (s == t)
            return b;     
        return 0;
    }
    b = min(b, N[s].back().c); 
    int v = N[s].back().id;
    b = dfs(N, G, v, t, b);
    // remove the wrong path.
    if (!b) {
        if (N[v].empty())
            N[s].pop_back();
        return 0;
    }
    // the right path.
    G[s][v] -= b;
    G[v][s] += b;
    if (!G[s][v]) {
        N[s].pop_back();
    } else {
        N[s].back().c = G[s][v];
        if (N[v].empty() && v != t)
           N[s].pop_back(); 
    }
    return b;
}

long long dinic_blocking_flow(vector<vector<struct node> > &N, 
        vector<vector<long long> > &G, int s, int t)
{
    long long f = 0;
    while (!N[s].empty()) {
        f += dfs(N, G, s, t, MAX);
    }
    return f;
}

int construct_layered_network(vector<vector<long long> > &G, 
        vector<vector<struct node> > &N, int s, int t)
{
    vector<long long> d(G.size(), INF);
    queue<int> q;
    d[s] = 0;
    q.push(s);
    while(!q.empty()) {
        int u = q.front(); 
        if (u == t)
            return 1;
        q.pop();
        auto cnt = G[u].size();
        for (unsigned long v = 1; v < cnt; ++v) {
            if (G[u][v]) {
                if (d[v] == INF) {
                    q.push(v);
                    d[v] = d[u] + 1;
                    struct node vec(v, G[u][v]);
                    N[u].push_back(vec);
                } else if (d[v] == d[u] + 1) {
                    struct node vec(v, G[u][v]);
                    N[u].push_back(vec);
                }
            }
        }
    }
    return 0;
}

long long dinic_max_flow(vector<vector<long long> > &G, 
        int s, int t)
{
    long long f = 0;
    vector<vector<struct node> > N(G.size());
    while (1) {
        for (unsigned long i = 0; i < G.size(); ++i)
            N[i].clear(); 
        if (!construct_layered_network(G, N, s, t))
            break; 
        f += dinic_blocking_flow(N, G, s, t);
    } 
    return f;
}

int main(int argc, char *argv[])
{
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<vector<long long> > G(n + 1, vector<long long> (n + 1, 0));
    for (int i = 0; i < m; ++i) {
        int u, v, c;
        cin >> u >> v >> c;
        G[u][v] += c; 
    }
    long long flow = dinic_max_flow(G, s, t); 
    cout << flow << endl;
    return 0;
}
