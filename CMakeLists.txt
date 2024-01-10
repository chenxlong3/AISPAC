cmake_minimum_required(VERSION 3.5)		# 设CMake最小版本号

project(link_rec)					# 设置工程名
include_directories(include)

set(CMAKE_BUILD_TYPE Release)
add_compile_options(-O3)
add_executable(format_graph exec_files/main.cpp include/SFMT/dSFMT/dSFMT.c)	# 生成可执行文件
add_executable(eval_by_mc exec_files/evaluate.cpp include/SFMT/dSFMT/dSFMT.c)
add_executable(IMA_only exec_files/IMA_only.cpp include/SFMT/dSFMT/dSFMT.c)
add_executable(MCGreedy_only exec_files/MCGreedy_only.cpp include/SFMT/dSFMT/dSFMT.c)
add_executable(RW exec_files/RW.cpp include/SFMT/dSFMT/dSFMT.c)

# target_link_libraries(link_rec ${Boost_LIBRARIES})
target_compile_features(format_graph PRIVATE cxx_std_11)
target_compile_features(IMA_only PRIVATE cxx_std_11)