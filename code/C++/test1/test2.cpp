#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


int main(void)
{
        ifstream ifs("BSDmakefile");
        vector<string> lst;

        for (string tmp; getline(ifs, tmp);)
                lst.push_back(tmp);
        
        for (unsigned i = 0; i < lst.size(); ++i)
                printf("%3u:  %s\n", i + 1, lst[i].c_str());
}
