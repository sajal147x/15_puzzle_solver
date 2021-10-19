//cs411 Fall 2021 hw6 
//15 puzzle solver using ida*
//Sajal Gupta
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <climits>
using namespace std;

struct boardState{
    vector<vector<int>> board;//a 2d vector for the 4x4 board
    string moveList;//a string containing the list of moves
    int zeroRowPos, zeroColPos, numDistance;
    
};

int manhattan_function(vector<vector<int>> currState){
    int h = 0;
    for(int x = 0; x < 4; x++){
        for(int y = 0; y < 4;y++){
            if (currState[x][y])
				h += abs(((currState[x][y] - 1) / 4) - x) + abs(((currState[x][y] - 1) % 4) - y);
			else
				h += abs(3 - x) + abs(3 - y);
		}
    }
    return h;
    
}
int misplacedtile_function(vector<vector<int>> currState){
    int h = 0; 
    vector<int> state;
    for(int x = 0; x < 4; x++){
        for(int y = 0; y < 4; y++){
            state.push_back(currState[x][y]);
        }
    }
    for(int i = 0; i < 15;i++){
        if(state.at(i)!=i+1){
            h++;
        }
    }
    if(state.at(15)!=0){
        h++;
    }
    return h;
}

//function that switches the position of the blank position with the number in 4 directions
vector<vector<int>> performMove(vector<vector<int>> currState, int blankPosRow, int blankPosCol,char move){
    if (move=='U'){
        swap(currState[blankPosRow][blankPosCol],currState[blankPosRow--][blankPosCol]);
    }
    if(move=='D'){
        swap(currState[blankPosRow][blankPosCol],currState[blankPosRow++][blankPosCol]);
    }
    if(move=='L'){
        swap(currState[blankPosRow][blankPosCol],currState[blankPosRow][blankPosCol--]);
    }
    if(move=='R'){
        swap(currState[blankPosRow][blankPosCol],currState[blankPosRow][blankPosCol++]);
    }
    return currState;
}

