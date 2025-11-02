#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <tuple>
#include <queue>
#include <set>
#include <stack>

using Node = std::pair<int, int>;

class Maze {
public:

	void loadFromFile(const std::string filename) {

		std::ifstream MazeFile(filename);
		std::string myText;
		while (std::getline(MazeFile,myText)) {
			std::vector<char> red;
			for (int i = 0; i < myText.length(); i++) {
				red.push_back(myText[i]);
			}
			maze.push_back(red);
		}
	}
	int getWidth() {
		return maze.at(0).size();
	}
	int getHeight() {
		return maze.size();
	}
	void printMaze() {
		for (const auto& row : maze) {       // loop over each row
			for (const auto& cell : row) {   // loop over each character in the row
				std::cout << cell;
			}
			std::cout << std::endl;          // new line after each row
		}
	}
	bool isWall(int r, int c) {
		return (maze[r][c] == '#');
	}
	Node getStart() {
		for (int i = 0; i < getHeight(); i++) {
			for (int j = 0; j < getWidth(); j++) {
				if (maze[i][j] == 'S'){
					return { i,j };
				}
			}

		}
		return { -1,-1 };
	}
	Node getEnd() {
		for (int i = 0; i < getHeight(); i++) {
			for (int j = 0; j < getWidth(); j++) {
				if (maze[i][j] == 'E') {
					std::cout << maze[i][j] << std::endl;

					return {i,j};
				}
			}

		}
		return { -1, -1 };
	}
	char get(int r, int c) {
		return maze[r][c];
	}
		std::vector<Node> getNeighbours(Node currentNode) {
			int width = getWidth();
			int height = getHeight();
			std::vector<Node> nextNodes;
			if (currentNode.first == 0 && currentNode.second == 0) {
				nextNodes.push_back({ currentNode.first,currentNode.second + 1 });
				nextNodes.push_back({ currentNode.first + 1,currentNode.second });
			}
			else if (currentNode.first == 0) {
				nextNodes.push_back({ currentNode.first,currentNode.second + 1 });
				nextNodes.push_back({ currentNode.first + 1,currentNode.second });
				nextNodes.push_back({ currentNode.first,currentNode.second - 1 });
			}
			else if (currentNode.second == 0) {
				nextNodes.push_back({ currentNode.first,currentNode.second + 1 });
				nextNodes.push_back({ currentNode.first + 1,currentNode.second });
				nextNodes.push_back({ currentNode.first-1,currentNode.second });
			}
			else if (currentNode.first == height && currentNode.second == width) {
				nextNodes.push_back({ currentNode.first,currentNode.second - 1 });
				nextNodes.push_back({ currentNode.first - 1,currentNode.second });
			}
			else if (currentNode.first == height) {
				nextNodes.push_back({ currentNode.first,currentNode.second + 1 });
				nextNodes.push_back({ currentNode.first - 1,currentNode.second });
				nextNodes.push_back({ currentNode.first,currentNode.second - 1 });
			}
			else if (currentNode.second == width) {
				nextNodes.push_back({ currentNode.first,currentNode.second - 1 });
				nextNodes.push_back({ currentNode.first + 1,currentNode.second });
				nextNodes.push_back({ currentNode.first - 1,currentNode.second });
			}
			else {
				nextNodes.push_back({ currentNode.first,currentNode.second + 1 });
				nextNodes.push_back({ currentNode.first + 1,currentNode.second });
				nextNodes.push_back({ currentNode.first,currentNode.second - 1 });
				nextNodes.push_back({ currentNode.first-1,currentNode.second });
			}
			return nextNodes;
			}
		std::vector<Node> reconstructPath(std::vector<std::pair<Node, Node>> path, Node startNode, Node endNode) {
			std::vector<Node> newPath;
			std::pair<Node, Node> currentNode = path[path.size() - 1];
			std::pair<Node, Node> newNode;
			for (int i = path.size() - 2; i >= 0; i--) {
				newNode = path[i];
				if (newNode.second == currentNode.first) {
					newPath.push_back(currentNode.first);
					currentNode = newNode;
				}

			}

			return newPath;
		}
		std::vector<Node> dfs(Maze& maze) {
			std::stack<Node> stack;
			std::vector<std::pair<Node, Node>> path;
			std::set<Node> visited;

			Node startNode = maze.getStart();
			stack.push(startNode);
			visited.insert(startNode);

			while (!stack.empty()) {
				Node currentNode = stack.top();
				stack.pop();
				std::vector<Node> neighbours = maze.getNeighbours(currentNode);
				for (Node n : neighbours) {
					if (!(visited.find(n) != visited.end())) {
						if (!maze.isWall(n.first, n.second)) {
							if (maze.get(n.first, n.second) != 'E') {
								path.push_back({ currentNode ,n });

								stack.push(n);
								visited.insert(n);
							}
							else {
								path.push_back({ currentNode ,n });

								return reconstructPath(path, startNode, n);
							}
						}
					}

				}
			}
			return {};
		}
		std::vector<Node> bfs(Maze& maze) {
			std::queue<Node> q;
			Node startNode = maze.getStart();
			std::vector<std::pair<Node, Node>> path;
			q.push(startNode);
			std::set<Node> visited;
			visited.insert(startNode);
			while (!q.empty()) {
				Node currentNode = q.front();
				q.pop();
				std::vector<Node> neighbours = maze.getNeighbours(currentNode);
				for (Node n : neighbours) {

					if ((visited.find(n) == visited.end())) {
						if (!maze.isWall(n.first, n.second)) {
							if (maze.get(n.first, n.second) != 'E') {
								path.push_back({ currentNode ,n });

								q.push(n);
								visited.insert(n);
							}
							else {
								path.push_back({ currentNode ,n });
								return reconstructPath(path, startNode, n);
							}
						}
					}
				}

			}
			return {};
		}
private:
	std::vector<std::vector<char>> maze;
};
