#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

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

int main(){
    string s1 = "This will be converted to hexadecimal";
    string s2;
    stringstream ss;

    cout << "string: " << s1 << endl;

    for (const auto &item : s1) {
        ss << hex << int(item);
    }
    s2 = ss.str();
    cout << "hexval: " << s2 << endl;

    ////==== 文字列への再変換
    string s3;
    string s4 = "536f6e7958512d4243343248";
    s3 = hex2bin(s4);
    cout << "re-string: " << s3 << endl;
    ////====

    return EXIT_SUCCESS;
}