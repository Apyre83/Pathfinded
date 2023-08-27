#include "pathfinding.h"


std::vector<std::vector<int> > generate_grid(int rows, int cols) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 4);

    std::vector<std::vector<int>> grid(rows, std::vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j] = (dis(gen) % 5 == 0);
        }
    }

    grid[0][0] = 0;
    grid[rows-1][cols-1] = 0;
    return (grid);
}


int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}


bool aStar(int startX, int startY, int goalX, int goalY, std::vector<std::vector<Node*>>& nodes) {
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> openSet;
    std::vector<std::vector<bool>> closedSet(GRID_SIZE_X, std::vector<bool>(GRID_SIZE_X, false));

    start->g = 0;
    start->h = heuristic(startX, startY, goalX, goalY);
    start->f = start->g + start->h;
    openSet.push(start);

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        if (current->x == goalX && current->y == goalY) {
            goal->type = 4;
            Node* temp = current;
            while (temp->parent) {
                temp->type = 2;
                temp = temp->parent;
            }
            return true;
        }

        closedSet[current->x][current->y] = true;

        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        for (int i = 0; i < 4; ++i) {
            int x = current->x + dx[i];
            int y = current->y + dy[i];

            if (x < 0 || y < 0 || x >= GRID_SIZE_X || y >= GRID_SIZE_Y) continue;
            if (closedSet[x][y]) continue;

            Node* neighbor = nodes[x][y];
            if (neighbor->type == 1) continue; // Skip obstacles

            int tentative_g = current->g + 1;
            if (tentative_g < neighbor->g || neighbor->g == 0) {
                neighbor->parent = current;
                neighbor->g = tentative_g;
                neighbor->h = heuristic(x, y, goalX, goalY);
                neighbor->f = neighbor->g + neighbor->h;

                openSet.push(neighbor);
            }
        }
    }

    return false;
}