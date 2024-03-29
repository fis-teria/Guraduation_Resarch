cmake_minimum_required(VERSION 3.0)
project(Guraduation_Resarch VERSION 1.1.0 LANGUAGES CXX)
# ビルドフラグの設定
find_package( OpenMP )

set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
# インクルードパスの追加
if(WIN32)
  include_directories(C:/local/include)
  link_directories(C:/local/lib)
else()
  include_directories(/usr/local/include)
  link_directories(/usr/local/lib)
endif()
# ビルドに関する設定
set(EXECUTABLES "small;match;canny;binaly;proto_binaly;exif;img2mp4;lbp_hog;match_Tsukuba_LBP;time;match_Tsukuba_HOG;match_Tsukuba_Canny;mov2img;divide;glid;hist;attach_img2mp4")
set(DEPENDENT_LIBS opencv_core;opencv_highgui;opencv_imgcodecs;opencv_imgproc;exiv2;opencv_videoio)
foreach(target IN LISTS EXECUTABLES)
  add_executable(${target} src/${target}.cpp)
  target_link_libraries(${target} ${DEPENDENT_LIBS})
endforeach()