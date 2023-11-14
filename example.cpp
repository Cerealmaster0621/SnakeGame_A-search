#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>

struct Point {
    int x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

namespace std {
    template <>
    struct hash<Point> {
        size_t operator()(const Point& p) const {
            return hash<int>()(p.x) ^ hash<int>()(p.y << 16);
        }
    };
}

struct Node {
    Point point;
    int gCost; // Cost from start to the current node
    int hCost; // Heuristic cost estimate to the goal
    Node* parent;

    int fCost() const {
        return gCost + hCost;
    }
};

struct CompareNode {
    bool operator()(const Node* a, const Node* b) const {
        return a->fCost() > b->fCost();
    }
};

// Manhattan distance heuristic
int heuristic(const Point& a, const Point& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// Check if point is within the grid and not an obstacle
bool isValid(const Point& point, int width, int height, const std::vector<std::vector<int>>& grid) {
    return point.x >= 0 && point.x < width && point.y >= 0 && point.y < height && grid[point.y][point.x] == 0;
}

std::vector<Point> getNeighbors(const Point& point) {
    return {
        {point.x - 1, point.y}, // Left
        {point.x + 1, point.y}, // Right
        {point.x, point.y - 1}, // Up
        {point.x, point.y + 1}  // Down
    };
}

std::vector<Point> aStar(const Point& start, const Point& goal, const std::vector<std::vector<int>>& grid) {
    int width = grid[0].size(), height = grid.size();
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> openSet;
    std::unordered_map<Point, Node*> allNodes;

    Node* startNode = new Node{start, 0, heuristic(start, goal), nullptr};
    openSet.push(startNode);
    allNodes[start] = startNode;

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        if (current->point == goal) {
            // Goal reached, construct the path
            std::vector<Point> path;
            while (current != nullptr) {
                path.push_back(current->point);
                current = current->parent;
            }
            for (auto node : allNodes) delete node.second;
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& neighborPoint : getNeighbors(current->point)) {
            if (!isValid(neighborPoint, width, height, grid)) continue;

            int newGCost = current->gCost + 1;
            Node* neighborNode = allNodes[neighborPoint];

            if (neighborNode == nullptr || newGCost < neighborNode->gCost) {
                if (neighborNode == nullptr) {
                    neighborNode = new Node{neighborPoint, newGCost, heuristic(neighborPoint, goal), current};
                    allNodes[neighborPoint] = neighborNode;
                    openSet.push(neighborNode);
                } else {
                    neighborNode->gCost = newGCost;
                    neighborNode->parent = current;
                    // Since priority_queue does not support decrease-key, we reinsert the node. This can be optimized.
                    openSet.push(neighborNode);
                }
            }
        }
    }

    for (auto node : allNodes) delete node.second;
    return {}; // No path found
}

int main() {
    // Example usage with a 10x10 grid
    std::vector<std::vector<int>> grid(10, std::vector<int>(10, 0));
    // Assuming 0 is empty space, 1 is snake body or wall

    Point start{0, 0};
    Point goal{9, 9};
    std::vector<Point> path = aStar(start, goal, grid);

    for (const Point& p : path) {
        std::cout << "(" << p.x << ", " << p.y << ") ";
    }

    return 0;
}

