#pragma once
#include <tuple>
#include <stdexcept>
#include <vector>
#include <string>
#include <type_traits>
#include <iostream>
#include "../libs/choc/javascript/choc_javascript_QuickJS.h"
#include "../libs/choc/gui/choc_WebView.h"

// Forward declaration of convertArg
template <typename ArgType>
ArgType convertArg(const choc::value::ValueView &arg, const std::string &argType);

template <typename... Args, std::size_t... Is>
std::tuple<Args...> createTupleFromArgs(const choc::value::ValueView &args, 
                                         const std::vector<std::string> &argTypes, 
                                         std::index_sequence<Is...>);

// Function to convert args to a tuple
template<typename... Args>
std::tuple<Args...> convertArgsToTuple(const choc::value::ValueView &args, const std::vector<std::string> &argTypes)
{
    if (args.size() == 0) {
        return std::make_tuple(); // Empty tuple for no arguments
    }
    
    // Ensure we have enough arguments for the provided types
    if (args.size() != sizeof...(Args)) {
        throw std::invalid_argument("Number of arguments does not match the expected number");
    }

    // Create tuple using index sequence to unpack Args...
    return createTupleFromArgs<Args...>(args, argTypes, std::index_sequence_for<Args...>{});
}

// Create tuple from arguments using index sequence
template <typename... Args, std::size_t... Is>
std::tuple<Args...> createTupleFromArgs(const choc::value::ValueView &args, 
                                         const std::vector<std::string> &argTypes, 
                                         std::index_sequence<Is...>)
{
    return std::make_tuple(convertArg<Args>(args[static_cast<uint32_t>(Is)], argTypes[Is])...);
}

// Helper function to convert choc::value::Value to the correct argument type
template <typename ArgType>
ArgType convertArg(const choc::value::ValueView &arg, const std::string &argType)
{
    if constexpr (std::is_same_v<ArgType, float>) {
        return static_cast<float>(arg.getFloat64());
    } else if constexpr (std::is_same_v<ArgType, int>) {
        return arg.getInt64();
    } else if constexpr (std::is_same_v<ArgType, std::string>) {
        return arg.getString();
    } else if constexpr (std::is_same_v<ArgType, bool>) {
        return arg.getBool();
    }
    // Add more conversions as needed
    throw std::invalid_argument("Unsupported argument type");
}

template <typename Func, typename... Args>
void bindFunction(choc::ui::WebView *webview, 
                  const std::string &bindingName, 
                  const std::vector<std::string> &argTypes, 
                  const std::string &returnType, 
                  Func func)
{
    ofLogNotice() << "registering " << bindingName << " with return type "<<    returnType << " and args ";

    webview->bind(bindingName, [argTypes, returnType, func](const choc::value::ValueView &args) -> choc::value::Value {
        // Check if the number of arguments matches
        if (args.size() != argTypes.size()) {
            return choc::value::Value(); // return undefined or empty value
        }

        // Convert arguments to a tuple for easier handling
        auto argTuple = convertArgsToTuple<Args...>(args, argTypes);

        return choc::value::Value();

        // // Call function based on return type
        // if (returnType == "bool")
        // {
        //     return choc::value::createBool(std::apply(func, argTuple));
        // }
        // else if (returnType == "int")
        // {
        //     return choc::value::createInt64(std::apply(func, argTuple));
        // }
        // else if (returnType == "float")
        // {
        //     return choc::value::createFloat64(std::apply(func, argTuple));
        // }
        // else if (returnType == "string")
        // {
        //     return choc::value::createString(std::apply(func, argTuple));
        // }
        // else if (returnType == "void")
        // {
        //     std::apply(func, argTuple);
        //     return choc::value::Value(); // Return undefined for void functions
        // }

        return choc::value::Value(); // return undefined for unsupported types
    });
}
