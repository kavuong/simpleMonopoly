#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "structs.h"
#include "monopolyGame.h"
bool makeLowerCase(char inputStr[]){
  for (int i = 0; i < strlen(inputStr); ++i)
    inputStr[i] = tolower(inputStr[i]);
  if (strcmp(inputStr, "yes") == 0)
    return true;
  else
    return false;
}

GameState setup(char ruleFile[], char boardFile[]){
  GameState gameState;
  int startingCash = 0;
  int turnLimit = 0;
  int playersToEnd = 0;
  int setMultiplier = 0;
  int housesBeforeHotels = 0;
  // true -> yes false -> no
  char buildHousesInput[4];
  char freeParkingInput[4];
  char auctionInput[4];
  bool buildHousesEvenly = NULL;
  bool moneyInFreeParking = NULL;
  bool auctionProp = NULL;
  int goLandingMultiplier = 0;

  char * token = NULL;
  int numBoardSpaces = 0;
  int spaceIndex = 1;
  int goCash = 0;
  char goNameInput[30];
  int setId = 0;
  int maxSetId = 0;
  int intraId = 0;
  char * propName = malloc(50 * sizeof(char));

  char * boardLine = malloc(1000 * sizeof(char));
  BoardSpace goSpace;
  goSpace.name = malloc(100 * sizeof(char));
  Rules gameRules;


  FILE * rules = fopen(ruleFile, "r");
  fscanf(rules, "Starting Cash: %d", &startingCash);
  fscanf(rules, " Turn Limit (-1 for no turn limit): %d", &turnLimit);
  fscanf(rules, " Number of Players Left To End Game: %d", &playersToEnd);
  fscanf(rules, " Property Set Multiplier: %d", &setMultiplier);
  fscanf(rules, " Number of Houses Before Hotels: %d", &housesBeforeHotels);
  fscanf(rules, " Must Build Houses Evenly: %s", buildHousesInput);
  fscanf(rules, " Put Money In Free Parking: %s", freeParkingInput);
  fscanf(rules, " Auction Properties: %s", auctionInput);
  fscanf(rules, " Salary Multiplier For Landing On Go: %d", &goLandingMultiplier);
  buildHousesEvenly = makeLowerCase(buildHousesInput);
  moneyInFreeParking = makeLowerCase(freeParkingInput);
  auctionProp = makeLowerCase(auctionInput);

  gameRules.startingCash = startingCash;
  gameRules.turnLimit = turnLimit;
  gameRules.playersLeftToEnd = playersToEnd;
  gameRules.propertyMultiplier = setMultiplier;
  gameRules.housesBeforeHotels = housesBeforeHotels;
  gameRules.buildHousesEvenly = buildHousesEvenly;
  gameRules.moneyInFreeParking = moneyInFreeParking;
  gameRules.auctionProp = auctionProp;
  gameRules.goLandingMultiplier = goLandingMultiplier;

  FILE * board = fopen(boardFile, "r");
  fscanf(board, "Number of Spaces,%d,,,,,,,,", &numBoardSpaces);
  fscanf(board, " ,,,,,,,,, ,,,,,,,,,");
  fscanf(board, " Type,Set Id,Intraset Id,Name,Property Cost,House Cost,Hotel Cost,Rent,Rent with House,Rent With Hotel");
  fscanf(board, " Go,%d%s", &goCash, goNameInput);
  token = strtok(goNameInput, ",");
  goSpace.type = false;
  goSpace.goCash = goCash;
  strcpy(goSpace.name, token);

  BoardSpace * boardSpaceArray = (BoardSpace *) malloc(numBoardSpaces * sizeof(BoardSpace));
  boardSpaceArray[0] = goSpace;
  // extra characters in input file
  fscanf(board, "\r\n");


  while (!feof(board)){
    if (!feof(board) && fgets(boardLine, 1000, board) != NULL) {
      char * lineInput = malloc(1000 * sizeof(char));

      BoardSpace boardSpace;
      boardSpace.name = malloc(100 * sizeof(char));
      /*sscanf(boardLine,
             "Property,%d,%d%[^\n]s,%d,%d,%d,%d,%d,%d",
             &setId,
             &intraId,
             propNameInput,
             &propCost,
             &houseCost,
             &hotelCost,
             &rent,
             &rentWithHouse,
             &rentWithHotel);
             */
      sscanf(boardLine, " Property,%d,%d%[^\n]s", &setId, &intraId, lineInput);
      boardSpace.type = true;
      boardSpace.setId = setId;
      if (setId > maxSetId)
        maxSetId = setId;
      boardSpace.intraId = intraId;
      token = strtok(lineInput, ",");
      strcpy(boardSpace.name, token);
      token = strtok(NULL, ",");
      boardSpace.propertyCost = atoi(token);
      token = strtok(NULL, ",");
      boardSpace.houseCost = atoi(token);
      token = strtok(NULL, ",");
      boardSpace.hotelCost = atoi(token);
      token = strtok(NULL, ",");
      boardSpace.rent = atoi(token);
      token = strtok(NULL, ",");
      boardSpace.houseRent = atoi(token);
      token = strtok(NULL, ",");
      boardSpace.hotelRent = atoi(token);

      boardSpace.owner = -1;

      boardSpaceArray[spaceIndex] = boardSpace;
      spaceIndex++;
      free(lineInput);
    }
    else break;
  }
  // set IDs start with 0; basically equal to maximum set ID + 1
  gameState.numBoardSets = maxSetId + 1;
  gameState.boardArr = boardSpaceArray;
  gameState.rules = gameRules;
  gameState.numTurns = 0;
  gameState.numBoardSpaces = numBoardSpaces;


  free(propName);
  free(boardLine);

  return gameState;
}


int main(int argc, char ** argv) {
  GameState gameState = setup(argv[1], argv[2]);
  playGame(&gameState, argv[3]);

}