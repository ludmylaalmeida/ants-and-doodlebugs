#include "ants_and_doodlebugs.h"
using namespace std;

World::World(int x, int y, int ANTSs, int DOODLEBUGSs) {
    vector<Organism *> column(y);
    for (int i = 0; i < x; i++) {
        grid.push_back(column);
    }

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (ANTSs > 0) {
                grid[i][j] = new Ants(i, j);
                ANTSs--;
            } else if (DOODLEBUGSs > 0) {
                grid[i][j] = new Doodlebugs(i, j);
                DOODLEBUGSs--;
            } else {
                grid[i][j] = new EmptyTile(i, j);
            }
        }
    }

    int x1, x2;
    int y1, y2;
    for (int i = 0; i < 100000; i++) {
        x1 = rand() % x;
        x2 = rand() % x;
        y1 = rand() % y;
        y2 = rand() % y;
        Organism::tile_swap(grid[x1][y1], grid[x2][y2]);
    }
}

World::~World() {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            delete grid[i][j];
        }
    }
}

void World::display(sf::RenderWindow &window) {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            grid[i][j]->display(window);
        }
    }
}

void World::move() {
    vector<Organism *> DOODLEBUGSs;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j]->who() == DOODLEBUGS) {
                DOODLEBUGSs.push_back(grid[i][j]);
            }
        }
    }

    vector<Organism *> ANTSs;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j]->who() == ANTS) {
                ANTSs.push_back(grid[i][j]);
            }
        }
    }

    for (auto DOODLEBUGS : DOODLEBUGSs) {
        DOODLEBUGS->move(*this);
    }

    for (auto ANTS : ANTSs) {
        ANTS->move(*this);
    }


}

void World::run_simulation() {
    sf::RenderWindow window(sf::VideoMode(400, 400), "Ants and Doodlebugs");
    bool pressed = false;

    sf::Texture bg_image;
    if (!bg_image.loadFromFile("bg.png")) {
        cout << "error loading image\n";
    }

    sf::Sprite background;
    background.setTexture(bg_image);

    //create logo image
    sf::Texture logo_image;
    if (!logo_image.loadFromFile("logo.png")) {
        cout << "error loading image\n";
    }
    sf::Sprite logo;
    logo.setTexture(logo_image);
    //set position of button in the screen
    logo.setPosition(50.0f, 100.0f);


    // create button image
    sf::Texture btn_image;
    if (!btn_image.loadFromFile("btn.png")) {
        cout << "error loading image\n";
    }
    sf::Sprite btn;
    btn.setTexture(btn_image);
    //set position of button in the screen
    btn.setPosition(120.0f, 180.0f);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {

                // get mouse position
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // get button position
                sf::Vector2f translatedPos = window.mapPixelToCoords(mousePos);

                // if mouse position contains button position
                if (btn.getGlobalBounds().contains(translatedPos)) {
                    btn.setColor(sf::Color::Transparent);
                    logo.setColor(sf::Color::Transparent);
                    background.setColor(sf::Color::Transparent);
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Return) {
                    pressed = true;
                }
//                else {
//                    cout << "Game Over\n";
//                    exit(1);
//                }
            }


        }
        if (pressed) {
            move();
            pressed = false;
            cout << "Press enter to continue\n";

        }
        window.clear();
        display(window);
        window.draw(background);
        window.draw(btn);
        window.draw(logo);
        window.display();
    }

}

void Organism::tile_swap(Organism *&pOrganism1, Organism *&pOrganism2) {
    swap(pOrganism1->x, pOrganism2->x);
    swap(pOrganism1->y, pOrganism2->y);
    swap(pOrganism1, pOrganism2);
    pOrganism1->fix_shape_position();
    pOrganism2->fix_shape_position();
}

void Organism::fix_shape_position() {
    shape.setPosition(sf::Vector2f(x * SQUARE_PIXELS, y * SQUARE_PIXELS));
}

