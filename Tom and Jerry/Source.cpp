#include "field.h"

int main()
{
	ifstream readFile("data.txt", ios::in);
	ofstream writeFile("output.txt", ios::out | ios::app);
	while(!readFile.eof())
	{
		
		Board scenario;
		// Reads the information for a single scenario
		// from the given file.
		scenario.read(readFile);

		// Look for the shortest path.
		scenario.findPath();

		// Print the results in a file.
		scenario.write(writeFile);
	}
	return 0;
}