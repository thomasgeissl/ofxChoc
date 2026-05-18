#pragma once

#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"
#include <type_traits>
#include <utility>

//==============================================================================
// Type converters: JS argument → C++ type
//
// JS numbers normally arrive as Float64, but choc may preserve Int32 for
// integer-typed return values (e.g. of.getWidth()). numVal() handles all
// numeric choc types safely.

inline double numVal(const choc::value::Value* v, double def = 0.0) noexcept
{
    if (!v) return def;
    const auto& t = v->getType();
    if (t.isFloat64()) return v->getFloat64();
    if (t.isFloat32()) return static_cast<double>(v->getFloat32());
    if (t.isInt32())   return static_cast<double>(v->getInt32());
    if (t.isInt64())   return static_cast<double>(v->getInt64());
    return def;
}

template<typename T>
T fromJS(const choc::value::Value* v, T defaultVal = {})
{
    if (!v) return defaultVal;
    if constexpr (std::is_same_v<T, bool>)
        return v->getBool();
    else if constexpr (std::is_same_v<T, std::string>)
        return std::string(v->getString());
    else
        return static_cast<T>(numVal(v));
}

//==============================================================================
// Type converters: C++ return value → choc::value::Value

template<typename T>
choc::value::Value toJS(T v)
{
    if constexpr (std::is_same_v<T, bool>)
        return choc::value::createBool(v);
    else if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>)
        return choc::value::createString(v);
    else if constexpr (std::is_integral_v<T>)
        return choc::value::createFloat64(static_cast<double>(v));
    else
        return choc::value::createFloat64(static_cast<double>(v));
}

//==============================================================================
// Internal: unpack ArgumentList into C++ args and call, wrapping the return

template<typename Ret, typename... Args, std::size_t... I>
choc::value::Value callAndWrap(Ret (*fn)(Args...), choc::javascript::ArgumentList args,
                                std::index_sequence<I...>)
{
    if constexpr (std::is_void_v<Ret>)
    {
        fn(fromJS<std::decay_t<Args>>(args[I])...);
        return {};
    }
    else
    {
        return toJS(fn(fromJS<std::decay_t<Args>>(args[I])...));
    }
}

//==============================================================================
// bindFn: register a plain C++ function pointer with automatic type marshalling.
// The function signature is deduced — no need to specify argument or return types.
//
// Usage:
//   bindFn(ctx, "jsName", &myCppFunction);

template<typename Ret, typename... Args>
void bindFn(choc::javascript::Context& ctx, const std::string& name, Ret (*fn)(Args...))
{
    ctx.registerFunction(name, [fn](choc::javascript::ArgumentList args) -> choc::value::Value
    {
        return callAndWrap(fn, args, std::make_index_sequence<sizeof...(Args)>{});
    });
}

//==============================================================================
// bindFn overload for member functions — same convenience, captures object ptr.
//
// Usage:
//   bindFn(ctx, "jsName", this, &MyClass::myMethod);

template<typename T, typename Ret, typename... Args, std::size_t... I>
choc::value::Value callMethodAndWrap(T* obj, Ret (T::*method)(Args...),
                                     choc::javascript::ArgumentList args,
                                     std::index_sequence<I...>)
{
    if constexpr (std::is_void_v<Ret>)
    {
        (obj->*method)(fromJS<std::decay_t<Args>>(args[I])...);
        return {};
    }
    else
    {
        return toJS((obj->*method)(fromJS<std::decay_t<Args>>(args[I])...));
    }
}

template<typename T, typename Ret, typename... Args>
void bindFn(choc::javascript::Context& ctx, const std::string& name,
            T* obj, Ret (T::*method)(Args...))
{
    ctx.registerFunction(name, [obj, method](choc::javascript::ArgumentList args) -> choc::value::Value
    {
        return callMethodAndWrap(obj, method, args, std::make_index_sequence<sizeof...(Args)>{});
    });
}
