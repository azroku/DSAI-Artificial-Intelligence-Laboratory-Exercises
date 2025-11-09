#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <algorithm>
#include <stack>

class Maze {
  std::vector<std::vector<std::string>> maze;

public:
  Maze(const std::string &filename) { loadFromFile(filename); }

  void loadFromFile(const std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Error opening file" << std::endl;
      return;
    }
    std::string line;
    while (std::getline(file, line)) {
      std::vector<std::string> row;
      for (char c : line) {
        row.push_back(std::string(1, c));
      }
      maze.push_back(row);
    }
  }
  int getWidth() {
    // here we asume that maze is of uniform width
    return maze.at(0).size();
  }
  int getHeight() { return maze.size(); }
  bool isWall(int r, int c) { return maze.at(r).at(c) == "#" ? true : false; }
  std::pair<int, int> getStart() {
    for (int i = 0; i < maze.size(); i++) {
      for (int j = 0; j < maze.at(i).size(); j++) {
        if (maze.at(i).at(j) == "S") {
          return std::make_pair(i, j);
        }
      }
    }
    return std::make_pair(-1, -1);
  }
  std::pair<int, int> getEnd() {
    for (int i = 0; i < maze.size(); i++) {
      for (int j = 0; j < maze.at(i).size(); j++) {
        if (maze.at(i).at(j) == "E") {
          return std::make_pair(i, j);
        }
      }
    }
    return std::make_pair(-1, -1);
  }
  std::vector<std::pair<int, int>> getNeighbours(std::pair<int, int> node) {
    std::vector<std::pair<int, int>> neighbours;
    int r = node.first;
    int c = node.second;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for(int i = 0; i < 4; i++) {
      int new_r = r + dr[i];
      int new_c = c + dc[i];

      if(new_r >= 0 && new_r < getHeight() && new_c >= 0 && new_c < getWidth()) {
        if(!isWall(new_r, new_c)) {
          neighbours.push_back({new_r, new_c});
        }
      }
    }
    return neighbours;
  }
};

std::pair<std::vector<std::pair<int, int>>, std::set<std::pair<int, int>>> 
bfs(Maze &maze) {
    std::pair<int, int> start = maze.getStart();
    std::pair<int, int> end = maze.getEnd();
    
    std::queue<std::pair<int, int>> queue;
    std::set<std::pair<int, int>> visited;
    std::map<std::pair<int, int>, std::pair<int, int>> parent;

    queue.push(start);
    visited.insert(start);
    parent[start] = {-1, -1};

    while(!queue.empty()) {
        std::pair<int, int> current = queue.front();
        queue.pop();

        if(current == end) {
            std::vector<std::pair<int, int>> path;
            while (current.first != - 1) {
                path.push_back(current);
                current = parent[current];
            }
            std::reverse(path.begin(), path.end());
            return {path, visited};
        }

        for(auto &neighbour : maze.getNeighbours(current)) {
            if(visited.find(neighbour) == visited.end()) {
                visited.insert(neighbour);
                parent[neighbour] = current;
                queue.push(neighbour);
            }
        }
    }

    return {{}, visited};
}


std::pair<std::vector<std::pair<int, int>>, std::set<std::pair<int, int>>> dfs(Maze &maze) {
    std::pair<int, int> start = maze.getStart();
    std::pair<int, int> end = maze.getEnd();

    std::stack<std::pair<int, int>> stack;
    std::set<std::pair<int, int>> visited;
    std::map<std::pair<int, int>, std::pair<int, int>> parent;

    stack.push(start);
    parent[start] = {-1, -1}; 

    while (!stack.empty()) {
        std::pair<int, int> current = stack.top();
        stack.pop();
        
        visited.insert(current);

        if (current == end) {
            std::vector<std::pair<int, int>> path;
            while (current.first != -1) {
                path.push_back(current);
                current = parent[current];
            }
            std::reverse(path.begin(), path.end()); 
            return {path, visited};
        }

        for (auto &neighbour : maze.getNeighbours(current)) {
            if (visited.find(neighbour) == visited.end()) {
                parent[neighbour] = current;
                stack.push(neighbour);
            }
        }
    }
    return {{}, visited}; 
}
