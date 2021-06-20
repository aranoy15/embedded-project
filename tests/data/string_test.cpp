//
// Created by afedoseev on 17.06.2021.
//

#include <gtest/gtest.h>

#define protected public
#define private public
#include <lib/data/string.hpp>
#undef protected
#undef private

TEST(string_test, create)
{
    const std::size_t length = 4;
    using string_t = lib::data::string<length>;

    {
        string_t str;

        EXPECT_EQ(str.capacity(), length) << "Capacity not equal length";
        EXPECT_EQ(str.size(), 0) << "Initial size not 0";

        str.append('A');
        str.append('B');

        EXPECT_EQ(str.free(), 2) << "Error calculate free size";
    }

    {
        string_t str("ABC");

        EXPECT_EQ(str.size(), 3) << "Created size not equal actual string size";
    }
}

TEST(string_test, copy)
{
    using namespace lib::data;

    {
        string<3> to_copy("");
        string<4> from_copy("AB");

        to_copy = from_copy;

        EXPECT_STREQ(to_copy.as_str(), "AB") << "Error copy operator";
    }

    {
        string<4> from_copy("AB");
        string<3> to_copy(from_copy);

        EXPECT_STREQ(to_copy.as_str(), "AB")
            << "Error copy constructor by string class";
    }

    {
        const char* from_copy = "AB";
        string<3> to_copy;

        to_copy = from_copy;

        EXPECT_STREQ(to_copy.as_str(), "AB") << "Error copy operator for str";
    }

    {
        const char* from_copy = "AB";
        string<3> to_copy(from_copy);

        EXPECT_STREQ(to_copy.as_str(), "AB") << "Error copy constructor by str";
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

TEST(string_test, operators)
{
    using namespace lib::data;
    const std::size_t size = 3;

    {
        string<size> data;
        data = data + 'A';
        data = data + 'B';

        EXPECT_STREQ(data.as_str(), "AB");
    }

    {
        string<size> data;
        data = 'A' + data;
        data = 'B' + data;

        EXPECT_STREQ(data.as_str(), "AB");
    }

    {
        string<size> data;
        data = data + "A";
        data = data + "B";

        EXPECT_STREQ(data.as_str(), "AB");
    }

    {
        string<size> data;
        data = "A" + data;
        data = "B" + data;

        EXPECT_STREQ(data.as_str(), "AB");
    }

    {
        string<size> data;
        string<2> data_append_1("A");
        string<2> data_append_2("B");

        data = data + data_append_1;
        data = data + data_append_2;

        EXPECT_STREQ(data.as_str(), "AB");
    }
}

TEST(string_test, starts_with)
{
    using namespace lib::data;

    {
        string<3> data("A");
        string<3> empty_data;

        EXPECT_TRUE(data.starts_with('A'));
        EXPECT_FALSE(data.starts_with('B'));
        EXPECT_FALSE(empty_data.starts_with('A'));
    }

    {
        string<3> data("AB");
        string<3> empty_data;
        const char* compare_data_1 = "ABC";
        const char* compare_data_2 = "AB";

        EXPECT_FALSE(data.starts_with(compare_data_1));
        EXPECT_TRUE(data.starts_with(compare_data_2));
        EXPECT_FALSE(empty_data.starts_with(compare_data_2));
    }

    {
        string<6> data("ABCDE");
        string<4> compare_data_1("ABC");
        string<4> compare_data_2("ABE");

        EXPECT_TRUE(data.starts_with(compare_data_1));
        EXPECT_FALSE(data.starts_with(compare_data_2));
    }
}

TEST(string_test, contains)
{
    using namespace lib::data;
    {
        string<8> data("ABCDEFG");
        const char* contains_data_1 = "BC";
        const char* contains_data_2 = "XZ";
        const char* contains_data_3 = "GH";

        EXPECT_TRUE(data.contains(contains_data_1, 2));
        EXPECT_FALSE(data.contains(contains_data_2, 2));
        EXPECT_FALSE(data.contains(contains_data_3, 2));
    }

    {
        string<3> data("ABC");
        string<0> empty_data;

        EXPECT_TRUE(data.contains('A'));
        EXPECT_TRUE(data.contains('B'));
        EXPECT_TRUE(data.contains('C'));
        EXPECT_FALSE(data.contains('F'));
        EXPECT_FALSE(data.contains('e'));
        EXPECT_FALSE(empty_data.contains("A"));
    }

    {
        string<16> data("Hello, there!!!");

        EXPECT_TRUE(data.contains("Hello"));
        EXPECT_TRUE(data.contains(", "));
        EXPECT_TRUE(data.contains("there!!!"));
        EXPECT_FALSE(data.contains("minute"));
        EXPECT_FALSE(data.contains("thar"));
        EXPECT_FALSE(data.contains("lla"));
    }

    {
        string<16> data("Hello, there!!!");

        EXPECT_TRUE(data.contains(string<10>("Hello")));
        EXPECT_TRUE(data.contains(string<10>(", ")));
        EXPECT_TRUE(data.contains(string<10>("there!!!")));
        EXPECT_FALSE(data.contains(string<10>("minute")));
        EXPECT_FALSE(data.contains(string<10>("thar")));
        EXPECT_FALSE(data.contains(string<10>("lla")));
    }
}

TEST(string_test, erase)
{
    using namespace lib::data;

    string<32> data("Hello, dear friends!!!");

    data.erase(7, 5);
    EXPECT_STREQ(data.as_str(), "Hello, friends!!!");

    data.erase(14, 4);
    EXPECT_STREQ(data.as_str(), "Hello, friends");

    data.erase(0, 7);
    EXPECT_STREQ(data.as_str(), "friends");
}

TEST(string_test, equal)
{
    using namespace lib::data;
    {
        string<3> data("A");
        string<3> wrong_data("AB");
        string<3> empty_data;

        EXPECT_TRUE(data.equal('A'));
        EXPECT_FALSE(data.equal('B'));
        EXPECT_FALSE(wrong_data.equal('A'));
        EXPECT_FALSE(empty_data.equal('A'));

        EXPECT_TRUE(data == 'A' and 'A' == data);
        EXPECT_FALSE(data == 'B' or 'B' == data);
        EXPECT_FALSE(wrong_data == 'A' or 'A' == wrong_data);
        EXPECT_FALSE(empty_data == 'A' or 'A' == empty_data);
    }

    {
        string<16> data("ABC EFG");
        const char* equal_str = "ABC EFG";
        const char* not_equal_str = "ABK EFG ";
        string<10> equal_data(equal_str);
        string<10> not_equal_data(not_equal_str);

        EXPECT_TRUE(data.equal(equal_str));
        EXPECT_FALSE(data.equal(not_equal_str));
        EXPECT_TRUE(data.equal(equal_data));
        EXPECT_FALSE(data.equal(not_equal_data));

        EXPECT_TRUE(data == equal_str and equal_str == data);
        EXPECT_FALSE(data == not_equal_str or not_equal_str == data);
        EXPECT_TRUE(data == equal_data);
        EXPECT_FALSE(data == not_equal_data);
    }
}
