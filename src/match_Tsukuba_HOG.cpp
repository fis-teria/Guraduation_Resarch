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
#include <numeric>
#include <algorithm>

// #define rootGunDai
#define MATCH00to01
#define root1001_1430match2_917
#define root1001_1435match2_1001_1030
std::string SAVE0 = "save_tsukuba_LBP.txt";
std::string SAVE1 = "save_tsukuba_Canny.txt";
std::string SAVE2 = "save_tsukuba_HOG.txt";
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
struct resulT
{
    double max;
    int num;
};

struct location
{
    double px;
    double py;
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
    else if (var < 1000)
    {
        back = dir + std::to_string(dir_num) + "_template/000" + std::to_string(var) + tag;
        return back;
    }
    else if (var < 10000)
    {
        back = dir + std::to_string(dir_num) + "_template/00" + std::to_string(var) + tag;
        return back;
    }
}

std::string make_tpath(std::string dir, int dir_num, int var, std::string tag)
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
    else if (var < 1000)
    {
        back = dir + std::to_string(dir_num) + "/000" + std::to_string(var) + tag;
        return back;
    }
    else if (var < 10000)
    {
        back = dir + std::to_string(dir_num) + "/00" + std::to_string(var) + tag;
        return back;
    }
}

int read_save()
{
    int save_num;
    std::ifstream ifs;

    ifs = std::ifstream(SAVE2.c_str());
    std::string str;
    getline(ifs, str);
    std::cout << str << std::endl;
    save_num = atoi(str.c_str());
    return save_num;
}

void save(int m)
{
    std::ofstream savefile;
    savefile = std::ofstream(SAVE2.c_str());
    savefile << m << std::endl;
    savefile.close();
}

void make_locate_image(const cv::Mat &src, const cv::Mat &dst, cv::Mat &locate_image)
{
    std::cout << "start make locate image" << std::endl;
    cv::Mat srcr, dstr;
    cv::resize(src, srcr, cv::Size(), 0.5, 0.5);
    cv::resize(dst, dstr, cv::Size(), 0.5, 0.5);
    locate_image = cv::Mat(srcr.rows, srcr.cols * 2, CV_8UC1);
    locate_image = cv::Scalar::all(0);
    std::cout << srcr.size << "\n"
              << locate_image.size << "\nstart make locate image first" << std::endl;

    for (int x = 0; x < locate_image.cols / 2; x++)
    {
        for (int y = 0; y < locate_image.rows; y++)
        {
            locate_image.at<float>(y, x) = srcr.at<float>(y, x);
        }
        std::cout << " x " << x << std::endl;
    }
    cv::imshow("locate", locate_image);

    std::cout << "start make locate image second\n"
              << locate_image.cols / 2 << std::endl;

    for (int x = 0; x < locate_image.cols / 2; x++)
    {
        for (int y = 0; y < locate_image.rows; y++)
        {
            // std::cout << y << " x " << x << std::endl;
            locate_image.at<float>(y, x + locate_image.cols / 2) = dstr.at<float>(y, x);
        }
    }

    std::cout << "clear make locate image" << std::endl;
    cv::imshow("locate2", locate_image);
}

