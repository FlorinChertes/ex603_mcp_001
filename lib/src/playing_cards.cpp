#include "playing_cards.h"

#include <iostream>
#include <stdexcept>
#include <string>

//#include <algorithm>
//#include <random>

namespace cards
{

	//Listing 5.14 Turn an enum value into a string
	std::string to_string(Suit suit)
	{
		using namespace std::literals;
		switch (suit)
		{
		case Suit::Hearts:
			return "Hearts"s;
		case Suit::Diamonds:
			return "Diamonds"s;
		case Suit::Clubs:
			return "Clubs"s;
		case Suit::Spades:
			return "Spades"s;
		default:
			return "?"s;
		}
	}
	//Listing 5.15 Convert card value to a string
	std::string to_string(FaceValue value)
	{
		using namespace std::literals;

		switch (value.value())
		{
		case 1:
			return "Ace"s;
		case 11:
			return "Jack"s;
		case 12:
			return "Queen"s;
		case 13:
			return "King"s;
		default:
			return std::to_string(value.value());
		}
	}

	//Listing 5.16 Show ace, jack, queen, king or number
	std::ostream& operator<<(std::ostream& os, const Card& card)
	{
		os << to_string(card.value())
			<< " of " << to_string(card.suit());
		return os;
	}

	
}