int DFS(boardState cur, vector<vector<int>>& solvedState, int& nodes, string& finalPath, int cost, int limit,int hFuncChoice )
{
	//Increment the number of nodes
	nodes++;

	if (cur.board == solvedState)
	{
		finalPath = cur.moveList;
		return -cost;
	}
	int estimate = cost + cur.numDistance;
	if (estimate > limit)
		return estimate;
	vector<vector<int>> temp;
	int min = INT_MAX, distance;
	//0 can move up
	if (cur.zeroRowPos > 0 && (cur.moveList.size() == 0 || cur.moveList.size() != 0 && cur.moveList[cur.moveList.size() - 1] != 'D'))
	{
		temp = performMove(cur.board, cur.zeroRowPos, cur.zeroColPos, 'U');
		if(hFuncChoice==0){
		    distance = DFS({ temp,  cur.moveList + "U",cur.zeroRowPos - 1, cur.zeroColPos, manhattan_function(temp)}, solvedState, nodes, finalPath, cost + 1, limit,hFuncChoice);
		}
		else if(hFuncChoice==1){
		    distance = DFS({ temp,  cur.moveList + "U",cur.zeroRowPos - 1, cur.zeroColPos, misplacedtile_function(temp) }, solvedState, nodes, finalPath, cost + 1, limit,hFuncChoice);
		}
		if (distance < 0) return distance;
		else if (distance < min) min = distance;
	}
	//0 can move down
	if (cur.zeroRowPos < 3 && (cur.moveList.size() == 0 || cur.moveList.size() != 0 && cur.moveList[cur.moveList.size() - 1] != 'U'))
	{
		temp = performMove(cur.board, cur.zeroRowPos, cur.zeroColPos, 'D');
		if(hFuncChoice==0){
		    distance = DFS({ temp, cur.moveList + "D" ,cur.zeroRowPos + 1, cur.zeroColPos, manhattan_function(temp)}, solvedState, nodes, finalPath, cost + 1, limit,hFuncChoice);
		}
		else if(hFuncChoice==1){
		    distance = DFS({ temp, cur.moveList + "D" ,cur.zeroRowPos + 1, cur.zeroColPos, misplacedtile_function(temp)}, solvedState, nodes, finalPath, cost + 1, limit,hFuncChoice);
		}
		if (distance < 0) return distance;
		else if (distance < min) min = distance;
	}
	//0 can move left
	if (cur.zeroColPos > 0 && (cur.moveList.size() == 0 || cur.moveList.size() != 0 && cur.moveList[cur.moveList.size() - 1] != 'R'))
	{
		temp = performMove(cur.board, cur.zeroRowPos, cur.zeroColPos, 'L');
		if(hFuncChoice==0){
		    distance = DFS({ temp,cur.moveList + "L", cur.zeroRowPos, cur.zeroColPos - 1, manhattan_function(temp)}, solvedState, nodes, finalPath, cost + 1, limit,hFuncChoice);
		}
		else if(hFuncChoice==1){
		    distance = DFS({ temp,cur.moveList + "L", cur.zeroRowPos, cur.zeroColPos - 1, misplacedtile_function(temp) }, solvedState, nodes, finalPath, cost + 1, limit,hFuncChoice);
		}
		if (distance < 0) return distance;
		else if (distance < min) min = distance;
	}
	//0 can move right
	if (cur.zeroColPos < 3 && (cur.moveList.size() == 0 || cur.moveList.size() != 0 && cur.moveList[cur.moveList.size() - 1] != 'L'))
	{
		temp = performMove(cur.board, cur.zeroRowPos, cur.zeroColPos, 'R');
		if(hFuncChoice==0){
		    distance = DFS({ temp,cur.moveList + "R" , cur.zeroRowPos, cur.zeroColPos + 1, manhattan_function(temp)}, solvedState, nodes, finalPath, cost + 1, limit,hFuncChoice);
		}
		else if(hFuncChoice==1){
		    distance = DFS({ temp,cur.moveList + "R" , cur.zeroRowPos, cur.zeroColPos + 1, misplacedtile_function(temp)}, solvedState, nodes, finalPath, cost + 1, limit,hFuncChoice);
		    
		}
		if (distance < 0) return distance;
		else if (distance < min) min = distance;
	}

	return min;
}
int main(){
    int hFuncChoice;
    vector<vector<int>> solvedState = { {1, 2, 3, 4},
										{5, 6, 7, 8},
										{9, 10, 11, 12},
										{13, 14, 15, 0} };
	vector<vector<int>> startState(4, vector<int>(4));
	int zeroRow, zeroCol, distance = 0;									
	cout << "> ";
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
		{
			cin >> startState[x][y];
			if (!startState[x][y])
			{
				zeroRow = x;
				zeroCol = y;
			}
		}
	cout<<"enter 0 for manhattan_distance or 1 for misplaced tiles: "<<endl<<">";
	cin>>hFuncChoice;
	distance=manhattan_function(startState);
	boardState state{ startState, "",zeroRow, zeroCol, distance };
	int nodes = 0;
	string finalPath;
	clock_t startTime = clock();
	for (distance = DFS(state, solvedState, nodes, finalPath, 0, distance,hFuncChoice); 1; nodes = 0, distance = DFS(state, solvedState, nodes, finalPath, 0, distance,hFuncChoice))
	{
		if (distance < 0)
		{
			cout << "Moves: " << finalPath << endl;
			cout << "Number of nodes expanded: " << nodes << endl;
			cout << "Time Taken: " << (double)(clock() - startTime) / CLOCKS_PER_SEC << endl;
			cout << "Memory Used: " << ((sizeof(boardState) + sizeof(int) + (sizeof(int) * 16)) * nodes) / 1024 << " kb" << endl;
			return 0;
		}
		//If no solution is found
		else if (distance == INT_MAX){
			cout<<"no sol";
		}
	}
	
    return 0;
}




