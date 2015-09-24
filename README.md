#Card Games

Authors: Hunter Wills, Tung (Alex) Tran (t.t.tran@wustl.edu)

This program was made using Visual Studio 2013 with c++11. It was made to play various poker games from the command line.

#A few more notes on design:
(i) We implement collectAnte() and bet() for Game. They are used similarly in every poker game
(ii) Players will keep playing one type of poker game until there are not enough players to 
play that game. At that point, we will ask players whether they want to play other type of poker.
(iii) If everyone is folded except one player. We will proceed to the closure of the game, since
letting the one player betting in the next rounds will simply make her/him lose chips for no
reasons. 

#Test cases: 
Here we include 3 cases for three games. We tested each game for more than 
10 times, and we also tested each exceptions thrown in the project. 

--------------------------------------------------------------------------
*Test 1:
./CardGames FiveCardDraw hunter alex

The round is about to begin! Prepare to play some Five Card Draw!
Collecting Ante

Current Player: hunter
Your Hand: 3S 4S 10S JC KH total chips= 19 current bet: 0 to call: 0
Bet 1/2 Check 0?
1

Current Player: alex
Your Hand: 3H 4C 5S 9D 10C total chips= 19 current bet: 1 to call: 1
Raise 1/2 Call 0 Fold 7?
0

CURRENT PLAYER: alex
YOUR HAND:
3H 4C 5S 9D 10C
Which cards do you want to discard?
0 1 2 3 4

CURRENT PLAYER: hunter
YOUR HAND:
3S 4S 10S JC KH
Which cards do you want to discard?
0 1 2

Here is what each player has after drawing their brand new CARDS!
alex
5C 5H 7D 8S KS
hunter
9H JC JH KH AH

Current Player: hunter
Your Hand: 9H JC JH KH AH total chips= 18 current bet: 0 to call: 0
Bet 1/2 Check 0?
0

Current Player: alex
Your Hand: 5C 5H 7D 8S KS total chips= 18 current bet: 0 to call: 0
Bet 1/2 Check 0?
0

The round has come to a close here are the post round stats:
hunter 1 0 current chips: 22
9H JC JH KH AH
alex 0 1 current chips: 18
5C 5H 7D 8S KS
Anybody wanna call it quits?
alex
Anybody wanna call it quits?
no
Anybody wanna join?
no
Do you want to play another game? yes/no
yes
What do you want to play? FiveCardDraw/SevenCardStud/TexasHoldEm
SvenCardStud
CardGames: I have no idea what kind of game you are trying to play, but I know FiveCardDraw, TexasHoldEm, and SevenCardStud, so we should do that.
--------------------------------------------------------------------------

--------------------------------------------------------------------------
*Test 2:
./CardGames SevenCardStud hunter alex

The round is about to begin! Prepare to play some Seven Card Stud!
Collecting Ante

Your Hand (hunter): 3D KS  JH
alex's Hand: 9S  * *
total chips= 21 current bet: 0 to call: 0
Bet 1/2 Check 0?
2

hunter's Hand: JH  * *
Your Hand (alex): 3C 10S  9S
total chips= 17 current bet: 2 to call: 2
Raise 1/2 Call 0 Fold 7?
0

Your Hand (hunter): 3D KS  JH QD
alex's Hand: 2C 9S  * *
total chips= 19 current bet: 0 to call: 0
Bet 1/2 Check 0?
1

hunter's Hand: JH QD  * *
Your Hand (alex): 3C 10S  2C 9S
total chips= 15 current bet: 1 to call: 1
Raise 1/2 Call 0 Fold 7?
0

Your Hand (hunter): 3D KS  7H JH QD
alex's Hand: 2C 9H 9S  * *
total chips= 18 current bet: 0 to call: 0
Bet 1/2 Check 0?
0

hunter's Hand: 7H JH QD  * *
Your Hand (alex): 3C 10S  2C 9H 9S
total chips= 14 current bet: 0 to call: 0
Bet 1/2 Check 0?
0

Your Hand (hunter): 3D KS  7H 8C JH QD
alex's Hand: 2C 9H 9S AC  * *
total chips= 18 current bet: 0 to call: 0
Bet 1/2 Check 0?
0

