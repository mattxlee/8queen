#include <cstring>
#include <cstdio>

#include <iostream>
#include <vector>
#include <stdexcept>

#define WIDTH 8
#define HEIGHT 8

#define EMPTY 0
#define QUEEN 1
#define RESERVED 2

#define TOTAL_NUM_OF_QUEENS 8

struct Position
{
    int x, y;

    bool isValid() const
    {
        return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
    }
};

struct Direction
{
    int dx, dy;

    void apply(Position& pos) const
    {
        pos.x += dx;
        pos.y += dy;
    }
};

typedef std::vector<Position> Vec_Position;
typedef std::vector<Direction> Vec_Direction;

Position makePosition(int x, int y)
{
    Position pos;
    pos.x = x;
    pos.y = y;
    return pos;
}

Direction makeDirection(int dx, int dy)
{
    Direction dir;
    dir.dx = dx;
    dir.dy = dy;
    return dir;
}

class DirectionMan
{
public:
    static DirectionMan& instance()
    {
        static DirectionMan inst;
        return inst;
    }

    Vec_Direction const& directions() const
    {
        return directions_;
    }

private:
    DirectionMan()
    {
        directions_.push_back(makeDirection(-1, 0));
        directions_.push_back(makeDirection(-1, 1));
        directions_.push_back(makeDirection(-1, -1));
        directions_.push_back(makeDirection(0, 1));
        directions_.push_back(makeDirection(0, -1));
        directions_.push_back(makeDirection(1, 0));
        directions_.push_back(makeDirection(1, 1));
        directions_.push_back(makeDirection(1, -1));
    }

private:
    Vec_Direction directions_;
};

class Board
{
public:
    Board()
    {
        memset(board_, 0, sizeof(board_));
    }

    bool canPutQueen(Position const& pos) const
    {
        if (!pos.isValid()) return false;
        else return board_[pos.x][pos.y] == EMPTY;
    }

    void putQueen(Position const& pos, Vec_Position& reservedPositions)
    {
        if (!canPutQueen(pos)) throw std::runtime_error("you can not put queen in specified position");

        board_[pos.x][pos.y] = QUEEN;
        reservedPositions.push_back(pos);

        Vec_Direction::const_iterator begin = DirectionMan::instance().directions().begin();
        Vec_Direction::const_iterator end = DirectionMan::instance().directions().end();

        for (; begin != end; ++begin)
        {
            reserveWithDir(pos, *begin, reservedPositions);
        }
    }

    void emptyPositions(Vec_Position const& positions)
    {
        Vec_Position::const_iterator begin = positions.begin(), end = positions.end();
        for (; begin != end; ++begin)
        {
            board_[begin->x][begin->y] = EMPTY;
        }
    }

    void print() const
    {
        for (int i = 0; i < WIDTH; ++i)
        {
            for (int j = 0; j < HEIGHT; ++j)
            {
                switch (board_[i][j])
                {
                    case EMPTY:
                        printf(" .");
                        break;
                    case RESERVED:
                        printf(" *");
                        break;
                    case QUEEN:
                        printf(" x");
                }
            }
            printf("\n");
        }
    }

private:
    void reserveWithDir(Position const& pos, Direction const& dir, Vec_Position& reservedPositions)
    {
        Position newPos(pos);
        dir.apply(newPos);

        while (newPos.isValid())
        {
            if (board_[newPos.x][newPos.y] == EMPTY)
            {
                board_[newPos.x][newPos.y] = RESERVED;
                reservedPositions.push_back(newPos);
            }
            dir.apply(newPos);
        }
    }

private:
    char board_[WIDTH][HEIGHT];
};

class Searcher
{
public:
    void go()
    {
        numOfResults = 0;
        search(0, 1);
    }

    void search(int startJ, int numOfQueens)
    {
        if (numOfQueens > TOTAL_NUM_OF_QUEENS)
        {
            printf("==== result: %d ====\n", ++numOfResults);
            board_.print();
            printf("\n");
        }
        else
        {
            Vec_Position reservedPositions;
            reservedPositions.reserve(32);
            for (int i = 0; i < WIDTH; ++i)
                for (int j = startJ; j < HEIGHT; ++j)
                {
                    Position pos = makePosition(i, j);
                    if (board_.canPutQueen(pos))
                    {
                        board_.putQueen(pos, reservedPositions);
                        search(j, numOfQueens + 1);
                        board_.emptyPositions(reservedPositions);
                        reservedPositions.clear();
                    }
                }
        }
    }

private:
    int numOfResults;
    Board board_;
};

int main()
{
    Searcher s;
    s.go();
    return 0;
}
