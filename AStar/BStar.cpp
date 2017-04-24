/*
	Stripped down A* algorithm for the static map.
	Will translate to c but written in c++ for easier debugging (read: I'm lazy)
*/
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

#define MAX_HEIGHT 10
#define MAX_WIDTH 10

struct node {
	node* parent;
	int x;
	int y;
	int inOpen = 0;
	int inClosed = 0;
	double f, g, h = 0;
};

void setCoordinates(node &node, int x, int y)
{
	node.x = x;
	node.y = y;
}

void initializemap(int map[10][10], int excludex, int excludey)
{
	for (int i = 0; i < MAX_WIDTH; ++i)
	{
		for (int j = 0; j < MAX_WIDTH; ++j)
		{
			map[i][j] = 1;
		}
	}
	map[0][0] = 0;
	map[0][1] = 0;
	map[0][2] = 0;
	map[0][3] = 0;
	map[1][0] = 0;
	map[1][3] = 0;
	map[2][0] = 0;
	map[2][2] = 0;
	map[2][3] = 0;
	map[2][4] = 0;
	map[2][5] = 0;
	map[2][6] = 0;
	map[2][7] = 0;
	map[3][0] = 0;
	map[3][3] = 0;
	map[3][7] = 0;
	map[3][9] = 0;
	map[4][0] = 0;
	map[4][1] = 0;
	map[4][2] = 0;
	map[4][3] = 0;
	map[4][4] = 0;
	map[4][5] = 0;
	map[4][7] = 0;
	map[4][9] = 0;
	map[5][0] = 0;
	map[5][3] = 0;
	map[5][7] = 0;
	map[5][8] = 0;
	map[5][9] = 0;
	map[6][0] = 0;
	map[6][3] = 0;
	map[6][5] = 0;
	map[6][6] = 0;
	map[6][9] = 0;
	map[7][3] = 0;
	map[7][5] = 0;
	map[7][9] = 0;
	map[8][3] = 0;
	map[8][4] = 0;
	map[8][5] = 0;
	map[8][9] = 0;
	map[9][9] = 0;

	if (excludex != -1)
	{
		map[excludex][excludey] = 1;
	}
}

// TODO fix calculation values for g and h
void setSuccessorValues(node& successor, node predecessor, node destination)
{
	successor.g = predecessor.g + abs(predecessor.y - successor.y) + abs(predecessor.x - successor.x);
	cout << "for node (" << successor.x << ", " << successor.y << ") g = " << successor.g;
	successor.h = sqrt(((destination.x - successor.x)*(destination.x - successor.x) + (destination.y - successor.y)*(destination.y - successor.y)));
	cout << " h = " << successor.g << endl;
	successor.f = successor.g + successor.h;
}
int main(void)
{
	int map[10][10] = { 1 };
	initializemap(map, -1, -1);

	int lowestf = 10000;
	int lowestfNode;

	node start;
	start.x = 0;
	start.y = 0;

	node destination;
	destination.x = 9;
	destination.y = 9;
	int currentx = start.x;
	int currenty = start.y;

	node successor;

	int skip = 0;

	node openList[42];
	int openListSize = 0;

	node closedList[42];
	int closedListSize = 0;

	openList[0] = start;
	++openListSize;

	int testint = 0;
	while (openListSize != 0)
	{
		for (int q = 0; q < openListSize; ++q)
		{
			if (openList[q].f < lowestf)
			{
				lowestf = openList[q].f;
				lowestfNode = q;
			}
		}
		//if (openList[openListSize].x == destination.x && openList[openListSize].y == destination.y)
		//{
		////	cout << "At destination" << endl;
		//	break;
		//}
		cout << "Node with lowest f with coordinates " << openList[lowestfNode].x << ", " << openList[lowestfNode].y << endl;
		map[openList[lowestfNode].x][openList[lowestfNode].y] = 1;	// Mark the node on the map as visited
		// Push the node onto the closed list
		closedList[closedListSize] = openList[lowestfNode];
		++closedListSize;

		// Popping the node from the open list
		for (int popped = lowestfNode; popped < openListSize; ++popped)
		{
			openList[popped] = openList[popped + 1];
		}
		--openListSize;

		// Get all the open neighboring nodes
		for (int x = closedList[closedListSize - 1].x - 1; x <= closedList[closedListSize - 1].x + 1; ++x)
		{
			for (int y = closedList[closedListSize - 1].y - 1; y <= closedList[closedListSize - 1].y + 1; ++y)
			{
				{
					if (y != closedList[closedListSize - 1].y || x != closedList[closedListSize - 1].x)
					{
						skip = 0;
						if (x == destination.x && y == destination.y)
						{
							cout << "At destinaton, stopping..." << endl;
							return 0;		// Closed list will instead be returned containing the set of nodes with coordinates highlighting the steps
						}
						if (y < MAX_HEIGHT && y >= 0 && x < MAX_WIDTH && x >= 0 && map[x][y] == 0)
						{
							cout << "Child coordinates (" << x << ", " << y << ") is on path" << endl;
							setCoordinates(successor, x, y);
							setSuccessorValues(successor, closedList[closedListSize - 1], destination);
							successor.parent = &closedList[closedListSize - 1]; // Set the successor to point to the parent

							for (int openCheck = 0; openCheck < openListSize; ++openCheck)
							{
								if (successor.x == openList[openCheck].x && successor.y == openList[openCheck].y && successor.f > openList[openCheck].f)
								{
									skip = 1;
								}
							}
							for (int closedCheck = 0; closedCheck < closedListSize; ++closedCheck)
							{
								if (successor.x == closedList[closedCheck].x && successor.y == closedList[closedCheck].y && successor.f > closedList[closedCheck].f)
								{
									skip = 1;
								}
							}
							if (skip != 1)
							{
								openList[openListSize] = successor;
								++openListSize;
							}
						}
					}
				}
			}
		}				//if (map[x][y] == 0)
		//if (map[currentx][currenty] == 0)
		//{
		//	cost = abs(destination.x - currentx) + abs(destination.y - currenty);
		//}
	}

	system("pause");
	return 0;
}
