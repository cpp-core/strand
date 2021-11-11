// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

// Compose generator **G** with operation **Op** returning
// **`Op`(`G`)**. This facilitates chaining operations that accept a
// single generator as input:
//
// *sampler<int>(0, 100) | take(4) | collect<std::vector>()*
template<class G, class Op>
requires is_generator_v<G>
auto operator|(G&& g, Op&& op) {
    return op(std::forward<G>(g));
}

// Compose tuple of generators **Gs** with operation **Op** returning
// **Op(Gs...)**. This facilitates chaining operations that accept
// more than one generator as input (e.g. zip).
//
// *sampler<int>(0, 10) ^ sampler<int>(0, 5) | zip() | take(5)* 
template<class Op, class... Gs>
requires (is_generator_v<Gs> && ...)
auto operator|(std::tuple<Gs...>&& tup, Op&& op) {
    return op(std::forward<std::tuple<Gs...>>(tup));
}

// Compose generators **G** and **H** into a tuple. This facilitates integrating
// multiple generators into a chain.
//
// *sampler<int>(0, 10) ^ sampler<int>(0, 5) | zip() | take(5)* 
template<class G, class H>
requires (is_generator_v<G> && is_generator_v<H>)
auto operator^(G&& g, H&& h) {
    return std::tuple{std::forward<G>(g), std::forward<H>(h)};
}

// Compose tuple of generators **Gs** and generator **G** into a new
// tuple. This facilitates integrating multiple generators into a
// chain.
//
// *sampler<int>(0, 10) ^ sampler<int>(0, 5) ^ sampler<int>(-10, 0) | zip() | take(5)* 
template<class G, class... Gs>
requires (is_generator_v<G> && (is_generator_v<Gs> && ...))
auto operator^(std::tuple<Gs...>&& tup, G&& g) {
    if constexpr (sizeof...(Gs) == 1)
        return std::tuple{std::move(std::get<0>(tup)),
	    std::forward<G>(g)};
    else if constexpr (sizeof...(Gs) == 2)
        return std::tuple{std::move(std::get<0>(tup)),
	    std::move(std::get<1>(tup)),
	    std::forward<G>(g)};
}

}; // coro
