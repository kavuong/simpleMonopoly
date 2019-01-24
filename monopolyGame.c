//
// Created by kevin on 1/15/2018.
//
// bankrupt
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "monopolyGame.h"
bool isGameOver(GameState * gameState){
  if (gameState->numPlayers <= gameState->rules.playersLeftToEnd || (gameState->rules.turnLimit != -1 && gameState->numTurns >= gameState->rules.turnLimit))
    return true;
  else return false;
}
void setUpPlayers(GameState * gameState, int numPlayers){
  gameState->playerArr = (Player *) malloc(numPlayers * sizeof(Player));

  for (int i = 0; i < numPlayers; ++i){
    (gameState->playerArr)[i].cash = (gameState->rules).startingCash;
    (gameState->playerArr)[i].position = 0;
    (gameState->playerArr)[i].inGame = true;
  }
  gameState->currPlayer = 0;
  gameState->numPlayers = numPlayers;
  gameState->numOrigPlayers = numPlayers;

}
void printGameState(GameState * gameState){
  for (int i = 0; i < gameState->numBoardSpaces; ++i){
    printf("%d | %s  |", i, (gameState->boardArr[i]).name);
    for (int j = 0; j < gameState->numOrigPlayers; ++j){
      if (gameState->playerArr[j].position == i && gameState->playerArr[j].inGame)
        printf(" %d", j);
    }
    printf("\n");
  }
}

