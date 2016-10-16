#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <algorithm>
#include <queue>
using namespace std;

class HanoiTower
{
private:
	vector <int> initialState;
	vector < vector <int> > visitedStates;
	vector <vector<int>> notToVisitStates;
	vector <vector<int>> hillUsed;
	int towersCount;
	int disksCount;
	bool solutionFound = false;
	int maximumNumberOfMoves;
	int minimumNumberOfMoves;
	int algorithmSteps;
	int lastDiskMoved;
	int lastKeyStateIndex;
	int minim(int x, int y)
	{
		return x < y ? x : y;
	}
	void setMaximumNumberOfMoves()
	{
		maximumNumberOfMoves = 1024;
	}
	bool isFinalState(vector<int> state)
	{
		vector<int>::iterator it;
		for (it = state.begin() + 1; it != state.end(); it++)
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

		int currentTower = currentState[disk];
		//the disk is already on that tower
		if (currentTower == tower)
		{
			return false;
		}
		vector<int>::iterator towerIterator;
		for (int i = 1; i < disk; i++)
		{
			if (currentState[i] == tower || currentState[i] == currentTower)
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
	bool diskIsAlreadyPlaced(vector<int> state, int currentDisk)
	{
		int disk = disksCount;
		while (state[disk] == towersCount)
		{
			disk--;
		}
		return disk < currentDisk;
	}
	vector<int> computeInitialState()
	{
		vector<int> initialState(disksCount + 1, 1);
		return initialState;
	}
	bool canMoveDisk(vector<int> state, int disk)
	{
		for (int i = 1; i < disk; i++)
		{
			if (state[i] == state[disk])
			{
				return false;
			}
		}
		return true;
	}
	/*
	If it can move the correct disk to the last tower, then it chooses that disk, otherwise:
	Get a random disk from the set of disks that haven't been placed yet on the last tower (in the correct order)
	For efficency, it ignores the disks that are already placed on the last tower, in the correct order.
	*/
	int getRandomDisk(vector<int> currentState)
	{
		vector<int> availableDisks;
		for (int i = 1; i <= disksCount; i++)
		{
			if (!diskIsAlreadyPlaced(currentState, i) && canMoveDisk(currentState, i))
			{
				availableDisks.push_back(i);
			}
		}
		int randomDiskIndex = rand() % availableDisks.size();
		return availableDisks[randomDiskIndex];
	}
	int getRandomTower()
	{
		return rand() % towersCount + 1;
	}
	int getNumberOfPlacedDisks(vector<int> state)
	{
		int disk = disksCount;
		while (state[disk] == towersCount)
		{
			disk--;
		}
		return disksCount - disk;
	}
	/*
	Returns true if this state has more disks placed on the last tower, in the correct order, than the last key state.
	*/
	bool isAKeyState(vector<int> currentState)
	{
		if (lastKeyStateIndex == 0 && currentState[disksCount] == towersCount) return true; //at least the biggest disk is placed
		int currentStatePlacedDisks = getNumberOfPlacedDisks(currentState);
		int lastKeyStatePlacedDisks = getNumberOfPlacedDisks(visitedStates[lastKeyStateIndex]);
		return currentStatePlacedDisks > lastKeyStatePlacedDisks;
	}
public:
	HanoiTower(int towersCount, int disksCount)
	{
		this->towersCount = towersCount;
		this->disksCount = disksCount;
		initializeProblem();
	}
	vector<int> getInitialState()
	{
		return initialState;
	}
	int getSolution()
	{
		if (solutionFound) return minimumNumberOfMoves;
		return -1;
	}
	void initializeProblem()
	{
		solutionFound = false;
		srand(time(NULL));
		notToVisitStates.clear();
		visitedStates.clear();
		initialState = computeInitialState();
		visitedStates.push_back(initialState);
		setMaximumNumberOfMoves();
		minimumNumberOfMoves = maximumNumberOfMoves;
		algorithmSteps = 0;
		lastKeyStateIndex = 0;
		lastDiskMoved = 0;
	}
	int getAlgorithmSteps()
	{
		return algorithmSteps;
	}
	int canMoveToLastDisk(vector<int> state)
	{
		int disk = disksCount;
		while (state[disk] == towersCount) disk--;
		for (int i = 1; i < disk; i++)
		{
			if (state[i] == towersCount) return -1;
		}
		if (canMoveDisk(state, disk)) return disk;
		return -1;
	}
	void randomSolve(vector<int> currentState)
	{
		if (!isFinalState(currentState) && algorithmSteps < maximumNumberOfMoves)
		{
			int disk = getRandomDisk(currentState);
			int tower = getRandomTower();
			if (canMoveToLastDisk(currentState) > 0)
			{
				disk = canMoveToLastDisk(currentState);
				tower = towersCount;
			}
			vector<int> nextState = getNextState(currentState, disk, tower);
			if (canTransit(currentState, disk, tower) && isANewState(nextState))
			{
				visitedStates.push_back(nextState);
				if (isAKeyState(nextState))
				{
					lastKeyStateIndex = visitedStates.size() - 1;
				}
				algorithmSteps++;
				randomSolve(nextState);
			}
			else
			{

				if (lastKeyStateIndex > 0)
				{
					visitedStates.erase(visitedStates.begin() + lastKeyStateIndex + 1, visitedStates.end());
				}
				algorithmSteps++;
				randomSolve(visitedStates[visitedStates.size() - 1]);
			}
		}
		else
		{
			if (isFinalState(currentState))
			{
				minimumNumberOfMoves = visitedStates.size();
				solutionFound = true;
			}
		}
	}

	/*
	Find the first solution using an backtracking algorithm
	For "efficiency", if the disk is already placed on the last tower in the correct order,
	then the disk won't be moved anymore.
	Also, for true efficiency, at a step, if it's possible, it moves the correct disk to the last tower.
	*/
	void bktSolve(vector <int> currentState)
	{
		if (!solutionFound)
		{
			for (int disk = 1; disk <= disksCount && !solutionFound && algorithmSteps < maximumNumberOfMoves; disk++)
			{
				if (!diskIsAlreadyPlaced(currentState, disk) && disk != lastDiskMoved && canMoveDisk(currentState, disk)) {
					for (int tower = 1; tower <= towersCount && !solutionFound && algorithmSteps < maximumNumberOfMoves; tower++)
					{
						if (canTransit(currentState, disk, tower))
						{
							if (canMoveToLastDisk(currentState) > 0)
							{
								disk = canMoveToLastDisk(currentState);
								tower = towersCount;
							}
							vector <int> nextState = getNextState(currentState, disk, tower);
							if (isANewState(nextState))
							{
								if (algorithmSteps < maximumNumberOfMoves)
								{
									algorithmSteps++;
									visitedStates.push_back(nextState);
									lastDiskMoved = disk;
									bktSolve(nextState);
									if (algorithmSteps < maximumNumberOfMoves)
										visitedStates.pop_back();
								}
							}
						}
					}
				}
				else if (isFinalState(currentState) && visitedStates.size() < minimumNumberOfMoves)
				{
					minimumNumberOfMoves = visitedStates.size() - 1;
					solutionFound = true;
				}
			}
		}

	}

	int hillFunction(vector<int> currentState, int disksCount, int towersCount){
		int sum = 0;
		for (int i = 1; i <= disksCount; ++i){
			if (currentState[i] > 1){
				if (currentState[i == towersCount]){
					sum += i*currentState[i];
				}
				else{
					sum += currentState[i];
				}
			}
		}
		return sum;
	}

	bool canBeUsed(vector<int> current_state){
		int i, j;
		for (i = 0; i < notToVisitStates.size(); ++i){
			for (j = 1; j < notToVisitStates[i].size(); ++j){
				if (notToVisitStates[i][j] != current_state[j]){
					break;
				}
			}
			if (j == notToVisitStates[i].size()){
				return false;
			}
		}
		return true;
	}


	vector<int> bestHillState(vector<int> currentState, int disksCount, int towersCount){
		int currentValueState = hillFunction(currentState, disksCount, towersCount);
		vector<int> bestState;
		int bestValueState = -1;
		for (int disk = 1; disk <= disksCount; ++disk){
			for (int tower = 1; tower <= towersCount; ++tower){
				if (!diskIsAlreadyPlaced(currentState, disk) && canTransit(currentState, disk, tower)){
					vector <int> newState = getNextState(currentState, disk, tower);
					if (canBeUsed(newState)){
						int newValueState = hillFunction(newState, disksCount, towersCount);
						if (newValueState > bestValueState){
							bestValueState = newValueState;
							bestState = newState;
						}
					}
				}
			}
		}
		if (currentValueState > bestValueState){
			bestState[0] = -1;
		}
		else{
			bestState[0] = 1;
		}
		return bestState;
	}

	void hillClimbingSolve(vector<int> currentState){
		bool negative_transition = false;
		visitedStates.push_back(currentState);
		while (!isFinalState(currentState)){
			vector<int> bestNextState = bestHillState(currentState, disksCount, towersCount);
			for (int i = 0; i < currentState.size(); ++i){
				cout << bestNextState[i] << " ";
			}
			cout << "\n";
			if (bestNextState[0] < 0){
				if (negative_transition){
					notToVisitStates.push_back(bestNextState);
					visitedStates.pop_back();
					currentState = visitedStates[visitedStates.size() - 1];
					negative_transition = false;
				}
				else{
					negative_transition = true;
					visitedStates.pop_back();
					currentState = bestNextState;
				}
			}
			else{
				negative_transition = false;
				visitedStates.push_back(bestNextState);
				currentState = bestNextState;
			}
		}
		cout << "I',done";
	}
	int aStarSolve(vector<int> currentState){
		visitedStates.push_back(currentState);
		queue <pair<vector<int>, int>> Q;
		Q.push(make_pair(currentState, 0));
		while (!Q.empty()){
			pair<vector<int>, int> current_pair = Q.front();
			Q.pop();
			for (int disk = 1; disk <= disksCount; ++disk){
				for (int tower = 1; tower <= towersCount; ++tower){
					if (!diskIsAlreadyPlaced(current_pair.first, disk) && canTransit(current_pair.first, disk, tower)){
						vector <int> newState = getNextState(current_pair.first, disk, tower);
						if (isFinalState(newState)){
							return current_pair.second + 1;
						}
						if (isANewState(newState)){
							visitedStates.push_back(newState);
							Q.push(make_pair(newState, current_pair.second + 1));
						}

					}
				}
			}
		}
	}

};

int main()
{
	HanoiTower hanoiTower(3, 3);
	/*hanoiTower.bktSolve(hanoiTower.getInitialState());
	cout << hanoiTower.getSolution() << " " << hanoiTower.getAlgorithmSteps()<<"\n";
	hanoiTower.initializeProblem();
	hanoiTower.randomSolve(hanoiTower.getInitialState());
	cout << hanoiTower.getSolution() << " " << hanoiTower.getAlgorithmSteps();
	int x;
	cin >> x;
	*/
	cout << hanoiTower.aStarSolve(hanoiTower.getInitialState());
}