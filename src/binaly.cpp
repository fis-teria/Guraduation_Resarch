#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

//https://stackoverflow.com/questions/35522507/c-converting-long-hex-string-to-binary
std::string hex2bin(std::string s) {
    std::string rc;
    int nLen = s.length();
    int tmp;
    for (int i(0); i + 1 < nLen; i += 2) {
        if (std::istringstream(s.substr(i, 2)) >> std::hex >> tmp) {
            rc.push_back(tmp);
        }
    }
    return rc;
}

int main()
{
    string fileName = "20220922_184525.JPG";    //ファイル名


    //ファイル名からバイナリファイルで読み込む
    std::ifstream ifs(fileName, std::ios::binary);

    //読込サイズを調べる。
    ifs.seekg(0, std::ios::end);
    long long int size = ifs.tellg();
    ifs.seekg(0);

    //読み込んだデータをchar型に出力する
    char *data = new char[size];
    ifs.read(data, size);

    //サイズを出力する
    std::cout << "size = "<< size <<"\n" ;

    //出力ファイル
    std::ofstream outputfile("binaly_prototype.bin");
    for (int i = 1; i < size + 1; i++)
    {
        std::stringstream ss;
        /*16進数文字列に変換する
        　char型をASCIIではなく、数値で出力する場合は先頭に+を付ける*/
        ss << std::hex << +data[i - 1];

        string test = ss.str();
        std::cout << hex2bin(test) << std::endl;
        //符号付きであるか判定する
        if (test.size() > 5)
        {
            //符号付きの場合fが付くため、fを削除する
            string test = ss.str().erase(0, 6);
            outputfile << test << " ";
        }
        else
        {
            //符号付きでない場合そのまま出力する
            outputfile << ss.str() << " ";
        }
        //16バイト毎に改行する
        if ((i % 16) == 0)
        {
            outputfile << "\n";
        }
    }
    std::cout << "\nEnd!\n"; 
    outputfile.close();
    delete data;
    return 0;
}