#include "../lib/catch2/catch.hpp"
#include "../include/Utils.h"

TEST_CASE("1: Is allowed time must return TRUE in the time i'm testing.") {
    REQUIRE( Utils::IsAllowedTime() == true);
}