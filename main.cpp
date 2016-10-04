//
//  main.cpp
//  GPMM
//
//  Created by Rogelio Espinoza on 9/22/16.
//  Copyright © 2016 Rogelio Espinoza. All rights reserved.
//

#include <iostream>
#include <string>
#include <array>
#include <ctype.h>

using namespace std;

//CONSTANTS
const int MinRows = 4;
const int MaxRows = 10;
const int MinCols = 4;
const int MaxCols = 10;
const int MinNumOfPieces = 3;
const int MaxNumOfPieces = 6;
const int player1Id = 1;
const int player2Id = 2;
const char emptyChar = ' ';
const char player1Char = 'x';
const char player2Char = 'o';

//VARIABLES
int rows, cols, numOfPieces, winnerId;
char *grid;
bool gridIsFull, haveWinner, wrapMode, removeMode, cpuMode, isFinished;

//HEADERS
void init();
void requestSettings();
void initializeGrid();
void printGrid();
void startGame();
void testGame();
void endGame();
int requestAnswer(int);
char requestPopMode();
void updateGrid(int, int, char);
void dropChip(int, int);
void popChip(int, int);
void playerTurn(int);
void checkCombo(int);
bool checkComboHorizontally(char);
bool checkComboVertically(char);
bool checkComboTLBRDiagonally(char);
bool checkComboTRBLDiagonally(char);
bool checkComboBRTLDiagonally(char);
bool checkComboBLTRDiagonally(char);
void checkGridCapacity();

int main() {
    // insert code here...
    init();
    //testGame();
    
    //cout << ((7+(6))%7);
    return 0;
}

void init()
{
    requestSettings();
    startGame();
}

void requestSettings()
{
    char cGameMode;
    
    cout << "Hello there! Welcome to Connect Four, the game that almost made the author lose his mind!" << endl;
    cout << "Do you want to play a [Q]uick Game or [C]ustom Game?" << endl;
    cin >> cGameMode;
    cGameMode = toupper(cGameMode);
    cin.clear();
    cin.ignore();
    
    while (!cin || (cGameMode != 'Q' && cGameMode != 'C'))
    {
        cout << "Invalid value, do you want to play a [Q]uick Game or [C]ustom Game?" << endl;
        cin >> cGameMode;
        cGameMode = toupper(cGameMode);
        cin.clear();
        cin.ignore();
    }
    
    if(cGameMode == 'Q')
    {
        cols = 7;
        rows = 7;
        numOfPieces = 4;
        wrapMode = true;
        removeMode = false;
        cpuMode = false;
    }
    else
    {
        //ROWS
        cout << "Please enter the number of ROWS for the grid: " << endl;
        cin >> rows;
        cin.clear();
        cin.ignore();
        
        while (!cin || rows < MinRows || rows > MaxRows)
        {
            cout << "Invalid value, please enter a valid number of ROWS for the grid: " << endl;
            cin >> rows;
            cin.clear();
            cin.ignore();
        }
        
        //COLS
        cout << "Thanks, now please enter the number of COLUMNS: " << endl;
        cin >> cols;
        cin.clear();
        cin.ignore();
        while (!cin || cols < MinCols || cols > MaxCols)
        {
            cout << "Invalid value, please enter a valid number of COLUMNS: " << endl;
            cin >> cols;
            cin.clear();
            cin.ignore();
        }
        
        //NUMBER OF PIECES
        cout << "Please enter the number of CONNECTED PIECES needed to win the game: " << endl;
        cin >> numOfPieces;
        cin.clear();
        cin.ignore();
        
        while (!cin || numOfPieces >= rows || numOfPieces >= cols)
        {
            cout << "Invalid value, please enter a valid number for the CONNECTED PIECES needed to win the game: " << endl;
            cin >> numOfPieces;
            cin.clear();
            cin.ignore();
        }

        //WRAP MODE
        char cWrapMode;
        cout << "Great. Do you wish to enable WRAPPING mode? [Y/N]" << endl;
        cin >> cWrapMode;
        cWrapMode = toupper(cWrapMode);
        cin.clear();
        cin.ignore();
        
        while (!cin || (cWrapMode != 'Y' && cWrapMode != 'N'))
        {
            cout << "Invalid value, please enter if you wish to enable WRAPPING mode: [Y/N]" << endl;
            cin >> cWrapMode;
            cWrapMode = toupper(cWrapMode);
            cin.clear();
            cin.ignore();
        }
        //Evaluate if wrapMode is active or not.
        wrapMode = cWrapMode == 'Y';
        
        //REMOVE MODE
        char cRemoveMode;
        cout << "Great. Do you wish to enable POP BASE mode? [Y/N]" << endl;
        cin >> cRemoveMode;
        cRemoveMode = toupper(cRemoveMode);
        cin.clear();
        cin.ignore();
        
        while (!cin || (cRemoveMode != 'Y' && cRemoveMode != 'N'))
        {
            cout << "Invalid value, please enter if you wish to enable POP BASE mode: [Y/N]" << endl;
            cin >> cRemoveMode;
            cRemoveMode = toupper(cRemoveMode);
            cin.clear();
            cin.ignore();
        }
        //Evaluate if removeMode is active or not.
        removeMode = cRemoveMode == 'Y';

        //AI MODE
		cpuMode = false;
		/*
        char cCpuMode;
        cout << "Great. Do you wish to play versus against the CPU? [Y/N]" << endl;
        cin >> cCpuMode;
        cCpuMode = toupper(cCpuMode);
        cin.clear();
        cin.ignore();
        
        while (!cin || (cCpuMode != 'Y' && cCpuMode != 'N'))
        {
            cout << "Invalid value, please enter if you wish to play versus against the CPU: [Y/N]" << endl;
            cin >> cCpuMode;
            cCpuMode = toupper(cCpuMode);
            cin.clear();
            cin.ignore();
        }
        //Evaluate if removeMode is active or not.
        cpuMode = cCpuMode == 'Y';
		*/

    }
    
    //CHECK VALUES
//    cout << "config: " << rows << "rows, " << cols << "cols, " << numOfPieces << "pcs. Wrap Mode is: " << wrapMode << ". Remove Base Mode is: " << removeMode << ". CPU Mode: " << cpuMode << "." << endl;
    
    initializeGrid();
    
    
}

