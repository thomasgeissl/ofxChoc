#pragma once
#include <functional>


// Primary template for function_traits
template <typename T>
struct function_traits;

// Specialization for function pointers
template <typename Ret, typename... Args>
struct function_traits<Ret(*)(Args...)> {
    using return_type = Ret;
    using argument_types = std::tuple<Args...>;
};

// Specialization for std::function
template <typename Ret, typename... Args>
struct function_traits<std::function<Ret(Args...)>> {
    using return_type = Ret;
    using argument_types = std::tuple<Args...>;
};

// Specialization for member function pointers
template <typename Ret, typename ClassType, typename... Args>
struct function_traits<Ret(ClassType::*)(Args...)> {
    using return_type = Ret;
    using argument_types = std::tuple<Args...>;
};

// Specialization for const member function pointers
template <typename Ret, typename ClassType, typename... Args>
struct function_traits<Ret(ClassType::*)(Args...) const> {
    using return_type = Ret;
    using argument_types = std::tuple<Args...>;
};

// Specialization for callable types (including lambdas)
template <typename T>
struct function_traits : function_traits<decltype(&T::operator())> {};

// A template specialization to handle any callable types
template <typename Ret, typename... Args>
struct function_traits<Ret(*)(Args...) noexcept> : function_traits<Ret(*)(Args...)> {};
