#pragma once
#include <tuple>
#include <stdexcept>
#include <vector>
#include <string>
#include <type_traits>
#include <iostream>
#include <typeinfo>
#include "../libs/choc/javascript/choc_javascript_QuickJS.h"
#include "../libs/choc/gui/choc_WebView.h"


// Forward declaration of convertArg for ArgumentList
template <typename ArgType>
ArgType convertArg(const choc::javascript::ArgumentList &args, size_t index, const std::string &argType);

template <typename... Args, std::size_t... Is>
std::tuple<Args...> createTupleFromArgs(const choc::javascript::ArgumentList &args, 
                                         const std::vector<std::string> &argTypes, 
                                         std::index_sequence<Is...>);

// Function to convert args from ArgumentList to a tuple
template<typename... Args>
std::tuple<Args...> convertArgsToTuple(const choc::javascript::ArgumentList &args, const std::vector<std::string> &argTypes)
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
std::tuple<Args...> createTupleFromArgs(const choc::javascript::ArgumentList &args, 
                                         const std::vector<std::string> &argTypes, 
                                         std::index_sequence<Is...>)
{
    return std::make_tuple(convertArg<Args>(args, static_cast<uint32_t>(Is), argTypes[Is])...);
}

// Helper function to convert choc::javascript::ArgumentList item to the correct argument type
template <typename ArgType>
ArgType convertArg(const choc::javascript::ArgumentList &args, size_t index, const std::string &argType)
{
    if constexpr (std::is_same_v<ArgType, float>) {
        return static_cast<float>(args[index]->getFloat64());
    } else if constexpr (std::is_same_v<ArgType, int>) {
        return args[index]->getInt64();
    } else if constexpr (std::is_same_v<ArgType, std::string>) {
        return args[index]->getString();
    } else if constexpr (std::is_same_v<ArgType, bool>) {
        return args[index]->getBool(); 
    }
    // Add more conversions as needed
    throw std::invalid_argument("Unsupported argument type");
}

// template <typename Func, typename... Args>
// void bindFunction(choc::javascript::Context &context, 
//                   const std::string &bindingName, 
//                   const std::vector<std::string> &argTypes, 
//                   const std::string &returnType, 
//                   Func func)
// {
//     ofLogNotice() << "registering " << bindingName << " with return type " << returnType << " and args ";

//     // Register the function with the QuickJS context
//     context.registerFunction(bindingName,
//         [argTypes, returnType, func](choc::javascript::ArgumentList args) mutable -> choc::value::Value
//         {
//             if (args.size() != argTypes.size()) {
//                 return choc::value::Value(); // Return undefined or empty value for mismatched args
//             }
//             ofLogNotice() << "calling function with " <<returnType;

//             // Convert arguments to a tuple for easier handling
//             auto argTuple = convertArgsToTuple<Args...>(args, argTypes);
//             auto hasArgs = true;      
//             if constexpr (std::tuple_size<decltype(argTuple)>::value == 0){
//                 hasArgs = false;
//             }


//             ofLogNotice() << " and hasArgs = " << hasArgs;



//             if (returnType == "int") {
//                 if(hasArgs){
//                     return choc::value::createInt64(std::apply(func, argTuple));
//                 }else{
//                     return choc::value::createInt64(func());
//                 }
//             }
//             // Handle return type
//             // if (returnType == "bool") {
//             //     return choc::value::createBool(std::apply(func, argTuple));
//             // } else if (returnType == "int") {
//             //     return choc::value::createInt64(std::apply(func, argTuple));
//             // } else if (returnType == "float") {
//             //     return choc::value::createFloat64(std::apply(func, argTuple));
//             // } else if (returnType == "string") {
//             //     return choc::value::createString(std::apply(func, argTuple));
//             // } else if (returnType == "void") {
//             //     std::apply(func, argTuple);
//             //     return choc::value::Value(); // Return undefined for void functions
//             // }

//             // return choc::value::createInt64(std::apply(func, argTuple));
//             return choc::value::Value(); // Return undefined for unsupported types
//         }
//     );
// }



template <typename Func, typename... Args>
void bindFunction(choc::javascript::Context &context, 
                  const std::string &bindingName, 
                  const std::vector<std::string> &argTypes, 
                  Func func)
{
    context.registerFunction(bindingName,
        [argTypes, func](choc::javascript::ArgumentList args) mutable -> choc::value::Value
        {
            auto argTuple = convertArgsToTuple<Args...>(args, argTypes);

            if constexpr (std::is_invocable_v<Func, Args...>) {
                // Call the function with arguments and deduce return type
                using ReturnType = std::invoke_result_t<Func, Args...>;

                if constexpr (std::is_same_v<ReturnType, int>) {
                    return choc::value::createInt64(std::apply(func, argTuple));
                } else if constexpr (std::is_same_v<ReturnType, float>) {
                    return choc::value::createFloat64(std::apply(func, argTuple));
                } else if constexpr (std::is_same_v<ReturnType, bool>) {
                    return choc::value::createBool(std::apply(func, argTuple));
                } else if constexpr (std::is_same_v<ReturnType, std::string>) {
                    return choc::value::createString(std::apply(func, argTuple));
                } else {
                    return choc::value::Value(); // Return undefined for unsupported types
                }
            }

            return choc::value::Value(); // In case function is not invocable
        }
    );
}
