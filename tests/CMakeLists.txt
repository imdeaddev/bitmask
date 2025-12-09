include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(Threads REQUIRED)

add_executable(deaddev_bitmask_tests tests.cpp)
target_link_libraries(deaddev_bitmask_tests PRIVATE Threads::Threads GTest::gtest GTest::gtest_main deaddev::bitmask)

include(GoogleTest)
gtest_discover_tests(deaddev_bitmask_tests)