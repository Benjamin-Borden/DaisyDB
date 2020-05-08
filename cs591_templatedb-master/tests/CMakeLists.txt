add_executable(basic_test basic_test.cpp)
add_executable(persistence_test persistence_test.cpp)

add_executable(10000_tiered_50_50 10000/10000_tiered_50_50.cpp)
add_executable(10000_leveled_50_50 10000/10000_leveled_50_50.cpp)
add_executable(10000_swap_50_50 10000/10000_swap_50_50.cpp)
add_executable(10000_tiered_20_80 10000/10000_tiered_20_80.cpp)
add_executable(10000_leveled_20_80 10000/10000_leveled_20_80.cpp)
add_executable(10000_swap_20_80 10000/10000_swap_20_80.cpp)
add_executable(10000_tiered_80_20 10000/10000_tiered_80_20.cpp)
add_executable(10000_leveled_80_20 10000/10000_leveled_80_20.cpp)
add_executable(10000_swap_80_20 10000/10000_swap_80_20.cpp)

add_executable(50000_tiered_50_50 50000/50000_tiered_50_50.cpp)
add_executable(50000_leveled_50_50 50000/50000_leveled_50_50.cpp)
add_executable(50000_swap_50_50 50000/50000_swap_50_50.cpp)
add_executable(50000_tiered_20_80 50000/50000_tiered_20_80.cpp)
add_executable(50000_leveled_20_80 50000/50000_leveled_20_80.cpp)
add_executable(50000_swap_20_80 50000/50000_swap_20_80.cpp)
add_executable(50000_tiered_80_20 50000/50000_tiered_80_20.cpp)
add_executable(50000_leveled_80_20 50000/50000_leveled_80_20.cpp)
add_executable(50000_swap_80_20 50000/50000_swap_80_20.cpp)

add_executable(100000_tiered_50_50 100000/100000_tiered_50_50.cpp)
add_executable(100000_leveled_50_50 100000/100000_leveled_50_50.cpp)
add_executable(100000_swap_50_50 100000/100000_swap_50_50.cpp)
add_executable(100000_tiered_20_80 100000/100000_tiered_20_80.cpp)
add_executable(100000_leveled_20_80 100000/100000_leveled_20_80.cpp)
add_executable(100000_swap_20_80 100000/100000_swap_20_80.cpp)
add_executable(100000_tiered_80_20 100000/100000_tiered_80_20.cpp)
add_executable(100000_leveled_80_20 100000/100000_leveled_80_20.cpp)
add_executable(100000_swap_80_20 100000/100000_swap_80_20.cpp)







target_link_libraries(basic_test PUBLIC templatedb gtest)
target_link_libraries(persistence_test PUBLIC templatedb gtest)

target_link_libraries(10000_tiered_50_50 PUBLIC templatedb gtest)
target_link_libraries(10000_leveled_50_50 PUBLIC templatedb gtest)
target_link_libraries(10000_swap_50_50 PUBLIC templatedb gtest)
target_link_libraries(10000_tiered_80_20 PUBLIC templatedb gtest)
target_link_libraries(10000_leveled_80_20 PUBLIC templatedb gtest)
target_link_libraries(10000_swap_80_20 PUBLIC templatedb gtest)
target_link_libraries(10000_tiered_20_80 PUBLIC templatedb gtest)
target_link_libraries(10000_leveled_20_80 PUBLIC templatedb gtest)
target_link_libraries(10000_swap_20_80 PUBLIC templatedb gtest)

target_link_libraries(50000_tiered_50_50 PUBLIC templatedb gtest)
target_link_libraries(50000_leveled_50_50 PUBLIC templatedb gtest)
target_link_libraries(50000_swap_50_50 PUBLIC templatedb gtest)
target_link_libraries(50000_tiered_80_20 PUBLIC templatedb gtest)
target_link_libraries(50000_leveled_80_20 PUBLIC templatedb gtest)
target_link_libraries(50000_swap_80_20 PUBLIC templatedb gtest)
target_link_libraries(50000_tiered_20_80 PUBLIC templatedb gtest)
target_link_libraries(50000_leveled_20_80 PUBLIC templatedb gtest)
target_link_libraries(50000_swap_20_80 PUBLIC templatedb gtest)

