/*
 * @ Created by jaehyung409 on 25. 9. 24..
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License.
 */

module;
#include <concepts>
#include <functional>
#include <type_traits>

#if defined(__clang__)
export module j:concepts;
#else
module j:concepts;
#endif

namespace j {
template <class T>
concept NumericKeyTraits =
    std::is_arithmetic_v<typename T::key_type> &&
    (std::is_same_v<typename T::key_compare, std::less<typename T::key_type>> ||
     std::is_same_v<typename T::key_compare, std::greater<typename T::key_type>> ||
     std::is_same_v<typename T::key_compare, std::less<>> || std::is_same_v<typename T::key_compare, std::greater<>>);

template <class K, class KeyType, class Comparator>
concept IsTransparentlyComparable = requires(const K &k, const KeyType &key, const Comparator &comp) {
    { comp(k, key) } -> std::convertible_to<bool>;
    { comp(key, k) } -> std::convertible_to<bool>;
};

template <class Traits, class SourceTree>
concept IsMergeable = std::is_same_v<typename Traits::key_type, typename SourceTree::key_type> &&
                      std::is_same_v<typename Traits::value_type, typename SourceTree::value_type> &&
                      std::is_same_v<typename Traits::key_compare, typename SourceTree::key_compare> &&
                      std::is_same_v<typename Traits::allocator_type, typename SourceTree::allocator_type>;

namespace detail {
template <class TR, class K, class M>
concept InsertOrAssignConstraint = !TR::_IS_SET && std::constructible_from<typename TR::value_type, K &&, M &&> &&
                                   std::assignable_from<typename TR::mapped_type &, M> &&
                                   IsTransparentlyComparable<K, typename TR::key_type, typename TR::key_compare>;

template <class TR, class K, class... Args>
concept TryEmplaceConstraint =
    !TR::_IS_SET && !TR::_MULTI && std::constructible_from<typename TR::mapped_type, Args...> &&
    IsTransparentlyComparable<K, typename TR::key_type, typename TR::key_compare>;
} // namespace detail
} // namespace j