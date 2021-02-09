#include <test_utils.h>

#include <cctype>
#include <variant>
#include <optional>
#include <chrono>

#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <iostream>
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template <typename K, typename V>
std::optional<V> find(K const key,
	std::map<K, V> const& m)
{
	auto pos = m.find(key);
	if (pos != m.end())
		return pos->second;
	return {};
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_utils_017()
{
	using namespace std::string_literals;

	std::cout << "*** test utils 017 ***" << std::endl;
	
	std::map<int, std::string> m {
		{ 1, "one"s }, { 2, "two"s }, { 3, "three"s } 
		};

	auto value = find(2, m);
	if (value) std::cout << *value << std::endl; // two
	
	value = find(4, m);
	if (value) std::cout << *value << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::string extract(std::string const& text,
	std::optional<int> start,
	std::optional<int> end)
{
	auto s = start.value_or(0);
	auto e = end.value_or(text.length());
	return text.substr(s, e - s);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_utils_018()
{
	using namespace std::string_literals;

	std::cout << "*** test utils 018 ***" << std::endl;

	auto v1 = extract("sample"s, {}, {});
	std::cout << v1 << std::endl; // sample

	auto v2 = extract("sample"s, 1, {});
	std::cout << v2 << std::endl; // ample
	
	auto v3 = extract("sample"s, 1, 4);
	std::cout << v3 << std::endl; // amp
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_utils_019()
{
	using namespace std::string_literals;

	std::cout << "*** test utils 019 ***" << std::endl;

	std::variant<int, double, std::string> v = 42;
	static_assert(std::variant_size_v<decltype(v)> == 3);
	
	std::cout << "index = " << v.index() << std::endl;
	
	v = 42.0;
	std::cout << "index = " << v.index() << std::endl;
	
	v = "42";
	std::cout << "index = " << v.index() << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
enum class Genre { Drama, Action, SF, Commedy };

struct Movie
{
	std::string title;
	std::chrono::minutes length;
	std::vector<Genre> genre;
};

struct Track
{
	std::string title;
	std::chrono::seconds length;
};

struct Music
{
	std::string title;
	std::string artist;
	std::vector<Track> tracks;
};

struct Software
{
	std::string title;
	std::string vendor;
};

using dvd = std::variant<Movie, Music, Software>;

//-----------------------------------------------------------------------------
//https://github.com/PacktPublishing/Modern-CPP-Programming-Cookbook-Second-Edition
//-----------------------------------------------------------------------------
void test_utils_020()
{
	using namespace std::chrono_literals;
	using namespace std::string_literals;
	
	std::vector<dvd> dvds
	{
	   Movie{ "The Matrix"s, 2h + 16min, { Genre::Action, Genre::SF } },
	   Music{ "The Wall"s, "Pink Floyd"s, { { "Mother"s, 5min + 32s }, { "Another Brick in the Wall"s, 9min + 8s } } },
	   Software{ "Windows"s, "Microsoft"s },
	};

	// void visitor
	for (auto const& d : dvds)
	{
		std::visit(	[](auto&& arg) {
						std::cout << arg.title << '\n'; },
					d );
	}


	// value returning visitor
	for (auto const& d : dvds)
	{
		dvd result = std::visit(
			[](auto&& arg) -> dvd {
				auto cpy{ arg };
				std::transform(std::begin(cpy.title), std::end(cpy.title),
					std::begin(cpy.title),
					[] (unsigned char c){return std::toupper(c);});

				return cpy; },
			d );

		std::visit( [](auto&& arg) {
						std::cout << arg.title << '\n'; },
					result);
	}

	// visitor with type matching
	struct visitor_functor
	{
		void operator()(Movie const& arg) const
		{
			std::cout << "Movie" << '\n';
			std::cout << "  Title: " << arg.title << '\n';
			std::cout << "  Length: " << arg.length.count() << "min" << '\n';
		}

		void operator()(Music const& arg) const
		{
			std::cout << "Music" << '\n';
			std::cout << "  Title: " << arg.title << '\n';
			std::cout << "  Artist: " << arg.artist << '\n';
			for (auto const& t : arg.tracks)
				std::cout << "    Track: " << t.title
				<< ", " << t.length.count() << "sec" << '\n';
		}

		void operator()(Software const& arg) const
		{
			std::cout << "Software" << '\n';
			std::cout << "  Title: " << arg.title << '\n';
			std::cout << "  Vendor: " << arg.vendor << '\n';
		}
	};

	for (auto const& d : dvds)
	{
		std::visit(visitor_functor(), d);
	}

	// visitor with type matching with lambda extpression
	for (auto const& d : dvds)
	{
		std::visit([](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, Movie>)
			{
				std::cout << "Movie" << '\n';
				std::cout << "  Title: " << arg.title << '\n';
				std::cout << "  Length: " << arg.length.count() << "min" << '\n';
			}
			else if constexpr (std::is_same_v<T, Music>)
			{
				std::cout << "Music" << '\n';
				std::cout << "  Title: " << arg.title << '\n';
				std::cout << "  Artist: " << arg.artist << '\n';
				for (auto const& t : arg.tracks)
					std::cout << "    Track: " << t.title
					<< ", " << t.length.count() << "sec" << '\n';
			}
			else if constexpr (std::is_same_v<T, Software>)
			{
				std::cout << "Software" << '\n';
				std::cout << "  Title: " << arg.title << '\n';
				std::cout << "  Vendor: " << arg.vendor << '\n';
			}

		}, d);
	}
}


