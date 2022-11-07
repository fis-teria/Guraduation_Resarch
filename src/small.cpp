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
  std::cout << argv[2] << " : " << argv[3] << std::endl;
  std::string tag = argv[4];
  for (int i = std::atoi(argv[2]); i < std::atoi(argv[3]) + 1; i++)
  {
    if (i < 10)
    {
      path = dir + "/00000" + std::to_string(i) + tag;
      dpath = dir + "_template/00000" + std::to_string(i) + tag;
    }
    else if (i < 100)
    {
      path = dir + "/0000" + std::to_string(i) + tag;
      dpath = dir + "_template/0000" + std::to_string(i) + tag;
    }
    else if (i < 1000)
    {
      path = dir + "/000" + std::to_string(i) + tag;
      dpath = dir + "_template/000" + std::to_string(i) + tag;
    }
    else if (i < 10000)
    {
      path = dir + "/00" + std::to_string(i) + tag;
      dpath = dir + "_template/00" + std::to_string(i) + tag;
    }

    std::cout << path << "\n";
    std::cout << dpath << std::endl;
    iim = cv::imread(path, 0);
    sim = iim.clone();
    std::cout << iim.cols << "," << iim.rows << std::endl;
    cv::GaussianBlur(iim, sim, cv::Size(3, 3), 1.0);
    cv::resize(sim, oim, Size(), 0.1, 0.1);
    cv::imwrite(dpath, oim);
  }

  return 0;
}
