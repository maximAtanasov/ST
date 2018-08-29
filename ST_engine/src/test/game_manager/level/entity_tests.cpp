/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */


#include <gtest/gtest.h>
#include <game_manager/level/entity.hpp>

TEST(entity_tests, test_set_collisions_box){
    //Set up
    ST::entity test_subject(0);

    //Test
    test_subject.set_collision_box(0,0,0,0);

    ASSERT_FALSE(test_subject.is_affected_by_physics);
    ASSERT_EQ(0, test_subject.get_col_x());
    ASSERT_EQ(0, test_subject.get_col_y());
}

TEST(entity_tests, test_entities_collision){
    //Set up
    ST::entity test_subject1(0);
    ST::entity test_subject2(1);

    test_subject1.x = 0;
    test_subject1.y = 0;
    test_subject1.set_collision_box(0, 0, 100, 100);

    test_subject2.x = 0;
    test_subject2.y = 0;
    test_subject2.set_collision_box(0, 0, 100, 100);

    //Test
    ASSERT_TRUE(test_subject1.collides(test_subject2));
}

TEST(entity_tests, test_entities_colliding_on_edge){
    //Set up
    ST::entity test_subject1(0);
    ST::entity test_subject2(1);

    test_subject1.x = 0;
    test_subject1.y = 0;
    test_subject1.set_collision_box(0, 0, 100, 100);

    test_subject2.x = 100;
    test_subject2.y = 100;
    test_subject2.set_collision_box(0, 0, 100, 100);

    //Test
    ASSERT_TRUE(test_subject1.collides(test_subject2));
}

TEST(entity_tests, test_entities_colliding_one_pixel_from_edge){
    //Set up
    ST::entity test_subject1(0);
    ST::entity test_subject2(1);

    test_subject1.x = 0;
    test_subject1.y = 0;
    test_subject1.set_collision_box(0, 0, 100, 100);

    test_subject2.x = 99;
    test_subject2.y = 99;
    test_subject2.set_collision_box(0, 0, 100, 100);

    //Test
    ASSERT_TRUE(test_subject1.collides(test_subject2));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}