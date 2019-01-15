#include <cstdint>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int64_t INF = -1;

struct node {
    int id;
    int64_t cap;
    // node(int id, int64_t cap) : id(id), cap(cap) {} 
};

struct glist {
    vector<int> pos;
    vector<struct node> v;
    glist(vector<int> pos) : pos(pos) {}
};

int64_t augment_and_update_graph(vector<struct glist> &G, 
        vector<int> &pre_node, int s, int t)
{
    int64_t bottleneck = INT64_MAX;
    int u = s;
    while (u != t) {
        int v = pre_node[u];
        int pos = G[v].pos[u];
        bottleneck = min(bottleneck, G[v].v[pos].cap);
        u = v;
    }
    u = s;
    while (u != t) {
        int v = pre_node[u];
        // add
        if (G[u].pos[v] != -1) {
            int pos = G[u].pos[v];
            G[u].v[pos].cap += bottleneck;    
        } else {
            struct node vec;
            vec.id = v;
            vec.cap = bottleneck;
            G[u].pos[v] = G[u].v.size();
            G[u].v.push_back(vec);
        }
        // sub
        int pos = G[v].pos[u];
        G[v].v[pos].cap -= bottleneck;
        u = v; 
    }
    return bottleneck;
}

int64_t find_one_path(vector<struct glist> &G, int s, int t, int64_t c)
{
    bool find = false;
    queue<int> q;
    vector<int> d(G.size(), -1);
    vector<int> pre_node(G.size(), -1);
    d[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        auto cnt = G[u].v.size();
        for (unsigned long i = 0; i < cnt; ++i) {
            if (G[u].v[i].cap >= c) {
                int v = G[u].v[i].id;
                if (d[v] == -1) {
                    pre_node[v] = u;
                    d[v] = 0;
                    q.push(v);
                }
                // find path s -> t
                if (v == t) {
                    find = true;
                    break;
                }
            }
        }
        if (find)
            break;
    }
    if (find) {
        return augment_and_update_graph(G, pre_node, t, s);
    }
    return 0;
} 

int64_t scaling_ford_fulkerson(vector<struct glist> &G, 
        int s, int t)
{
    int64_t flow = 0, max_cap = 0;
    auto cnt = G[s].v.size();
    for (unsigned long i = 0; i < cnt; ++i)
        max_cap += G[s].v[i].cap;
    while (max_cap >= 1) {
        while(1) {
            int64_t inc = find_one_path(G, s, t, max_cap);
            if (!inc)
                break;
            flow += inc;
        } 
        max_cap = max_cap / 2;
    }
    return flow;
}

int main(int argc, char *argv[])
{
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    struct glist init_glist(vector<int> (n + 1, -1));
    vector<struct glist> G(n + 1, init_glist);
    for (int i = 0; i < m; ++i) {
        int u, v, c;
        cin >> u >> v >> c;
        if (G[u].pos[v] == -1) {
            struct node vec;
            vec.id = v;
            vec.cap = c; 
            G[u].pos[v] = G[u].v.size();
            G[u].v.push_back(vec);
        } else {
            int pos = G[u].pos[v];
            G[u].v[pos].cap += c;
        }
    }
    int64_t flow = scaling_ford_fulkerson(G, s, t); 
    cout << flow << endl;
    return 0;
}
