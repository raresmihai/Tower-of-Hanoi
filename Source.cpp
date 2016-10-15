#include <iostream>
#include <vector>
#define maximumNumberOfMoves 1000000
using namespace std;

class HanoiTower
{
private:
	vector < vector <int> > visitedStates;
	int towersCount;
	int disksCount;
	int minimumNumberOfMoves = maximumNumberOfMoves;
	bool isFinalState(vector<int> state)
	{
		vector<int>::iterator it;
		for (it = state.begin(); it != state.end(); it++)
		{
			if (*it != towersCount)
			{
				return false;
			}
		}
		return true;
	}
	bool canTransit(vector<int> currentState, int disk, int tower)
	{
		//the disk is already on that tower
		if (currentState[disk] == tower)
		{
			return false;
		}
		vector<int>::iterator towerIterator;
		for (int i = 1; i < disk; i++)
		{
			if (currentState[i] == tower)
			{
				return false;
			}
		}
		return true;
	}
	vector<int> getNextState(vector<int> currentState, int disk, int tower)
	{
		currentState[disk] = tower;
		return currentState;
	}
	bool isANewState(vector<int> currentState)
	{
		vector< vector<int> >::iterator it;
		it = find(visitedStates.begin(), visitedStates.end(), currentState);
		return it == visitedStates.end();
	}
public:
	HanoiTower(int towersCount, int disksCount)
	{
		this->towersCount = towersCount;
		this->disksCount = disksCount;
	}
	vector<int> getInitialState()
	{
		vector<int> initialState(towersCount + 1, 1);
		return initialState;
	}
	int getSolution()
	{
		return minimumNumberOfMoves;
	}
	void solve(vector <int> currentState, int numberOfMoves)
	{
		for (int disk = 1; disk <= disksCount; disk++)
		{
			for (int tower = 1; tower <= towersCount; tower++)
			{
				if (canTransit(currentState, disk, tower))
				{
					vector <int> nextState = getNextState(currentState, disk, tower);
					if (isANewState(nextState))
					{
						if (isFinalState(nextState) && numberOfMoves < minimumNumberOfMoves)
						{
							minimumNumberOfMoves = numberOfMoves;
						}
						else
						{
							visitedStates.push_back(nextState);
							solve(nextState,numberOfMoves + 1);
							visitedStates.pop_back();
						}
					}
				}
			}
		}
	}
};

