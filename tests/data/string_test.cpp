//
// Created by afedoseev on 17.06.2021.
//

#include <gtest/gtest.h>
#include <lib/data/string.hpp>

TEST(string_test, create)
{
    const std::size_t length = 16;
    using string_t = lib::data::string<length>;

    string_t str;

    EXPECT_EQ(str.capacity(), length) << "Capacity not equal length";
    EXPECT_EQ(str.size(), 0) << "Initial size not 0";
}

TEST(string_test, copy)
{
    using namespace lib::data;

    {
        string<3> to_copy = "";
        string<4> from_copy = "AB";

        to_copy = from_copy;

        EXPECT_STREQ(to_copy.as_str(), "AB");
    }

    {
        string<4> from_copy = "AB";
        string<3> to_copy(from_copy);

        EXPECT_STREQ(to_copy.as_str(), "AB");
    }

    {

    }
}

TEST(string_test, append)
{
    const std::size_t length = 3;
    using string_t = lib::data::string<length>;

    string_t str;

    str.append('A');

    const char* data = str.as_str();

    EXPECT_EQ(data[0], 'A');
    EXPECT_EQ(data[1], '\0');

    str.append('B');

    EXPECT_EQ(data[0], 'A');
    EXPECT_EQ(data[1], 'B');
    EXPECT_EQ(data[2], '\0');

    str.clear();

    const char* str_append = "CD";
    str.append(str_append);

    EXPECT_EQ(data[0], 'C');
    EXPECT_EQ(data[1], 'D');
    EXPECT_EQ(data[2], '\0');

    str.clear();

    lib::data::string<4> test_data("EF");

    str.append(test_data);

    EXPECT_EQ(data[0], 'E');
    EXPECT_EQ(data[1], 'F');
    EXPECT_EQ(data[2], '\0');
}
