// Copyright Daniel Wallin 2009. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <luabind/luabind.hpp>
#include <luabind/luabind_memory.hpp>

namespace {

struct Base
{
    Base(int value_)
      : value(value_)
    {}

    virtual ~Base()
    {}

    int g() const
    {
        return value;
    }

    int value;
};

struct Derived : Base
{
    Derived()
      : Base(2)
    {}

    int f() const
    {
        return 1;
    }
};

struct Unregistered : Base
{
    Unregistered()
      : Base(3)
    {}
};

luabind::unique_ptr<Base> make_derived()
{
    return luabind::unique_ptr<Base>(new Derived);
}

luabind::unique_ptr<Base> make_unregistered()
{
    return luabind::unique_ptr<Base>(new Unregistered);
}

} // namespace unnamed

void test_main(lua_State* L)
{
    using namespace luabind;

    module(L) [
        class_<Base>("Base")
            .def("g", &Base::g),
        class_<Derived, Base>("Derived")
            .def("f", &Derived::f),
        def("make_derived", &make_derived),
        def("make_unregistered", &make_unregistered)
    ];

    DOSTRING(L,
        "x = make_derived()\n"
        "assert(x:f() == 1)\n"
    );

    DOSTRING(L,
        "x = make_unregistered()\n"
        "assert(x:g() == 3)\n"
    );
}