//INITIALIZE GRID.
void initializeGrid()
{
    /*
    cols = 6;
    rows = 10;
    numOfPieces = 3;
    */
    grid = new char[rows * cols];
    
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < cols; c++)
        {
            grid[r * cols + c] = emptyChar;
        }
    }
    printGrid();
    
}

//PRINT GRID.
void printGrid()
{
    //Print headers.
    for(int h = 1; h <= cols; h++)
    {
        cout << "  " << h << " ";
    }
    cout << endl;
    
    //Print cells.
    int cellCounter = 0;
    for(int r = 0; r < rows; r++)
    {
        cout << "|" ;
        for(int c = 0; c < cols; c++)
        {
            int cell = r * cols + c;
            cout << "(" << grid[cell] << ")|";
            //cout << "|" << r * rows + c << "|";
            //cout << "|" << cellCounter++ << "|";
        }
        
        cout << endl;
    }
    
    cout << endl;
}

//UPDATE GRID.
void updateGrid(int col, int playerNum, char action)
{
    switch(action)
    {
        case 'D' :
            dropChip(col, playerNum);
            break;
        case 'P':
            popChip(col, playerNum);
            break;
        default:
            break;
    }

}

void dropChip(int col, int playerNum)
{
    int playerTag = playerNum == player1Id? player1Char : player2Char;
    //Check if column is full.
    bool colIsFull = true;
    
    for(int r = rows - 1; r >= 0; r--)
    {
        int cell = r * cols + col;
        
        //Check if current cell is empty.
        if(grid[cell] == emptyChar)
        {
            //colIsFull << "The cell " << r * rows + col << " in column " << col << " is empty." << endl;
            colIsFull = false;
            grid[cell] = playerTag;
            break;
        }
    }
    
    //If column was full, request another column. Else, check for combo.
    if(colIsFull)
    {
        //Request another column.
        cout << "The column " << col + 1 << " is full. Please choose another column:" << endl;
        playerTurn(playerNum);
    }
    else
    {
        //Check for combo.
        checkCombo(playerNum);
        
        //Check if grid is full.
        checkGridCapacity();
        
    }
}

