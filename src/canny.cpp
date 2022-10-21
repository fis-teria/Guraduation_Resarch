#include <opencv2/opencv.hpp>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <numeric>
#include <algorithm>

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

void check_sky_edge(const cv::Mat src)
{
    Mat gim;
    Mat simx, simy;
    Mat result = Mat(src.rows, src.cols, CV_8UC3);
    int edge_Inte = 0;
    int edge_Inte2 = 0;
    int edge_Inte3 = 0;
    float theta;
    float dx, dy;
    std::vector<int> edge_sum;
    double sum;
    int count = 0;
    GaussianBlur(src, gim, Size(3, 3), 1.0);
    imshow("d", gim);
    Sobel(gim, simx, CV_32FC1, 1, 0);
    Sobel(gim, simy, CV_32FC1, 0, 1);

    std::ofstream outputfile("gra/r_file/canny_histgram.csv");

    for (int x = 0; x < src.cols; x++)
    {
        for (int y = 0; y < src.rows; y++)
        {
            dx = simx.at<float>(y, x);
            dy = simx.at<float>(y, x);
            edge_Inte = sqrt(dx * dx + dy * dy) + 0.5; // intに変更するときの少数値切り捨て対策

            theta = (atan2(dy, dx) + CV_PI - (CV_PI / 4)) * (180 / (2 * CV_PI));
            if (edge_Inte >= 0)
            {
                if (theta > 47.5 && theta <= 112.5) // 90
                {
                    if (y - 1 > 0)
                        edge_Inte2 = sqrt(simx.at<float>(y - 1, x) * simx.at<float>(y - 1, x) + simy.at<float>(y - 1, x) * simy.at<float>(y - 1, x)) + 0.5;
                    if (y + 1 < src.rows)
                        edge_Inte3 = sqrt(simx.at<float>(y + 1, x) * simx.at<float>(y + 1, x) + simy.at<float>(y + 1, x) * simy.at<float>(y + 1, x)) + 0.5;
                    if (edge_Inte > edge_Inte2 && edge_Inte > edge_Inte3)
                    {
                        result.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
                        std::cout << edge_Inte << ", " << theta << std::endl;
                        edge_sum.resize(count + 1);
                        edge_sum[count] += edge_Inte;
                        count++;
                        outputfile << edge_Inte << std::endl;
                    }
                    else
                        result.at<Vec3b>(y, x) = Vec3b(0, 0, 0);
                }
                else if (theta >= -22.5 && theta <= 22.5) // 0
                {
                    if (x - 1 > 0)
                        edge_Inte2 = sqrt(simx.at<float>(y, x - 1) * simx.at<float>(y, x - 1) + simy.at<float>(y, x - 1) * simy.at<float>(y, x - 1)) + 0.5;
                    if (x + 1 < src.cols)
                        edge_Inte3 = sqrt(simx.at<float>(y, x + 1) * simx.at<float>(y, x + 1) + simy.at<float>(y, x + 1) * simy.at<float>(y, x + 1)) + 0.5;
                    if (edge_Inte > edge_Inte2 && edge_Inte > edge_Inte3)
                    {
                        result.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
                        std::cout << edge_Inte << ", " << theta << std::endl;
                        edge_sum.resize(count + 1);
                        edge_sum[count] += edge_Inte;
                        count++;
                        outputfile << edge_Inte << std::endl;
                    }
                    else
                        result.at<Vec3b>(y, x) = Vec3b(0, 0, 0);
                }
                else if (theta > 22.5 && theta <= 47.5) // 45
                {
                    if (x - 1 > 0 && y - 1 > 0)
                        edge_Inte2 = sqrt(simx.at<float>(y - 1, x - 1) * simx.at<float>(y - 1, x - 1) + simy.at<float>(y - 1, x - 1) * simy.at<float>(y - 1, x - 1)) + 0.5;
                    if (x + 1 < src.cols && y + 1 < src.rows)
                        edge_Inte3 = sqrt(simx.at<float>(y + 1, x + 1) * simx.at<float>(y + 1, x + 1) + simy.at<float>(y + 1, x + 1) * simy.at<float>(y + 1, x + 1)) + 0.5;
                    if (edge_Inte > edge_Inte2 && edge_Inte > edge_Inte3)
                    {
                        result.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
                        std::cout << edge_Inte << ", " << theta << std::endl;
                        edge_sum.resize(count + 1);
                        edge_sum[count] += edge_Inte;
                        count++;
                        outputfile << edge_Inte << std::endl;
                    }
                    else
                        result.at<Vec3b>(y, x) = Vec3b(0, 0, 0);
                }
                else if (theta > 112.5 && theta <= 157.5) // 135
                {
                    if (x - 1 > 0 && y + 1 < src.rows)
                        edge_Inte2 = sqrt(simx.at<float>(y + 1, x - 1) * simx.at<float>(y + 1, x - 1) + simy.at<float>(y + 1, x - 1) * simy.at<float>(y + 1, x - 1));
                    if (x + 1 < src.cols && y - 1 < 0)
                        edge_Inte3 = sqrt(simx.at<float>(y - 1, x + 1) * simx.at<float>(y - 1, x + 1) + simy.at<float>(y - 1, x + 1) * simy.at<float>(y - 1, x + 1));
                    if (edge_Inte > edge_Inte2 && edge_Inte > edge_Inte3)
                    {
                        result.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
                        std::cout << edge_Inte << ", " << theta << std::endl;
                        edge_sum.resize(count + 1);
                        edge_sum[count] += edge_Inte;
                        count++;
                        outputfile << edge_Inte << std::endl;
                    }
                    else
                        result.at<Vec3b>(y, x) = Vec3b(0, 0, 0);
                }
            }
            edge_Inte2 = 0;
            edge_Inte3 = 0;
        }
    }
    outputfile.close();
    sum = std::accumulate(edge_sum.begin(), edge_sum.end(), 0.0);
    std::cout << sum / count << ", " << *max_element(edge_sum.begin(), edge_sum.end()) << ", " << *min_element(edge_sum.begin(), edge_sum.end()) << std::endl;
    imshow("a", src);
    imshow("b", simx);
    imshow("c", simy);
    imshow("g", result);
    waitKey(0);
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
    Mat aim = imread(make_cpath(dir, 0, i), 0);
    // test00
    path = make_path(dir, 0, i);
    rim = imread(path, IMREAD_GRAYSCALE);
    check_sky_edge(aim(Range(0, aim.rows / 5), Range(2 * aim.cols / 5, 3 * aim.cols / 5)));
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
    /*
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