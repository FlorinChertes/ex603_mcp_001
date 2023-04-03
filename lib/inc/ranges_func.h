#pragma once

#include <ranges>
#include <vector>

template <std::ranges::range R>
constexpr auto to_vector(R&& r)
{
    using elem_t = std::decay_t<std::ranges::range_value_t<R>>;
    return std::vector<elem_t>(r.begin(), r.end());
}


template <typename Container, std::ranges::range R>
requires std::convertible_to<std::ranges::range_value_t<R>,
    typename Container::value_type>
Container operator |(R&& r, Container) {
    return Container{ r.begin(), r.end() };
}