void popChip(int col, int playerNum)
{
    char playerTag = playerNum == player1Id? player1Char : player2Char;
    
    //Get the bottom cell at the chosen column.
    int bottomCellNum = ((rows - 1) * cols) + col;
    cout << "The bottom cell is: " << bottomCellNum << endl;
    if(grid[bottomCellNum] == playerTag)
    {
        //Then by row.
        for(int r = rows - 2; r >= 0; r--)
        {
            int cellNum = col + cols * r;
            int nxtCellNum = col + cols * (r + 1);
            char *cell = &grid[cellNum];
            char *nxtCell = &grid[nxtCellNum];
            
            *nxtCell = *cell;
            
            if(r == 0)
            {
                *cell = emptyChar;
            }
            
            //cout << "Current cell is: " << cellNum << " and it contains: " << *cell << "." <<endl;
            //cout << "Current cell is: " << nxtCellNum << " and it contains: " << *nxtCell << "." <<endl;
        }
        
        //Check if any player managed to get a combo thanks to the rearranging.
        checkCombo(player1Id);
        checkCombo(player2Id);
    }
    else
    {
        //Request another column.
        cout << "The character at the bottom of column " << col + 1 << " isn't yours. Please choose another column:" << endl;
        playerTurn(playerNum);
    }
}

void checkGridCapacity()
{
    gridIsFull = true;
    
    for(int r = 0; r < rows; r++)
    {
        //Then by column.
        for(int c = 0; c < cols; c++)
        {
            int cellNum = r * rows + c;
            char *cell = &grid[cellNum];
            
            if(*cell == emptyChar)
            {
                gridIsFull = false;
                return;
            }
            
        }
    }
    
    isFinished = true;
    
    
}
void checkComboOptimized(int playerNum)
{
    char tag = playerNum == player1Id? player1Char : player2Char;
    int counter = 0;
    
    int directions[8][2] =
    {
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0}, /*{0,  0},*/ {1,  0},
        {-1,  1}, {0,  1}, {1,  1}
    };
    
    //Cycle through directions.
    for(int d = 0; d < 8; d++)
    {
        int rowIndex = 0;
        int highRowAmount;
        int lowRowAmount;
        int directionX = directions[d][0];
        
        //Get row values.
        switch(directionX)
        {
            case -1:
                highRowAmount = rowIndex;
                lowRowAmount = -1;
                break;
            case 0:
                highRowAmount = 0;
                lowRowAmount = 0;
                break;
            case 1:
                highRowAmount = rows;
                lowRowAmount = rowIndex;
                break;
            default:
                break;
        }
        
        //Check first by rows.
        for(rowIndex; highRowAmount > lowRowAmount; rowIndex + directionX)
        {
            int colIndex = 0;
            int highColAmount;
            int lowColAmount;
            int directionY = directions[d][1];
            
            //Get row values.
            switch(directionY)
            {
                case -1:
                    highColAmount = rowIndex;
                    lowColAmount = -1;
                    break;
                case 0:
                    highColAmount = 0;
                    lowColAmount = 0;
                    break;
                case 1:
                    highColAmount = cols;
                    lowColAmount = colIndex;
                    break;
                default:
                    break;
            }
            
            //Then by column.
            for(colIndex; highColAmount > lowColAmount; colIndex + directionY)
            {
                int cellNum = rowIndex * cols + colIndex;
                char *cell = &grid[cellNum];
                cout << "Current cell is: " << cellNum << " and it contains: " << *cell << "." <<endl;
                
                //If tag is the same as the current cell, increase counter and break.
                if(*cell == tag)
                {
                }
                else
                {
                }
            }
        }
        
    }
    
    
}
void checkCombo(int playerNum)
{
    char currentPlayerId = playerNum == player1Id? player1Char : player2Char;
    //Check horizontally.
    bool horizontalCombo = checkComboHorizontally(currentPlayerId);
    bool verticalCombo = checkComboVertically(currentPlayerId);
    bool diagonalTLBRCombo = checkComboTLBRDiagonally(currentPlayerId);
    //bool diagonalBRTLCombo = checkComboBRTLDiagonally(currentPlayerId);
    bool diagonalTRBLCombo = checkComboTRBLDiagonally(currentPlayerId);
    //bool diagonalBLTRCombo = checkComboBLTRDiagonally(currentPlayerId);
    //Check vertically.
    //Check diagonally.
    
    //Verify if we have a winner.
//    cout << "PLAYER " << playerNum << endl << "=====================" << endl;
//    cout << "HCombo: " << horizontalCombo << endl;
//    cout << "VCombo: " << verticalCombo << endl;
//    cout << "TLBRCombo: " << diagonalTLBRCombo << endl;
//    //cout << "BRTLCombo: " << diagonalBRTLCombo << endl;
//    cout << "TRBLCombo: " << diagonalTRBLCombo << endl;
//    //cout << "BLTRCombo: " << diagonalBLTRCombo << endl;
//    cout << "=====================" << endl;
    
    if(horizontalCombo || verticalCombo || diagonalTLBRCombo || diagonalTRBLCombo)
    {
        haveWinner = true;
        isFinished = true;
        winnerId = playerNum;
    }
    
}

