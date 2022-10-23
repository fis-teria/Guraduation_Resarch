#include <opencv2/opencv.hpp>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <numeric>
#include <algorithm>

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
void gaussian(const cv::Mat &src, cv::Mat &dst, int ksize, double sigma)
{
    dst = cv::Mat(src.rows, src.cols, CV_8UC3);
    double gauss_const = 1 / (2 * CV_PI * sigma * sigma);
    int size = ksize;
    double gauss_weight[size][size];
    double gauss_Sum = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            gauss_weight[i][j] = gauss_const * expl(-((-1 + i) * (-1 + i) + (-1 + j) * (-1 + j)) / (2 * sigma * sigma));
        }
    }
    for (int x = size / 2; x < src.cols - size / 2; x++)
    {
        for (int y = size / 2; y < src.rows - size / 2; y++)
        {
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    if (x + i - -(int)(size / 2) < 0)
                        std::cout << "1" << std::endl;
                    dst.at<cv::Vec3b>(y, x)[0] += (src.at<cv::Vec3b>(y + j - (int)(size / 2), x + i - (int)(size / 2))[0] * gauss_weight[i][j]);
                    dst.at<cv::Vec3b>(y, x)[1] += (src.at<cv::Vec3b>(y + j - (int)(size / 2), x + i - (int)(size / 2))[1] * gauss_weight[i][j]);
                    dst.at<cv::Vec3b>(y, x)[2] += (src.at<cv::Vec3b>(y + j - (int)(size / 2), x + i - (int)(size / 2))[2] * gauss_weight[i][j]);
                }
            }
        }
    }
    //cv::imshow("x", dst);
}
void check_sky_edge(const cv::Mat src)
{
    std::cout << "start check_sky_edge" << std::endl;
    cv::Mat gim;
    cv::Mat simx, simy;
    cv::Mat result = cv::Mat(src.rows, src.cols, CV_8UC3);
    int edge_Inte = 0;
    int edge_Inte2 = 0;
    int edge_Inte3 = 0;
    float theta;
    float dx, dy;
    std::vector<int> edge_sum;
    double sum;
    int count = 0;
    //imshow("w", src);
    gaussian(src, gim, 3, 0.8);
    //cv::GaussianBlur(src, gim, cv::Size(5, 5), 0, 0, cv::BORDER_DEFAULT);
    //cv::imshow("d", gim);
    //cv::waitKey(0);
    cv::Sobel(gim, simx, CV_32FC1, 1, 0);
    cv::Sobel(gim, simy, CV_32FC1, 0, 1);

    std::ofstream outputfile("gra/r_file/canny_histgram.csv");
    std::cout << simx.cols << " " << src.cols << std::endl;
    for (int x = 0; x < src.cols; x++)
    {
        for (int y = 0; y < src.rows; y++)
        {
            dx = simx.at<float>(y, x);
            dy = simx.at<float>(y, x);
            edge_Inte = sqrt(dx * dx + dy * dy) + 0.5; // intに変更するときの少数値切り捨て対策

            theta = (atan2(dy, dx) + CV_PI - (CV_PI / 4)) * (180 / (2 * CV_PI));
            if (1)
            {
                if (theta > 47.5 && theta <= 112.5) // 90
                {
                    if (y - 1 > 0)
                        edge_Inte2 = sqrt(simx.at<float>(y - 1, x) * simx.at<float>(y - 1, x) + simy.at<float>(y - 1, x) * simy.at<float>(y - 1, x)) + 0.5;
                    if (y + 1 < src.rows)
                        edge_Inte3 = sqrt(simx.at<float>(y + 1, x) * simx.at<float>(y + 1, x) + simy.at<float>(y + 1, x) * simy.at<float>(y + 1, x)) + 0.5;
                    if (edge_Inte > edge_Inte2 && edge_Inte > edge_Inte3)
                    {
                        result.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
                        // std::cout << edge_Inte << ", " << theta << std::endl;
                        edge_sum.resize(count + 1);
                        edge_sum[count] += edge_Inte;
                        count++;
                        outputfile << edge_Inte << std::endl;
                    }
                    else
                        result.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
                }
                else if (theta >= -22.5 && theta <= 22.5) // 0
                {
                    if (x - 1 > 0)
                        edge_Inte2 = sqrt(simx.at<float>(y, x - 1) * simx.at<float>(y, x - 1) + simy.at<float>(y, x - 1) * simy.at<float>(y, x - 1)) + 0.5;
                    if (x + 1 < src.cols)
                        edge_Inte3 = sqrt(simx.at<float>(y, x + 1) * simx.at<float>(y, x + 1) + simy.at<float>(y, x + 1) * simy.at<float>(y, x + 1)) + 0.5;
                    if (edge_Inte > edge_Inte2 && edge_Inte > edge_Inte3)
                    {
                        result.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
                        // std::cout << edge_Inte << ", " << theta << std::endl;
                        edge_sum.resize(count + 1);
                        edge_sum[count] += edge_Inte;
                        count++;
                        outputfile << edge_Inte << std::endl;
                    }
                    else
                        result.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
                }
                else if (theta > 22.5 && theta <= 47.5) // 45
                {
                    if (x - 1 > 0 && y - 1 > 0)
                        edge_Inte2 = sqrt(simx.at<float>(y - 1, x - 1) * simx.at<float>(y - 1, x - 1) + simy.at<float>(y - 1, x - 1) * simy.at<float>(y - 1, x - 1)) + 0.5;
                    if (x + 1 < src.cols && y + 1 < src.rows)
                        edge_Inte3 = sqrt(simx.at<float>(y + 1, x + 1) * simx.at<float>(y + 1, x + 1) + simy.at<float>(y + 1, x + 1) * simy.at<float>(y + 1, x + 1)) + 0.5;
                    if (edge_Inte > edge_Inte2 && edge_Inte > edge_Inte3)
                    {
                        result.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
                        // std::cout << edge_Inte << ", " << theta << std::endl;
                        edge_sum.resize(count + 1);
                        edge_sum[count] += edge_Inte;
                        count++;
                        outputfile << edge_Inte << std::endl;
                    }
                    else
                        result.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
                }
                else if (theta > 112.5 && theta <= 157.5) // 135
                {
                    if (x - 1 > 0 && y + 1 < src.rows)
                        edge_Inte2 = sqrt(simx.at<float>(y + 1, x - 1) * simx.at<float>(y + 1, x - 1) + simy.at<float>(y + 1, x - 1) * simy.at<float>(y + 1, x - 1));
                    if (x + 1 < src.cols && y - 1 < 0)
                        edge_Inte3 = sqrt(simx.at<float>(y - 1, x + 1) * simx.at<float>(y - 1, x + 1) + simy.at<float>(y - 1, x + 1) * simy.at<float>(y - 1, x + 1));
                    if (edge_Inte > edge_Inte2 && edge_Inte > edge_Inte3)
                    {
                        result.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
                        // std::cout << edge_Inte << ", " << theta << std::endl;
                        edge_sum.resize(count + 1);
                        edge_sum[count] += edge_Inte;
                        count++;
                        outputfile << edge_Inte << std::endl;
                    }
                    else
                        result.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
                }
            }
            edge_Inte2 = 0;
            edge_Inte3 = 0;
        }
    }
    outputfile.close();
    sum = std::accumulate(edge_sum.begin(), edge_sum.end(), 0.0);
    //std::cout << "a" << std::endl;
    // std::cout << sum / count << ", " << *max_element(edge_sum.begin(), edge_sum.end()) << ", " << *min_element(edge_sum.begin(), edge_sum.end()) << std::endl;
    /*
    cv::imshow("a", src);
    cv::imshow("b", simx);
    cv::imshow("c", simy);
    cv::imshow("g", result);
    cv::waitKey(0);
    */
}

