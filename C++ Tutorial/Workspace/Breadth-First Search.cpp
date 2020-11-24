#include <iostream>
#include <vector>
#include <queue>

struct Vertex {
	std::vector<int> neighbors; // Vertex neighbors
	int d; // BFS distance
};

class Graph {
  public:
    int v; // number of vertices
    std::vector<Vertex> vertices; // graph, each vertex stores a list of neighbors labels
  public:
	Graph (int v) {
		this->v = v;
		this->vertices = std::vector<Vertex>(v);
	}
	void add_edge(int v1, int v2) {
		vertices[v1].neighbors.push_back(v2);
	}
	void print_graph() {
		std::vector<std::vector<int>> adj_matrix = std::vector<std::vector<int>>(v, std::vector<int>(v));
		for(size_t i=0; i<vertices.size(); i++) {
			Vertex& v(vertices[i]);
			for(int n: v.neighbors) {
				adj_matrix[i][n] = 1;
			} 
		}
		size_t n = adj_matrix.size();
		for(size_t i=0; i<n; i++) {
			size_t m = adj_matrix[i].size();
			for(size_t j=0; j<m; j++) {
				std::cout << adj_matrix[i][j];
				if(j+1 != m) {
					std::cout << " ";
				}
			}
			std::cout << "\n";
		}
	}
	
	void BFS(int start) {
		for(Vertex& v: vertices) {
			v.d = -1;
		}
		vertices[start].d = 0;
		std::queue<int> q;
		q.push(start);
		while(!q.empty()) {
			Vertex& curr = vertices[q.front()];
			q.pop();
			for(int n: curr.neighbors) {
				Vertex& v(vertices[n]);
				if(v.d == -1) {
					v.d = curr.d + 1;
					q.push(n);
				}
			}
		}
	}
};

void testcase() {
	int n, m, v; std::cin >> n >> m >> v;
	Graph myGraph(n);
	for(int i=0; i<m; i++) {
		int a,b; std::cin >> a >> b;
		myGraph.add_edge(a, b);
		myGraph.add_edge(b, a);
	}
	//myGraph.print_graph();
	myGraph.BFS(v);
	std::vector<Vertex>& res(myGraph.vertices);
	for(size_t i=0; i<res.size(); i++) { // print found distances
		Vertex& v(res[i]);
		std::cout << v.d;
		if(i+1 != res.size()) {
			std::cout << " ";
		}
	}
	std::cout << std::endl;
}


int main() {
	std::ios_base::sync_with_stdio(false);
	int t; std::cin >> t;
	for(int i=0; i<t; i++) {
		testcase();
	}
	
	return 0;
}

