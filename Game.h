//
// Game.h
// Authors: Hunter Wills
//
//
//#include "stdafx.h"

#ifndef _GAME_H_
#define _GAME_H_
#include "Deck.h"
#include "Card.h"
#include "Hand.h"
#include "Player.h"
#include <memory>

class Game {
protected:
    static shared_ptr<Game> game;
    Deck main_deck;
    vector<shared_ptr<Player>> players;
	unsigned int totalPot;
public:
    static shared_ptr<Game> instance();// return game throw exception if nullptr
    static void start_game(const string);// if game is not nullptr throw exception also throw exception if unknown game else create game
    static void stop_game(); //throw exception if game is nullptr else make ptr nullptr
    void add_player(const string, bool isAI);//throw exception if player name is in use otherwise add new player
    shared_ptr<Player> find_player(const string);//nullptr if player not in game
	void remove_player(const string name);
	bool allButOneFolded();
	void virtual printPlayerCards(string name);
	int numPlayers();
    //virtual ~Game();
	//===========================
	void bet();
	void resetChips();
	void collectAnte();
    virtual int before_turn(Player &);
    virtual int turn(Player &);
    virtual int after_turn(Player &);
    virtual int before_round();
    virtual int round();
    virtual int after_round();
};

#endif
