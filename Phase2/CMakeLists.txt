cmake_minimum_required(VERSION 3.16)

project(SteamClone LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(steam_clone
    src/main.cpp
 "src/User.h" "src/User.cpp"  "src/Session.h" "src/Session.cpp" "src/GameListing.h" "src/GameListing.cpp"
 "src/Format.h" "src/Format.cpp" "src/UserAccounts.h" "src/UserAccounts.cpp" "src/GameCatalog.h" "src/GameCatalog.cpp"
 "src/TransactionLog.h" "src/TransactionLog.cpp" "src/FrontEnd.h" "src/FrontEnd.cpp")

if(MSVC)
    target_compile_options(steam_clone PRIVATE /W4)
else()
    target_compile_options(steam_clone PRIVATE -Wall -Wextra -Wpedantic)
endif()
