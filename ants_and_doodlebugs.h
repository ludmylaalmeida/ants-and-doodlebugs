#include <iostream>
#include <vector>
#include <cstdlib>
#include <SFML/Graphics.hpp>

#ifndef ANTS_AND_DOODLEBUGS_ANTS_AND_DOODLEBUGS_H
#define ANTS_AND_DOODLEBUGS_ANTS_AND_DOODLEBUGS_H

const int X_SIZE = 20;
const int Y_SIZE = 20;
const float SQUARE_PIXELS = 20.0;

class Index_out_of_bounds {
};

enum Organism_type {
    EMPTY, ANTS, DOODLEBUGS, NUMBER_OF_TILES
};
enum Direction {
    UP, DOWN, LEFT, RIGHT, NUMBER_OF_DIRECTIONS
};

class World;

class Organism {
public:
    Organism() : x(0), y(0), shape() {}

    Organism(int x = 0, int y = 0, float radius = SQUARE_PIXELS, size_t point_count = 30) : x(x), y(y), shape(radius,
                                                                                                              point_count) {}
    virtual void display(sf::RenderWindow &window) = 0;

    static void tile_swap(Organism *&pOrganism1, Organism *&pOrganism2);

    virtual Organism_type who() = 0;

    virtual void move(World &w) {};

    virtual void breed(World &w) {};

protected:
    int x;
    int y;
    sf::CircleShape shape;

    void fix_shape_position();
};

class Critters : public Organism {
public:
    Critters(int x = 0, int y = 0) : Organism(x, y, (SQUARE_PIXELS - 2) / 2, 30) {
        shape.setPosition(sf::Vector2f(x * SQUARE_PIXELS, y * SQUARE_PIXELS));
    }

    void display(sf::RenderWindow &window) = 0;
private:
};

class Ants : public Critters {
public:
    // constructor
    Ants(int x = 0, int y = 0) : Critters(x, y) {
        shape.setFillColor(sf::Color(16, 190, 16));
        time_steps = 0;
        total_time = 0;
        offspring = false;
    }

    void display(sf::RenderWindow &window) {
        window.draw(shape);
    }

    virtual Organism_type who() { return ANTS; }

    virtual void move(World &w);

    virtual void breed(World &w);

private:
    int time_steps;
    int total_time;
    bool offspring;
};

class Doodlebugs : public Critters {
public:
    Doodlebugs(int x = 0, int y = 0) : Critters(x, y) {
        shape.setFillColor(sf::Color(205, 41, 0));
        time_steps = 0;
        doodlebug_did_not_eat_only_move = 0;
    }

    void display(sf::RenderWindow &window) {
        window.draw(shape);
    }

    virtual Organism_type who() { return DOODLEBUGS; }

    virtual void move(World &w);

    virtual void breed(World &w);

    virtual void starve(World &w);

private:
    int time_steps;
    int doodlebug_did_not_eat_only_move;

};



class EmptyTile : public Organism {
public:
    EmptyTile(int x = 0, int y = 0) : Organism(x, y, (SQUARE_PIXELS - 2) / 2, 4) {
        shape.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256, 255));
        //tell the shape where it lives START HERE
        shape.setPosition(sf::Vector2f(x * SQUARE_PIXELS, y * SQUARE_PIXELS));
    }

    void display(sf::RenderWindow &window) {
        //window.draw(shape);
    }

    virtual Organism_type who() { return EMPTY; }

};

class World {
public:
    // initialize
    World(int x = X_SIZE, int y = Y_SIZE, int ANTSs = 100, int DOODLEBUGSs = 5);

    ~World();

    //write the big 5
    void display(sf::RenderWindow &window);

    void run_simulation();

    void move();

    std::vector<Organism *> &operator[](int index);

    const std::vector<Organism *> &operator[](int index) const;

private:
    std::vector<std::vector<Organism *>> grid;
};

#endif //ANTS_AND_DOODLEBUGS_ANTS_AND_DOODLEBUGS_H
