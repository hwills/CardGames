//#include "stdafx.h"

#include "Deck.h"

using namespace std;

//constructor for deck
Deck::Deck(char * filename)
{

	//load a file being aware of what errors it may have
	int success = load(filename);

	if (success == errors::fileOpenError)
	{

		//tell them we have an error opening the file and throw the error into the air!
		cout << "FILE OPEN ERROR!!!" << endl;
		throw errors::fileOpenError;

	}

}

Deck::Deck()
{
    //initialize empty deck (nothing in the cards vector)
}

void Deck::addCard(Card c)
{
    cards.push_back(c);
}

//load a file of cards into the the decks cards object
int Deck::load(char * filename)
{
	return parse_cards(cards, filename);
}

//randomize the order of cards in a deck
int Deck::shuffle()
{

	//create random number generator
	std::random_device rd;	
	std::mt19937 g(rd());
	
	//suffle the cards using that rng
	std::shuffle(cards.begin(), cards.end(), g);

	return noError;

}

//get the number of cards in the deck
int Deck::size() const
{

	return cards.end() - cards.begin();

}

//print out all of the carsd in the deck using the << operator to an ostream
ostream & operator<< (ostream & out, const Deck & d)
{

	//iterate through all the cards
	auto it = d.cards.begin();

	while (it < d.cards.end())
	{

		Card card = *it;		//get the card at this position
		string cardString = rankStrings[card.cardRank] + suitStrings[card.cardSuit];	//get the card's info
		out << cardString << endl;	//print the card to ostream
		++it;

	}

	return out;

}

//hand default constructor
Hand::Hand()
{

	//this does not have to do anything since we are only using a vector as a member variable

}

//hand copy constructor
Hand::Hand(const Hand & other)
{

	//iterate through all the other cards adding those cards to this hand object
	auto it = other.cards.begin();
	while (it < other.cards.end())
	{

		cards.push_back(*it);
		++it;

	}

}

//hand assignment operator
Hand & Hand::operator= (const Hand & other)
{

	//make sure we arent referring to the same hand object
	if (!(*this == other))
	{

		//clear out the cards we currently hold 
		cards.clear();

		//add all of the other cards to this hand object
		auto it = other.cards.begin();
		while (it < other.cards.end())
		{

			cards.push_back(*it);
			++it;

		}

	}

	return *this;

}

//get the number of cards in the deck
int Hand::size() const
{

	return cards.end() - cards.begin();

}

//hand equality comparator
bool Hand::operator== (const Hand & other) const
{

	//if the hands aren't the same size, don't bother doing the rest of the checks
	if (size() != other.size())
	{

		return false;

	}

	//go through all cards in both hands
	auto it1 = cards.begin();
	auto it2 = other.cards.begin();

	//only need to check one iterator since both are incrementing across the same regions
	while (it1 < cards.end())
	{

		//check that the cards at this position are equal
		if ((*it1).cardRank == (*it2).cardRank && (*it1).cardSuit == (*it2).cardSuit)
		{

			++it1;
			++it2;

		}
		//if cards are not equal then the hands are not equal
		else
		{

			return false;

		}
	}

	//if all cards were equal, then the hands were equal
	return true;

}

//hand less than comparator (sorts by lexigraphical order with lowest card then next lowest, and so on)
bool Hand::operator< (const Hand & other) const
{

	//iterate through all cards checking for order
	auto it1 = cards.begin();
	auto it2 = other.cards.begin();

	while (it1 < cards.end() && it2 < other.cards.end())
	{

		//if cards are equal, then move on to next card
		if ((*it1).cardRank == (*it2).cardRank && (*it1).cardSuit == (*it2).cardSuit)
		{

			++it1;
			++it2;

		}
		//if the first card is less than the second, then the first hand is less than the second
		else if (*it1 < *it2)
		{

			return true;
		
		}
		//if the second card is less than the first, then the second hand is less than the first
		else
		{

			return false;
		
		}

	}

	//if the size of the first hand is greater than (or equal to) that of the second then it comes second
	if (size() >= other.size())
	{

		return false;

	}

	//if all cards are equal and the first hand is smaller than the second then it comes first
	return true;

}

Card & Hand::operator[] (const size_t index)
{
    if(index < size())
    {
        return *(cards.begin() + index);
    }
    else
    {
        throw errors::index_out_of_bounds;
    }
}

void Hand::remove_card (const size_t index)
{
    if(index < size())
    {
        auto it = cards.begin()+index;
        cards.erase(it);
    }
    else
    {
        throw errors::index_out_of_bounds;
    }
}

