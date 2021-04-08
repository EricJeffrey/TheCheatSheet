#if !defined(UTILITY_HPP)
#define UTILITY_HPP

#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>

using std::string;

// todo move to another better place
enum SortOrder { favorNumber, lastModified };

const string &CONTENT_TYPE_JSON();

const string &CONTENT_TYPE_PLAIN();

std::optional<SortOrder> SortOrderKeyToOrder(const string &key);

template <size_t I> struct visit_impl {
    template <typename T, typename F> static void visit(T &tup, size_t idx, F fun) {
        if (idx == I - 1)
            fun(std::get<I - 1>(tup));
        else
            visit_impl<I - 1>::visit(tup, idx, fun);
    }
};

template <> struct visit_impl<0> {
    template <typename T, typename F> static void visit(T &tup, size_t idx, F fun) {
        // static_assert(true);
    }
};

template <typename F, typename... Ts>
void visit_at(std::tuple<Ts...> const &tup, size_t idx, F fun) {
    visit_impl<sizeof...(Ts)>::visit(tup, idx, fun);
}

// this will keep recursive and generating I specialization, and only one of these specialization
// will invoke fun() at runtime
template <typename F, typename... Ts> void visit_at(std::tuple<Ts...> &tup, size_t idx, F fun) {
    visit_impl<sizeof...(Ts)>::visit(tup, idx, fun);
}

#endif // UTILITY_HPP