int main(int argc, char **argv)
{
    /*
    if(argc != 3) {
      fprintf(stderr,"usage: %s in_image out_image\n",argv[0]);
      return 1;
    }
    */
    cv::Mat rim, sim, tim;
    cv::Mat c_rim, c_sim, c_tim;
    cv::Mat cs_rim, cs_sim, cs_tim;
    std::string dir = "Test0";
    std::cout << dir << "\n";
    std::string path;
    std::string dpath;
    int i = atoi(argv[1]);
    int j = atoi(argv[2]);
    cv::Mat aim = cv::imread(make_cpath(dir, 0, i), 1);
    // Mat aim = imread("sample.png", 1);
    if (aim.empty())
        return -1;
    // test00
    path = make_path(dir, 0, i);
    rim = cv::imread(path, cv::IMREAD_GRAYSCALE);
    check_sky_edge(aim(cv::Range(0, aim.rows / 5), cv::Range(2 * aim.cols / 5, 3 * aim.cols / 5)));
    //check_sky_edge(aim);
    cv::Canny(cv::imread(make_cpath(dir, 0, i)), c_rim, 100, 200);
    cv::Canny(rim, cs_rim, 200, 400);

    // test01
    dpath = make_path(dir, 1, j);
    sim = cv::imread(dpath, cv::IMREAD_GRAYSCALE);
    cv::Canny(cv::imread(make_cpath(dir, 1, j)), c_sim, 100, 400);
    cv::Canny(sim, cs_sim, 500, 600);

    /*** 画像を表示 ***/
    /*
    cv::imshow("1", rim);
    cv::imshow("2", sim);
    cv::imshow("3", c_rim);
    cv::imshow("4", c_sim);
    cv::imshow("5", cs_rim);
    cv::imshow("6", cs_sim);
    cv::imwrite("gra/slide/01.jpg", rim);
    cv::imwrite("gra/slide/02.jpg", sim);
    cv::imwrite("gra/slide/03.jpg", c_rim);
    cv::imwrite("gra/slide/04.jpg", c_sim);
    cv::imwrite("gra/slide/05.jpg", cs_rim);
    cv::imwrite("gra/slide/06.jpg", cs_sim);
    */
    cv::waitKey(0);
    return 0;
}