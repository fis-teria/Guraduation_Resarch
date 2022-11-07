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

//#define rootGunDai
#define MATCH00to01
#define root1001_1430match2_917
#define root1001_1435match2_1001_1030
std::string SAVE = "save.txt";
/*
0917_1349
start 50
end 4546

1001_1030
start 33
end 2469

1001_1435
start 27
end 4548
*/
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

struct resultLH
{
  double max_l;
  double max_sl;
  double max_h;
  double max_sh;
  int num;
};

void print_elapsed_time(clock_t begin, clock_t end)
{
  float elapsed = (float)(end - begin) / CLOCKS_PER_SEC;
  printf("Elapsed Time: %15.7f sec\n", elapsed);
}

std::string make_path(std::string dir, int dir_num, int var, std::string tag)
{
  std::string back;
  if (var < 10)
  {
    back = dir + std::to_string(dir_num) + "_template/00000" + std::to_string(var) + tag;
    return back;
  }
  else if (var < 100)
  {
    back = dir + std::to_string(dir_num) + "_template/0000" + std::to_string(var) + tag;
    return back;
  }
  else if (1 < 1000)
  {
    back = dir + std::to_string(dir_num) + "_template/000" + std::to_string(var) + tag;
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

std::string make_rpathLH(std::string dir, int dir_num, int var)
{
  std::string back;
  if (var < 10)
  {
    back = dir + std::to_string(dir_num) + "LH/00000" + std::to_string(var) + ".csv";
    return back;
  }
  else if (var < 100)
  {
    back = dir + std::to_string(dir_num) + "LH/0000" + std::to_string(var) + ".csv";
    return back;
  }
  else if (1 < 1000)
  {
    back = dir + std::to_string(dir_num) + "LH/000" + std::to_string(var) + ".csv";
    return back;
  }
}

std::string make_cpath(std::string dir, int dir_num, int var, std::string tag)
{
  std::string back;
  if (var < 10)
  {
    back = dir + std::to_string(dir_num) + "/00000" + std::to_string(var) + tag;
    return back;
  }
  else if (var < 100)
  {
    back = dir + std::to_string(dir_num) + "/0000" + std::to_string(var) + tag;
    return back;
  }
  else if (1 < 1000)
  {
    back = dir + std::to_string(dir_num) + "/000" + std::to_string(var) + tag;
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

void templete_Canny(int execute_num)
{
  cv::Mat rim, sim, tim;
  cv::Mat c_rim, c_sim, c_tim;
  cv::Mat cs_rim, cs_sim, cs_tim;
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
  std::string dir;
  std::string r_dir;
  std::string tag;
  std::cout << dir << "\n";
  std::string path;
  std::string dpath;
  std::string result_path;
  int c_tnum1, c_tnum2;
  int cs_tnum1, cs_tnum2;
  int c_mnum1, c_mnum2;
  int cs_mnum1, cs_mnum2;

  double min_t, max_t;
  double min_c, max_c;
  double min_cs, max_cs;
  cv::Point p_min_t, p_max_t;
  cv::Point p_min_c, p_max_c;
  cv::Point p_min_cs, p_max_cs;

  switch (execute_num)
  {
  case 10: // Match 00 to 01
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
    dir = "Test0";
    r_dir = "result0";
    tag = ".jpg";

    break;
  case 11: // Match 01 to 00
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
    dir = "Test0";
    r_dir = "result0";
    tag = ".jpg";

    break;
  default:
    std::cout << "error! can't use this number!!!!" << std::endl;
    std::cout << "prease use one of these number\nGunDai Root\n11 base 00 template 01\n12 base 01 template 00\n\nTsukuba Root\n21 base 1001_1030 template 0917_1349\n22 bas 1001_1030 template 1001_1435" << std::endl;
    return;
  }
  for (int m = load_num; m < num_t; m++)
  {

    //テンプレート画像へのパス
    /*srand((unsigned int)time(NULL));
    i = rand() % num_t;
    sleep(1);
    */
    path = make_path(dir, path_t, m, tag);
    result_path = make_rpath(r_dir, path_t, m);
    std::cout << path << std::endl;
    std::cout << result_path << std::endl;
    std::ofstream outputfile(result_path);

    rim = cv::imread(path, cv::IMREAD_GRAYSCALE);
    cv::Canny(cv::imread(make_cpath(dir, path_t, m, tag)), c_rim, c_tnum1, c_tnum2);

    cv::Canny(rim, cs_rim, cs_tnum1, cs_tnum2);

    for (int k = 1; k < num_m; k++)
    {
      clock_t begin = clock();
      /*マッチング画像へのパス
      srand((unsigned int)time(NULL));
      j = rand() % 490;
      */
      dpath = make_path(dir, path_m, k, tag);
      std::cout << "match" << path << " : " << dpath << std::endl;

      sim = cv::imread(dpath, cv::IMREAD_GRAYSCALE);

      cv::matchTemplate(rim, sim(cv::Range(7, 87), cv::Range(10, 91)), tim, cv::TM_CCOEFF_NORMED);
      cv::minMaxLoc(tim, &min_t, &max_t, &p_min_t, &p_max_t);

      cv::Canny(cv::imread(make_cpath(dir, path_m, k, tag)), c_sim, c_mnum1, c_mnum2);
      cv::matchTemplate(c_rim, c_sim(cv::Range(c_sim.rows / 10, (9 * c_sim.rows) / 10), cv::Range(c_sim.cols / 10, (9 * c_sim.cols) / 10)), c_tim, cv::TM_CCOEFF_NORMED);
      cv::minMaxLoc(c_tim, &min_c, &max_c, &p_min_c, &p_max_c);

      cv::Canny(sim, cs_sim, cs_mnum1, cs_mnum2);
      cv::matchTemplate(cs_rim, cs_sim(cv::Range(cs_sim.rows / 10, (9 * cs_sim.rows) / 10), cv::Range(cs_sim.cols / 10, (9 * cs_sim.cols) / 10)), cs_tim, cv::TM_CCOEFF_NORMED);
      cv::minMaxLoc(cs_tim, &min_cs, &max_cs, &p_min_cs, &p_max_cs);

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
      outputfile << k << "," << max_t << "," << max_c << "," << max_cs << std::endl;
      // sleep(1);
      clock_t end = clock();
      print_elapsed_time(begin, end);
    }

    outputfile.close();
    //類似度を昇順にソート
    std::sort(results.begin(), results.end(), [](const result &x, const result &y)
              { return x.max_t < y.max_t; });

    //最大の類似度を持つマッチング画像へのパス
    dpath = make_path(dir, path_m, results[result_size - 1].num, tag);
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
}

int LBP_filter[3][3] = {{64, 32, 16},
                        {128, 0, 8},
                        {1, 2, 4}};

void cvt_LBP(const cv::Mat &src, cv::Mat &lbp)
{
  lbp = cv::Mat(src.rows, src.cols, CV_8UC1);
  lbp = cv::Scalar::all(0);
  cv::Mat padsrc;
  // cv::cvtColor(padsrc, padsrc, cv::COLOR_BGR2GRAY);
  copyMakeBorder(src, padsrc, 1, 1, 1, 1, cv::BORDER_REPLICATE);

  // cv::imshow("first", src);
  // cv::imshow("third", lbp);
  for (int x = 1; x < padsrc.cols - 1; x++)
  {
    for (int y = 1; y < padsrc.rows - 1; y++)
    {
      for (int i = 0; i < 3; i++)
      {
        for (int j = 0; j < 3; j++)
        {
          if (padsrc.at<unsigned char>(y - 1 + j, x - 1 + i) >= padsrc.at<unsigned char>(y, x))
            lbp.at<unsigned char>(y - 1, x - 1) += LBP_filter[i][j];
        }
      }
    }
  }
  // cv::imshow("second", lbp);
}

#define N_BIN 9             // ヒストグラムのビン数
#define THETA (180 / N_BIN) // 分解能

#define CELL_SIZE 20                     // セルの大きさ（ピクセル数）
#define BLOCK_SIZE 3                     // ブロックの大きさ（セル数）奇数
#define R (CELL_SIZE * (BLOCK_SIZE)*0.5) // ブロックの大きさの半分（ピクセル数）

// 積分画像生成
std::vector<cv::Mat> calculateIntegralHOG(const cv::Mat &image)
{
  // X, Y方向に微分
  cv::Mat xsobel, ysobel;
  cv::Sobel(image, xsobel, CV_32F, 1, 0);
  cv::Sobel(image, ysobel, CV_32F, 0, 1);

  // 角度別の画像を生成しておく
  std::vector<cv::Mat> bins(N_BIN);
  for (int i = 0; i < N_BIN; i++)
  {
    bins[i] = cv::Mat(image.size(), CV_32F);
    bins[i] = cv::Scalar::all(0);
  }

  // X, Y微分画像を勾配方向と強度に変換
  cv::Mat Imag, Iang;
  cv::cartToPolar(xsobel, ysobel, Imag, Iang, true);
  // 勾配方向を[0, 180)にする
  cv::add(Iang, cv::Scalar(180), Iang, Iang < 0);
  cv::add(Iang, cv::Scalar(-180), Iang, Iang >= 180);
  // 勾配方向を[0, 1, ..., 8]にする準備（まだfloat）
  Iang /= THETA;

  // 勾配方向を強度で重みをつけて、角度別に投票する
  for (int y = 0; y < image.rows; y++)
  {
    for (int x = 0; x < image.cols; x++)
    {
      int ind = Iang.at<float>(y, x);
      bins[ind].at<float>(y, x) += Imag.at<float>(y, x);
    }
  }
  std::cout << "covert HOG" << std::endl;

  // 角度別に積分画像生成
  std::vector<cv::Mat> integrals(N_BIN);
  for (int i = 0; i < N_BIN; i++)
  {
    // 積分画像をつくる、OpenCVの関数がある
    integral(bins[i], integrals[i]);
  }

  return integrals;
}

// ある矩形領域の勾配ヒストグラムを求める
// ここでいう矩形はHOG特徴量のセルに該当
void calculateHOGInCell(cv::Mat &hogCell, cv::Rect roi, const std::vector<cv::Mat> &integrals)
{
  int x0 = roi.x, y0 = roi.y;
  int x1 = x0 + roi.width, y1 = y0 + roi.height;
  for (int i = 0; i < N_BIN; i++)
  {
    cv::Mat integral = integrals[i];
    float a = integral.at<double>(y0, x0);
    float b = integral.at<double>(y1, x1);
    float c = integral.at<double>(y0, x1);
    float d = integral.at<double>(y1, x0);
    hogCell.at<float>(0, i) = (a + b) - (c + d);
  }
}

// HOG特徴量を計算する
// pt: ブロックの中心点
cv::Mat getHOG(cv::Point pt, const std::vector<cv::Mat> &integrals)
{
  // ブロックが画像からはみ出していないか確認
  if (pt.x - R < 0 ||
      pt.y - R < 0 ||
      pt.x + R >= integrals[0].cols ||
      pt.y + R >= integrals[0].rows)
  {
    return cv::Mat();
  }

  // 与点を中心としたブロックで、
  // セルごとに勾配ヒストグラムを求めて連結
  cv::Mat hist(cv::Size(N_BIN * BLOCK_SIZE * BLOCK_SIZE, 1), CV_32F);
  cv::Point tl(0, pt.y - R);
  int c = 0;
  for (int i = 0; i < BLOCK_SIZE; i++)
  {
    tl.x = pt.x - R;
    for (int j = 0; j < BLOCK_SIZE; j++)
    {
      hist.colRange(c, c + N_BIN);
      calculateHOGInCell(hist,
                         cv::Rect(tl, tl + cv::Point(CELL_SIZE, CELL_SIZE)),
                         integrals);
      tl.x += CELL_SIZE;
      c += N_BIN;
    }
    tl.y += CELL_SIZE;
  }
  // L2ノルムで正規化
  // cv::normalize(hist, hist, 1, 0, NORM_L2);
  cv::normalize(hist, hist, 1, 0, 4);
  return hist;
}

void cvt_HOG(const cv::Mat &src, cv::Mat &hog)
{
  // 積分画像生成
  std::vector<cv::Mat> integrals = calculateIntegralHOG(src);
  // ある点(x, y)のHOG特徴量を求めるには
  // Mat hist = getHOG(Point(x, y), integrals);
  // とする。histはSize(81, 1) CV_32FのMat

  // 表示用画像を用意（半分の輝度に）y
  cv::Mat image = src.clone();
  image *= 0.5;

  // 格子点でHOG計算
  cv::Mat meanHOGInBlock(cv::Size(N_BIN, 1), CV_32F);
  for (int y = CELL_SIZE / 2; y < image.rows; y += CELL_SIZE)
  {
    for (int x = CELL_SIZE / 2; x < image.cols; x += CELL_SIZE)
    {
      // (x, y)でのHOGを取得
      cv::Mat hist = getHOG(cv::Point(x, y), integrals);
      // ブロックが画像からはみ出ていたら continue
      if (hist.empty())
        continue;

      // ブロックごとに勾配方向ヒストグラム生成
      meanHOGInBlock = cv::Scalar(0);
      for (int i = 0; i < N_BIN; i++)
      {
        for (int j = 0; j < BLOCK_SIZE * BLOCK_SIZE; j++)
        {
          meanHOGInBlock.at<float>(0, i) += hist.at<float>(0, i + j * N_BIN);
        }
      }
      // L2ノルムで正規化（強い方向が強調される）
      normalize(meanHOGInBlock, meanHOGInBlock, 1, 0, cv::NORM_L2);

      // 角度ごとに線を描画
      cv::Point center(x, y);
      for (int i = 0; i < N_BIN; i++)
      {
        double theta = (i * THETA + 90.0) * CV_PI / 180.0;
        cv::Point rd(CELL_SIZE * 0.5 * cos(theta), CELL_SIZE * 0.5 * sin(theta));
        cv::Point rp = center - rd;
        cv::Point lp = center - -rd;
        cv::line(image, rp, lp, cv::Scalar(255 * meanHOGInBlock.at<float>(0, i), 255, 255));
      }
    }
  }

  hog = image.clone();
}

void templete_LBPandHOG(int execute_num)
{
  cv::Mat rim, sim, tim;
  cv::Mat l_rim, l_sim, l_tim;
  cv::Mat sl_rim, sl_sim, sl_tim;
  cv::Mat h_rim, h_sim, h_tim;
  cv::Mat sh_rim, sh_sim, sh_tim;
  /*
  rim = base image
  sim = normal template iamge
  tim = result normal template matching
  l_rim = LBP  before resize rim
  l_sim = LBP  before resize sim
  l_tim = result cim template matching
  sl_rim = LBP  after resize rim
  sl_sim = LBP  after resize sim
  sl_tim = result cim template matching
  h_rim = HOG  before resize rim
  h_sim = HOG  before resizesim
  h_tim = result cim template matching
  sh_rim = HOG  after resize rim
  sh_sim = HOG  after resize sim
  sh_tim = result cim template matching

  */
  int i, j; // seed num
  int num_t, num_m;
  int path_t, path_m;
  int load_num = read_save();
  std::cout << load_num << std::endl;
  int average = 0;
  int result_size = 0;
  std::vector<resultLH> results;
  std::string dir;
  std::string r_dir;
  std::string tag;
  std::cout << dir << "\n";
  std::string path;
  std::string dpath;
  std::string result_path;
  int c_tnum1, c_tnum2;
  int cs_tnum1, cs_tnum2;
  int c_mnum1, c_mnum2;
  int cs_mnum1, cs_mnum2;

  double min_l, max_l;
  double min_sl, max_sl;
  double min_h, max_h;
  double min_sh, max_sh;
  cv::Point p_min_l, p_max_l;
  cv::Point p_min_sl, p_max_sl;
  cv::Point p_min_h, p_max_h;
  cv::Point p_min_sh, p_max_sh;

  switch (execute_num)
  {
  case 10: // Match 00 to 01
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
    dir = "Test0";
    r_dir = "result0";
    tag = ".jpg";

    break;
  case 11: // Match 01 to 00
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
    dir = "Test0";
    r_dir = "result0";
    tag = ".jpg";

    break;
  default:
    std::cout << "error! can't use this number!!!!" << std::endl;
    std::cout << "prease use one of these number\nGunDai Root\n11 base 00 template 01\n12 base 01 template 00\n\nTsukuba Root\n21 base 1001_1030 template 0917_1349\n22 bas 1001_1030 template 1001_1435" << std::endl;
    return;
  }

  for (int m = load_num; m < num_t; m++)
  {

    //テンプレート画像へのパス
    /*srand((unsigned int)time(NULL));
    i = rand() % num_t;
    sleep(1);
    */
    path = make_path(dir, path_t, m, tag);
    result_path = make_rpathLH(r_dir, path_t, m);
    std::cout << path << std::endl;
    std::cout << result_path << std::endl;
    std::ofstream outputfile(result_path);

    rim = cv::imread(path, cv::IMREAD_GRAYSCALE);
    // LBP
    cvt_LBP(cv::imread(make_cpath(dir, path_t, m, tag), 0), l_rim);
    // small LBP
    cvt_LBP(rim, sl_rim);
    // HOG
    cvt_HOG(cv::imread(make_cpath(dir, path_t, m, tag), 0), h_rim);
    // small HOG
    cvt_HOG(rim, sh_rim);

    for (int k = 0; k < num_m; k++)
    {
      clock_t begin = clock();
      /*マッチング画像へのパス
      srand((unsigned int)time(NULL));
      j = rand() % 490;
      */
      dpath = make_path(dir, path_m, k, tag);
      std::cout << "match" << path << " : " << dpath << std::endl;

      sim = cv::imread(dpath, cv::IMREAD_GRAYSCALE);

      // LBP
      cvt_LBP(cv::imread(make_cpath(dir, path_m, k, tag)), l_sim);
      if (l_sim.empty())
      {
        return;
      }
      cv::matchTemplate(l_rim, l_sim(cv::Range(70, 870), cv::Range(100, 910)), l_tim, cv::TM_CCOEFF_NORMED);
      cv::minMaxLoc(l_tim, &min_l, &max_l, &p_min_l, &p_max_l);
      // small LBP
      cvt_LBP(sim, sl_sim);
      if (sl_sim.empty())
      {
        return;
      }
      cv::matchTemplate(sl_rim, sl_sim(cv::Range(7, 87), cv::Range(10, 91)), sl_tim, cv::TM_CCOEFF_NORMED);
      cv::minMaxLoc(sl_tim, &min_sl, &max_sl, &p_min_sl, &p_max_sl);
      // HOG
      cvt_HOG(cv::imread(make_cpath(dir, path_m, k, tag), 0), h_sim);
      if (h_sim.empty())
      {
        return;
      }
      cv::matchTemplate(h_rim, h_sim(cv::Range(70, 870), cv::Range(100, 910)), h_tim, cv::TM_CCOEFF_NORMED);
      cv::minMaxLoc(h_tim, &min_h, &max_h, &p_min_h, &p_max_h);
      // small HOG
      cvt_HOG(sim, sh_sim);
      if (sh_sim.empty())
      {
        return;
      }
      cv::matchTemplate(sh_rim, sh_sim(cv::Range(7, 87), cv::Range(10, 91)), sh_tim, cv::TM_CCOEFF_NORMED);
      cv::minMaxLoc(sh_tim, &min_sh, &max_sh, &p_min_sh, &p_max_sh);

      results.resize(result_size + 1);
      results[result_size].max_l = max_l;
      results[result_size].max_sl = max_sl;
      results[result_size].max_h = max_sl;
      results[result_size].max_sh = max_sl;
      results[result_size].num = k;
      result_size++;
      // printf("max = %f at (%d,%d)\n", max, p_max.x, p_max.y);
      outputfile << k << "," << max_l << "," << max_sl << "," << max_h << "," << max_sh << std::endl;
      // sleep(1);
      clock_t end = clock();
      print_elapsed_time(begin, end);
    }

    outputfile.close();
    //類似度を昇順にソート
    //最大の類似度を持つマッチング画像へのパス
    /*
    dpath = make_path(dir, path_m, results[result_size - 1].num);
    std::cout << path << " : " << dpath << std::endl;
    std::cout
        << "all max = " << results[result_size - 1].max_t << "(x ,y ) = (" << results[result_size - 1].x_t << ", " << results[result_size - 1].y_t << ")" << std::endl;
    */
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
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cout << "prease select number\nGunDai Root\n10 base 00 template 01\n11 base 01 template 00\n"
              << std::endl;
    return 1;
  }
  int execute_num = atoi(argv[1]);
  // templete_Canny(execute_num);
  templete_LBPandHOG(execute_num);

  return 0;
}
