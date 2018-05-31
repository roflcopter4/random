#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;


int
main(int argc, char **argv)
{
        ifstream ifs("./BSDmakefile");
#if 0
        stringstream buffer;
        buffer << ifs.rdbuf();
        ifs.close();

        string s = buffer.str();
        vector<string> lst;
        int64_t i, j;

        for (i = j = 0; j >= 0; i = j + 1)
                lst.push_back(s.substr(i, ((j = s.find('\n', i)) - i)));

        string result;
        for (const auto &piece : lst)
                result += piece + "\n";

        cout << result << endl;
#endif

        vector<string> lst;
        for (string tmp; getline(ifs, tmp);)
                lst.push_back(tmp);
        

        for (unsigned i = 0; i < lst.size(); ++i)
                printf("%3u:  %s\n", i+1, lst[i].c_str());
}
