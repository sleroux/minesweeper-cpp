#include "field.hpp"

#include <numeric>
#include <cstdlib>
#include <iostream>
#include <algorithm>

Field::Field(const Difficulty difficulty) : 
    difficulty_(difficulty), has_touched_(false) {
    auto bombPositions = GenerateBombLocations();
    auto isBomb = [&](const Position& p) { return ValueAt(p) < 0; };

    for (int h = 0; h < size().height; h++) {
        std::vector<int> dataRow;
        std::vector<int*> selectionRow;
        for (int w = 0; w < size().width; w++) {
            auto pos = Position(w, h);
            if (std::find(bombPositions.begin(), bombPositions.end(), pos) != bombPositions.end()) {
                dataRow.push_back(-1);
            } else {
                dataRow.push_back(NumberOfBombsAdjacent(pos, bombPositions));
            }
            selectionRow.push_back(nullptr);
        }
        data_.push_back(dataRow);
        selection_.push_back(selectionRow);
    }
}

// Public API
void Field::PrintData() {
    for_each(data_.begin(), data_.end(), [](std::vector<int>& row) {
        for_each(row.begin(), row.end(), [](int& value) {
            if (value < 0) {
                std::cout << "[x]";
            } else {
                std::cout << "[" << value << "]";
            }
        });
        std::cout << std::endl;
    });
}

void Field::Print() {
    for_each(selection_.begin(), selection_.end(), [](std::vector<int*>& row) {
        for_each(row.begin(), row.end(), [](int* value) {
            if (value == nullptr) {
                std::cout << "[ ]";
            } else {
                std::cout << "[" << *value << "]";
            }
        });
        std::cout << std::endl;
    });
}

// Private API
std::vector<Position> Field::GenerateBombLocations() {
    std::vector<Position> bombs;
    srand(time(0));

    while (bombs.size() < difficulty_.numberOfBombs()) {
        int x = rand() % size().width;
        int y = rand() % size().height;
        auto pos = Position(x, y);

        if (std::find(bombs.begin(), bombs.end(), pos) == bombs.end()) {
            bombs.push_back(pos);
        }
    }
    return bombs;
}

int Field::NumberOfBombsAdjacent(const Position& pos, std::vector<Position> &bombPositions) {
    auto adjacents = GetAdjacentPositions(pos);
    auto accumulator = [&](int count, Position& adjacentPos) {
        if (!InBounds(adjacentPos)) {
            return count;
        }

        auto isBomb = std::find(
            bombPositions.begin(), 
            bombPositions.end(), adjacentPos) != bombPositions.end();

        return count += isBomb ? 1 : 0;
    };

    return std::accumulate(adjacents.begin(), adjacents.end(), 0, accumulator);
}

const int Field::SelectPosition(const Position& pos) {
    auto selectedRow = &selection_.at(pos.y);
    auto dataRow = &data_.at(pos.y);
    selectedRow->at(pos.x) = &dataRow->at(pos.x);
    return (*selectedRow->at(pos.x));
}

bool Field::TouchPosition(const Position& pos) {
    if (!InBounds(pos)) {
        return true;
    }

    auto isBomb = [&](const Position& p) { return ValueAt(p) < 0; };

    if (isBomb(pos) && has_touched_) {
        // Boom.
        return false;
    }

    if (isBomb(pos) && !has_touched_) {
        // First selection, move the bomb to the first empty spot moving left -> right
        // from the top left corner of the field
        return true;
    }

    auto value = SelectPosition(pos);

    if (value == 0) {
        // Touch all adjacent neighbours that haven't been selected yet.
        auto neighbours = GetAdjacentPositions(pos);
        std::for_each(neighbours.begin(), neighbours.end(), [&](Position& neighbour) {
            if (SelectedValueAt(neighbour) == nullptr) {
                TouchPosition(neighbour);
            }
        });
    }

    if (!has_touched_) {
        has_touched_ = true;
    }

    return true;
}

std::vector<Position> Field::GetAdjacentPositions(const Position& pos) {
    auto adjacents = pos.Adjacents();
    std::vector<Position> boundedAdjacents;
    std::for_each(adjacents.begin(), adjacents.end(), [&](Position& p) {
        if (InBounds(p)) {
            boundedAdjacents.push_back(p);
        }
    });
    return boundedAdjacents;
}

std::vector<Position> Field::GetUnselectedAdjacentPositions(const Position& pos) {
    auto adjacents = GetAdjacentPositions(pos);
    return std::accumulate(
        adjacents.begin(), 
        adjacents.end(), 
        std::vector<Position>(), 
        [&](std::vector<Position> accum, Position& pos) 
    {
        if (SelectedValueAt(pos) != nullptr) {
            accum.push_back(pos);
        }
        return accum;
    });
}
