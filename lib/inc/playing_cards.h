
//#include <array>
//#include <compare>
//#include <variant>

//#include <iostream>
#include <stdexcept>
//s#include <string>


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

	private:
		FaceValue value_{1};
		Suit suit_{};
	};

	std::ostream& operator<<(std::ostream& os, const Card& card);  // Listing 5.10
} 