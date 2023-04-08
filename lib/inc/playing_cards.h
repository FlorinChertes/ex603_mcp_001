
#include <array>
#include <ranges>
#include <algorithm>
#include <compare>
#include <variant>
#include <stdexcept>

#pragma once
namespace cards
{ 
	enum class Suit {
		Hearts,
		Diamonds,
		Clubs,
		Spades
	};

	class FaceValue
	{
	public:
		explicit FaceValue(int value) : value_(value)
		{
		if (value_ < 1 || value_ > 13)
			{
				throw std::invalid_argument("Face value invalid");
			}
		}
	
		int value() const
		{
			return value_;
		}
		
		auto operator<=>(const FaceValue&) const = default;

	private:
		int value_;
	};

	class Card
	{
	public:
		Card() = default;
		Card(FaceValue value, Suit suit) :
			value_(value),
			suit_(suit)
		{}

		FaceValue value() const { return value_; }
		Suit suit() const { return suit_; }

		auto operator<=>(const Card&) const = default;
	private:
		FaceValue value_{1};
		Suit suit_{};
	};

	//Listing 5.26 A joker
	struct Joker
	{
	};

	std::ostream& operator<<(std::ostream& os, const Card& card);  // Listing 5.10

	Suit& operator++(Suit& suit);

	std::array<Card, 52> create_deck();
	void shuffle_deck(std::array<Card, 52>& deck);

	std::ostream& operator<<(std::ostream& os, const std::variant<Card, Joker>& card);
	std::array<std::variant<Card, Joker>, 54> create_extended_deck();

} 