//turn a hand into a c++ style string object
string Hand::asString() const
{

	string toRet;

	//iteratre through all cards adding them to the string object
	auto it = cards.begin();
	while (it < cards.end())
	{

		Card card = *it;
		string cardString = rankStrings[card.cardRank] + suitStrings[card.cardSuit];
		toRet += cardString + " ";		//put spaces betwen cards
		++it;

	}

	return toRet;

}

//print our a hand to the ostream
ostream & operator<< (ostream & out, const Hand & h)
{

	//iterate through all the cards in the hand pring them out
	auto it = h.cards.begin();
	while (it < h.cards.end())
	{

		Card card = *it;
		string cardString = rankStrings[card.cardRank] + suitStrings[card.cardSuit];
		cout << cardString << " ";
		++it;
	
	}

	return out;

}

//put a card from the deck into a hand
int operator<< (Hand & h, Deck & d)
{

	const int offset = 1;

	//get the last card in the deck
	Card c = *(d.cards.end() - offset);

	//remove it from the deck
	d.cards.pop_back();

	//put the card in the hand
	h.cards.push_back(c);

	//sort the hand according to < operator
	sort(h.cards.begin(), h.cards.end());

	return errors::noError;
}

//put a card from the deck into another deck
int operator<< (Deck & to, Deck & from)
{

	const int offset = 1;

	//get the last card in the deck
	Card c = *(from.cards.end() - offset);

	//remove it from the deck
	from.cards.pop_back();

	//put the card in the deck
	to.cards.push_back(c);

	return errors::noError;
}

