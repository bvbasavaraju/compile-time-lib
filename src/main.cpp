#include <container/algorithms.hpp>
#include <container/list.hpp>
#include <debug/show_type.hpp>
#include <iostream>

struct super_type {
    auto attendence() const {
        std::cout << "Present" << std::endl;
    }
    
    virtual void name() const = 0;
};
struct type1 : public super_type {
    void name() const {
        std::cout << "type1: ";
    }

    auto operator()() const -> bool {
        return false;
    }
};
struct type2 : public super_type {
    void name() const {
        std::cout << "type2: ";
    }

    auto operator()() const -> bool {
        return true;
    }
};

int main() {
    std::cout << "This is CompileTimeLibrary main" << std::endl;

    using empty_list = ctl::list<>;
    using types1 = ctl::append_t<empty_list, type1>;
    using types_all = ctl::append_t<types1, type2>;

    static_assert(std::is_same_v<types_all, ctl::list<type1, type2> >,
                  "Type1 or Type2 are not in list");

    // instantiable list
    using itypes_all = ctl::ilist<type1, type2>;
    itypes_all ita{};

    {
        // Callable list
        using ctypes_all = ctl::clist<type1, type2>;
        ctypes_all{}();

        // with predicate!
        constexpr auto fn = [](auto const& obj) {
            obj.name();
            obj.attendence();
        };

        ctypes_all{}(fn);

        // Operator overloading
        using numList = ctl::clist<std::integral_constant<uint32_t, 1>,std::integral_constant<uint32_t, 2>, std::integral_constant<uint32_t, 3>>;
        std::cout << "operator|: " << (numList{} | 0) << std::endl;
        std::cout << "operator&: " << (numList{} & 0xFF) << std::endl;
        std::cout << "operator^: " << (numList{} ^ 0) << std::endl;
        std::cout << "operator^: " << (numList{} ^ 0xFF) << std::endl;
        std::cout << "operator||: " << (ctypes_all{} || false) << std::endl;
        std::cout << "operator&&: " << (ctypes_all{} && true) << std::endl; 
        std::cout << "operator+: " << (numList{} + 10) << std::endl;
        std::cout << "operator-: " << (numList{} - 10) << std::endl;
        std::cout << "operator*: " << (numList{} * 10) << std::endl;
    }

    return 0;
}
