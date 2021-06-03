cmake_minimum_required(VERSION 3.19)
project(TecRoyale C)
add_executable(TecRoyale
        main.c
        Table.c
        Table.h
        Warrior.c
        Warrior.h thread_structure.c thread_structure.h lottery_sched.c lottery_sched.h my_mutex.c my_mutex.h my_thread.c my_thread.h realtime_sched.c realtime_sched.h roundRobin_sched.c roundRobin_sched.h ini.c ini.h )
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "-lncurses")
find_package(Curses REQUIRED)
set(CURSES_NEED_NCURSES TRUE)

if (CURSES_FOUND AND CURSES_HAVE_NCURSES_H)
    include_directories(${CURSES_INCLUDE_DIR})
    target_link_libraries (TecRoyale ${CURSES_LIBRARIES})
endif()
include_directories(${CURSES_INCLUDE_DIR})
