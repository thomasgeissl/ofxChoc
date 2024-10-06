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
#include "./log.h"

// Helper function to convert choc::javascript::ArgumentList item to the correct argument type
template <typename ArgType>
ArgType convertArg(const choc::javascript::ArgumentList &args, size_t index)
{
    ofLogNotice() << "convert arg " << index;
    auto arg = args[index];
    if constexpr (std::is_same_v<ArgType, float>)
    {
        ofLogNotice() << "expect float arg";
    }
    if constexpr (std::is_same_v<ArgType, int>)
    {
        ofLogNotice() << "expect int arg";
    }
    if constexpr (std::is_same_v<ArgType, bool>)
    {
        ofLogNotice() << "expect bool arg";
    }
    if constexpr (std::is_same_v<ArgType, std::string>)
    {
        ofLogNotice() << "expect string arg";
    }
    if (arg->isFloat64())
    {
        ofLogNotice() << "got float64";
        return arg->getFloat64();
    }
    if (arg->isInt64())
    {
        ofLogNotice() << "got int64";
        return arg->getInt64();
    }
    if (arg->isBool())
    {
        ofLogNotice() << "got bool";
        return arg->getBool();
    }
    if (arg->isString())
    {
        ofLogNotice() << "got string";
        return 0;
        // return arg->getString();
    }
    ofLogNotice() << "no match";
    // Add more conversions as needed
    throw std::invalid_argument("Unsupported argument type");
}

template <typename Tuple, std::size_t... Is>
Tuple convertArgsToTupleHelper(const choc::javascript::ArgumentList &args, std::index_sequence<Is...>)
{
    ofLogNotice() << "convert args to tuple helper";
    // Use std::index_sequence to unpack tuple indices correctly
    return std::make_tuple(convertArg<std::tuple_element_t<Is, Tuple>>(args, Is)...);
}

template <typename Tuple>
Tuple convertArgsToTuple(const choc::javascript::ArgumentList &args)
{
    ofLogNotice() << "convert args to tuple";
    // Ensure we are converting to the correct type of tuple
    return convertArgsToTupleHelper<Tuple>(args, std::make_index_sequence<std::tuple_size_v<Tuple>>{});
}

template <typename Func>
void bindFunction(choc::javascript::Context &context,
                  const std::string &bindingName,
                  Func func)
{
    context.registerFunction(bindingName,
                             [func](choc::javascript::ArgumentList args) mutable -> choc::value::Value
                             {
                                 // Get the tuple type representing the argument types
                                 using ArgsTuple = typename function_traits<Func>::argument_types;
                                 auto argsSize = args.size();

                                 // Initialize argTuple with the correct argument types (ArgsTuple)
                                 ArgsTuple argTuple;
                                 logTuple(argTuple);
                                 if constexpr (std::tuple_size<ArgsTuple>::value > 0)
                                 {
                                     if (args.size() > 0)
                                     {
                                         ofLogNotice() << "converting args to tuple";
                                         // Convert arguments to tuple only if arguments are expected
                                         argTuple = convertArgsToTuple<ArgsTuple>(args);
                                         auto expectedArgsSize = std::tuple_size<decltype(argTuple)>::value;
                                         ofLogNotice() << "done";
                                     }
                                 }

                                 // Check for no arguments case
                                 if constexpr (std::tuple_size<ArgsTuple>::value == 0)
                                 {
                                     ofLogNotice() << "calling without args";
                                     // Call the function with no arguments
                                     if constexpr (std::is_invocable_v<Func>)
                                     {
                                         using ReturnType = std::invoke_result_t<Func>;
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
                                     ofLogNotice() << "calling with args";
                                     ofLogNotice() << typeid(func).name();

                                     // Check if the function can be called with the provided arguments
                                     if constexpr (std::is_invocable_v<decltype(func), ArgsTuple>())
                                     {
                                         using ReturnType = std::invoke_result_t<Func, ArgsTuple>;

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
                                     else
                                     {
                                         ofLogNotice() << "Function not invocable with given args";
                                         return choc::value::Value(); // Return undefined if the function is not invocable
                                     }
                                 }
                             });
}