bool checkComboHorizontally(char tag)
{
//    cout << "HORIZONTAL CHECK" << endl;
    int counter{0};
    int brokenCounter{0};
    int loopCounter{0};
    
    //Check first by rows.
    for(int r = 0; r < rows; r++)
    {
        //Then by column.
        for(int c = 0; c < cols; c++)
        {
            int cellNum = r * cols + c;
            char *cell = &grid[cellNum];
//            cout << "Current cell is: " << cellNum << " and it contains: " << *cell << "." <<endl;
            
            //If tag is the same as the current cell, increase counter and break.
            if(*cell == tag)
            {
                counter++;
                if(wrapMode)
                {
                    if(brokenCounter < 1) loopCounter++;
                    if(c == cols - 1) counter += loopCounter;
                }
                if(counter >= numOfPieces) return true;
            }
            else
            {
                brokenCounter++;
                counter = 0;
            }
//            cout << "Counter is: " << counter << ", Loops: " << loopCounter << ", Breaks: " << brokenCounter << "." << endl;
        }
        
        counter = 0;
        loopCounter = 0;
        brokenCounter = 0;
    }
    
    //Return if counter is greater or equal to the required number of pieces to win.
    return false;
}

bool checkComboVertically(char tag)
{
//    cout << "VERTICAL CHECK" << endl;
    
    int counter{0};
    int brokenCounter{0};
    int loopCounter{0};
    
    //Check first by column.
    for(int c = 0; c < cols; c++)
    {
        //Then by row.
        for(int r = 0; r < rows; r++)
        {
            int cellNum = c + cols * r;
            char *cell = &grid[cellNum];
//            cout << "Current cell is: " << cellNum << " and it contains: " << *cell << "." <<endl;
            if(*cell == tag)
            {
                counter++;
                if(wrapMode)
                {
                    if(brokenCounter < 1) loopCounter++;
                    if(r == rows - 1) counter += loopCounter;
                }
                if(counter >= numOfPieces) return true;
            }
            else
            {
                brokenCounter++;
                counter = 0;
            }
//            cout << "Counter is: " << counter << "." << endl <<endl;
        }
        
        counter = 0;
        loopCounter = 0;
        brokenCounter = 0;
    }
    
    //Return if counter is greater or equal to the required number of pieces to win.
    return false;
}

bool checkComboTLBRDiagonally(char tag)
{
//    cout << endl << "TLBR DIAGONAL CHECK" << endl;
    
    int counter = 0;
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < cols; c++)
        {
            
            int cellNum = r * cols + c;
            char *cell = &grid[cellNum];
            
            if(*cell == tag)
            {
                counter++;
//                cout << "Current cell is: " << cellNum << " and it contains: " << *cell << "." <<endl;
                
                if(r + 1 < rows && (((c + 1 < cols) && !wrapMode) || wrapMode) )
                {
                    for(int dr = r + 1, dc = c + 1; ((dc < cols && !wrapMode) || wrapMode) && dr < rows; dr++, dc++)
                    {
                        int dCellNum = dr * cols + (dc % cols);
                        char *dCell = &grid[dCellNum];
                        
//                        cout << "Current dCell is: " << dCellNum << " and it contains: " << *dCell << "." <<endl;
                        if(*dCell == tag)
                        {
                            counter++;
                            if(counter >= numOfPieces) return true;
                        }
                        else
                        {
                            counter = 0;
                        }
//                        cout << "Counter is: " << counter << "." << endl <<endl;
                    }
                    
                    counter = 0;
                }
            }
            else
            {
                counter = 0;
            }
        }
        counter = 0;
    }
    return false;
}

bool checkComboTRBLDiagonally(char tag)
{
//    cout << endl << "TRBL DIAGONAL CHECK" << endl;
    int counter = 0;
    
    for(int r = 0; r < rows; r++)
    {
        for(int c = cols - 1; c >= 0; c--)
        {
            int cellNum = r * cols + c;
            char *cell = &grid[cellNum];
            
            if(*cell == tag)
            {
                counter++;
//                cout << "Current ROW: " << r << " COL: " << c << " CELL: " << cellNum << " and it contains: " << *cell << "." <<endl;
                
                if(r + 1 < rows && (((c - 1 >= 0) && !wrapMode) || wrapMode))
                {
                    for(int dr = r + 1, dc = c - 1;  (((dc >= 0) && !wrapMode) || wrapMode ) && dr < rows; dr++, dc--)
                    {
                        int dCellNum = dr * cols + ((cols + dc) % cols);
                        char *dCell = &grid[dCellNum];
                        
//                        cout << "Current DROW: " << dr << " DCOL: " << (dc % cols) << " DCELL: " << dCellNum << " and it contains: " << *dCell << "." <<endl;
                        if(*dCell == tag)
                        {
                            counter++;
                            if(counter >= numOfPieces) return true;
                        }
                        else
                        {
                            counter = 0;
                        }
//                        cout << "Counter is: " << counter << "." << endl <<endl;
                    }
                    counter = 0;
                }
            }
            else
            {
                counter = 0;
            }
            
        }
        counter = 0;
    }
    return false;
}