// For every time step, the doodlebug will move to an adjacent cell containing an ant and eat the ant.
void Doodlebugs::move(World &w) {

    ++time_steps;
    bool doodlebug_ate = false;
    bool up = false, down = false, left = false, right = false;

    while (!doodlebug_ate) {

        // this makes sure the doodlebug tried to eat in every direction, if it did get out of loop
        if (up && down && left && right) {
            break;
        }

        //  For every time step, the doodlebug will move to an adjacent cell containing an ant and eat the ant.
        switch (rand() % NUMBER_OF_DIRECTIONS) {
            case UP:
                if (y > 0) {
                    if (w[x][y - 1]->who() == ANTS) {
                        w[x][y - 1] = new EmptyTile(x, y - 1);
                        doodlebug_ate = true;
                    }
                }
                up = true;
                break;
            case DOWN:
                if (y < Y_SIZE - 1) {
                    if (w[x][y + 1]->who() == ANTS) {
                        w[x][y + 1] = new EmptyTile(x, y + 1);
                        doodlebug_ate = true;
                    }
                }
                down = true;
                break;
            case LEFT:
                if (x > 0) {
                    //go left
                    if (w[x - 1][y]->who() == ANTS) {
                        w[x - 1][y] = new EmptyTile(x - 1, y);
                        doodlebug_ate = true;
                    }
                }
                left = true;
                break;
            case RIGHT:
                if (x < X_SIZE - 1) {
                    if (w[x + 1][y]->who() == ANTS) {
                        w[x + 1][y] = new EmptyTile(x + 1, y);
                        doodlebug_ate = true;
                    }
                }
                right = true;
                break;
        }

    }

    if (doodlebug_ate) {
        doodlebug_did_not_eat_only_move = 0;
    }

    // if tried to eat ant and it did not succeed, move
    if (up && down && left && right && !doodlebug_ate) {

        ++doodlebug_did_not_eat_only_move;

        up = false;
        down = false;
        right = false;
        left = false;

        bool doodlebug_move = false;

        while (!doodlebug_move) {

            // this makes sure the doodlebug tried to move in every direction, if it did get out of loop
            if (up && down && left && right) {
                break;
            }

            switch (rand() % NUMBER_OF_DIRECTIONS) {

                case UP:
                    if (y > 0) {
                        if (w[x][y - 1]->who() == EMPTY) {
                            Organism::tile_swap(w[x][y], w[x][y - 1]);
                            doodlebug_move = true;
                        }
                    }
                    up = true;
                    break;
                case DOWN:
                    if (y < Y_SIZE - 1) {
                        if (w[x][y + 1]->who() == EMPTY) {
                            Organism::tile_swap(w[x][y], w[x][y + 1]);
                            doodlebug_move = true;
                        }
                    }
                    down = true;
                    break;
                case LEFT:
                    if (x > 0) {
                        //go left
                        if (w[x - 1][y]->who() == EMPTY) {
                            Organism::tile_swap(w[x][y], w[x - 1][y]);
                            doodlebug_move = true;
                        }
                    }
                    left = true;
                    break;
                case RIGHT:
                    if (x < X_SIZE - 1) {
                        if (w[x + 1][y]->who() == EMPTY) {
                            Organism::tile_swap(w[x][y], w[x + 1][y]);
                            doodlebug_move = true;
                        }
                    }
                    right = true;
                    break;
            }
        }
    }

    //doodlebug_fed = doodlebug_ate;
    if (time_steps == 3 && doodlebug_did_not_eat_only_move >= 3) {
        starve(w);
    }

    if (time_steps == 8) {
        time_steps = 0;
        breed(w);
    }

}

void Doodlebugs::breed(World &w) {

    //create a new ant in an adjacent(up, down, left, right) cell that is empty -- use enum Direction
    bool doodlebugs_created = false;
    bool up = false, down = false, left = false, right = false;

    // check if its empty
    while (!doodlebugs_created) {

        // this makes sure the doodlebug tried to breed in every direction, if it did get out of loop
        if (up && down && left && right) {
            break;
        }

        // You are not permitted to always choose the same direction though, it must be randomized.
        switch (rand() % NUMBER_OF_DIRECTIONS) {

            case UP:
                if (y > 0) {
                    if (w[x][y - 1]->who() == EMPTY) {
                        w[x][y - 1] = new Doodlebugs(x, y - 1);
                        doodlebugs_created = true;
                    }
                }
                up = true;
                break;
            case DOWN:
                if (y < Y_SIZE - 1) {
                    if (w[x][y + 1]->who() == EMPTY) {
                        //Organism::tile_swap(w[x][y], w[x][y + 1]);
                        w[x][y + 1] = new Doodlebugs(x, y + 1);
                        doodlebugs_created = true;
                    }
                }
                down = true;
                break;
            case LEFT:
                if (x > 0) {
                    //go left
                    if (w[x - 1][y]->who() == EMPTY) {
                        w[x - 1][y] = new Doodlebugs(x - 1, y);
                        doodlebugs_created = true;
                    }
                }
                left = true;
                break;
            case RIGHT:
                if (x < X_SIZE - 1) {
                    if (w[x + 1][y]->who() == EMPTY) {
                        w[x + 1][y] = new Doodlebugs(x + 1, y);
                        doodlebugs_created = true;
                    }
                }
                right = true;
                break;
        }
    }

    // If breeding fails because there is no room then the ant will also reset its timer.
    if (!doodlebugs_created) {
        time_steps = 0;
    }

}

