enable_testing()

add_executable(unit_test1 MACOSX_BUNDLE test1.cpp player.h enemy.h bubble.h)
target_link_libraries(unit_test1 simplebitmap)
add_test(NAME player_test COMMAND unit_test1)


add_executable(unit_test2 MACOSX_BUNDLE test2.cpp player.h enemy.h bubble.h)
target_link_libraries(unit_test2 simplebitmap)
add_test(NAME enemy_test COMMAND unit_test2)

add_executable(unit_test3 MACOSX_BUNDLE test3.cpp player.h enemy.h bubble.h)
target_link_libraries(unit_test3 simplebitmap)
add_test(NAME bubble_test COMMAND unit_test3)