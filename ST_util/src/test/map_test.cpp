#include <gtest/gtest.h>
#include <ST_util/map.hpp>
#include <algorithm>

//fills a vector with unique random values of type size_t
static std::vector<size_t> fill_vector(size_t amount){
    std::vector<size_t> result;
    for(size_t i = 0; i < amount; i++){
        auto random = static_cast<size_t>(ST::random_int(0xfffffff));
        for(auto j : result) {
            while(random == j || random <= 0){
                random = static_cast<size_t>(ST::random_int(0xfffffff));
            }
        }
        result.emplace_back(random);
    }
    std::sort(result.begin(), result.end());
    for(size_t i = 0; i < result.size()-1; i++){
        if(result[i] == result[i+1]){
            return fill_vector(amount);
        }
    }
    return result;
}

TEST(map_tests, create_map) {
    auto test_map = new ST::map<int*>();
    EXPECT_EQ(true, static_cast<bool>(test_map));
    delete test_map;
}

TEST(map_tests, create_map_on_stack) {
    auto test_map = ST::map<int*>();
    EXPECT_EQ(0, test_map.get_size());
}

TEST(map_tests, add_to_map_on_stack) {
    auto test_map = ST::map<int*>();
    test_map.add(12, new int(3));
    EXPECT_EQ(3, *static_cast<int*>(test_map.get(12)));
    delete test_map.get(12);
}

TEST(map_tests, add_to_map) {
    auto test_map = new ST::map<int*>();
    test_map->add(12, new int(3));
    EXPECT_EQ(3, *static_cast<int*>(test_map->get(12)));
    delete test_map->get(12);
    delete test_map;
}

TEST(map_tests, get_non_existant) {
    auto test_map = new ST::map<int*>();
    EXPECT_EQ(nullptr, static_cast<int*>(test_map->get(32)));
    delete test_map;
}

TEST(map_tests, add_many_to_map_sequential) {
    auto test_map = new ST::map<int*>();

    size_t fill_amount = 0xffff;

    for(size_t i = 1; i < fill_amount; i++){
        test_map->add(i, new int(i));
    }

    for(size_t i = 1; i < fill_amount; i++) {
        ASSERT_EQ(i, *static_cast<int*>(test_map->get(i)));
        delete test_map->get(i);
    }
    delete test_map;
}

TEST(map_tests, add_many_to_map_random) {
    auto test_map = new ST::map<int*>();
    std::vector<size_t> keys;

    size_t fill_amount = 1000;
    keys = fill_vector(fill_amount);

    for(size_t i = 0; i < keys.size(); i++){
        test_map->add(keys.at(i), new int(i));
    }

    for(size_t i = 0; i < keys.size(); i++) {
        ASSERT_EQ(i, *static_cast<int*>(test_map->get(keys.at(i))));
        delete test_map->get(keys.at(i));
    }
    delete test_map;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}