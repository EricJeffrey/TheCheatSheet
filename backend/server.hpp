#if !defined(SERVER_HPP)
#define SERVER_HPP

#include <optional>
#include <tuple>

// helper template for the parameter type of StrController.handler
template <typename... Args> using ParamsType = std::tuple<std::optional<Args>...>;

void startServer();

#endif // SERVER_HPP
