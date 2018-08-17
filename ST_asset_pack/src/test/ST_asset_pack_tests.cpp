/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#define TESTING

#include <main.cpp>
#include <gtest/gtest.h>

TEST(ST_asset_pack_tests, test_wrong_argument_count){

    testing::internal::CaptureStderr();

    auto args = static_cast<char**>(malloc(4));
    args[0] = const_cast<char*>("a");
    args[1] = const_cast<char*>("b");
    ASSERT_EQ(-1, asset_pack_main(2, args));

    std::string output = testing::internal::GetCapturedStderr();
    ASSERT_EQ("Not enough arguments!\n", output);
}

TEST(ST_asset_pack_tests, test_pack_to_binary){

    testing::internal::CaptureStdout();

    auto args = static_cast<char**>(malloc(50));
    args[0] = const_cast<char*>("");
    args[1] = const_cast<char*>("-p");
    args[2] = const_cast<char*>("no_asset");
    ASSERT_EQ(0, asset_pack_main(3, args));

    std::string output = testing::internal::GetCapturedStdout();
    remove("no_asset");
    ASSERT_EQ("Binary generated!\n", output);
}

TEST(ST_asset_pack_tests, test_unpack_binary){

    testing::internal::CaptureStdout();

    auto args = static_cast<char**>(malloc(50));
    args[0] = const_cast<char*>("");
    args[1] = const_cast<char*>("-u");
    args[2] = const_cast<char*>("no_asset");
    ASSERT_EQ(0, asset_pack_main(3, args));

    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("Binary unpacked!\n", output);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}