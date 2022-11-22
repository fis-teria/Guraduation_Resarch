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
std::string SAVE = "save_tsukuba.txt";
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
        back = dir + std::to_string(dir_num) + "/00000" + std::to_string(var) + ".csv";
        return back;
    }
    else if (var < 100)
    {
        back = dir + std::to_string(dir_num) + "/0000" + std::to_string(var) + ".csv";
        return back;
    }
    else if (1 < 1000)
    {
        back = dir + std::to_string(dir_num) + "/000" + std::to_string(var) + ".csv";
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


void templete_LBP(int execute_num)
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

    double min_sl, max_sl;
    cv::Point p_min_sl, p_max_sl;

    switch (execute_num)
    {
    case 21:          // Match 1001_1030 to 0917_1349
        num_t = 4548; // 1001_1030
        num_m = 4546; // 0917_1349
        path_t = 1;
        path_m = 0;
        c_tnum1 = 200;
        c_tnum2 = 500;
        cs_tnum1 = 200;
        cs_tnum2 = 400;
        c_mnum1 = 200;
        c_mnum2 = 500;
        cs_mnum1 = 200;
        cs_mnum2 = 400;
        dir = "gra/Tsukuba0";
        r_dir = "gra/result_Tsukuba0";
        tag = ".png";
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

        rim = cv::imread(path, cv::IMREAD_GRAYSCALE);
        // small LBP
        cvt_LBP(rim, sl_rim);

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

            //small LBP
            cvt_LBP(sim, sl_sim);
            cv::matchTemplate(sl_rim, sl_sim(cv::Range(sl_sim.rows / 10, (9 * sl_sim.rows) / 10), cv::Range(sl_sim.cols / 10, (9 * sl_sim.cols) / 10)), sl_tim, cv::TM_CCOEFF_NORMED);
            cv::minMaxLoc(sl_tim, &min_sl, &max_sl, &p_min_sl, &p_max_sl);


            // sleep(1);
            clock_t end = clock();
            print_elapsed_time(begin, end);
        }

        //類似度を昇順にソート
        //最大の類似度を持つマッチング画像へのパス
        /*
        dpath = make_path(dir, path_m, results[result_size - 1].num);
        std::cout << path << " : " << dpath << std::endl;
        std::cout
            << "all max = " << results[result_size - 1].max_t << "(x ,y ) = (" << results[result_size - 1].x_t << ", " << results[result_size - 1].y_t << ")" << std::endl;
        */
        save(m);
    }
    save(0);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "prease select number\nTsukuba Root\n21 base 1001_1030 template 0917_1349\n22 bas 1001_1030 template 1001_1435" << std::endl;
        return 1;
    }
    int execute_num = atoi(argv[1]);

    templete_LBP(execute_num);

    return 0;
}