//REQUEST USER TO INPUT ANSWER.
int requestAnswer(int playerNum)
{
    int answer;
    cin >> answer;
    cin.clear();
    cin.ignore();
    
    //Validate answer.
    while (!cin || answer > cols || answer < 1)
    {
        cout << "Sorry Player " << playerNum << "! Invalid value, please enter a valid number for the column to drop your chip in:" << endl;
        //cout << "Sorry Player " << playerNum << ", but that column's about to burst! Please choose a different column to drop your chip in:" << endl;
        cin >> answer;
        cin.clear();
        cin.ignore();
    }
    
    return answer;
}

char requestPopMode()
{
    //REMOVE MODE
    char action;
    cout << "Do you want to [D]rop your chip or [P]op the chip at the bottom?" << endl;
    cin >> action;
    action = toupper(action);
    cin.clear();
    cin.ignore();
    
    while (!cin || (action != 'D' && action != 'P'))
    {
        cout << "Invalid value, please enter if you wish to [D]rop your chip or [P]op the chip at the bottom?" << endl;
        cin >> action;
        action = toupper(action);
        cin.clear();
        cin.ignore();
    }
    
    return action;
}

//START PLAYER TURN.
void playerTurn(int playerNum)
{
    char playerAction = 'D';
    
    if(!isFinished)
    {
        cout << "It's Player "<< playerNum << "'s turn! Choose the column:" << endl;
        
        //Get user's answer.
        int answer = requestAnswer(playerNum);
        
        //
        if(removeMode)
        {
            playerAction = requestPopMode();
        }
        //Adjust to zero based.
        updateGrid(answer - 1, playerNum, playerAction);
        printGrid();
    }
    else
    {
        endGame();
    }
}

//START GAME.
void startGame()
{
    //WELCOME MESSAGE
    cout << "Let the games begin!" << endl;
    
    int players[2] = { player1Id, player2Id };
    int rounds = 0;
    
    while(!isFinished)
    {
        playerTurn(players[rounds % 2]);
        rounds++;
    }
    
    endGame();
    
}
void testGame()
{
    cols = 7;
    rows = 7;
    numOfPieces = 4;
    wrapMode = true;
    removeMode = false;
    cpuMode = false;
    
    initializeGrid();
    
    //TLBR
    grid[0] = player1Char;
    grid[1] = player1Char;
    grid[2] = player1Char;
    grid[6] = player1Char;
    grid[12] = player1Char;
    grid[7] = player1Char;
    grid[14] = player1Char;
    grid[20] = player1Char;
    grid[26] = player1Char;
    
    //TRBL
    grid[5] = player2Char;
    grid[10] = player2Char;
    grid[15] = player2Char;
    
    //BRTL
    grid[59] = player1Char;
    grid[52] = player1Char;
    grid[45] = player1Char;
    
    //BLTR
    grid[44] = player2Char;
    grid[49] = player2Char;
    grid[54] = player2Char;
    
    printGrid();
    
    checkCombo(player1Id);
    checkCombo(player2Id);
}
//END GAME.
void endGame()
{
	if (gridIsFull)
	{
		cout << "Draw game!" << endl;
	}
	else
	{
		cout << "Player " << winnerId << " wins!!" << endl;
	}
    bool replay;
    char cReplay;
    cout << "Wanna play again? [Y/N]" << endl;
    cin >> cReplay;
    cReplay = toupper(cReplay);
    cin.clear();
    cin.ignore();
    
    while (!cin || (cReplay != 'Y' && cReplay != 'N'))
    {
        cout << "Invalid value, please enter if you wish replay: [Y/N]" << endl;
        cin >> cReplay;
        cReplay = toupper(cReplay);
        cin.clear();
        cin.ignore();
    }
    
    replay = cReplay == 'Y';
    
    if(replay)
    {
        isFinished = false;
        init();
    }
    
    
}
