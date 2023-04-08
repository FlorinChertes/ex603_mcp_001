#include "playing_cards.h"

#include <iostream>
#include <string>
#include <random>

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

	// Listing 5.18 increment our enum
	Suit& operator++(Suit& suit)
	{
		// Dangerous! See https://stackoverflow.com/questions/3475152/why-cant-i-increment-a-variable-of-an-enumerated-type
		using IntType = typename std::underlying_type_t<Suit>;

		if (suit == Suit::Spades)
			suit = Suit::Hearts;
		else
			suit = static_cast<Suit>(static_cast<IntType>(suit) + 1);
		return suit;
	}


	//Listing 5.19 Generating the deck of cards
	std::array<Card, 52> create_deck()
	{
		std::array<Card, 52> deck;

		int value = 1;
		Suit suit = Suit::Hearts;

		std::ranges::generate(deck, [&value, &suit]() {
			if (value > 13)
			{
				value = 1;
				++suit;
			}
			return Card{ FaceValue(value++), suit };
			});

		return deck;
	}

	// Listing 5.22 Shuffle the cards
	void shuffle_deck(std::array<Card, 52>& deck)
	{
		std::random_device rd;
		std::mt19937 gen{ rd() };
		std::ranges::shuffle(deck, gen);
	}

	//Listing 5.31 Stream out cards and jokers
	std::ostream& operator<<(std::ostream& os, const std::variant<Card, Joker>& card)
	{
		if (std::holds_alternative<Joker>(card))
			os << "JOKER";
		else
			os << std::get<Card>(card);
		return os;
	}

	//Listing 5.28 Create an extended deck
	std::array<std::variant<Card, Joker>, 54> create_extended_deck()
	{
		std::array<std::variant<Card, Joker>, 54> deck{ Joker{} , Joker{} };
		std::array<Card, 52> cards = create_deck();
		std::ranges::copy(cards, deck.begin() + 2);
		return deck;
	}
}