#include <iostream>
#include <vector>
#include <queue>

#define INF -1

using namespace::std;

int dfs(vector<vector<int> > &N, vector<vector<int> > &G,
        int s, int t)
{
    if (N[s].empty()) {
        if (s == t)
            return 1;
        return 0;
    }
    int v = N[s].back();
    int b = dfs(N, G, v, t);
    // remove the wrong path.
    if (!b) {
        if (N[v].empty())
            N[s].pop_back();
        return 0;
    }
    // the right path.
    N[s].pop_back();
    for (unsigned long i = 0; i < G[s].size(); ++i) {
        if (G[s][i] == v)
            G[s][i] = -1;
    }
    G[v].push_back(s);
    return b;
}

int dinic_blocking_flow(vector<vector<int> > &N, 
        vector<vector<int> > &G, int s, int t)
{
    int f = 0;
    while (!N[s].empty()) {
        f += dfs(N, G, s, t);
    }
    return f;
}

int construct_layered_network(vector<vector<int> > &G,
        vector<vector<int> > &N, int s, int t)
{
    vector<int> d(G.size(), INF);
    queue<int> q;
    d[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        if (u == t)
            return 1;
        q.pop();
        auto cnt = G[u].size();
        for (unsigned long i = 0; i < cnt; ++i) {
            if (G[u][i] != INF) {
                auto v = G[u][i];
                if (d[v] == INF) {
                    q.push(v);
                    d[v] = d[u] + 1;
                    N[u].push_back(v);
                } else if (d[v] == d[u] + 1) {
                    N[u].push_back(v);
                }
            }
        }
    }
    return 0;
}

int dinic_max_flow(vector<vector<int> > &G, int s, int t)
{
    int f = 0;
    vector<vector<int> > N(G.size());
    while (1) {
        for (unsigned long i = 0; i < N.size(); ++i)
            N[i].clear();
        if (!(construct_layered_network(G, N, s, t)))
            break;
        f += dinic_blocking_flow(N, G, s, t);
    }
    return f;
}

void get_data(int n, int m, vector<vector<int> > &G)
{
    int t = n * n + 1;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            int id = (i - 1) * n + j;
            // all odd or even 
            if ((i % 2 && j % 2) || (!(i % 2) && !(j % 2))) {
                if (i - 1 > 0 && j - 2 > 0)
                    G[id].push_back((i - 2) * n + j - 2);
                if (i - 2 > 0 && j - 1 > 0)
                    G[id].push_back((i - 3) * n + j - 1);
                if (i - 2 > 0 && j + 1 <= n)
                    G[id].push_back((i - 3) * n + j + 1);
                if (i - 1 > 0 && j + 2 <= n)
                    G[id].push_back((i - 2) * n + j + 2);
                if (i + 1 <= n && j + 2 <= n)
                    G[id].push_back(i * n + j + 2);
                if (i + 2 <= n && j + 1 <= n)
                    G[id].push_back((i + 1) * n + j + 1);
                if (i + 2 <= n && j - 1 > 0)
                    G[id].push_back((i + 1) * n + j - 1);
                if (i + 1 <= n && j - 2 > 0)
                    G[id].push_back(i * n + j - 2);
                G[0].push_back(id);
            } else {
                G[id].push_back(t); 
            }
        }
    }
    for (int k = 0; k < m; ++k) {
        int i, j;
        cin >> i >> j;
        int id = (i - 1) * n + j;
        if ((i % 2 && j % 2) || (!(i % 2) && !(j % 2))) {
            G[0][(id - 1) / 2] = INF;
            G[id].clear();
        } else {
            G[id].clear();
        }
    }
}

int main(int argc, char *argv[])
{
    int n, m;
    cin >> n >> m;
    int s = 0, t = n * n + 1;
    vector<vector <int> > G(n * n + 2);
    get_data(n, m, G);
    int flow = dinic_max_flow(G, s, t);
    cout << n * n - m - flow << endl;
    return 0;
}