//check to poker hands to see if the first has a better rank than the second
bool poker_rank(const Hand & h1, const Hand & h2)
{

	//get the type of poker hands (represented by integers)
	int h1_type = poker_type(h1);
	int h2_type = poker_type(h2);
	const int handsize = 5;
	const int offset = 1;
	const int firstPosition = 0;

	//if both hands are of the same type (and they each have the proper number of cards), then find out which is the higher of that type
	if (h1_type == h2_type && h1.size() == handsize)
	{

		//straights and straight flushes are sorted only by the highest card in their hand
		if (h1_type == poker_ranks::straightFlush || h1_type == poker_ranks::striaght)
		{

			return h2.cards[handsize - offset].cardRank < h1.cards[handsize - offset].cardRank;

		}
		//full houses and four of a kinds are sorted by their more frequent card then their less frequent card
		else if (h1_type == poker_ranks::fullHouse || h1_type == poker_ranks::fourOfAKind)
		{

			//get the count of the rank of the first card in the hand
			int count1 = rank_count(&h1.cards[firstPosition], &h1.cards[handsize - offset] + offset, *h1.cards.begin());
			int count2 = rank_count(&h2.cards[firstPosition], &h2.cards[handsize - offset] + offset, *h2.cards.begin());
			
			const int triplet = 3;

			Card h1big;
			Card h2big;
			Card h1small;
			Card h2small;
			
			//set the more frequent and less frequent cards from hand 1
			if (count1 >= triplet)
			{

				h1big = *h1.cards.begin();
				h1small = *h1.cards.end();
			
			}
			else
			{

				h1big = *h1.cards.end();
				h1small = *h1.cards.begin();
			
			}
			
			//set the more frequent and less frequent cards from hand 2
			if (count2 >= triplet)
			{

				h2big = *h2.cards.begin();
				h2small = *h2.cards.end();
			
			}
			else
			{

				h2big = *h2.cards.end();
				h2small = *h2.cards.begin();

			}

			return h2big.cardRank < h1big.cardRank || (h2big.cardRank == h1big.cardRank && h2small.cardRank < h1small.cardRank);

		}
		//flush hands and unranked hands are sorted by the highest card down
		else if (h1_type == poker_ranks::flush || h1_type == poker_ranks::noRank)
		{
			
			//start iterators at the last card in either hand
			int it = handsize - offset;

			//check if we have reached the first card
			while (it >= firstPosition)
			{

				//if the second hand's card comes before the first hands card, the first hand is greater
				if (h2.cards[it].cardRank < h1.cards[it].cardRank)
				{

					return true;

				}
				//if the second hands card comes after the first hands card, the second hand is greater
				if (h2.cards[it].cardRank > h1.cards[it].cardRank)
				{

					return false;
				
				}

				--it;

			}

			//if both hands have the same ranked cards, then the first hand is not greater
			return false;
		
		}
		//three of a kind hands are sorted by the rank of their three of a kind card
		else if (h1_type == poker_ranks::threeOfAKind)
		{

			//with three cards of the same rank, one of the three will always be at the middle position of the hands since the hands are sorted and we have a maximum of 5 cards
			const int midVal = 2;
			return h2.cards[midVal].cardRank < h1.cards[midVal].cardRank;

		}
		//two pairs hands are sorted by their first pair, then their second pair, then their remaining cards
		else if (h1_type == poker_ranks::twoPairs)
		{

			//begin with the highest card in the hands
			int it1 = handsize - offset;
			int it2 = handsize - offset;

			Card h1one;
			bool seth1one = false;
			Card h2one;
			bool seth2one = false;
			Card h1bigpair;
			bool seth1bigpair = false;
			Card h2bigpair;
			bool seth2bigpair = false;
			Card h1smallpair;
			bool seth1smallpair = false;
			Card h2smallpair;
			bool seth2smallpair = false;

			//go through hands setting the two pairs and the additional card
			while (it1 >= firstPosition && it2 >= firstPosition)
			{

				const int onlyOne = 1;
				const int pair = 2;
				int count1;
				int count2;

				//if we are at the first card in the hand, the cound is one
				if (it1 == firstPosition)
				{

					count1 = onlyOne;
				
				}
				//otherise we check the local region to see if there is another one of this card
				else
				{

					count1 = rank_count(&h1.cards[it1] - offset, &h1.cards[it1] + offset, h1.cards[it1]);
				
				}
				//if we are at the first card in the hand, the cound is one
				if (it2 == firstPosition)
				{

					count2 = onlyOne;

				}
				//otherise we check the local region to see if there is another one of this card
				else
				{

					count2 = rank_count(&h2.cards[it2] - offset, &h2.cards[it2] + offset, h2.cards[it2]);
				
				}
				if (count1 == onlyOne)
				{

					//if we havent already set the one card, we set it
					if (!seth1one)
					{

						h1one = h1.cards[it1];
						seth1one = true;

					}

					--it1;

				}
				if (count2 == onlyOne)
				{

					//if we havent already set the one card, we set it
					if (!seth2one)
					{

						h2one = h2.cards[it2];
						seth2one = true;

					}

					--it2;
				
				}
				if (count1 == pair)
				{
					//first we attempt to set the larger pair, but if we have already set it we set the smaller pair (unless it is also already set)
					if (!seth1bigpair)
					{

						h1bigpair = h1.cards[it1];
						seth1bigpair = true;

					}
					else if (!seth1smallpair)
					{

						h1smallpair = h1.cards[it1];
						seth1smallpair = true;

					}

					//decrement twice since we already acounted for both cards in the pair
					--it1;
					--it1;

				}
				if (count2 == 2)
				{
					//first we attempt to set the larger pair, but if we have already set it we set the smaller pair (unless it is also already set)
					if (!seth2bigpair)
					{

						h2bigpair = h2.cards[it2];
						seth2bigpair = true;

					}
					else if (!seth2smallpair)
					{

						h2smallpair = h2.cards[it2];
						seth2smallpair = true;

					}

					//recrement twice since we already accounted for both cards in the pair
					--it2;
					--it2;

				}
			}

			return (h2bigpair.cardRank < h1bigpair.cardRank) || (h2bigpair.cardRank == h1bigpair.cardRank && h2smallpair.cardRank < h1smallpair.cardRank) || (h2bigpair.cardRank == h1bigpair.cardRank && h2smallpair.cardRank == h1smallpair.cardRank && h2one.cardRank < h1one.cardRank);
		
		}
		//finally, if we only have one pair, we sort by the piar then the remaining cards
		else
		{

			int it1 = handsize - offset;
			int it2 = handsize - offset;

			const int onlyOne = 1;
			const int pair = 2;

			Card h1pair;
			Card h2pair;

			while (it1 >= firstPosition && it2 >= firstPosition)
			{

				int count1;
				int count2;

				//if we are at the first card in the hand, the cound is one
				if (it1 == firstPosition)
				{

					count1 = onlyOne;

				}
				//otherise we check the local region to see if there is another one of this card
				else
				{

					count1 = rank_count(&h1.cards[it1] - offset, &h1.cards[it1] + offset, h1.cards[it1]);
				
				}

				//if we are at the first card in the hand, the cound is one
				if (it2 == firstPosition)
				{

					count2 = onlyOne;
				
				}
				//otherise we check the local region to see if there is another one of this card
				else
				{

					count2 = rank_count(&h2.cards[it2] - offset, &h2.cards[it2] + offset, h2.cards[it2]);
				
				}

				if (count1 == onlyOne)
				{

					//we dont have a pair yet so just pass it.
					--it1;

				}
				if (count2 == onlyOne)
				{

					//we dont have a pair yet so just pass it.
					--it2;
				}
				if (count1 == pair)
				{

					//we found the pair so set it as found
					h1pair = h1.cards[it1];
					//decrement twice since we found the piar
					--it1;
					--it1;

				}
				if (count2 == pair)
				{

					//we dond the pair sp set it as found
					h2pair = h2.cards[it2];
					//decrement twice since we found the pair
					--it2;
					--it2;

				}

			}
			//if the pairs are equal, check the remaining cards
			if (h1pair.cardRank == h2pair.cardRank)
			{
				it1 = handsize - offset;
				it2 = handsize - offset;
				while (it1 >= firstPosition && it2 >= firstPosition)
				{

					//if this card is a card in the pair just pass it
					if (h1.cards[it1].cardRank == h1pair.cardRank || h2.cards[it2].cardRank == h2pair.cardRank)
					{

						if (h1.cards[it1].cardRank == h1pair.cardRank)
						{

							--it1;

						}
						if (h2.cards[it2].cardRank == h2pair.cardRank)
						{

							--it2;

						}
					}
					//if this card isnt a card in the pair compare
					else
					{

						//if the second hands card is smaller, then this hand is greater
						if (h2.cards[it2].cardRank < h1.cards[it1].cardRank)
						{

							return true;

						}
						//if the second hands card is greater, then the second hand is greater
						if (h2.cards[it2].cardRank > h1.cards[it1].cardRank)
						{

							return false;

						}
						--it1;
						--it2;
					}
				}

				//if we go through all cards finding the same ranks, then we are not greater than the second hand
				return false;

			}
			//if the pairs are not equal return which one is larger
			else
			{

				return h2pair.cardRank < h1pair.cardRank;

			}
		}
	}
	//if they are different types, then return which is the higher type
	else
	{

		return h1_type > h2_type;

	}
}

