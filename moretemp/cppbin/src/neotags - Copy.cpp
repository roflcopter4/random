#include "neotags.h"
#include <clocale>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using uint = unsigned int;

#ifdef DOSISH
#   define WIN_THREADS
#endif

#ifdef USE_PTHREADS
#  include <pthread.h>
#endif

struct pdata {
        int threadnum;
        const string *vim_buf;
        const string *lang;
        const string *order;
        const vector<string> *skip;
        const vector<string> *equiv;
        vector<struct lldata> *lst;
        uint start;
        uint num;
};

static void *do_search(void *vdata);
static bool ll_cmp(const struct lldata *A, const struct lldata *B);
static void Search(vector<struct lldata> *tags, const string *vim_buf,
                   const string *lang, const string *order,
                   const vector<string> *skip, const vector<string> *equiv);

static vector<string> get_colon_data(char *oarg);

char *program_name;

#ifdef DOSISH
#  define __CONST__
#  define SEPCHAR ';'
#  include <malloc.h>
#  define VDECL(TYPE_, NAME_, NUM_) \
        auto NAME_ = static_cast<TYPE_ *>(_malloca((NUM_) * sizeof(TYPE_)))
#  define FREEA(PTR_) _freea(static_cast<void *>(PTR_))
#else
#  define VDECL(TYPE_, NAME_, NUM_) TYPE_ NAME_[NUM_]
#  define FREEA(PTR_)
#  define __CONST__ const
#  define SEPCHAR ':'
#endif

#define REQUIRED_INPUT 8


//bool invalidChar (const int c) 
//{  
//    return !(c >= 0 && c < 128);   
//} 
//void stripUnicode(string & str) 
//{ 
//    str.erase(remove_if(str.begin(),str.end(), invalidChar), str.end());  
//}


int
main(int argc, char *argv[])
{
        setlocale(LC_NUMERIC, "");
        int files_read    = 0;
        program_name = handle_progname(*argv++);
        if (isatty(0))
                errx(1, "This program can't be run manually.");
        if (--argc != REQUIRED_INPUT)
                errx(2, "Error: Wrong number of paramaters (%d, need %d).",
                        argc, REQUIRED_INPUT);
        cerr << "Program ID: " PROG_ID << endl;

        auto files   = get_colon_data(*argv++);
        auto ctlang  = string(*argv++);
        auto vimlang = string(*argv++);
        auto order   = string(*argv++);
        bool strip   = xatoi(*argv++);
        //++argv;  // arg not used in C++ but required for C implementation
        auto nchars  = xatoi(*argv++);
        auto skip    = get_colon_data(*argv++);
        auto equiv   = get_colon_data(*argv++);

        //string vim_buf;
        vector<struct lldata> tags;

        for (uint i = 0;
             i < files.size() && files.at(i).at(0) && files.at(i + 1).at(0);
             i += 2)
                files_read += getlines(&tags, &files.at(i), &files.at(i + 1));

        if (files_read == 0)
                errx(1, "Error: no files were successfully read.");
        else
                warnx("Read %d files\n", files_read);

        //stringstream buffer;
        //buffer << cin.rdbuf();
        const char *otmp = new char[nchars + 1];
        char *tmp = const_cast<char *>(otmp);
        int ch;
        //fread(tmp, 1llu, nchars, stdin);
        for (size_t i = 0; i < nchars; ++i)
                if ((ch = getchar()) > 0 && ch < 127)
                        *tmp++ = ch;
                else if (ch == EOF)
                        break;
        *tmp = '\0';
        string vim_buf(otmp);
        delete[] otmp;

        if (strip) {
                cerr << "Stripping comments..." << endl;
                struct lldata tmp = {vim_buf, '\0'};
                char *buf = strip_comments(&tmp, vimlang.c_str());
                if (buf) {
                        vim_buf = buf;
                        delete[] buf;
                }
        }

        Search(&tags, &vim_buf, &ctlang, &order, &skip, &equiv);

        return 0;
}


static vector<string>
get_colon_data(char *oarg)
{
        unsigned num = 0;
        char *arg = oarg;

        if (*arg != '\0') {
                do if (*arg == SEPCHAR) {
                        *arg++ = '\0';
                        ++num;
                } while (*arg++);
        }

        // The loop above will miss the last element, so we increment num.
        arg = oarg;
        vector<string> data(++num);

        for (unsigned i = 0; i < (num - 1); ++i) {
                while (*arg++)
                        ;
                data[i] = oarg;
                oarg = arg;
        }

        return data;
}


static bool  /* Comparison function for qsort */
ll_cmp(const struct lldata *A, const struct lldata *B)
{
        bool ret;

        if (A->kind == B->kind) {
                //if (A->s.length() == B->s.length())
                        // ret = memcmp(A->s, B->s, A->s.length());
                        ret = A->s < B->s;
                /*else
                        ret = A->s.length() < B->s.length();*/
        } else
                ret = A->kind < B->kind;

        return ret;
}


/* ========================================================================== */


static bool
in_order(const vector<string> *equiv, const string *order, char *group)
{
        // `group' is actually a pointer to a char, not a C string.
        for (const auto& item : *equiv) {
                if (*group == item.at(0)) {
                        *group = item.at(1);
                        break;
                }
        }

        //return order->find_first_of(*group) != (-1lu);
        return strchr(order->c_str(), *group) != nullptr;
}


