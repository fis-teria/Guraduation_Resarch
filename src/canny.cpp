#include <opencv2/opencv.hpp>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;

int main(int argc, char **argv)
{
    /*
    if(argc != 3) {
      fprintf(stderr,"usage: %s in_image out_image\n",argv[0]);
      return 1;
    }
    */
    cv::Mat iim, sim, oim;
    std::string dir = argv[1];
    std::cout << dir << "\n";
    std::string path;
    std::string dpath;
    int i= atoi(argv[2]);
    if (i < 10)
    {
        path = dir + "/00000" + std::to_string(i) + ".jpg";
    }
    else if (i < 100)
    {
        path = dir + "/0000" + std::to_string(i) + ".jpg";
    }
    else if (1 < 1000)
    {
        path = dir + "/000" + std::to_string(i) + ".jpg";
    }
    std::cout << path << "\n";
    iim = cv::imread(path, 0);
    cv::GaussianBlur(iim, sim, cv::Size(3, 3), 1.0);
    cv::resize(sim, oim, Size(), 0.1, 0.1);
    cv::Canny(oim, oim, atoi(argv[3]), atoi(argv[4]));
    /*** 画像を表示 ***/
    cv::imshow("original", sim);
    cv::imshow("canny", oim);
    cv::waitKey(0);
    return 0;
}