vector<Card> & Hand::getCards(){
	return cards;
}


int poker_type(const Hand & h)
{
	int handSize = h.size();
	const int quad = 4;			//number of cards in a four of a kind
	const int triplet = 3;		//number of cards in a three of a kind
	const int pair = 2;			//number of cards in a two of a kind
	if (handSize == 5)
	{
		//checks the overall hand for a flush or a straight
		bool flush = h.cards[0].cardSuit == h.cards[1].cardSuit && h.cards[0].cardSuit == h.cards[2].cardSuit && h.cards[0].cardSuit == h.cards[3].cardSuit && h.cards[0].cardSuit == h.cards[4].cardSuit;
		bool straight = (h.cards[0].cardRank + 1) == h.cards[1].cardRank && (h.cards[1].cardRank + 1) == h.cards[2].cardRank && (h.cards[2].cardRank + 1) == h.cards[3].cardRank && (h.cards[3].cardRank + 1) == h.cards[4].cardRank;

		//checks for any multiples of a rank
		int maxCount = 0;		//maximum number of cards of the same rank in hand
		int secondCount = 0;	//second best amount of cards of the same rank (for full house and two pairs)
		int j;
		for (j = 0; j < handSize; j++)		//go through all cards in hand beginning with first one
		{

			Card cardOne = h.cards[j];		//store card currently examined
			int crntCount = 1;				//there are one of that rank so far

			int k;
			for (k = j + 1; k < handSize; ++k)	//go through rest of cards finding the rest of that rank
			{

				Card cardTwo = h.cards[k];

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

			return poker_ranks::straightFlush;

		}
		else if (maxCount == quad)
		{

			return poker_ranks::fourOfAKind;

		}
		else if (maxCount == triplet && secondCount == pair)
		{

			return poker_ranks::fullHouse;

		}
		else if (flush)
		{

			return poker_ranks::flush;

		}
		else if (straight)
		{

			return poker_ranks::striaght;

		}
		else if (maxCount == triplet)
		{

			return poker_ranks::threeOfAKind;

		}
		else if (maxCount == pair && secondCount == pair)
		{

			return poker_ranks::twoPairs;

		}
		else if (maxCount == pair)
		{

			return poker_ranks::onePair;

		}
		else
		{

			return poker_ranks::noRank;

		}
	}
	return poker_ranks::INVALID;
}

void Hand::add_card(Card c){
	cards.push_back(c);
	sort(cards.begin(), cards.end());
}
