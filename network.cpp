/**
   @file network.cpp
   @author Luke Zeller
   
   Implementation of methods for class Network, as defined
   in network.h
*/

#include <algorithm>
#include <queue>
#include "network.h"

Network::Network(int nodes, int num_terminals, std::vector<int> terminals,
             std::vector< std::pair<int, int> > edge_list)
    : G_(nodes, edge_list)
    , is_up_(edge_list.size(), true)
    , K_(num_terminals)
    , terminals_(terminals) {}

int Network::get_diameter()
{
    return get_diameter(G_.edges.size());
}

int Network::get_diameter(int level)
{
    std::vector< std::vector<int> > dist(K_, std::vector<int>(G_.N));

    for (int t_i = 0; t_i < K_; t_i++) {
        int root = terminals_[t_i];

        bool visited[G_.N] = {false};
        visited[root] = true;
        
        std::queue<int> q;
        q.push(root);
        
        while (q.size()) {
            int u = q.front();
            q.pop();
            for (int id : G_.incident_id_list[u]) {
                if (id >= level || !is_up_[id])
                    continue;
                int v = G_.edges[id].other(u);
                if (!visited[v])  {
                    dist[t_i][v] = dist[t_i][u] + 1;
                    q.push(v);
                    visited[v] = true;
                }
            }
        }
        for (int v : terminals_)
            if (!visited[v])
                return -1;
    }
    
    int d = 0;
    for (int i = 0; i < K_; i++)
        for (int j = 0; j < K_; j++)
            d = std::max(dist[i][terminals_[j]], d);
    return d;
}

int Network::get_nodes()
{
    return G_.N;
}

int Network::get_edges()
{
    return G_.edges.size();
}

int Network::get_num_up()
{
    int c = 0;
    for (int i = 0; i < G_.edges.size(); i++)
        if (is_up_[i])
            c++;
    return c;
}

bool Network::is_up(int edge_id)
{
    return is_up_[edge_id];
}

void Network::set_state(int edge_id, bool new_state)
{
    is_up_[edge_id] = new_state;
}