target_link_libraries(100000_tiered_50_50 PUBLIC templatedb gtest)
target_link_libraries(100000_leveled_50_50 PUBLIC templatedb gtest)
target_link_libraries(100000_swap_50_50 PUBLIC templatedb gtest)
target_link_libraries(100000_tiered_80_20 PUBLIC templatedb gtest)
target_link_libraries(100000_leveled_80_20 PUBLIC templatedb gtest)
target_link_libraries(100000_swap_80_20 PUBLIC templatedb gtest)
target_link_libraries(100000_tiered_20_80 PUBLIC templatedb gtest)
target_link_libraries(100000_leveled_20_80 PUBLIC templatedb gtest)
target_link_libraries(100000_swap_20_80 PUBLIC templatedb gtest)









add_test(NAME basic_test COMMAND ${CMAKE_BINARY_DIR}/tests/basic_test)
add_test(NAME persistence_test COMMAND ${CMAKE_BINARY_DIR}/tests/persistence_test)

add_test(NAME 10000_tiered_50_50 COMMAND ${CMAKE_BINARY_DIR}/tests/10000_tiered_50_50)
add_test(NAME 10000_leveled_50_50 COMMAND ${CMAKE_BINARY_DIR}/tests/10000_leveled_50_50)
add_test(NAME 10000_swap_50_50 COMMAND ${CMAKE_BINARY_DIR}/tests/10000_swap_50_50)
add_test(NAME 10000_tiered_80_20 COMMAND ${CMAKE_BINARY_DIR}/tests/10000_tiered_80_20)
add_test(NAME 10000_leveled_80_20 COMMAND ${CMAKE_BINARY_DIR}/tests/10000_leveled_80_20)
add_test(NAME 10000_swap_80_20 COMMAND ${CMAKE_BINARY_DIR}/tests/10000_swap_80_20)
add_test(NAME 10000_tiered_20_80 COMMAND ${CMAKE_BINARY_DIR}/tests/10000_tiered_20_80)
add_test(NAME 10000_leveled_20_80 COMMAND ${CMAKE_BINARY_DIR}/tests/10000_leveled_20_80)
add_test(NAME 10000_swap_20_80 COMMAND ${CMAKE_BINARY_DIR}/tests/10000_swap_20_80)

add_test(NAME 50000_tiered_50_50 COMMAND ${CMAKE_BINARY_DIR}/tests/50000_tiered_50_50)
add_test(NAME 50000_leveled_50_50 COMMAND ${CMAKE_BINARY_DIR}/tests/50000_leveled_50_50)
add_test(NAME 50000_swap_50_50 COMMAND ${CMAKE_BINARY_DIR}/tests/50000_swap_50_50)
add_test(NAME 50000_tiered_80_20 COMMAND ${CMAKE_BINARY_DIR}/tests/50000_tiered_80_20)
add_test(NAME 50000_leveled_80_20 COMMAND ${CMAKE_BINARY_DIR}/tests/50000_leveled_80_20)
add_test(NAME 50000_swap_80_20 COMMAND ${CMAKE_BINARY_DIR}/tests/50000_swap_80_20)
add_test(NAME 50000_tiered_20_80 COMMAND ${CMAKE_BINARY_DIR}/tests/50000_tiered_20_80)
add_test(NAME 50000_leveled_20_80 COMMAND ${CMAKE_BINARY_DIR}/tests/50000_leveled_20_80)
add_test(NAME 50000_swap_20_80 COMMAND ${CMAKE_BINARY_DIR}/tests/50000_swap_20_80)

add_test(NAME 100000_tiered_50_50 COMMAND ${CMAKE_BINARY_DIR}/tests/100000_tiered_50_50)
add_test(NAME 100000_leveled_50_50 COMMAND ${CMAKE_BINARY_DIR}/tests/100000_leveled_50_50)
add_test(NAME 100000_swap_50_50 COMMAND ${CMAKE_BINARY_DIR}/tests/100000_swap_50_50)
add_test(NAME 100000_tiered_80_20 COMMAND ${CMAKE_BINARY_DIR}/tests/100000_tiered_80_20)
add_test(NAME 100000_leveled_80_20 COMMAND ${CMAKE_BINARY_DIR}/tests/100000_leveled_80_20)
add_test(NAME 100000_swap_80_20 COMMAND ${CMAKE_BINARY_DIR}/tests/100000_swap_80_20)
add_test(NAME 100000_tiered_20_80 COMMAND ${CMAKE_BINARY_DIR}/tests/100000_tiered_20_80)
add_test(NAME 100000_leveled_20_80 COMMAND ${CMAKE_BINARY_DIR}/tests/100000_leveled_20_80)
add_test(NAME 100000_swap_20_80 COMMAND ${CMAKE_BINARY_DIR}/tests/100000_swap_20_80)
