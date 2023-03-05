#pragma once

#include <memory>  // IWYU pragma: export
#include <vector>

namespace JE
{

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename... Args>
inline auto CreateScope(Args&&... args) -> Scope<T>
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
inline auto CreateRef(Args&&... args) -> Ref<T>
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using Vector = std::vector<T>;

}  // namespace JE