std::vector<std::string> split(std::string &input, char delimiter)
{
    std::istringstream stream(input);
    std::string field;
    std::vector<std::string> result;
    while (getline(stream, field, delimiter))
    {
        result.push_back(field);
    }
    return result;
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

int position_Check(const cv::Mat src, std::string path, int now_locate)
{
    cv::Mat rim, sim, tim;
    cv::Mat sl_rim, sl_sim, sl_tim;
    /*
    rim = base image
    sim = normal template iamge
    tim = result normal template matching
    sl_rim = LBP  after resize rim
    sl_sim = LBP  after resize sim
    sl_tim = result cim template matching

    */
    int i, j; // seed num
    int num_m;
    int path_m;
    // int load_num = read_save();
    std::vector<resulT> result;
    int average = 0;
    int result_size = 0;
    int start, end;
    std::string dir;
    std::string tag;
    std::string dpath;

    double min_sl, max_sl;
    cv::Point p_min_sl, p_max_sl;

    // Match  0917_1349 to 1001_1435
    num_m = 4820; // 0917_1349
    path_m = 2;
    dir = "gra/Tsukuba0";
    tag = ".png";

    start = now_locate - 100;
    if (start < 2)
    {
        start = 2;
    }
    end = now_locate + 100;
    if (end > num_m)
    {
        end = num_m;
    }
    std::cout << start << " : " << end << std::endl;

    // HOG
    cvt_HOG(src, sl_rim);

    //  std::cout << "start matching" << std::endl;
    for (int k = start; k < end; k++)
    {
        dpath = make_tpath(dir, 2, k, tag);
        // std::cout << "match " << path << " : " << dpath << std::endl;

        sim = cv::imread(dpath, cv::IMREAD_GRAYSCALE);
        // clock_t begin = clock();
        // cvt_LBP(sim, sl_sim);
        // HOG
        cvt_HOG(sim, sl_sim);
        cv::matchTemplate(sl_rim, sl_sim(cv::Range(sl_sim.rows / 10, (9 * sl_sim.rows) / 10), cv::Range(sl_sim.cols / 10, (9 * sl_sim.cols) / 10)), sl_tim, cv::TM_CCOEFF_NORMED);
        cv::minMaxLoc(sl_tim, &min_sl, &max_sl, &p_min_sl, &p_max_sl);
        // clock_t endc = clock();
        // print_elapsed_time(begin, endc);

        result.resize(result_size + 1);
        result[result_size].max = max_sl;
        result[result_size].num = k;
        result_size++;
        // std::cout << k << std::endl;
    }
    std::sort(result.begin(), result.end(), [](const resulT &alpha, const resulT &beta)
              { return alpha.max < beta.max; });
    /*
    std::cout << result[result_size - 1].num << " " << result_size << " " << result.size() << std::endl;
    for (int j = 0; j < result_size - 1; j++)
    {
        std::cout << result[j].num << std::endl;
    }
    */
    return result[result.size() - 1].num;
}

int main(int argc, char **argv)
{
    /*
    if (argc != 2)
    {
        std::cout << "Error Too many or too few command line arguments" << std::endl;
        return 0;
    }
*/

    cv::Mat img, src, dst, locate_image;
    cv::Mat img_lst[2];
    cv::Mat srcr, dstr;
    std::string dir, rdir;
    std::string tag;
    std::string path;
    int now_locate = 0;
    int time = 0;
    int load_num = read_save();
    if (load_num == -1)
    {
        std::cout << "Error command line num only use 0, 1, 2" << std::endl;
        return 0;
    }

    dir = "gra/Tsukuba0";
    rdir = "gra/result_HOG_Tsukuba0";
    tag = ".png";

    std::ifstream ifs("gra/Tsukuba00_locate/location.csv");
    std::ifstream jfs("gra/Tsukuba02_locate/location.csv");

    std::string line;
    std::vector<location> locate_main;
    std::vector<location> locate_tmp;
    int lm_size = 0;
    int lt_size = 0;
    while (getline(ifs, line))
    {

        std::vector<std::string> strvec = split(line, ',');

        locate_main.resize(lm_size + 1);
        locate_main[lm_size].px = std::stod(strvec[0]);
        locate_main[lm_size].py = std::stod(strvec[1]);
        // std::cout << locate_main[lm_size].px << " x " << locate_main[lm_size].py << std::endl;
        lm_size++;
    }
    while (getline(jfs, line))
    {

        std::vector<std::string> strvec2 = split(line, ',');

        locate_tmp.resize(lt_size + 1);
        locate_tmp[lt_size].px = std::stod(strvec2[0]);
        locate_tmp[lt_size].py = std::stod(strvec2[1]);
        // std::cout << locate_tmp[lt_size].px << " x " << locate_tmp[lt_size].py << std::endl;
        lt_size++;
    }
    ///*
    std::string location_path;
    location_path = "gra/r_file/location_HOG.csv";
    std::ofstream outputfile(location_path, std::ios::app);
    for (int i = load_num; i < 4546; i++)
    {
        std::cout << "start positioning " << i << " times" << std::endl;
        path = make_tpath(dir, 0, i, tag);
        // std::cout << path << std::endl;
        img = cv::imread(path, 1);
        cv::cvtColor(img, src, cv::COLOR_BGR2GRAY);
        
        clock_t begin = clock();
        now_locate = position_Check(src, path, i);
        std::cout << now_locate << std::endl;
        clock_t end = clock();
        print_elapsed_time(begin, end);

        // time = 200000 - ((end - begin) / CLOCKS_PER_SEC) * 1000000;

        dst = cv::imread(make_tpath(dir, 2, now_locate, tag), 1);
        // locate_image = cv::imread(make_path(dir, 2, now_locate, tag), 0);
        // make_locate_image(img, dst, locate_image);
        // cv::imshow("get image", img);
        cv::resize(img, srcr, cv::Size(), 0.5, 0.5);
        cv::resize(dst, dstr, cv::Size(), 0.5, 0.5);
        img_lst[0] = srcr.clone();
        img_lst[1] = dstr.clone();
        std::cout << img_lst[0].size << " " << img_lst[1].size << std::endl;
        cv::hconcat(img_lst, 2, locate_image);
        // std::cout << "show locate image " << locate_image.size << std::endl;
        // cv::imshow("locate image", locate_image);
        cv::imwrite(make_tpath(rdir, 0, i, tag), locate_image);
        outputfile << locate_tmp[now_locate].px << "," << locate_tmp[now_locate].py << "," << now_locate << std::endl;
        //  usleep(10000000);
        //  cv::waitKey(0);
        save(i);
    }
    outputfile.close();
    //*/
    return 0;
}