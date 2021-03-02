
#pragma once

#include <iostream>
#include <cstddef>

template<typename T, std::size_t N>
constexpr std::size_t arraySize(T(&)[N]) noexcept
{
	return N;
}


class controlbase
{
public:
	virtual void draw() = 0;
	virtual ~controlbase() {}
};

template <class T>
class control : public controlbase
{
	T* derived() { return static_cast<T*>(this); }
public:
	virtual void draw() override
	{
		derived()->erase_background();
		derived()->paint();
	}
};




class button : public control<button>
{
public:
	void erase_background()
	{
		std::cout << "erasing button background..." << '\n';
	}
	void paint()
	{
		std::cout << "painting button..." << '\n';
	}
};

class checkbox : public control<checkbox>
{
public:
	void erase_background()
	{
		std::cout << "erasing checkbox background..."
			<< '\n';
	}
	void paint()
	{
		std::cout << "painting checkbox..." << '\n';
	}
};
