
#include <cstddef>

template<typename T, std::size_t N>
constexpr std::size_t arraySize(T(&)[N]) noexcept
{
	return N;
}
