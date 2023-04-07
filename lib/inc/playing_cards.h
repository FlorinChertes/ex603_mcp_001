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

	struct Card
	{
		int value;
		Suit suit;
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

} 