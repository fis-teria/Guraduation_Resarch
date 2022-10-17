#include <opencv2/opencv.hpp>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;

std::string make_path(std::string dir, int dir_num, int var)
{
    std::string back;
    if (var < 10)
    {
        back = dir + std::to_string(dir_num) + "_template/00000" + std::to_string(var) + ".jpg";
        return back;
    }
    else if (var < 100)
    {
        back = dir + std::to_string(dir_num) + "_template/0000" + std::to_string(var) + ".jpg";
        return back;
    }
    else if (1 < 1000)
    {
        back = dir + std::to_string(dir_num) + "_template/000" + std::to_string(var) + ".jpg";
        return back;
    }
}

std::string make_rpath(std::string dir, int dir_num, int var)
{
    std::string back;
    if (var < 10)
    {
        back = dir + std::to_string(dir_num) + "_CSV/00000" + std::to_string(var) + ".csv";
        return back;
    }
    else if (var < 100)
    {
        back = dir + std::to_string(dir_num) + "_CSV/0000" + std::to_string(var) + ".csv";
        return back;
    }
    else if (1 < 1000)
    {
        back = dir + std::to_string(dir_num) + "_CSV/000" + std::to_string(var) + ".csv";
        return back;
    }
}

std::string make_cpath(std::string dir, int dir_num, int var)
{
    std::string back;
    if (var < 10)
    {
        back = dir + std::to_string(dir_num) + "/00000" + std::to_string(var) + ".jpg";
        return back;
    }
    else if (var < 100)
    {
        back = dir + std::to_string(dir_num) + "/0000" + std::to_string(var) + ".jpg";
        return back;
    }
    else if (1 < 1000)
    {
        back = dir + std::to_string(dir_num) + "/000" + std::to_string(var) + ".jpg";
        return back;
    }
}

int main(int argc, char **argv)
{
    /*
    if(argc != 3) {
      fprintf(stderr,"usage: %s in_image out_image\n",argv[0]);
      return 1;
    }
    */
    Mat rim, sim, tim;
    Mat c_rim, c_sim, c_tim;
    Mat cs_rim, cs_sim, cs_tim;
    std::string dir = "Test0";
    std::cout << dir << "\n";
    std::string path;
    std::string dpath;
    int i = atoi(argv[1]);
    int j = atoi(argv[2]);
    // test00
    path = make_path(dir, 0, i);
    rim = imread(path, IMREAD_GRAYSCALE);
    Canny(imread(make_cpath(dir, 0, i)), c_rim, 100, 200);
    Canny(rim, cs_rim, 200, 400);

    // test01
    dpath = make_path(dir, 1, j);
    sim = imread(dpath, IMREAD_GRAYSCALE);
    Canny(imread(make_cpath(dir, 1, j)), c_sim, 100, 400);
    Canny(sim, cs_sim, 500, 600);

    /*** 画像を表示 ***/
    cv::imshow("1", rim);
    cv::imshow("2", sim);
    cv::imshow("3", c_rim);
    cv::imshow("4", c_sim);
    cv::imshow("5", cs_rim);
    cv::imshow("6", cs_sim);
    cv::waitKey(0);
    return 0;
}