cmake_minimum_required(VERSION 3.19)
project(TEC_Royale)



set(CMAKE_C_STANDARD 11)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
add_executable(TEC_Royale
        DataStructures.c
        DataStructures.h
        errorCodes.h
        my_thread.c
        my_thread.h
        Warrior.c
        Warrior.h
        roundRobin_sched.c main.c roundRobin_sched.h lottery_sched.h lottery_sched.c realtime_sched.c realtime_sched.h)
target_link_libraries(TEC_Royale ${CURSES_LIBRARIES})