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

int LBP_filter[3][3] = {{64, 32, 16},
                        {128, 0, 8},
                        {1, 2, 4}};

void cvt_LBP(const cv::Mat &src, cv::Mat &dst)
{
    dst = cv::Mat(src.rows, src.cols, CV_8UC1);
    cv::Mat padsrc;
    // cv::cvtColor(padsrc, padsrc, cv::COLOR_BGR2GRAY);
    copyMakeBorder(src, padsrc, 1, 1, 1, 1, cv::BORDER_REPLICATE);

    for (int x = 1; x < padsrc.cols - 1; x++)
    {
        for (int y = 1; y < padsrc.rows - 1; y++)
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (padsrc.at<unsigned char>(y - 1 + j, x - 1 + i) >= padsrc.at<unsigned char>(y, x))
                        dst.at<unsigned char>(y - 1, x - 1) += LBP_filter[i][j];
                }
            }
        }
    }
    std::cout << dst.cols << "  " << padsrc.cols << std::endl;
}


#define N_BIN 9             // ヒストグラムのビン数
#define THETA (180 / N_BIN) // 分解能

#define CELL_SIZE 20 // セルの大きさ（ピクセル数）
#define BLOCK_SIZE 3 // ブロックの大きさ（セル数）奇数
#define R (CELL_SIZE*(BLOCK_SIZE)*0.5) // ブロックの大きさの半分（ピクセル数）

// 積分画像生成
std::vector<cv::Mat> calculateIntegralHOG(const cv::Mat& image) {
  // X, Y方向に微分
  cv::Mat xsobel, ysobel;
  cv::Sobel(image, xsobel, CV_32F, 1, 0);
  cv::Sobel(image, ysobel, CV_32F, 0, 1);

  // 角度別の画像を生成しておく
  std::vector<cv::Mat> bins(N_BIN);
  for (int i = 0; i < N_BIN; i++)
    bins[i] = cv::Mat::zeros(image.size(), CV_32F);

  // X, Y微分画像を勾配方向と強度に変換
  cv::Mat Imag, Iang;
  cv::cartToPolar(xsobel, ysobel, Imag, Iang, true);
  // 勾配方向を[0, 180)にする
  cv::add(Iang, cv::Scalar(180), Iang, Iang < 0);
  cv::add(Iang, cv::Scalar(-180), Iang, Iang >= 180);
  // 勾配方向を[0, 1, ..., 8]にする準備（まだfloat）
  Iang /= THETA;

  // 勾配方向を強度で重みをつけて、角度別に投票する
  for (int y = 0; y < image.rows; y++) {
    for (int x = 0; x < image.cols; x++) {
      int ind = Iang.at<float>(y, x);
      bins[ind].at<float>(y, x) += Imag.at<float>(y, x);
    }
  }

  // 角度別に積分画像生成
  std::vector<cv::Mat> integrals(N_BIN);
  for (int i = 0; i < N_BIN; i++) {
    // 積分画像をつくる、OpenCVの関数がある
    integral(bins[i], integrals[i]);
  }

  return integrals;
}

// ある矩形領域の勾配ヒストグラムを求める
// ここでいう矩形はHOG特徴量のセルに該当
void calculateHOGInCell(cv::Mat& hogCell, cv::Rect roi, const std::vector<cv::Mat>& integrals) {
  int x0 = roi.x, y0 = roi.y;
  int x1 = x0 + roi.width, y1 = y0 + roi.height;
  for (int i = 0; i < N_BIN; i++) {
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
cv::Mat getHOG(cv::Point pt, const std::vector<cv::Mat>& integrals) {
  // ブロックが画像からはみ出していないか確認
  if (pt.x - R < 0 ||
      pt.y - R < 0 ||
      pt.x + R >= integrals[0].cols ||
      pt.y + R >= integrals[0].rows
      ) {
    return cv::Mat();
  }

  // 与点を中心としたブロックで、
  // セルごとに勾配ヒストグラムを求めて連結
  cv::Mat hist(cv::Size(N_BIN*BLOCK_SIZE*BLOCK_SIZE, 1), CV_32F);
  cv::Point tl(0, pt.y - R);
  int c = 0;
  for (int i = 0; i < BLOCK_SIZE; i++) {
    tl.x = pt.x - R;
    for (int j = 0; j < BLOCK_SIZE; j++) {
      hist.colRange(c, c+N_BIN);
      calculateHOGInCell(hist,
                         cv::Rect(tl, tl+cv::Point(CELL_SIZE, CELL_SIZE)),
                         integrals);
      tl.x += CELL_SIZE;
      c += N_BIN;
    }
    tl.y += CELL_SIZE;
  }
  // L2ノルムで正規化
  //cv::normalize(hist, hist, 1, 0, NORM_L2);
  cv::normalize(hist, hist, 1, 0, 4);
  return hist;
}

void cvt_HOG(const cv::Mat &src, cv::Mat &dst){
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
  for (int y = CELL_SIZE/2; y < image.rows; y += CELL_SIZE) {
    for (int x = CELL_SIZE/2; x < image.cols; x += CELL_SIZE) {
      // (x, y)でのHOGを取得
      cv::Mat hist = getHOG(cv::Point(x, y), integrals);
      // ブロックが画像からはみ出ていたら continue
      if (hist.empty()) continue;

      // ブロックごとに勾配方向ヒストグラム生成
      meanHOGInBlock = cv::Scalar(0);
      for (int i = 0; i < N_BIN; i++) {
        for (int j = 0; j < BLOCK_SIZE*BLOCK_SIZE; j++) {
          meanHOGInBlock.at<float>(0, i) += hist.at<float>(0, i+j*N_BIN);
        }
      }
      // L2ノルムで正規化（強い方向が強調される）
      normalize(meanHOGInBlock, meanHOGInBlock, 1, 0, cv::NORM_L2);

      // 角度ごとに線を描画
      cv::Point center(x, y);
      for (int i = 0; i < N_BIN; i++) {
        double theta = (i * THETA + 90.0 ) * CV_PI / 180.0;
        cv::Point rd(CELL_SIZE*0.5*cos(theta), CELL_SIZE*0.5*sin(theta));
        cv::Point rp = center -   rd;
        cv::Point lp = center -  -rd;
        cv::line(image, rp, lp, cv::Scalar(255*meanHOGInBlock.at<float>(c cd 0, i), 255, 255));
      }
    }
  }

  dst = image.clone();
}


int main()
{

    cv::Mat src, src2, dst_LBP, dst_HOG, dst_LBP2, dst_HOG2;
    src = cv::imread("Test00/000159.jpg", 0);
    src2 = cv::imread("Test01/000001.jpg", 0);
    cvt_LBP(src, dst_LBP);
    cvt_HOG(src, dst_HOG);
    cvt_LBP(src2, dst_LBP2);
    cvt_HOG(src2, dst_HOG2);
    //std::cout << dst_HOG.cols << "  " << src.cols << std::endl;
    cv::imshow("a", src);
    cv::imshow("b", dst_LBP);
    cv::imshow("c", dst_HOG);
    cv::imshow("d", src2);
    cv::imshow("e", dst_LBP2);
    cv::imshow("f", dst_HOG2);
    cv::waitKey(0);
    return 0;
}