bool setMultiplier(GameState * gameState, int setId){
  bool setMultiplierApplies = true;
  int spaceSet[20];
  int spaceSetCounter = 0;
  for (int i = 1; i < gameState->numBoardSpaces; ++i){
    if (gameState->boardArr[i].setId == setId) {
      spaceSet[spaceSetCounter] = i;
      spaceSetCounter++;
    }
  }
  int dummyOwner = gameState->boardArr[spaceSet[0]].owner;
  for (int j = 1; j < spaceSetCounter; ++j){
    if (gameState->boardArr[spaceSet[j]].owner == dummyOwner)
      continue;
    else {
      setMultiplierApplies = false;
      break;
    }
  }
  return setMultiplierApplies;

}
int calcTotAssets(GameState * gameState, int player){
  int propertyAssets = 0;
  for (int i = 1; i < gameState->numBoardSpaces; ++i){
    if (gameState->boardArr[i].owner == player)
      propertyAssets += gameState->boardArr[i].propertyCost;
  }
  return gameState->playerArr[player].cash + propertyAssets;
}
void whoWon(GameState * gameState){
  printGameState(gameState);
  int playersLeft = 0;
  int playersLeftArr[gameState->numPlayers];
  int numWinners = 0;
  int winners[gameState->numPlayers];
  int maxNetWorth = 0;
  for (int i = 0; i < gameState->numOrigPlayers; ++i){
    if (gameState->playerArr[i].inGame) {
      playersLeftArr[playersLeft] = i;
      playersLeft++;
    }
  }
  if (gameState->numPlayers == 1){
    winners[0] = playersLeftArr[0];
    numWinners = 1;
  }
  else{
    maxNetWorth = calcTotAssets(gameState, playersLeftArr[0]);
    for (int j = 1; j < playersLeft; ++j){
      if (calcTotAssets(gameState, playersLeftArr[j]) > maxNetWorth)
        maxNetWorth = gameState->playerArr[playersLeftArr[j]].cash;
    }
    for (int k = 0; k < playersLeft; ++k){
      if (calcTotAssets(gameState, playersLeftArr[k]) == maxNetWorth){
        winners[numWinners] = playersLeftArr[k];
        numWinners++;
      }
    }
  }

  printf("The winners are\n");
  for (int j = 0; j < numWinners; ++j) {
    printf("Player %d\n", winners[j]);
  }

  exit(0);
}
void playGame(GameState * gameState, char randomFileName[]){
  int numPlayers = 0;
  printf("How many players will be playing: ");
  scanf(" %d", &numPlayers);
  setUpPlayers(gameState, numPlayers);
  FILE * randomFile = fopen(randomFileName, "r");
  bool diceRolled = false;
  while (!isGameOver(gameState)){
    int playerInput = 0;
    char charPlayerInput = ' ';
    char inputStreamArray[100];

    printGameState(gameState);
    while (true) {
      printf("Player %d please choose an action\n", gameState->currPlayer);
      if (diceRolled)
        printf("1: End Turn\n");
      else
        printf("1: Roll Dice\n");
      printf("2: Inspect Player\n");
      printf("3: Leave Game\n");
      printf("Your action: ");
      // extra newline after inputting players
      scanf("\n");
      fgets(inputStreamArray, 100, stdin);
      int numArgsRead = sscanf(inputStreamArray, " %d", &playerInput);
      if (numArgsRead != 1 || playerInput <= 0 || playerInput >= 4){
        printf("Please input a valid move.\n");
        continue;
      }
      else
        break;
    }
    // roll dice
    if (playerInput == 1){
      if (!diceRolled) {
        // DOESN'T INCLUDE LANDING ON GO
        int goCount = 0;
        // taking input from randomNumFile, converting to

        int rand1;
        int rand2;
        fscanf(randomFile, " %d", &rand1);
        fscanf(randomFile, " %d", &rand2);
        int roll = rand1 % 6 + rand2 % 6 + 2;
        int *playerPosition = &gameState->playerArr[gameState->currPlayer].position;
        int *playerCash = &gameState->playerArr[gameState->currPlayer].cash;
        int spacesToGo = gameState->numBoardSpaces - *playerPosition;

        // if roll is normal and player does not pass or land on go
        if (roll < spacesToGo)
          *playerPosition += roll;
          // land on go
        else if (roll == spacesToGo) {
          *playerPosition = 0;
        } else {
          goCount = roll / gameState->numBoardSpaces;
          *playerPosition = (*playerPosition + roll) % gameState->numBoardSpaces;
          if (*playerPosition == 0)
            goCount--;
        }
        printf("You rolled a %d!\n", roll);
        diceRolled = true;
        BoardSpace *currentBoardSpace = &(gameState->boardArr[*playerPosition]);

        // adding salary based on how many times GO is PASSED (NOT landed on)
        *playerCash += goCount * gameState->boardArr[0].goCash;
        // handling roll based on what their position is
        // handling LANDING ON GO (as opposed to passing GO)
        if (*playerPosition == 0)
          *playerCash += gameState->boardArr[0].goCash * gameState->rules.goLandingMultiplier;
          // property is unowned
        else if (gameState->boardArr[*playerPosition].owner == -1) {
          if (*playerCash >= (*currentBoardSpace).propertyCost) {
            printf("Player %d would you like to buy %s for $%d?\n",
                   gameState->currPlayer,
                   (*currentBoardSpace).name,
                   (*currentBoardSpace).propertyCost);
            printf("The rent on this property is $%d\n", (*currentBoardSpace).rent);
            printf("You currently have $%d\n", *playerCash);
            printf("Y for yes, N for no:\n");

            scanf("\n");
            fgets(inputStreamArray, 100, stdin);
            int numArgsRead = sscanf(inputStreamArray, " %c", &charPlayerInput);
            charPlayerInput = tolower(charPlayerInput);
            if (numArgsRead != 1 || (charPlayerInput != 'y' && charPlayerInput != 'n')) {
              printf("Input error\n");
              continue;
            }
            else if (tolower(charPlayerInput) == 'y'){
              (*currentBoardSpace).owner = gameState->currPlayer;
              *playerCash -= (*currentBoardSpace).propertyCost;
            }
            else if (tolower(charPlayerInput) == 'n')
              continue;
          }
          else{
            printf("Player %d you do not have enough money to purchase %s\n", gameState->currPlayer, (*currentBoardSpace).name);
            printf("%s costs $%d, but you only have $%d\n", (*currentBoardSpace).name, (*currentBoardSpace).propertyCost, *playerCash);
            continue;
          }
        }
          // landing on someone else's property
        else if (gameState->boardArr[*playerPosition].owner != gameState->currPlayer){
          bool doesMultiplierExist = setMultiplier(gameState, gameState->boardArr[*playerPosition].setId);
          int paymentAmount = 0;
          if (doesMultiplierExist)
            paymentAmount = (*currentBoardSpace).rent * gameState->rules.propertyMultiplier;
          else
            paymentAmount = (*currentBoardSpace).rent;
          // bankrupt case
          if (paymentAmount > *playerCash){
            gameState->playerArr[gameState->boardArr[*playerPosition].owner].cash += *playerCash;
            *playerCash = 0;
            for (int i = 1; i < gameState->numBoardSpaces; ++i){
              if (gameState->boardArr[i].owner == gameState->currPlayer)
                gameState->boardArr[i].owner = gameState->boardArr[*playerPosition].owner;
            }
            gameState->playerArr[gameState->currPlayer].inGame = false;

            printf("Player %d went bankrupt to Player %d", gameState->currPlayer, gameState->boardArr[*playerPosition].owner);
            gameState->numPlayers--;
            diceRolled = false;
            bool isValidPlayer = false;
            while (!isValidPlayer) {
              if (gameState->currPlayer == gameState->numOrigPlayers - 1)
                gameState->currPlayer = 0;
              else
                gameState->currPlayer++;
              if (gameState->playerArr[gameState->currPlayer].inGame)
                isValidPlayer = true;
            }
            gameState->numTurns++;
            continue;
          }
          else {
            *playerCash -= paymentAmount;
            gameState->playerArr[gameState->boardArr[*playerPosition].owner].cash += paymentAmount;

            printf("Player %d payed Player %d $%d for landing on %s\n",
                   gameState->currPlayer,
                   gameState->boardArr[*playerPosition].owner,
                   paymentAmount,
                   (*currentBoardSpace).name);
          }
          continue;
        }
        else if (gameState->boardArr[*playerPosition].owner == gameState->currPlayer)
          continue;
      }
        // if dice is rolled and "1" is inputted, then program will end turn
        // this is the code for end turn
      else{
        diceRolled = false;
        bool isValidPlayer = false;
        while (!isValidPlayer) {
          if (gameState->currPlayer == gameState->numOrigPlayers - 1)
            gameState->currPlayer = 0;
          else
            gameState->currPlayer++;
          if (gameState->playerArr[gameState->currPlayer].inGame)
            isValidPlayer = true;
        }
        gameState->numTurns++;
        continue;
      }

    }
    else if (playerInput == 2){
      bool doesPlayerExist = false;
      printf("Which player would you like to inspect?\n");
      for (int i = 0; i < gameState->numOrigPlayers; ++i){
        if (gameState->playerArr[i].inGame)
          printf("%d for Player %d\n", i, i);
      }
      fgets(inputStreamArray, 100, stdin);
      int numArgsRead = sscanf(inputStreamArray, " %d", &playerInput);
      if (playerInput >= 0 && playerInput < gameState->numOrigPlayers) {
        if (gameState->playerArr[playerInput].inGame) {
          doesPlayerExist = true;
        }
      }

      if (numArgsRead != 1 || !doesPlayerExist) {
        printf("Error occurred, couldn't locate specified player.\n");
      }
      else{
        printf("Your choice: Player %d\n", playerInput);
        printf("\tCash: $%d\n", gameState->playerArr[playerInput].cash);
        printf("\tProperties owned\n");
        for (int j = 0; j < gameState->numBoardSets; ++j){
          printf("\t\t%d: ", j);
          int ownedSpaces = 0;
          // not checking GO space
          for (int k = 1; k < gameState->numBoardSpaces; ++k){
            // checks if owner is indeed the player and that the set is in the one we are printing
            if (gameState->boardArr[k].owner == playerInput && gameState->boardArr[k].setId == j) {
              if (ownedSpaces == 0)
                printf("%s ", gameState->boardArr[k].name);
              else {
                printf("| %s ", gameState->boardArr[k].name);
              }
              ownedSpaces++;
            }
          }
          printf("\n");
        }
      }
      continue;
    }
    else if (playerInput == 3){
      gameState->numPlayers--;
      gameState->playerArr[gameState->currPlayer].inGame = false;
      for (int k = 1; k < gameState->numBoardSpaces; ++k){
        if (gameState->boardArr[k].owner == gameState->currPlayer)
          gameState->boardArr[k].owner = -1;
      }
      gameState->playerArr[gameState->currPlayer].cash = 0;
      diceRolled = false;
      bool isValidPlayer = false;
      while (!isValidPlayer) {
        if (gameState->currPlayer == gameState->numOrigPlayers - 1)
          gameState->currPlayer = 0;
        else
          gameState->currPlayer++;
        if (gameState->playerArr[gameState->currPlayer].inGame)
          isValidPlayer = true;
      }
      gameState->numTurns++;
    }
  }
  whoWon(gameState);

}