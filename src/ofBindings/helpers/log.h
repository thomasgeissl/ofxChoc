#pragma once
#include <iostream>
#include <tuple>
#include <cstddef>

// Base case: when the tuple is empty, do nothing
template <std::size_t Index, typename... Types>
void logTupleElement(const std::tuple<Types...>& tup) {
    if constexpr (Index < sizeof...(Types)) {
        std::cout << std::get<Index>(tup);
        if constexpr (Index + 1 < sizeof...(Types)) {
            std::cout << ", ";
        }
        logTupleElement<Index + 1>(tup);
    }
}

// Main function to log the tuple
template <typename... Types>
void logTuple(const std::tuple<Types...>& tup) {
    std::cout << "(";
    logTupleElement<0>(tup);
    std::cout << ")" << std::endl;
}