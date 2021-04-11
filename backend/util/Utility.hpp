#if !defined(UTILITY_HPP)
#define UTILITY_HPP

#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>

using std::string;

const string &CONTENT_TYPE_JSON();
const string &CONTENT_TYPE_PLAIN();
const string &CONTENT_HTML();

// todo user better encrypt method
struct EncryptHelper {
    const static int32_t k = 5;
    string encrypt(const string &text) {
        string res(text.size(), '0');
        for (size_t i = 0; i < text.size(); i++) {
            int32_t v = text[i] + k;
            if (v > INT8_MAX)
                v %= INT8_MAX;
            res[i] = static_cast<char>(v);
        }
        return res;
    }
    string decrypt(const string &text) {
        string res(text.size(), '0');
        for (size_t i = 0; i < text.size(); i++) {
            int32_t v = text[i] - k;
            if (v < 0)
                v += INT8_MAX;
            res[i] = static_cast<char>(v);
        }
        return res;
    }
};

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

/* index tuple at runtime, recursively generating specialization, with only one of witch will invoke
 * fun() at runtime, see
 * https://stackoverflow.com/questions/28997271/c11-way-to-index-tuple-at-runtime-without-using-switch
 */
template <typename F, typename... Ts> void visit_at(std::tuple<Ts...> &tup, size_t idx, F fun) {
    visit_impl<sizeof...(Ts)>::visit(tup, idx, fun);
}

#endif // UTILITY_HPP
