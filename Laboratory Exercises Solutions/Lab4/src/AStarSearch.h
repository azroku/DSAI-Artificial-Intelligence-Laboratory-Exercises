#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <functional>
#include <algorithm>
#include "PuzzleState.h"
#include <memory>

// Define a type for our heuristic function for cleaner code
using HeuristicFunction = std::function<int(const PuzzleState&, const PuzzleState&)>;

// 1. A Node struct for the priority queue
struct Node {
    std::shared_ptr<PuzzleState> state;
    int g_cost; // Cost from start to current state
    int f_cost; // g_cost + h_cost (estimated total cost)
    std::shared_ptr<Node> parent; // Pointer to parent node to reconstruct the path

    Node(std::shared_ptr<PuzzleState> s, int g, int f, std::shared_ptr<Node> p)
        : state(s), g_cost(g), f_cost(f), parent(p) {
    }
};

// 2. A custom comparator for the min-priority queue
struct CompareNodes {
    bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
        // We want the node with the SMALLER f_cost to have HIGHER priority
        return a->f_cost > b->f_cost;
    }
};

// 3. The A* Search Function
inline std::vector<PuzzleState> aStarSearch(const PuzzleState& start, const PuzzleState& goal, HeuristicFunction h) {
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNodes> open_set;

    // Use a map to track visited states and avoid redundant nodes in the open set
    // The key is the PuzzleState, the value is the node's f_cost
    std::map<PuzzleState, int> visited_costs;

    auto start_ptr = std::make_shared<PuzzleState>(start);
    int start_h_cost = h(*start_ptr, goal);
    auto start_node = std::make_shared<Node>(start_ptr, 0, start_h_cost, nullptr);

    open_set.push(start_node);
    visited_costs[*start_ptr] = start_h_cost;

    while (!open_set.empty()) {
        std::shared_ptr<Node> current_node = open_set.top();
        open_set.pop();

        if (*(current_node->state) == goal) {
            std::cout << "Goal reached! Reconstructing path..." << std::endl;
            std::vector<PuzzleState> path;
            std::shared_ptr<Node> temp = current_node;
            while (temp != nullptr) {
                path.push_back(*(temp->state)); // Dereference the pointer to copy the state
                temp = temp->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Generate successors
        for (const auto& successor_state : current_node->state->getSuccessors()) {
            int new_g_cost = current_node->g_cost + 1;

            // Check if we've visited this state before with a lower or equal cost
            auto it = visited_costs.find(successor_state);
            if (it != visited_costs.end() && it->second <= (new_g_cost + h(successor_state, goal))) {
                continue; // Skip this path, it's not better
            }

            // This is a better path, so update cost and push to queue
            int h_cost = h(successor_state, goal);
            int f_cost = new_g_cost + h_cost;

            auto successor_ptr = std::make_shared<PuzzleState>(successor_state);
            auto successor_node = std::make_shared<Node>(successor_ptr, new_g_cost, f_cost, current_node);

            open_set.push(successor_node);
            visited_costs[*successor_ptr] = f_cost;
        }
    }

    std::cout << "No path found!" << std::endl;
    return {};
}