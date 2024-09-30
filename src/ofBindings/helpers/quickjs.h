#pragma once
#include <tuple>
#include <stdexcept>
#include <vector>
#include <string>
#include <type_traits>
#include <iostream>
#include <typeinfo>
#include "../libs/choc/javascript/choc_javascript_QuickJS.h"
#include "./functionTraits.h"

// Helper function to convert choc::javascript::ArgumentList item to the correct argument type
template <typename ArgType>
ArgType convertArg(const choc::javascript::ArgumentList &args, size_t index)
{
    // Implement the conversion logic based on your requirements
    // This is just an example; adjust based on your actual argument handling
    if constexpr (std::is_same_v<ArgType, float>)
    {
        return static_cast<float>(args[index]->getFloat64());
    }
    else if constexpr (std::is_same_v<ArgType, int>)
    {
        return args[index]->getInt64();
    }
    else if constexpr (std::is_same_v<ArgType, std::string>)
    {
        return args[index]->getString();
    }
    else if constexpr (std::is_same_v<ArgType, bool>)
    {
        return args[index]->getBool();
    }
    ofLogNotice() << "no match";
    // Add more conversions as needed
    throw std::invalid_argument("Unsupported argument type");
}

// Helper to convert argument list to tuple
template <typename Tuple, std::size_t... I>
Tuple convertArgsToTupleImpl(const choc::javascript::ArgumentList &args, std::index_sequence<I...>)
{
    if constexpr (sizeof...(I) == 0)
    {
        // Return empty tuple if no arguments
        return std::tuple<>();
    }
    else
    {
        return std::make_tuple(convertArg<typename std::tuple_element<I, Tuple>::type>(args, I)...);
    }
}
// Main conversion function that deduces the argument types
template <typename... Args>
std::tuple<Args...> convertArgsToTuple(const choc::javascript::ArgumentList &args)
{
    return convertArgsToTupleImpl<std::tuple<Args...>>(args, std::index_sequence_for<Args...>{});
}

template <typename Func, typename... Args>
void bindFunction(choc::javascript::Context &context,
                  const std::string &bindingName,
                  Func func)
{
    ofLogNotice() << "Binding function: " << bindingName;

    context.registerFunction(bindingName,
                             [func](choc::javascript::ArgumentList args) mutable -> choc::value::Value
                             {
                                 ofLogNotice() << "Function invoked with args size: " << args.size();

                                 using ArgsTuple = typename function_traits<Func>::argument_types;

                                 // Convert args to a tuple, but only if there are any expected arguments
                                 ofLogNotice() << "Converting arguments to tuple";
                                 auto argTuple = (args.size() > 0) ? convertArgsToTuple<ArgsTuple>(args) : std::tuple<>();

                                 // Check for no arguments case
                                 if constexpr (std::tuple_size<ArgsTuple>::value == 0)
                                 {
                                     ofLogNotice() << "No args to pass";
                                     // Call the function with no argume
                                     if constexpr (std::is_invocable_v<Func>)
                                     {
                                         using ReturnType = std::invoke_result_t<Func, Args...>;
                                         if constexpr (std::is_same_v<ReturnType, int>)
                                         {
                                             return choc::value::createInt64(func());
                                         }
                                         else if constexpr (std::is_same_v<ReturnType, float>)
                                         {
                                             return choc::value::createFloat64(func());
                                         }
                                         else if constexpr (std::is_same_v<ReturnType, bool>)
                                         {
                                             return choc::value::createBool(func());
                                         }
                                         else if constexpr (std::is_same_v<ReturnType, std::string>)
                                         {
                                             return choc::value::createString(func());
                                         }
                                         else
                                         {
                                             return choc::value::Value(); // Return undefined for unsupported types
                                         }
                                     }
                                 }
                                 else
                                 {
                                     ofLogNotice() << "Has args to pass";
                                     // Check if the function can be called with the provided arguments
                                     if constexpr (std::is_invocable_v<Func, Args...>)
                                     {
                                         ofLogNotice() << "Invoking function with arguments";
                                         using ReturnType = std::invoke_result_t<Func, Args...>;

                                         if constexpr (std::is_same_v<ReturnType, int>)
                                         {
                                             return choc::value::createInt64(std::apply(func, argTuple));
                                         }
                                         else if constexpr (std::is_same_v<ReturnType, float>)
                                         {
                                             return choc::value::createFloat64(std::apply(func, argTuple));
                                         }
                                         else if constexpr (std::is_same_v<ReturnType, bool>)
                                         {
                                             return choc::value::createBool(std::apply(func, argTuple));
                                         }
                                         else if constexpr (std::is_same_v<ReturnType, std::string>)
                                         {
                                             return choc::value::createString(std::apply(func, argTuple));
                                         }
                                         else
                                         {
                                             return choc::value::Value(); // Return undefined for unsupported types
                                         }
                                     }
                                 }

                                 ofLogNotice() << "Function not invocable with given args";
                                 return choc::value::Value(); // Return undefined if the function is not invocable
                             });
}
