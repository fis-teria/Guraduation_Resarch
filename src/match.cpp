#include <opencv2/opencv.hpp>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <omp.h>
#include <time.h>
using namespace cv;

#define MATCH00to01
std::string SAVE = "save.txt";

struct result
{
  double max_t;
  double max_c;
  double max_cs;
  int num;
  int x_t;
  int y_t;
  int x_c;
  int y_c;
};

void print_elapsed_time(clock_t begin, clock_t end)
{
  float elapsed = (float)(end - begin) / CLOCKS_PER_SEC;
  printf("Elapsed Time: %15.7f sec\n", elapsed);
}

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

int read_save()
{
  int save_num;
  std::ifstream ifs(SAVE.c_str());
  std::string str;
  getline(ifs, str);
  std::cout << str << std::endl;
  save_num = atoi(str.c_str());
  return save_num;
}

void save(int m)
{
  std::ofstream savefile(SAVE.c_str());
  savefile << m << std::endl;
  savefile.close();
}

int main(int argc, char **argv)
{
  /*
  if (argc != 3)
  {
    fprintf(stderr, "usage: %s R_image S_image\n", argv[0]);
    return 1;
  }
  */

  Mat rim, sim, tim;
  Mat c_rim, c_sim, c_tim;
  Mat cs_rim, cs_sim, cs_tim;
  /*
  rim = base image
  sim = normal template iamge
  tim = result normal template matching
  c_rim = Canny-converted before resize rim
  c_sim = Canny-converted before resizesim
  c_tim = result cim template matching
  cs_rim = Canny-converted after resize rim
  cs_sim = Canny-converted after resize sim
  cs_tim = result cim template matching

  */
  int i, j; // seed num
  int num_t, num_m;
  int path_t, path_m;
  int load_num = read_save();
  std::cout << load_num << std::endl;
  int average = 0;
  int result_size = 0;
  std::vector<result> results;
  std::string dir = "Test0";
  std::string r_dir = "result0";
  std::cout << dir << "\n";
  std::string path;
  std::string dpath;
  std::string result_path;
  int c_tnum1,c_tnum2;
  int cs_tnum1,cs_tnum2;
  int c_mnum1,c_mnum2;
  int cs_mnum1,cs_mnum2;
  
  double min_t, max_t;
  double min_c, max_c;
  double min_cs, max_cs;
  Point p_min_t, p_max_t;
  Point p_min_c, p_max_c;
  Point p_min_cs, p_max_cs;

#ifdef MATCH00to01
  num_t = 671;
  num_m = 489;
  path_t = 0;
  path_m = 1;
  c_tnum1 = 100;
  c_tnum2 = 200;
  cs_tnum1 = 200;
  cs_tnum2 = 400;
  c_mnum1 = 100;
  c_mnum2 = 400;
  cs_mnum1 = 500;
  cs_mnum2 = 600;
#endif

#ifndef MATCH00to01
  num_t = 489;
  num_m = 671;
  path_t = 1;
  path_m = 0;
  c_tnum1 = 100;
  c_tnum2 = 400;
  cs_tnum1 = 500;
  cs_tnum2 = 600;
  c_mnum1 = 100;
  c_mnum2 = 200;
  cs_mnum1 = 200;
  cs_mnum2 = 400;
#endif
  for (int m = load_num; m < num_t; m++)
  {

    //テンプレート画像へのパス
    /*srand((unsigned int)time(NULL));
    i = rand() % num_t;
    sleep(1);
    */
    path = make_path(dir, path_t, m);
    result_path = make_rpath(r_dir, path_t, m);
    std::cout << path << std::endl;
    std::cout << result_path << std::endl;
    std::ofstream outputfile(result_path);

    rim = imread(path, IMREAD_GRAYSCALE);
    Canny(imread(make_cpath(dir, path_t, m)), c_rim, c_tnum1, c_tnum2);

    Canny(rim, cs_rim, cs_tnum1, cs_tnum2);

    for (int k = 1; k < num_m; k++)
    {
      clock_t begin = clock();
      /*マッチング画像へのパス
      srand((unsigned int)time(NULL));
      j = rand() % 490;
      */
      dpath = make_path(dir, path_m, k);
      std::cout << "match" << path << " : " << dpath << std::endl;

      sim = imread(dpath, IMREAD_GRAYSCALE);

      matchTemplate(rim, sim(Range(7, 87), Range(10, 91)), tim, TM_CCOEFF_NORMED);
      minMaxLoc(tim, &min_t, &max_t, &p_min_t, &p_max_t);

      Canny(imread(make_cpath(dir, path_m, k)), c_sim, c_mnum1, c_mnum2);
      matchTemplate(c_rim, c_sim(Range(70, 870), Range(100, 910)), c_tim, TM_CCOEFF_NORMED);
      minMaxLoc(c_tim, &min_c, &max_c, &p_min_c, &p_max_c);

      Canny(sim, cs_sim, cs_mnum1, cs_mnum2);
      matchTemplate(cs_rim, cs_sim(Range(7, 87), Range(10, 91)), cs_tim, TM_CCOEFF_NORMED);
      minMaxLoc(cs_tim, &min_cs, &max_cs, &p_min_cs, &p_max_cs);

      results.resize(result_size + 1);
      results[result_size].max_t = max_t;
      results[result_size].max_c = max_c;
      results[result_size].max_cs = max_cs;
      results[result_size].num = k;
      results[result_size].x_t = p_max_t.x;
      results[result_size].y_t = p_max_t.y;
      results[result_size].x_c = p_max_c.x;
      results[result_size].y_c = p_max_c.y;
      result_size++;
      // printf("max = %f at (%d,%d)\n", max, p_max.x, p_max.y);
      outputfile << k << "," << max_t << "," << max_c << ","<< max_cs << std::endl;
      // sleep(1);
      clock_t end = clock();
      print_elapsed_time(begin, end);
    }

    outputfile.close();
    //類似度を昇順にソート
    std::sort(results.begin(), results.end(), [](const result &x, const result &y)
              { return x.max_t < y.max_t; });

    //最大の類似度を持つマッチング画像へのパス
    dpath = make_path(dir, path_m, results[result_size - 1].num);
    std::cout << path << " : " << dpath << std::endl;
    std::cout
        << "all max = " << results[result_size - 1].max_t << "(x ,y ) = (" << results[result_size - 1].x_t << ", " << results[result_size - 1].y_t << ")" << std::endl;

    save(m);
    /*
    cv::namedWindow("template");
    cv::imshow("template", rim);
    cv::namedWindow("result");
    cv::imshow("result", imread(dpath, 0));
    cv::waitKey(0);
    */
  }
  save(0);

  return 0;
}
