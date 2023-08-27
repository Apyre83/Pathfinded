#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <SFML/Graphics.hpp>

#include <vector>
#include <map>
#include <cmath>
#include <queue>

#include <random>

#include <iostream>

#include <utility>



#define EMPTY 0
#define WALL 1
#define START 2
#define END 3
#define OPEN_SET 4
#define CLOSED_SET 5
#define PATH 6
#define CURRENT 7


#define GRID_SIZE_X 20
#define GRID_SIZE_Y 20


struct Node {
    int x, y;
    int g, h, f;
    Node* parent;
    int type; // 0 = libre, 1 = obstacle, 2 = chemin, 3 = début, 4 = fin

    Node(int x_, int y_, int type_, Node* parent_ = nullptr)
        : x(x_), y(y_), g(0), h(0), f(0), parent(parent_), type(type_) {}
};


struct CompareNode {
    bool operator()(Node* a, Node* b) {
        return a->f > b->f;
    }
};

extern Node* start;
extern Node* goal;


std::vector<std::vector<int> > generate_grid(int rows, int cols);
bool aStar(int startX, int startY, int goalX, int goalY, std::vector<std::vector<Node*>>& nodes);

#endif // PATHFINDING_H
