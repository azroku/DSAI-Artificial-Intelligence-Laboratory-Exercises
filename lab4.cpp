#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <functional>
#include <cmath>
#include <algorithm>

using namespace std;

class PuzzleState {
public:
    vector<vector<int>> tiles;

    //default
    PuzzleState() : tiles(3, vector<int>(3, 0)) {}

    //2D vector
    PuzzleState(vector<vector<int>> t) : tiles(t) {}

    pair<int, int> findBlank() const {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (tiles[i][j] == 0)
                    return make_pair(i, j);
        return make_pair(-1, -1);
    }

    vector<PuzzleState> getNeighbors() const {
        vector<PuzzleState> neighbors;
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        pair<int, int> blank = findBlank();
        int x = blank.first;
        int y = blank.second;

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i], ny = y + dy[i];
            if (nx >= 0 && nx < 3 && ny >= 0 && ny < 3) {
                vector<vector<int>> newTiles = tiles;
                swap(newTiles[x][y], newTiles[nx][ny]);
                neighbors.push_back(PuzzleState(newTiles));
            }
        }
        return neighbors;
    }

    bool operator<(const PuzzleState &other) const {
        return tiles < other.tiles;
    }

    bool operator==(const PuzzleState &other) const {
        return tiles == other.tiles;
    }

    void print() const {
        for (auto &row : tiles) {
            for (int val : row)
                cout << val << " ";
            cout << endl;
        }
        cout << endl;
    }
};


int misplacedTiles(const PuzzleState &current, const PuzzleState &goal) {
    int count = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (current.tiles[i][j] != 0 && current.tiles[i][j] != goal.tiles[i][j])
                count++;
    return count;
}

int manhattanDistance(const PuzzleState &current, const PuzzleState &goal) {
    int distance = 0;
    int goalPos[9][2];

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            int val = goal.tiles[i][j];
            goalPos[val][0] = i;
            goalPos[val][1] = j;
        }

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            int val = current.tiles[i][j];
            if (val != 0)
                distance += abs(i - goalPos[val][0]) + abs(j - goalPos[val][1]);
        }

    return distance;
}


struct Node {
    PuzzleState state;
    int g;
    int f;
    bool operator>(const Node &other) const { return f > other.f; }
};

vector<PuzzleState> reconstructPath(map<PuzzleState, PuzzleState> &cameFrom,
                                    PuzzleState current) {
    vector<PuzzleState> path;
    while (cameFrom.find(current) != cameFrom.end()) {
        path.push_back(current);
        current = cameFrom[current];
    }
    path.push_back(current);
    reverse(path.begin(), path.end());
    return path;
}

vector<PuzzleState> aStarSearch(const PuzzleState &start,
                                const PuzzleState &goal,
                                function<int(const PuzzleState&, const PuzzleState&)> heuristic) {
    priority_queue<Node, vector<Node>, greater<Node>> openSet;
    map<PuzzleState, int> gScore;
    map<PuzzleState, PuzzleState> cameFrom;
    set<PuzzleState> closedSet;

    gScore[start] = 0;
    openSet.push((Node){start, 0, heuristic(start, goal)});

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.state == goal)
            return reconstructPath(cameFrom, current.state);

        if (closedSet.count(current.state))
            continue;

        closedSet.insert(current.state);

        vector<PuzzleState> neighbors = current.state.getNeighbors();
        for (size_t i = 0; i < neighbors.size(); i++) {
            PuzzleState neighbor = neighbors[i];
            if (closedSet.count(neighbor))
                continue;

            int tentative_g = gScore[current.state] + 1;

            if (!gScore.count(neighbor) || tentative_g < gScore[neighbor]) {
                cameFrom[neighbor] = current.state;
                gScore[neighbor] = tentative_g;
                int f = tentative_g + heuristic(neighbor, goal);
                openSet.push((Node){neighbor, tentative_g, f});
            }
        }
    }

    return vector<PuzzleState>(); 
}


int main() {
    vector<vector<int>> start = {
        {2, 8, 3},
        {1, 6, 4},
        {7, 0, 5}
    };

    vector<vector<int>> goal = {
        {1, 2, 3},
        {8, 0, 4},
        {7, 6, 5}
    };

    PuzzleState s(start), g(goal);

    cout << "Running A* with Manhattan Distance Heuristic...\n";
    vector<PuzzleState> path = aStarSearch(s, g, manhattanDistance);

    if (path.empty()) {
        cout << "No solution found!\n";
    } else {
        cout << "Solution found in " << path.size() - 1 << " moves:\n\n";
        for (size_t i = 0; i < path.size(); i++)
            path[i].print();
    }

    return 0;
}
