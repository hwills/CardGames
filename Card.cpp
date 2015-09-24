//Card.cpp : Contains all card related functions.
//Authors : Hunter Wills (hwills@wustl.edu)

//#include "stdafx.h"
#include "Card.h"

using namespace std;


Card::Card() :cardRank(TWO), cardSuit(CLUBS) {}
Card::Card(int r, int s) : cardRank((Card::rank)r), cardSuit((Card::suit)s) {}

int parse_cards(vector<Card> & cards, char * file_name)
{

	ifstream ifs;
	ifs.open(file_name);

	if (ifs.is_open())
	{

		while (!ifs.eof())	//continue reading until we reach the end of the file
		{

			vector<Card> tmpHand;	//holds cards temporarally to make sure they perfectly fill a hand on each line (exactly five cards)
			string line, word;
			getline(ifs, line);		//read off each line of the file
			istringstream iss(line);

			while (iss >> word)		//read off each word to parse card information from it
			{

				Card card;
				const int word_first_index = 0;
				const int word_second_index = 1;
				const int word_last_index = word.size() - 1;
				const int normal_card_size = 2;
				const int card_size_if_ten = 3;
				const int beginCommentLength = 2;
				const char commentCharacter = '/';
				char suitChar = word[word_last_index];
				bool valid = true;
				

				if (word.size() >= beginCommentLength && word[word_first_index] == commentCharacter && word[word_second_index] == commentCharacter)		//if we hit a comment, we want to stop reading cards from this line
				{
					break;
				}

				if (word.size() == normal_card_size || word.size() == card_size_if_ten)		//a normal card will have 2 characters unless it has a 10 in which case it will have 3 characters
				{

					if (suitChar == 'C' || suitChar == 'c')
					{
						card.cardSuit = Card::suit::CLUBS;
					}

					else if (suitChar == 'D' || suitChar == 'd')
					{
						card.cardSuit = Card::suit::DIAMONDS;
					}

					else if (suitChar == 'H' || suitChar == 'h')
					{
						card.cardSuit = Card::suit::HEARTS;
					}

					else if (suitChar == 'S' || suitChar == 's')
					{
						card.cardSuit = Card::suit::SPADES;
					}

					else
					{
						valid = false;
					}

					if (word.size() == card_size_if_ten && word[word_first_index] == '1' && word[word_second_index] == '0')		//special check if a card has rank 10 since this is the only rank with two characters
					{
						card.cardRank = Card::rank::TEN;
					}
					else
					{

						switch (word[word_first_index])
						{
						case 'A' :
						case 'a' :
							card.cardRank = Card::rank::ACE;
							break;
						case 'K' :
						case 'k' :
							card.cardRank = Card::rank::KING;
							break;
						case 'Q':
						case 'q':
							card.cardRank = Card::rank::QUEEN;
							break;
						case 'J':
						case 'j':
							card.cardRank = Card::rank::JACK;
							break;
						case '9':
							card.cardRank = Card::rank::NINE;
							break;
						case '8':
							card.cardRank = Card::rank::EIGHT;
							break;
						case '7':
							card.cardRank = Card::rank::SEVEN;
							break;
						case '6':
							card.cardRank = Card::rank::SIX;
							break;
						case '5':
							card.cardRank = Card::rank::FIVE;
							break;
						case '4':
							card.cardRank = Card::rank::FOUR;
							break;
						case '3':
							card.cardRank = Card::rank::THREE;
							break;
						case '2':
							card.cardRank = Card::rank::TWO;
							break;
						default :
							valid = false;	//a card that does not fall into any above catagory has an invalid rank
						}
					}

					if (valid)	//only add valid cards
					{
						tmpHand.push_back(card);
					}
				}
			}

		

				for (Card card : tmpHand)	//go through cards from the line adding each to overall vector
				{
					cards.push_back(card);
				}
		}
	}
	else
	{
		cout << "Error opening file!" << endl;
		return errors::fileOpenError;
	}

	

	ifs.close();//close the filestream when done
	return errors::noError;
}

int print_cards(const vector<Card> & cards)
{

	for (Card card : cards)
	{

		string cardString;
		const int firstIndex = 0;

		if (card.cardRank < firstIndex || card.cardRank >= numRanks || card.cardSuit < firstIndex || card.cardSuit >= numSuits)		//all cards that correspond to indexes of the card suit and rank arrays will be valid all others are errors
		{	
			cout << "Error reading cards from vector!" << endl;
			return errors::cardReadError;
		}

		cardString = rankStrings[card.cardRank] + suitStrings[card.cardSuit];
		cout << cardString << endl;
	}

	return errors::noError;
}

