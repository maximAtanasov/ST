#include <gtest/gtest.h>
#include <ST_util/pool_allocator_256.hpp>
#include <future>

TEST(allocate_one, pool_allocator_256_tests){
    ST::pool_allocator_256<uint64_t> allocator;

    uint64_t* test = allocator.allocate();
    ASSERT_TRUE(test);
}

TEST(allocate_max, pool_allocator_256_tests){
    ST::pool_allocator_256<uint64_t> allocator;
    for(uint16_t i = 0; i < 256; i++){
        uint64_t* test = allocator.allocate();
        ASSERT_TRUE(test);
    }
}


TEST(allocate_and_deallocate_max, pool_allocator_256_tests){
    ST::pool_allocator_256<uint64_t> allocator;
    for(uint16_t i = 0; i < 256; i++){
        uint64_t* test = allocator.allocate();
        ASSERT_TRUE(test);
        allocator.deallocate(test);
    }
}

TEST(allocate_and_deallocate_stress_test, pool_allocator_256_tests){
    ST::pool_allocator_256<uint64_t> allocator;

    for(uint32_t j = 0; j < 100000; j++) {
        for (uint16_t i = 0; i < 256; i++) {
            uint64_t *test = allocator.allocate();
            ASSERT_TRUE(test);
            allocator.deallocate(test);
        }
    }

    for(uint32_t j = 0; j < 100000; j++) {
        uint64_t* memory[256];
        for (auto & i : memory) {
            i = allocator.allocate();
            ASSERT_TRUE(i);
        }
        for (auto & i : memory) {
            allocator.deallocate(i);
        }
    }
}

int thread_work(ST::pool_allocator_256<uint64_t>& allocator){
    for(uint32_t j = 0; j < 30000; j++) {
        for (uint16_t i = 0; i < 256; i++) {
            uint64_t *test = allocator.allocate();
            allocator.deallocate(test);
        }
    }
    return 0;
}

TEST(allocate_and_deallocate_multithreaded, pool_allocator_256_tests){
    ST::pool_allocator_256<uint64_t> allocator;

    auto thread_1 = std::async(thread_work, std::ref(allocator));
    auto thread_2 = std::async(thread_work, std::ref(allocator));

    thread_1.get();
    thread_2.get();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}