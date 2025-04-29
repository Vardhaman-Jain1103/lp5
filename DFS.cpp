#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) : V(V) {
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected graph
    }

    void DFS(int src, vector<bool>& visited) {
        visited[src] = true;
        cout << src << " ";

        for (int neighbor : adj[src]) {
            if (!visited[neighbor]) {
                DFS(neighbor, visited);
            }
        }
    }

    void parallelDFS(int src, vector<bool>& visited) {
        stack<int> s;
        s.push(src);

        #pragma omp parallel
        {
            while (!s.empty()) {
                int node;
                #pragma omp critical
                {
                    if (!s.empty()) {
                        node = s.top();
                        s.pop();
                    } else {
                        node = -1;
                    }
                }

                if (node != -1 && !visited[node]) {
                    #pragma omp critical
                    visited[node] = true;

                    cout << node << " ";

                    #pragma omp parallel for
                    for (int i = 0; i < adj[node].size(); i++) {
                        int neighbor = adj[node][i];
                        if (!visited[neighbor]) {
                            #pragma omp critical
                            s.push(neighbor);
                        }
                    }
                }
            }
        }
    }
};

// DFS Execution
void runDFS(Graph& g, int start) {
    cout << "Sequential DFS: ";
    vector<bool> visited(g.V, false);
    auto start_time = high_resolution_clock::now();
    g.DFS(start, visited);
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end_time - start_time);
    cout << "\nExecution Time: " << duration.count() << " microseconds\n";
}

// Parallel DFS Execution
void runParallelDFS(Graph& g, int start) {
    cout << "Parallel DFS: ";
    vector<bool> visited(g.V, false);
    auto start_time = high_resolution_clock::now();
    g.parallelDFS(start, visited);
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end_time - start_time);
    cout << "\nExecution Time: " << duration.count() << " microseconds\n";
}