void Doodlebugs::starve(World &w) {

    // kill doodlebug at it's position
    w[x][y] = new EmptyTile(x, y);

}

void Ants::move(World &w) {

    time_steps++;
    total_time++;
    bool ant_move = false;
    bool up = false, down = false, left = false, right = false;

    while (!ant_move) {

        // this makes sure the ant tried to move in every direction, if it did get out of loop
        if (up && down && left && right) {
            break;
        }

        // You are not permitted to always choose the same direction though, it must be randomized.
        switch (rand() % NUMBER_OF_DIRECTIONS) {

            case UP:
                if (y > 0) {
                    if (w[x][y - 1]->who() == EMPTY) {
                        Organism::tile_swap(w[x][y], w[x][y - 1]);
                    }
                    ant_move = true;
                }
                up = true;
                break;
            case DOWN:
                if (y < Y_SIZE - 1) {
                    if (w[x][y + 1]->who() == EMPTY) {
                        Organism::tile_swap(w[x][y], w[x][y + 1]);
                    }
                    ant_move = true;
                }
                down = true;
                break;
            case LEFT:
                if (x > 0) {
                    //go left
                    if (w[x - 1][y]->who() == EMPTY) {
                        Organism::tile_swap(w[x][y], w[x - 1][y]);
                    }
                    ant_move = true;
                }
                left = true;
                break;
            case RIGHT:
                if (x < X_SIZE - 1) {
                    if (w[x + 1][y]->who() == EMPTY) {
                        Organism::tile_swap(w[x][y], w[x + 1][y]);
                    }
                    ant_move = true;
                }
                right = true;
                break;
        }
    }

    // If an ant survives for three time steps, at the end of the time step (i. e. after moving) the ant will breed.
    //Once an offspring is produced an ant cannot produce an offspring again until it has survived three more time steps.
    if (time_steps == 3 && !offspring && total_time % 3 == 0) {
        time_steps = 0;
        breed(w);
    }
}

void Ants::breed(World &w) {
    //create a new ant in an adjacent(up, down, left, right) cell that is empty -- use enum Direction
    bool ant_created = false;
    bool up = false, down = false, left = false, right = false;

    // check if its empty
    while (!ant_created) {

        // this makes sure the doodlebug tried to breed in every direction, if it did get out of loop
        if (up && down && left && right) {
            break;
        }

        switch (rand() % NUMBER_OF_DIRECTIONS) {

            case UP:
                if (y > 0) {
                    if (w[x][y - 1]->who() == EMPTY) {
                        w[x][y - 1] = new Ants(x, y - 1);
                        ant_created = true;
                    }
                }
                up = true;
                break;
            case DOWN:
                if (y < Y_SIZE - 1) {
                    if (w[x][y + 1]->who() == EMPTY) {
                        //Organism::tile_swap(w[x][y], w[x][y + 1]);
                        w[x][y + 1] = new Ants(x, y + 1);
                        ant_created = true;
                    }
                }
                down = true;
                break;
            case LEFT:
                if (x > 0) {
                    //go left
                    if (w[x - 1][y]->who() == EMPTY) {
                        w[x - 1][y] = new Ants(x - 1, y);
                        ant_created = true;
                    }
                }
                left = true;
                break;
            case RIGHT:
                if (x < X_SIZE - 1) {
                    if (w[x + 1][y]->who() == EMPTY) {
                        w[x + 1][y] = new Ants(x + 1, y);
                        ant_created = true;
                    }
                }
                right = true;
                break;

        }
    }

    offspring = ant_created;

    // if there is no empty cell, no breeding occurs

    //ant reset the time -- already doing this at turn

}


vector<Organism *> &World::operator[](int index) {
    if (index >= grid.size()) {
        throw Index_out_of_bounds();
    }
    return grid[index];
}

const vector<Organism *> &World::operator[](int index) const {
    if (index >= grid.size()) {
        throw Index_out_of_bounds();
    }
    return grid[index];
}
