#pragma once

#include <tuple>

template<std::size_t I, typename Seq> struct offset_sequence;

template<std::size_t I, std::size_t... Is>
struct offset_sequence<I, std::index_sequence<Is...>> {
    using type = std::index_sequence<I + Is...>;
};

template<std::size_t I, std::size_t N>
using make_offset_sequence = typename offset_sequence<I, std::make_index_sequence<N>>::type;

template<std::size_t I, typename Tuple>
auto& Get(Tuple&& tuple) {
    return std::get<I>(tuple);
}

template<typename... Ts, std::size_t... I>
auto Get(const std::tuple<Ts...>& tuple, std::index_sequence<I...>) {
    return std::tie(Get<I>(tuple)...);
}

template<std::size_t N, typename... Ts>
auto Drop(const std::tuple<Ts...>& tuple) {
    constexpr auto n = sizeof...(Ts);
    return Get(tuple, make_offset_sequence<N, n - N>{});
}

template<typename... Ts>
auto Tail(const std::tuple<Ts...>& tuple) {
    return Drop<1>(tuple);
}
