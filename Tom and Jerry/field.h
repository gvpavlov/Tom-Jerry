#ifndef FIELD_H_
#define FIELD_H_

#include <fstream>
#include <list>
#include <stack>
#include <vector>
using namespace std;

enum Direction {Up, Down, Left, Right, First};

struct Square
{
	int x;
	int y;
	char symbol;
	bool visited;
	int turns;
	Square(const int _x, const int _y, int _turns = 0,
		   char _symbol = '0', bool _visited = false)
		   : x(_x), y(_y), turns(_turns), symbol(_symbol),
			 visited(_visited) {}
};

typedef list<Square>::iterator I;

class Board
{
private:
	I tom;
	I jerry;
	list<Square> board;
	int turns;
	bool foundJerry = false;
	void makeBoard(int, int, const int, const int,
					const int, const int);
	void makeObsticle(int*);
	bool findSquare(const int, const int, I&);
	void makeStep(I&, Direction&, list<I>&,
					vector<int>&, vector<list<I> >&);
	void move(Direction&, I&, Direction&, list<I>&,
					vector<int>&, vector<list<I> >&);
	void getNewCoordinates(Direction&, I&, int&, int&);
public:
	void read(ifstream&);
	void write(ofstream&);
	void findPath();
};

#endif