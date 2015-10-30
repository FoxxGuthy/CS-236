#include "Pathfinder.h"



Pathfinder::Pathfinder()
{
	/**
	we initialze validMaze here for later when we use getMaze. If not generated or imported maze is there, it will be ready
	// is there a better way to just fill the whole stinking array in one line? the way you can initailze vectors?
	*/
	for (int i = 0; i < ROWS; i++) 
	{
		for (int j = 0; j < COLS; j++)
		{
			for (int k = 0; k < PAGE; k++)
			{
				validMaze[i][j][k] = OPEN;
			}
		}
	}
	srand(time(NULL));
}


Pathfinder::~Pathfinder()
{
}

//Part 1------------------------------------------------------------------------------------------------------------
/**
It is important to realize how the array is being traversed. 
Below I moved acrros columns in a row, then go to a new page. This order is important, here
In other places we're just filling the array so order does not matter, and so are incorrect as I 
only had this realization after they were made
*/
string Pathfinder::getMaze()
{
	// having set validMaze to all zeros, it its ready to be return if nothing new has been generated/imported
	// we will just need to_string the maze
	ostringstream terrainList;
	for (int i = 0; i < PAGE; i++)
	{
		for (int j = 0; j < ROWS; j++)
		{
			for (int k = 0; k < COLS; k++)
			{
				string convert = to_string(validMaze[j][k][i]); //to_string converts the int to a string
				terrainList << convert << " ";
			}
			terrainList << "\n";
		}
		terrainList << "\n\n";
	}
	return terrainList.str();
}

void Pathfinder::createRandomMaze()
{
	for (int i = 0; i < PAGE; i++)
	{
		for (int j = 0; j < ROWS; j++)
		{
			for (int k = 0; k < COLS; k++)
			{
				int terrain = rand() % 2; // will randomly give 1 or 0. redfined each time. n^3 here. not super great. 
				validMaze[i][j][k] = terrain;
			}
		}
	}
	validMaze[BEGIN][BEGIN][BEGIN] = OPEN; // sets 0,0,0 and 4,4,4 as 1's. 
	validMaze[END][END][END] = OPEN;
	randomMaze = true;
}

//Part 2------------------------------------------------------------------------------------------------------------

bool Pathfinder::importMaze(string file_name)
{
	importSucess = false;
	//cout << "being import" << endl;
	ifstream inFile;
	inFile.open(file_name.c_str());

	if (inFile.fail()) // bad file name
		return false;

	string newSpace;
	for (int i = 0; i < PAGE; i++)
	{
		for (int j = 0; j < ROWS; j++)
		{
			for (int k = 0; k < COLS; k++)
			{
				if (inFile.eof()) // if there's less data than we need. also could probably do inFile.fail()
					return false;
				
				inFile >> newSpace;

				for (unsigned n = 0; n < newSpace.length(); n++)
				{
					if (!isdigit(newSpace[n]))
						return false; // string is anything but a number. will not catch negatives or things graeter than 1
				}

				if (atoi(newSpace.c_str()) > 1 || atoi(newSpace.c_str()) < 0) // has to be 0 or 1
					return false;
					
					inputTestMaze[j][k][i] = atoi(newSpace.c_str());
			}	
		}	
	}
	//cout << "end of intake" << endl;
	if (!inFile.eof()) // if its done with all of this and there's still more, too much in file. FAIL
		return false;

	if (inputTestMaze[BEGIN][BEGIN][BEGIN] != OPEN || inputTestMaze[END][END][END] != OPEN) // begining and exit must be 1
		return false;
	
	// if it was valid, need to assign it to the acutal maze. 
	for (int i = 0; i < PAGE; i++)
	{
		for (int j = 0; j < ROWS; j++)
		{
			for (int k = 0; k < COLS; k++)
			{
				validMaze[j][k][i] = inputTestMaze[j][k][i];
			}
		}
	}
	importSucess = true;
	return true;
}

//Part 3------------------------------------------------------------------------------------------------------------

vector<string> Pathfinder::solveMaze()
{	
	pathList.clear();
	if (!importSucess && !randomMaze)
		return pathList; // don't execute, just return empty vector if the import failed. 

	for (int i = 0; i < PAGE; i++)
	{
		for (int j = 0; j < ROWS; j++)
		{
			for (int k = 0; k < COLS; k++)
			{
				inputTestMaze[j][k][i] = validMaze[j][k][i];
			}
		}
	}

	if (findPath(BEGIN, BEGIN, BEGIN))
	{
		reverse(pathList.begin(), pathList.end()); // the test driver wants the first item to be (0,0,0) so we need to reverse the vector
		return pathList;
	}

	else // there was no solution
	{
		pathList.clear();
		return pathList;
	}	
}

bool Pathfinder::findPath(int x, int y, int z)
{
	if (x > END || y > END || z > END || x < BEGIN || y < BEGIN || z < BEGIN) // if that takes out of the array, not the way
		return false;

	else if (inputTestMaze[x][y][z] == WALL) // ran into a wall. 
		return false;

	else if (inputTestMaze[x][y][z] == VISITED) // if we've already been there, that's not the way
		return false;

	else if ((x == END) && (y == END) && (z == END)) // Base Case 1: this is the END. 
	{
		string theWay = "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
		pathList.push_back(theWay);
		return true;
	}

	else if (inputTestMaze[x][y][z] == VISITED) // if we've already been there, that's not the way
		return false;

	

	else if (inputTestMaze[x][y][z] == WALL) // ran into a wall. 
		return false;


	else {

		inputTestMaze[x][y][z] = VISITED;
		
		if (findPath(x + 1, y, z) || findPath(x, y + 1, z) || findPath(x, y, z + 1) || findPath(x - 1, y, z) ||  findPath(x, y - 1, z) ||  findPath(x, y, z - 1)	)
		{
			string theWay = "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
			pathList.push_back(theWay);
			return true;
		}
	}
}

