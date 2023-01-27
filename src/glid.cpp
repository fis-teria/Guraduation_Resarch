#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char *argv[])
{

    cv::Mat input_image = cv::imread("gra/slide/TemplateImage.jpeg"); // 分割する画像の取得
    if (input_image.empty() == true)
    {
        std::cerr << "入力画像が見つからない" << std::endl;
        return false;
    }
    int col_num = input_image.cols; // 列方向の分割数
    int row_num = input_image.rows; // 行方向の分割数

    for (int x = 0; x < col_num; x++)
    {
        if(x % 20 == 0)
        cv::line(input_image, cv::Point(x,0), cv::Point(x,row_num), cv::Scalar(255, 255, 255));
    }
    for (int y = 0; y < row_num; y++)
    {
        if(y % 20 == 0)
        cv::line(input_image, cv::Point(0,y), cv::Point(col_num,y), cv::Scalar(255, 255, 255));
    }
    // 画像の出力
    std::string filename = "gra/slide/TempImg_divide/glid.png";
    cv::imwrite(filename, input_image);

    return 0;
}

