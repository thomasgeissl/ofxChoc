#pragma once
#include <functional>
#include <tuple>

// Specialization for callable types (including lambdas)
template <typename T>
struct function_traits : function_traits<decltype(&T::operator())> {};

// Specialization for const member function pointers (used by lambdas)
template <typename Ret, typename ClassType, typename... Args>
struct function_traits<Ret(ClassType::*)(Args...) const> {
    using return_type = Ret;
    using argument_types = std::tuple<Args...>;
};
