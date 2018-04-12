#include <gtest/gtest.h>
#include <ST_util/map.hpp>

TEST(map_tests, create_map) {
    auto test_map = new ST::map<int*>();
    EXPECT_EQ(true, static_cast<bool>(test_map));
}

TEST(map_tests, add_to_map) {
    auto test_map = new ST::map<int*>();
    test_map->add(12, new int(3));
    EXPECT_EQ(3, *test_map->get(12));
}

/*TEST(map_tests, add_many_to_map) {
    auto test_map = new ST::map<int*>();
    std::vector<size_t> keys;

    for(int i = 0; i < 10; i++){
        keys.emplace_back(ST::random_int(99999));
    }

    for(int i = 0; i < keys.size(); i++){
        test_map->add(keys.at(i), new int(i));
    }

    for(int i = 0; i < keys.size(); i++){
        ASSERT_EQ(i, *test_map->get(keys.at(i)));
    }
}*/

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}