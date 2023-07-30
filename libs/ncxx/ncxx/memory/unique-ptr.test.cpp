#include <ncxx/memory/unique-ptr.hpp>

#include <catch2/catch_test_macros.hpp>

namespace NOS {

TEST_CASE("UniquePtr object delete on exit scope", "[memory]")
{
    static bool dtorCalled = false;

    struct DtorObject
    {
        ~DtorObject() { dtorCalled = true; }
    };

    {
        UniquePtr<DtorObject> ptr{new DtorObject()};
    }

    CHECK(dtorCalled);
}

} // namespace NOS