hunter's Hand: 7H 8C JH QD  * *
Your Hand (alex): 3C 10S  2C 9H 9S AC
total chips= 14 current bet: 0 to call: 0
Bet 1/2 Check 0?
0

Your Hand (hunter): 3D 8S KS  7H 8C JH QD
alex's Hand: 2C 9H 9S AC  * * *
total chips= 18 current bet: 0 to call: 0
Bet 1/2 Check 0?
1

hunter's Hand: 7H 8C JH QD  * * *
Your Hand (alex): 3C 9C 10S  2C 9H 9S AC
total chips= 14 current bet: 1 to call: 1
Raise 1/2 Call 0 Fold 7?
0

Here is what each player had:
hunter's Hand: 3D 7H 8C 8S JH QD KS
alex's Hand: 2C 3C 9C 9H 9S 10S AC
The round has come to a close here are the post round stats:
alex 1 1 current chips: 23
9C 9H 9S 10S AC
hunter 1 1 current chips: 17
8C 8S JH QD KS
Anybody wanna call it quits?
alex
Anybody wanna call it quits?
hunter
Anybody wanna call it quits?
no
Anybody wanna join?
no
Do you want to play another game? yes/no
no
--------------------------------------------------------------------------

--------------------------------------------------------------------------
*Test 3:
./CardGames TexasHoldEm hunter alex

The round is about to begin! Prepare to play some Texas Hold 'Em!
Collecting Ante

Shared Cards Between All Players :
Your Hand (hunter): 2S QH
alex's Hand: * *
total chips= 16 current bet: 0 to call: 0
Bet 1/2 Check 0?
2

Shared Cards Between All Players :
hunter's Hand: * *
Your Hand (alex): 3D 10C
total chips= 22 current bet: 2 to call: 2
Raise 1/2 Call 0 Fold 7?
0

Shared Cards Between All Players : 2D 6D QD
Your Hand (hunter): 2S QH
alex's Hand: * *
total chips= 14 current bet: 0 to call: 0
Bet 1/2 Check 0?
1

Shared Cards Between All Players : 2D 6D QD
hunter's Hand: * *
Your Hand (alex): 3D 10C
total chips= 20 current bet: 1 to call: 1
Raise 1/2 Call 0 Fold 7?
2

Shared Cards Between All Players : 2D 6D QD
Your Hand (hunter): 2S QH
alex's Hand: * *
total chips= 13 current bet: 3 to call: 2
Raise 1/2 Call 0 Fold 7?
0

Shared Cards Between All Players : 2D 5D 6D QD
Your Hand (hunter): 2S QH
alex's Hand: * *
total chips= 11 current bet: 0 to call: 0
Bet 1/2 Check 0?
0

Shared Cards Between All Players : 2D 5D 6D QD
hunter's Hand: * *
Your Hand (alex): 3D 10C
total chips= 17 current bet: 0 to call: 0
Bet 1/2 Check 0?
0

Shared Cards Between All Players : 2D 4D 5D 6D QD
Your Hand (hunter): 2S QH
alex's Hand: * *
total chips= 11 current bet: 0 to call: 0
Bet 1/2 Check 0?
0

Shared Cards Between All Players : 2D 4D 5D 6D QD
hunter's Hand: * *
Your Hand (alex): 3D 10C
total chips= 17 current bet: 0 to call: 0
Bet 1/2 Check 0?
0

Shared Cards Between All Players : 2D 4D 5D 6D 9S QD
Your Hand (hunter): 2S QH
alex's Hand: * *
total chips= 11 current bet: 0 to call: 0
Bet 1/2 Check 0?
0

Shared Cards Between All Players : 2D 4D 5D 6D 9S QD
hunter's Hand: * *
Your Hand (alex): 3D 10C
total chips= 17 current bet: 0 to call: 0
Bet 1/2 Check 0?
0

Here is what each player had:
hunter's Down Cards: 2S QH
alex's Down Cards: 3D 10C
And here are the shared cards: 2D 4D 5D 6D 9S QD
The round has come to a close here are the post round stats:
hunter 2 1 current chips: 23
4D 5D 6D 9S QD QH
alex 2 1 current chips: 29
4D 5D 6D 9S 10C QD
Anybody wanna call it quits?
alex
Anybody wanna call it quits?
hunter
Anybody wanna call it quits?
no
Anybody wanna join?
no
Do you want to play another game? yes/no
no
