#ifndef __clang__
#include <concepts>
#else
namespace std {
template<class Derived, class Base>
concept derived_from =
    std::is_base_of_v<Base, Derived>&& std::is_convertible_v<const volatile Derived*, const volatile Base*>;
}
#endif
