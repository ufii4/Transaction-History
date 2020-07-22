cmake_minimum_required(VERSION 3.16)
project(Transaction_History)

set(CMAKE_CXX_STANDARD 14)

add_executable(Transaction_History src/main.cpp src/Transaction.cpp src/Transaction.h src/History.cpp src/History.h src/ECE150_Project3.h)