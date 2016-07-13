#include "field.h"

void Board::makeBoard(int row, int column, const int tomX,
			const int tomY, const int jerryX, const int jerryY)
{
	for(int i = 0; i <= column; i++)
		for(int j = 0; j <= row; j++)
			board.push_back(Square(j, i));

	// Assign squares to Tom and Jerry.
	findSquare(tomX, tomY, tom);
	(*tom).symbol = 'T';
	(*tom).visited = true; // Used in the path finding.
	findSquare(jerryX, jerryY, jerry);
	(*jerry).symbol = 'J';
	turns = 0; // Used in final print.
}

bool Board::findSquare(const int x, const int y, I& it)
{
	// Points the iterator to the square with the given
	// coordinates if such exists, otherwise returns false.
	for(I tmp = board.begin(); tmp != board.end(); tmp++)
	{
		if((*tmp).x == x && (*tmp).y == y)
		{
			it = tmp;
			return true;
		}
	}
	return false;
}

void Board::makeObsticle(int* obsticle)
{
	// !!!!!!!!!!!
	// !IMPORTANT!
	// !!!!!!!!!!!
	// Not sure whether we should take for granted that
	// obsticles aren't overlapping. If they aren't the
	// verification part of this funcion is obsolete.

	// Verify the obsticle won't overlap with another.
	I tmp;
	for(int i = obsticle[1]; i <= obsticle[3]; i++)
		for(int j = obsticle[0]; j <= obsticle[2]; j++)
		{
			findSquare(j, i, tmp);
			if((*tmp).symbol != '0')
				return;
		}

	// Cycle through all of the squares that are part
	// of the current obsticle and mark them as such.
	for(int i = obsticle[1]; i <= obsticle[3]; i++)
		for(int j = obsticle[0]; j <= obsticle[2]; j++)
		{
			findSquare(j, i, tmp);
			if((*tmp).symbol == '0')
				(*tmp).symbol = '*';
		}
}

void Board::read(ifstream& readFile)
{
	int r, c, tx, ty, jx, jy;
	// Get field size, Tom and Jerry's coordinates.
	readFile >> r >> c >> tx >> ty >> jx >> jy;
	makeBoard(r, c, tx, ty, jx, jy);

	// Get the coordinates of the obsticles.
	int coordinate, obsticle[4], i = 0;
	readFile >> coordinate;
	while(coordinate != -1)
	{
		obsticle[i++] = coordinate;
		readFile >> coordinate;
		if (i == 4)
		{
			makeObsticle(obsticle);
			i = 0;
		}
	}
}

void Board::write(ofstream& writeFile)
{
	// Print number of turns.
	if(foundJerry)
		writeFile << turns << '\n';
	else
		writeFile << "Impossible!" << '\n';

	// Prints the field.
	while(!board.empty())
	{
		stack<Square> row;
		int n = board.back().x;
		for(int i = 0; i <= n; i++)
		{
			row.push(board.back());
			board.pop_back();
		}

		while(!row.empty())
		{
			writeFile << row.top().symbol;
			row.pop();
		}
		writeFile << '\n';
	}
	writeFile << '\n'; // to differentiate
}

void Board::getNewCoordinates(Direction& dir, I& it, int& x, int& y)
{
	switch(dir)
	{
		case Down:  x = (*it).x; y = (*it).y - 1; break;
		case Up:	x = (*it).x; y = (*it).y + 1; break;
		case Left:  x = (*it).x - 1; y = (*it).y; break;
		case Right: x = (*it).x + 1; y = (*it).y; break;
		default: return;
	}
}

void Board::move(Direction& direction, I& it, Direction& dir,
					list<I>& shortestPath, vector<int>& turns,
					vector<list<I> >& paths)
{
	// Gets the new coordinates and a new iterator.
	int x, y;
	getNewCoordinates(direction, it, x, y);
	I itNew = it;
	if(findSquare(x, y, itNew))
	{
		if(!(*itNew).visited && !((*itNew).symbol == '*'))
		{
			(*itNew).turns = (*it).turns;
			if((direction != dir) && !(dir == First))
				(*itNew).turns++;
			(*it).visited = true;
			shortestPath.push_back(it);

			if((*itNew).symbol == 'J')
			{
				paths.push_back(shortestPath);
				turns.push_back((*itNew).turns);
				foundJerry = true;
				return;
			}
			
			makeStep(itNew, direction, shortestPath, turns,	paths);
		}
	}
}

void Board::makeStep(I& it, Direction& dir,
					list<I>& shortestPath, vector<int>& turns,
					vector<list<I> >& paths)
{	
	Direction d;
	move(d = Up, it, dir, shortestPath, turns, paths);
	move(d = Right, it, dir, shortestPath, turns, paths);
	move(d = Down, it, dir, shortestPath, turns, paths);
	move(d = Left, it, dir, shortestPath, turns, paths);

	// If it reaches here, it wasn't able to move in any			// direction so it goes back 1 square and resets the visit.
	if(!shortestPath.empty())
	{
		(*shortestPath.back()).visited = false;
		shortestPath.pop_back();
	}
}

void Board::findPath()
{
	vector<list<I> > paths;
	vector<int> turns;
	list<I> shortestPath;
	Direction dir = First;
	makeStep(tom, dir, shortestPath, turns, paths);

	// Finds the path with the least turns and squares.
	int minIndex = 0;
	for(int i = 1; i < turns.size(); i++)
	{
		if(turns[minIndex] >= turns[i])
		{
			if(paths[minIndex].size() > paths[i].size())
				minIndex = i;
		}
	}

	// Changes the symbols of the squares which are on the path.
	if(!turns.empty())
	{
		this->turns = turns[minIndex];
		for(list<I>::iterator it = paths[minIndex].begin();
				it != paths[minIndex].end(); it++)
		{
			if((*(*it)).symbol == '0')
				(*(*it)).symbol = '+';
		}
	}
}