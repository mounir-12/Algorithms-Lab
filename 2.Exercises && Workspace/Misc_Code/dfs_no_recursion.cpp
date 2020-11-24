#include <iostream>
#include <vector>

// check: https://www.geeksforgeeks.org/iterative-depth-first-traversal/
void dfs(std::vector<std::vector<int>> matrix) {
	std::vector<int> path;
	
	int V = matrix.size();
	std::vector<bool> visited(V, false);
	std::vector<int> lifo;
	lifo.push_back(0);
	
	while(!lifo.empty()) {
		int curr_node = lifo.back();
		lifo.pop_back();
		if(curr_node == -1) {
			path.pop_back();
			continue;
		}
		path.push_back(curr_node);
		visited[curr_node] = true;
		
		lifo.push_back(-1); // push_back dummy node (delimiter) to split the children of curr_node from other nodes
							// allows us to know when we finished dfs traversal of curr_node to pop it from "path"
		for(int i=0; i < matrix[curr_node].size(); i++) {
			if(matrix[curr_node][i] and !visited[i]) lifo.push_back(i); // push all unvisited children
		}
		
		std::cout << "curr path: ";
		for(int i=0; i<path.size(); i++) {
			std::cout << path[i] << (i == path.size()-1? "" : "->");
		}
		std::cout << std::endl;
	}
}

int main() {
	int V = 6; // nb of nodes
	std::vector<std::vector<int>> matrix(V, std::vector<int>(V, 0));
	matrix[0][2] = 1;
	matrix[0][1] = 1; matrix[1][3] = 1; matrix[2][4] = 1;
	matrix[2][5] = 1;
	// add some cycles
	matrix[1][0] = 1; matrix[1][1] = 1; matrix[4][2] = 1; matrix[4][0] = 1;
	
	dfs(matrix);
	return 0;
}
