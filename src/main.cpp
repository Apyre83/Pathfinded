#include "pathfinding.h"


std::vector<std::vector<int>> grid;
Node* start = new Node(0, 0, 3);
Node* goal = new Node(GRID_SIZE_X - 1, GRID_SIZE_Y - 1, 4);

void    handleEvents(sf::RenderWindow& window, int& mode, std::vector<std::vector<Node*>>& nodes, bool &isSolvable) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
            mode = (mode + 1) % 2;

            if (mode == 1) {
                isSolvable = aStar(start->x, start->y, goal->x, goal->y, nodes);
                nodes[goal->x][goal->y]->type = 4;
            }
            else {
                for (int i = 0; i < GRID_SIZE_X; ++i) {
                    for (int j = 0; j < GRID_SIZE_Y; ++j) {
                        nodes[i][j]->parent = nullptr;
                        nodes[i][j]->g = 0;
                        nodes[i][j]->h = 0;
                        nodes[i][j]->f = 0;
                        nodes[i][j]->type = grid[j][i];
                    }
                }
                nodes[start->x][start->y]->type = 3;
                nodes[goal->x][goal->y]->type = 4;
            }
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C && mode == 0) { /* Clear grid */
            for (int i = 0; i < GRID_SIZE_X; ++i) {
                for (int j = 0; j < GRID_SIZE_Y; ++j) {
                    grid[j][i] = 0;
                    nodes[i][j]->type = 0;
                    nodes[i][j]->parent = nullptr;
                    nodes[i][j]->g = 0;
                    nodes[i][j]->h = 0;
                    nodes[i][j]->f = 0;
                }
            }
            nodes[start->x][start->y]->type = 3;
            nodes[goal->x][goal->y]->type = 4;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G && mode == 0) { /* Generate new grid */
            grid = generate_grid(GRID_SIZE_X, GRID_SIZE_Y);
            for (int i = 0; i < GRID_SIZE_X; ++i) {
                for (int j = 0; j < GRID_SIZE_Y; ++j) {
                    nodes[i][j]->type = grid[j][i];
                    nodes[i][j]->parent = nullptr;
                    nodes[i][j]->g = 0;
                    nodes[i][j]->h = 0;
                    nodes[i][j]->f = 0;
                }
            }
            nodes[start->x][start->y]->type = 3;
            nodes[goal->x][goal->y]->type = 4;
        }
    }
}



int main() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "A* Pathfinding", sf::Style::Fullscreen);
	sf::Vector2u size = window.getSize();

	grid = generate_grid(GRID_SIZE_X, GRID_SIZE_Y);

	const int	cellSizeX = size.x / grid[0].size() * .75f;
	const int	cellSizeY = size.y / grid.size();


    std::vector<std::vector<Node*>> nodes(GRID_SIZE_Y, std::vector<Node*>(GRID_SIZE_X));
    for (int i = 0; i < GRID_SIZE_X; ++i) {
        for (int j = 0; j < GRID_SIZE_Y; ++j) {
            nodes[i][j] = new Node(i, j, grid[j][i]);
        }
    }
	delete nodes[start->x][start->y];
	delete nodes[goal->x][goal->y];
	nodes[start->x][start->y] = start;
	nodes[goal->x][goal->y] = goal;

    /*
     * Mode 0 = Waiting for start
     * Mode 1 = Simulated
     */
    int mode = 0;

    sf::Font font;
    if (!font.loadFromFile("res/race.ttf")) {
        std::cout << "Error loading font" << std::endl;
        return 1;
    }

    sf::RectangleShape rightSeparator(sf::Vector2f(size.x * 0.25, 5.0f));
    rightSeparator.setPosition({GRID_SIZE_X * cellSizeX * 1.0f, size.y * 0.5f});
    rightSeparator.setFillColor(sf::Color::White);


    sf::Text    modeInfos(font, "Mode: Editing", 20);
    modeInfos.setFillColor(sf::Color::White);
    modeInfos.setPosition({GRID_SIZE_X * cellSizeX * 1.0f + 10.0f, 10.0f});

    sf::Text    keybindsInfo(font, "Keybinds:\n\tLeft click: Add obstacle\n\tRight click: Remove obstacle\n\tSpace: Start/Stop simulation\n\tC: Clear grid\n\tG: Generate new grid", 20);
    keybindsInfo.setFillColor(sf::Color::White);
    keybindsInfo.setPosition({GRID_SIZE_X * cellSizeX * 1.0f + 10.0f, size.y * 0.5f + 10.0f});

    sf::RectangleShape    noSolutionBackground(sf::Vector2f(GRID_SIZE_X * cellSizeX, GRID_SIZE_Y * cellSizeY));
    noSolutionBackground.setPosition({0, 0});
    noSolutionBackground.setFillColor(sf::Color(255, 255, 255, 200));
    sf::Text    noSolution(font, "No solution found", 50);
    noSolution.setFillColor(sf::Color::Black);
    noSolution.setPosition({GRID_SIZE_X * cellSizeX / 2 - noSolution.getLocalBounds().width / 2, GRID_SIZE_Y * cellSizeY / 2 - noSolution.getLocalBounds().height / 2});



    bool isSolvable = false;

    while (window.isOpen()) {

        handleEvents(window, mode, nodes, isSolvable);

        if (mode == 0 && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            int x = (mousePos.x) / cellSizeX;
            int y = (mousePos.y) / cellSizeY;

            if (x >= 0 && y >= 0 && x < GRID_SIZE_X && y < GRID_SIZE_Y) {
                if (nodes[x][y]->type != 1) {
                    nodes[x][y]->type = 1;
                    grid[y][x] = 1;
                }
            }
        }

        if (mode == 0 && sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            int x = (mousePos.x) / cellSizeX;
            int y = (mousePos.y) / cellSizeY;

            if (x >= 0 && y >= 0 && x < GRID_SIZE_X && y < GRID_SIZE_Y) {
                if (nodes[x][y]->type != 0) {
                    nodes[x][y]->type = 0;
                    grid[y][x] = 0;
                }
            }
        }


        modeInfos.setString("Mode: " + std::string(mode == 0 ? "Editing" : "Pathfinded"));


        window.clear();

        for (int i = 0; i < GRID_SIZE_X; ++i) {
            for (int j = 0; j < GRID_SIZE_Y; ++j) {
                sf::RectangleShape cell(sf::Vector2f(cellSizeX, cellSizeY));
                cell.setPosition({i * cellSizeX * 1.0f, j * cellSizeY * 1.0f});

                switch (nodes[i][j]->type) {
                    case 0:
                        cell.setFillColor(sf::Color::White);
                        break;
                    case 1:
                        cell.setFillColor(sf::Color(100, 100, 100));
                        break;
                    case 2:
                        cell.setFillColor(sf::Color::Green);
                        break;
                    case 3:
                        cell.setFillColor(sf::Color::Blue);
                        break;
                    case 4:
                        cell.setFillColor(sf::Color::Red);
                        break;
                }
				cell.setOutlineThickness(5);
				cell.setOutlineColor(sf::Color::Black);

                window.draw(cell);
            }
        }

        if (mode == 1 && isSolvable == false) {
            window.draw(noSolutionBackground);
            window.draw(noSolution);
        }

        window.draw(rightSeparator);
        window.draw(modeInfos);
        window.draw(keybindsInfo);

        window.display();
    }

    return 0;
}

