//
// Created by kevin on 1/14/2018.
//
#include <stdbool.h>
#ifndef HOARDING_STRUCTS_H
#define HOARDING_STRUCTS_H
typedef struct BOARDSPACE_STRUCT {
  // GO -> false, property -> true
  bool type;
  int setId;
  int intraId;
  char * name;
  int propertyCost;
  int houseCost;
  int hotelCost;
  int rent;
  int houseRent;
  int hotelRent;
  // -1 -> bank, else simply player ID
  int owner;


  int goCash;


} BoardSpace;

typedef struct PLAYER_STRUCT {
  int cash;
  int position;
  bool inGame;
} Player;
typedef struct RULES_STRUCT {
  int startingCash;
  int turnLimit;
  int playersLeftToEnd;
  int propertyMultiplier;
  int housesBeforeHotels;
  // true -> yes false -> no
  bool buildHousesEvenly;
  bool moneyInFreeParking;
  bool auctionProp;
  int goLandingMultiplier;
} Rules;
typedef struct GAMESTATE_STRUCT {
  BoardSpace * boardArr;
  Player * playerArr;
  Rules rules;
  int numPlayers;
  int numTurns;
  int numBoardSpaces;
  // for keeping track of whose turn it is
  int currPlayer;

  int numBoardSets;

  int numOrigPlayers;

} GameState;
#endif //HOARDING_STRUCTS_H
