#include <iostream>
#include <vector>
#include <set>
#include <stack>

#define white 0
#define gray 1
#define black 2

struct Vertex {
	std::set<int> neighbors; // Vertex neighbors
	int color; // DFS color
	int d; // DFS discover time
	int f; // DFS finish time
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
		vertices[v1].neighbors.insert(v2);
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
	
	void DFS(int start) {
		for(Vertex& v: vertices) {
			v.color = white;
			v.d = -1;
			v.f = -1;
		}
		int time = 0;
		DFS_visit(start, time); // fully visit the start vertex first
		/*for(size_t i=0; i<vertices.size(); i++) { // continue visiting all unvisited vertices
			if(vertices[i].color == white) { // not yet visited
				DFS_visit(i, time);
			}
		}*/
	}
	
	void DFS_visit(int id, int& time) {
		Vertex& v(vertices[id]);
		v.d = time;
		time++;
		v.color = gray; // discovering this vertex
		for(int n: v.neighbors) {
			if(vertices[n].color == white) {
				DFS_visit(n, time);
			}
		}
		v.color = black;
		v.f = time;
		time++;
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
	myGraph.DFS(v);
	std::vector<Vertex>& res(myGraph.vertices);
	for(size_t i=0; i<res.size(); i++) {
		Vertex& v(res[i]);
		std::cout << v.d;
		if(i+1 != res.size()) {
			std::cout << " ";
		}
	}
	std::cout<< "\n";
	for(size_t i=0; i<res.size(); i++) {
		Vertex& v(res[i]);
		std::cout << v.f;
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