static inline string
stolower(string s)
{
        transform(s.begin(), s.end(), s.begin(),
                  [](unsigned char c) { return tolower(c); });
        return s;
}


static bool
is_correct_lang(const string *lang, __CONST__ string *match_lang)
{
#ifdef DOSISH
        if ((*match_lang)[match_lang->size()] == '\r')
                match_lang->pop_back();
#endif
        if (stolower(*match_lang) == stolower(*lang))
                return true;

        if ((stolower(*lang) == "c" || stolower(*lang) == "c\\+\\+") &&
            (stolower(*match_lang) == "c++" || stolower(*match_lang) == "c"))
                return true;

        return false;
}


static bool
skip_tag(const vector<string> *skip, const string *find)
{
        for (const auto& tag : *skip)
                if (tag == *find)
                        return true;

        return false;
}


/*============================================================================*/


static void
Search(vector<struct lldata> *tags,
       const string *vim_buf,
       const string *lang,
       const string *order,
       const vector<string> *skip,
       const vector<string> *equiv)
{
        if (tags->empty()) {
                cerr << "No tags found!" << endl;
                return;
        }
        
        //int num_threads = find_num_cpus();
        //if (num_threads == 0)
        //        num_threads = 4;
        //cerr << "Using " << num_threads << " cpus." << endl;

        //VDECL(pthread_t, tid, num_threads);
        const int num_threads = 1;
        //pthread_t tid[1];

        //for (int i = 0; i < num_threads; ++i) {
        const int i = 0;
        auto tmp  = new struct pdata;
        const uint quot = (tags->size() / num_threads);

        uint64_t num;
        //if (i == (num_threads - 1))
                //num = tags->size() - ((num_threads - 1) * quot);
        //else
                num = quot;

        tmp->threadnum = i;
        tmp->vim_buf   = vim_buf;
        tmp->lang  = lang;
        tmp->order = order;
        tmp->skip  = skip;
        tmp->equiv = equiv;
        tmp->lst   = tags;
        tmp->start = i * quot;
        tmp->num   = static_cast<uint>(num);

        errno = 0;
                //if (pthread_create(tid + i, nullptr, do_search, tmp) != 0 || errno)
                //        err(1, "pthread_create failed");
        //}

        auto out = vector<vector<struct lldata> *>(num_threads);
        //for (unsigned th = 0; th < num_threads ; ++th) {
        void *ret = do_search(tmp);
                //pthread_join(tid[th], &tmp);
        out[0] = static_cast<vector<struct lldata> *>(ret);
        //}

        size_t total = 0;
        for (auto &out_i : out)
                total += out_i->size();

        size_t offset = 0;
        auto alldata  = vector<struct lldata *>(total);

        for (auto &out_i : out)
                if (!out_i->empty())
                        for (auto& item : *out_i)
                                alldata[offset++] = &item;

        sort(alldata.begin(), alldata.end(), ll_cmp);

        /* Always display the first item. */
        if (alldata.empty())
                errx(4, "WTF");
        cout << alldata.at(0)->kind << "\n" << alldata.at(0)->s << endl;

        for (size_t i = 1; i < alldata.size(); ++i) {
                if (alldata.at(i)->s != alldata.at(i - 1)->s)
                        cout << alldata.at(i)->kind << "\n"
                             << alldata.at(i)->s << endl;
        }

        for (auto &Tout : out)
                delete Tout;
        //FREEA(tid);
}


static void *
do_search(void *vdata)
{
#define CUR_STR ((*data->lst)[i].s)
        const auto data = static_cast<struct pdata *>(vdata);
        const auto end  = data->start + data->num;
        auto ret = new vector<struct lldata>;

        for (uint i = data->start; i < end; ++i) {
                if (CUR_STR.at(0) == '!')
                        continue;
                vector<string> lst;
                int64_t First, Last;

                // The format for substr is given by the starting index and the
                // length, not by start index and last index, so we have to do
                // some shenanigans here to make it work.
                for (First = Last = 0; Last >= 0; First = Last + 1)
                        lst.push_back(CUR_STR.substr(
                            First, (Last = CUR_STR.find('\t', First)) - First)
                        );

                string name = lst.at(0);
                string match_lang;
                char kind = '\0';

                for (uint it = 2; it < lst.size(); ++it) {
                        string tok = lst.at(it);
                        if (tok.size() == 1)
                                kind = tok.at(0);
                        else if (tok.substr(0, 9) == "language:")
                                match_lang = tok.substr(9);
                }

                if (!kind || match_lang.empty())
                        continue;

                // Prune tags. Include only those that are:
                //    1) of a type in the `order' list,
                //    2) of the correct language (applies mainly to C
                //       and C++, generally ctags filters languages),
                //    3) are not included in the `skip' list, and
                //    4) are present in the current vim buffer
                // If invalid, just move on. 
                if (in_order(data->equiv, data->order, &kind) &&
                    is_correct_lang(data->lang, &match_lang) &&
                    //!skip_tag(data->skip, &name) &&
                    strstr(data->vim_buf->c_str(), name.c_str()) != nullptr)
                     //data->vim_buf->find(name) != string::npos)
                {
                        struct lldata tmp = { string(name), kind };
                        ret->push_back(tmp);
                }
        }

        delete data;
        //pthread_exit(static_cast<void *>(ret));

        return static_cast<void *>(ret); // NOTREACHED
}