void usage_message(string program_name, string message)
{

	cout << program_name <<": "<< message << endl;
}

bool Card::operator< (const Card & crd) const
{
	return (cardRank < crd.cardRank) || ((cardRank == crd.cardRank) && (cardSuit < crd.cardSuit));		//sorts first by rank and then by suit
}

bool Card::operator== (const Card & crd) const
{
	return (cardRank == crd.cardRank) && (cardSuit == crd.cardSuit);		//equality
}

int print_poker_rank(vector<Card> & cards)
{

	const int handSize = 5;		//size of a valid hand
	const int quad = 4;			//number of cards in a four of a kind
	const int triplet = 3;		//number of cards in a three of a kind
	const int pair = 2;			//number of cards in a two of a kind

	vector<Card> crntHand;	//the current five cards we are detemining the rank of

	int itr = 0;
	while (itr + handSize <= cards.size())	//go through each hand while we still have a hand's worth of cards to check
	{

		int i = 0;
		while (i < handSize)	//built a hand out of the next cards
		{

			crntHand.push_back(cards[itr+i]);	//itr+i to get the cards after the current start point
			++i;

		}

		sort(crntHand.begin(), crntHand.end());		//sorts the crntHand according to the less than operator

		//checks the overall hand for a flush or a straight
		bool flush = crntHand[0].cardSuit == crntHand[1].cardSuit && crntHand[0].cardSuit == crntHand[2].cardSuit && crntHand[0].cardSuit == crntHand[3].cardSuit && crntHand[0].cardSuit == crntHand[4].cardSuit;
		bool straight = (crntHand[0].cardRank + 1) == crntHand[1].cardRank && (crntHand[1].cardRank + 1) == crntHand[2].cardRank && (crntHand[2].cardRank + 1) == crntHand[3].cardRank && (crntHand[3].cardRank + 1) == crntHand[4].cardRank;

		//checks for any multiples of a rank
		int maxCount = 0;		//maximum number of cards of the same rank in hand
		int secondCount = 0;	//second best amount of cards of the same rank (for full house and two pairs)
		int j;
		for (j = 0; j < handSize; j++)		//go through all cards in hand beginning with first one
		{

			Card cardOne = crntHand[j];		//store card currently examined
			int crntCount = 1;				//there are one of that rank so far

			int k;
			for (k = j + 1; k < handSize; ++k)	//go through rest of cards finding the rest of that rank
			{

				Card cardTwo = crntHand[k];	

				if (cardOne.cardRank == cardTwo.cardRank)	//check if ranks are the same
				{

					++j;		//increment j so we dont double count the matching card
					++crntCount;	//increment count of the rank seen

				}
				else		//since cards are sorted, we know we will not hit any more of that rank once we find the first that is not of that rank so we can break
				{

					break;

				}
			}

			if (crntCount > maxCount)		//if we found a new maximum set it as so
			{
				
				secondCount = maxCount;
				maxCount = crntCount;

			}
			else if (crntCount > secondCount)		//if we found a new second most cards set it as so
			{

				secondCount = crntCount;

			}
		}

		//go through all hand possibilities starting with the best down to the worst and declare which we match
		if (flush && straight)
		{

			cout << "straight flush" << endl;

		}
		else if (maxCount == quad)
		{

			cout << "four of a kind" << endl;

		}
		else if (maxCount == triplet && secondCount == pair)
		{

			cout << "full house" << endl;

		}
		else if (flush)
		{

			cout << "flush" << endl;

		}
		else if (straight)
		{

			cout << "straight" << endl;

		}
		else if (maxCount == triplet)
		{

			cout << "three of a kind" << endl;

		}
		else if (maxCount == pair && secondCount == pair)
		{

			cout << "two pairs" << endl;

		}
		else if (maxCount == pair)
		{

			cout << "one pair" << endl;

		}
		else
		{

			cout << "no rank" << endl;

		}

		crntHand.clear();		//remove the cards we were examining so we can examine the next hand
		itr += handSize;				//incerment the hand start we are on by a whole hand since we have just examined that hand.
	}
	return 0;		//no error
}

Card & Card::operator= (const Card & other)
{
	cardRank = other.cardRank;
	cardSuit = other.cardSuit;
	return *this;
}

int rank_count(const Card * begin, const Card * end, const Card card)
{
	int rank = card.cardRank;
	int count = 0;
	while (begin != end)
	{
		if ((*begin).cardRank == rank)
		{
			++count;
		}
		++begin;
	}
	return count;
}
