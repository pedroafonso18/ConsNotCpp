#include "../lib/catch_amalgamated.hpp"
#include "../include/Utils.h"

TEST_CASE("1: CleanCpf must return the clean string without dots and dashes.") {
    REQUIRE( Utils::cleanCpf("000.000.000-00") == "00000000000");
}

TEST_CASE("2: CleanCpf must clean all extra non-numeric digits.") {
    REQUIRE( Utils::cleanCpf("ABC000Ta(((00-0xxz)$$!$$#@000hD00") == "00000000000");
}