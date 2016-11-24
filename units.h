#pragma once

#include <iostream>
#include <type_traits>

namespace units {

template <int L, int M, int T, int A, int Cd, int Mol> struct Unit {

    template <int L2, int M2, int T2, int A2, int Cd2, int Mol2, typename R
        = Unit<L + L2, M + M2, T + T2, A + A2, Cd + Cd2, Mol + Mol2> >
    R operator*(const Unit<L2, M2, T2, A2, Cd2, Mol2>& /*rhs*/)
    {
        return R();
    }

    template <int L2, int M2, int T2, int A2, int Cd2, int Mol2, typename R
        = Unit<L - L2, M - M2, T - T2, A - A2, Cd - Cd2, Mol - Mol2> >
    R operator/(const Unit<L2, M2, T2, A2, Cd2, Mol2>& /*rhs*/)
    {
        return R();
    }
};

template <int L, int M, int T, int A, int Cd, int Mol>
std::ostream& operator<<(std::ostream& os, const Unit<L, M, T, A, Cd, Mol>&)
{
    os << "L" << L << "M" << M << "T" << T << "A" << A << "Cd" << Cd << "Mol"
       << Mol;
    return os;
}

using Dimensionless = Unit<0, 0, 0, 0, 0, 0>;
using Meter = Unit<1, 0, 0, 0, 0, 0>;
using Kilogram = Unit<0, 1, 0, 0, 0, 0>;
using Second = Unit<0, 0, 1, 0, 0, 0>;
using Amper = Unit<0, 0, 0, 1, 0, 0>;
using Candela = Unit<0, 0, 0, 0, 1, 0>;
using Mol = Unit<0, 0, 0, 0, 0, 1>;
using Newton = decltype(Kilogram() * Meter() / (Second() * Second()));

template <typename V> struct is_dimensionless : std::false_type {
};

template <typename V, class U,
    class = std::enable_if<std::is_arithmetic<V>::value> >
class Value {
    V _value;

public:
    using value_type = V;
    using unit_type = U;

    Value(const V& value)
        : _value(value)
    {
    }

    V value() const { return _value; }

    Value<V, U> operator+() { return Value<V, U>(+_value); }
    Value<V, U> operator-() { return Value<V, U>(-_value); }

    template <typename V1,
        class V2 = decltype(std::declval<V>() + std::declval<V1>())>
    Value<V2, U> operator+(const Value<V1, U>& rhs)
    {
        return Value<V2, U>(value() + rhs.value());
    }

    template <typename V1,
        class V2 = decltype(std::declval<V>() + std::declval<V1>())>
    Value<V2, U> operator+(const V1& rhs)
    {
        return Value<V2, U>(value() + rhs);
    }

    template <typename V1, class U1,
        class V2 = decltype(std::declval<V>() * std::declval<V1>()),
        class U2 = decltype(U() * U1())>
    Value<V2, U2> operator*(const Value<V1, U1>& rhs)
    {
        return Value<V2, U2>(value() * rhs.value());
    }

    template <typename V1, class U1,
        class V2 = decltype(std::declval<V>() / std::declval<V1>()),
        class U2 = decltype(U() / U1())>
    Value<V2, U2> operator/(const Value<V1, U1>& rhs)
    {
        return Value<V2, U2>(value() / rhs.value());
    }

    template <typename V1 = V>
    operator typename std::enable_if<is_dimensionless<U>::value, V1>::type()
        const
    {
        return _value;
    }
};

template <typename V, class U>
std::ostream& operator<<(std::ostream& os, const Value<V, U>& v)
{
    os << v.value() << U();
    return os;
}

template <> struct is_dimensionless<Dimensionless> : std::true_type {
};

template <typename V>
struct is_dimensionless<Value<V, Dimensionless> > : std::true_type {
};

#define UNITS_VALUE_TYPE_NAME(unit) unit##Value
#define UNITS_DECLARE_VALUE_TYPE(unit, literal)                               \
    template <typename V> using UNITS_VALUE_TYPE_NAME(unit) = Value<V, unit>; \
    UNITS_VALUE_TYPE_NAME(unit)<long double> operator"" _##literal(           \
        long double v)                                                        \
    {                                                                         \
        return UNITS_VALUE_TYPE_NAME(unit)<long double>(v);                   \
    }
#define UNITS_DECLARE_ADDITIONAL_LITERAL(unit, factor, literal)               \
    UNITS_VALUE_TYPE_NAME(unit)<long double> operator"" _##literal(           \
        long double v)                                                        \
    {                                                                         \
        return UNITS_VALUE_TYPE_NAME(unit)<long double>((factor)*v);          \
    }

UNITS_DECLARE_VALUE_TYPE(Meter, m)
UNITS_DECLARE_ADDITIONAL_LITERAL(Meter, 0.01, cm)
UNITS_DECLARE_ADDITIONAL_LITERAL(Meter, 0.1, dm)
UNITS_DECLARE_VALUE_TYPE(Kilogram, kg)
UNITS_DECLARE_ADDITIONAL_LITERAL(Kilogram, 0.001, gr)
UNITS_DECLARE_ADDITIONAL_LITERAL(Kilogram, 1000, tonne)
UNITS_DECLARE_VALUE_TYPE(Newton, N)
UNITS_DECLARE_VALUE_TYPE(Second, s)
UNITS_DECLARE_ADDITIONAL_LITERAL(Second, 60, minute)
UNITS_DECLARE_ADDITIONAL_LITERAL(Second, 3600, hour)
UNITS_DECLARE_VALUE_TYPE(Amper, A)
UNITS_DECLARE_VALUE_TYPE(Candela, Cd)
UNITS_DECLARE_VALUE_TYPE(Mol, Mol)
}
