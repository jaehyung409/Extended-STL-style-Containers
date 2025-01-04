//
// Created by jaehy on 25. 1. 4.
//

#ifndef DATASTRUCTURE_DEFAULT_VALUE_H
#define DATASTRUCTURE_DEFAULT_VALUE_H

#include <type_traits>

template <typename T>
struct DefaultValue {
    static constexpr T value = std::conditional_t<std::is_arithmetic<T>::value, std::integral_constant<T, 0>, std::integral_constant<T*, nullptr>>::value;
};

#endif //DATASTRUCTURE_DEFAULT_VALUE_H
