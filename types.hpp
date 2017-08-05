#ifndef TYPES_HPP
#define TYPES_HPP

#include <array>

class Size {
public:
    const int width;
    const int height;

    explicit Size(int width, int height) : width(width), height(height) {}
};

class Position {
public:
    const int x;
    const int y;


    explicit Position(int x, int y) : x(x), y(y) {}

    std::array<Position, 8> Adjacents() const {
        return std::array<Position, 8>({
            Left(),
            Right(),
            Top(),
            Bottom(),
            TopLeft(),
            TopRight(),
            BottomLeft(),
            BottomRight()
        });
    }

    inline bool operator==(const Position& other) {
        return (this->x == other.x) && (this->y == other.y);
    }

private:
    inline Position Left() const { return Position(x - 1, y); }
    inline Position Right() const { return Position(x + 1, y); }
    inline Position Top() const { return Position(x, y - 1); }
    inline Position Bottom() const { return Position(x, y + 1); }

    inline Position TopLeft() const { return Position(x - 1, y - 1); }
    inline Position TopRight() const { return Position(x + 1, y - 1); }
    inline Position BottomLeft() const { return Position(x - 1, y + 1); }
    inline Position BottomRight() const { return Position(x + 1, y + 1); }
};


class Difficulty {
public:
    const static Difficulty Easy() { return Difficulty(0); }
    const static Difficulty Intermediate() { return Difficulty(1); }
    const static Difficulty Hard() { return Difficulty(2); }

    const int numberOfBombs() {
        switch (d) {
            case 0: return 10;
            case 1: return 40;
            case 2: return 99;
        }

        return 0;
    }

    const Size fieldSize() {
        switch (d) {
            case 0: return Size(8, 8);
            case 1: return Size(16, 16);
            case 2: return Size(16, 30);
        }
        return Size(0, 0);
    }

private:
    Difficulty(int d) : d(d) {}
    const int d;
};

#endif
