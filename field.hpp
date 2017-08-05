#ifndef FIELD_HPP
#define FIELD_HPP

#include <vector>
#include <array>

#include "types.hpp"

class Field {
public:

    explicit Field(Difficulty difficulty);

    bool TouchPosition(const Position& pos);

    void PlaceFlag(const Position& pos);

    // Print the raw bomb/count data
    void PrintData();

    // Prints the user selection view of the field.
    void Print();

private:

    Difficulty difficulty_;

    bool has_touched_;

    // Raw underlying bomb/number data.
    std::vector< std::vector<int> > data_;

    // Field from the user's selection perspective. Starts empty. When a user selects a field
    // the corresponding pointer is set to the data_ field value's address.

    std::vector< std::vector<int*> > selection_;

    inline const Size size()  { return difficulty_.fieldSize(); }

    inline bool InBounds(const Position& pos) {
        return (pos.x >= 0 && pos.x < size().width && pos.y >= 0 && pos.y < size().height);
    }

    inline const int ValueAt(const Position& pos) {
        auto dataRow = data_.at(pos.y);
        return dataRow.at(pos.x);
    }

    inline const int* SelectedValueAt(const Position& pos) {
        auto selectionRow = selection_.at(pos.y);
        return selectionRow.at(pos.x);
    }

    inline bool PositionIsSelected(const Position& pos) {
        return SelectedValueAt(pos) != nullptr;
    }

    // Neighbour checking methods
    int NumberOfBombsAdjacent(const Position& pos, std::vector<Position> &bombPositions);

    void SelectAdjacents(const Position& pos);

    std::vector<Position> GetAdjacentPositions(const Position& pos);

    std::vector<Position> GetUnselectedAdjacentPositions(const Position& pos);

    std::vector<Position> GenerateBombLocations();

    const int SelectPosition(const Position& pos);
};

#endif
