//
// Created by kevin on 1/15/2018.
//
#include "structs.h"
#ifndef HOARDING_MONOPOLYGAME_H
#define HOARDING_MONOPOLYGAME_H
bool isGameOver(GameState * gameState);
void setUpPlayers(GameState * gameState, int numPlayers);
void printGameState(GameState * gameState);
bool setMultiplier(GameState * gameState, int setId);
int calcTotAssets(GameState * gameState, int player);
void whoWon(GameState * gameState);
void playGame(GameState * gameState, char randFileName[]);
#endif //HOARDING_MONOPOLYGAME_H
