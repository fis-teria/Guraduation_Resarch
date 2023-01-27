#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char *argv[])
{

    cv::Mat input_image; // 分割する画像の取得
    cv::Mat gray;
    cv::Mat sobel_X, sobel_Y;
    int col_num = input_image.cols; // 列方向の分割数
    int row_num = input_image.rows; // 行方向の分割数

    for (int i = 0; i < 703; i++)
    {
        std::cout << "now " << i << "times" << std::endl;
        std::string filename;
        std::string location_path;
        if (i < 10)
        {
            filename = "gra/slide/TempImg_divide/output_00" + std::to_string(i) + ".png";
            location_path = "gra/slide/TempImg_divide/hist/hist000" + std::to_string(i) + ".csv";
        }
        else if (i < 100)
        {
            filename = "gra/slide/TempImg_divide/output_0" + std::to_string(i) + ".png";
            location_path = "gra/slide/TempImg_divide/hist/hist00" + std::to_string(i) + ".csv";
        }
        else if (i < 1000)
        {
            filename = "gra/slide/TempImg_divide/output_" + std::to_string(i) + ".png";
            location_path = "gra/slide/TempImg_divide/hist/hist0" + std::to_string(i) + ".csv";
        }
        std::cout << filename << std::endl;
        std::cout << location_path << std::endl;
        input_image = cv::imread(filename, 1);
        if (input_image.empty() == true)
        {
            std::cerr << "入力画像が見つからない" << std::endl;
            return false;
        }
        std::ofstream outputfile(location_path);
        cv::cvtColor(input_image, gray, cv::COLOR_BGR2GRAY);
        cv::Sobel(gray, sobel_X, CV_32F, 1, 0);
        cv::Sobel(gray, sobel_Y, CV_32F, 0, 1);
        for (int x = 0; x < input_image.cols; x++)
        {
            for (int y = 0; y < input_image.rows; y++)
            {
                float dx = sobel_X.at<float>(y, x);
                float dy = sobel_Y.at<float>(y, x);
                float rad = atan2(dy, dx);
                float ang = (180 / (2 * M_PI)) * (rad + M_PI);
                outputfile << ang << "," << std::endl;
            }
        }
        outputfile.close();
    }

    // 画像の出力
    std::string filename = "gra/slide/TempImg_divide/glid.png";
    cv::imwrite(filename, input_image);

    return 0;
}
