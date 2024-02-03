cmake_minimum_required(VERSION 3.20)
project(sppl_homework_05)
set(PostgreSQL_ADDITIONAL_VERSIONS "16")
add_executable(sppl_homework_05 main.cpp)
add_subdirectory("C:\\Users\\Макс\\MVS  projects\\repos\\libpqxx-7.8.1" libpqxx-build) # Укажите путь к libpq++
target_compile_features(sppl_homework_05 PRIVATE cxx_std_17) # Включите C++17
target_link_libraries(sppl_homework_05 pqxx) 