#include "Postman/PostUtility.H"

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <stdarg.h>

#include <iostream>
#include <fstream>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include <execinfo.h>

#define MAX_FRAME 16

using namespace std;

PostError::PostError(const char* format, ...) {
    char buf[1024];
    va_list argptr;
    va_start(argptr, format);
    vsnprintf(buf, 1023, format, argptr);
    va_end(argptr);
    msg_ = string(buf);
};

void Error(const char* format, ...) {
    fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!Error: ");
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
    fprintf(stderr, "\r\n");

    void* addrlist[MAX_FRAME];
    int addrlen = backtrace(addrlist, MAX_FRAME);
    if (addrlen == 0) { perror("Empty backtrace\n"); exit(1); }
    char** symbollist = backtrace_symbols(addrlist, addrlen);
    printf("Calling stack:\n");
    for (int i=0; i<addrlen; i++) {
        printf("<S%02d> %s\n", i, symbollist[i]);
    }
    exit(1);
};

void Warn(const char* format, ...) {
    printf("!!!!!!!!!!Warning: ");
    va_list argptr;
    va_start(argptr, format);
    vprintf(format, argptr);
    va_end(argptr);
    printf("\r\n");
    return;
};

void println(const char* format, ...) {
    va_list argptr;
    va_start(argptr, format);
    vprintf(format, argptr);
    va_end(argptr);
    printf("\r\n");
    return;
};

void debugging(const char* format, ...) {
    printf("DEBUG: ");
    va_list argptr;
    va_start(argptr, format);
    vprintf(format, argptr);
    va_end(argptr);
    printf("\r\n");
    return;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

vector<string> tokenize(const char * line, char delim, bool withEmptyToken/*=true*/)
{
    vector<string> allwords ;
    char buf[strlen(line)+1] ;
    int i = 0, j = 0, wStart = 0 ;
    while (line[i] != '\0') {
        if (line[i] == delim) {
            buf[j] = '\0' ;
            if (withEmptyToken || i > wStart) {
                allwords.push_back(string(buf)) ;
            }
            // debugging("Token: %d %c %s", i, line[i], &buf[wStart]);
            wStart = i+1 ;
            j = 0;
        }
        else {
            buf[j] = line[i];
            j += 1;
        }
        i += 1;
    }
    buf[j] = '\0' ;
    if (withEmptyToken || i > wStart) allwords.push_back(string(buf)) ;
    //     debugging("Token: %d %c %s", i, line[i], &buf[wStart]);
    return vector<string>(allwords) ;
};

std::vector<std::string> tokenize(const std::string & line, char delim, bool withEmptyToken/*=true*/)
{ return tokenize(line.c_str(), delim, withEmptyToken); } ;

vector<string> tokenizeWithStrip(const char * line, char delim, bool withEmptyToken/*=true*/)
{
    vector<string> allwords ;
    char buf[strlen(line)+1] ;
    int i = 0, wStart = 0 ;
    while (line[i] != '\0') {
        buf[i] = line[i] ;
        if (buf[i] == delim) {
            buf[i] = '\0' ;
            if (withEmptyToken || i > wStart) {
                allwords.push_back(stripString(string(&buf[wStart]))) ;
            }
            wStart = i+1 ;
        }
        i++ ;
    }
    buf[i] = '\0' ;
    if (withEmptyToken || i > wStart) allwords.push_back(stripString(string(&buf[wStart]))) ;
    return vector<string>(allwords) ;
};

std::vector<std::string> tokenizeWithStrip(const std::string & line, char delim, bool withEmptyToken/*=true*/)
{ return tokenizeWithStrip(line.c_str(), delim, withEmptyToken); } ;

vector<string> parseSignalName(const string & line, char delim)
{
    vector<string> all_words;
    string sigName;
    string sigConfig;
    size_t found = line.find_first_of(':');
    if(found == std::string::npos){
        throw PostError("Signal Configuration Error: %s", line.c_str());
    }else{
        sigName = line.substr(0, found);
        sigConfig = line.substr(found+1);
    }

    all_words.push_back(sigName);
    all_words.push_back(sigConfig);
    return all_words;
};

//getNumber cannot process Letter 'x','X','e','E', need to arrange letter inputs alphabatically
double getNumber(const string & ss, char flag)
{
    string s = ss;
    s.erase(0, s.find(flag)+1U);

    if ( s.empty() || !isdigit(s[0])) return 0.0;

    std::istringstream i(s);

    double dd;
    i >> dd;
    return dd;
};

vector<int> parseNumbetList(const string & s) {
    vector<string> xs = splitString(s, ',') ;
    vector<int> ns;
    for (const string & x: xs) {
        vector<string> ys = splitString(x, '-');
        if (ys.size() == 0 || ys.size() > 2) { Warn("Ignore word %s in %s", x.c_str(), s.c_str()); continue; }
        if (ys.size() == 1) { ns.push_back(stoi(x)); continue; }
        int n1 = stoi(ys[0]), n2 = stoi(ys[1]);
        if (n1 == n2) ns.push_back(n1) ;
        else if (n1 < n2) { for (int i=n1; i<=n2; i++) ns.push_back(i); }
        else              { for (int i=n1; i>=n2; i--) ns.push_back(i); }
    };
    return vector<int>(ns);
};

string stripString(const string & str, char strip)
{
    if (str.length() <= 0) return string() ;
    int spos = 0, epos = str.length()-1 ;
    while (spos < str.length() && (str[spos] == ' ' || str[spos] == '\t' || str[spos] == '\r' || str[spos] == '\n' || str[spos] == strip) )
        spos += 1 ;
    while (epos >= 0 && (str[epos] == ' ' || str[epos] == '\t' || str[epos] == '\r' || str[epos] == '\n' || str[epos] == strip) )
        epos -= 1 ;
    if (epos >= spos) return string(str.substr(spos, epos-spos+1)) ;
    else return string() ;
} ;

std::string removeEndSpaces(const std::string &s)
{
  int last = s.size() - 1;
  while (last >= 0 && s[last] == ' ')
    --last;
  return string(s.substr(0, last + 1));
};

std::string capitalizeString(const std::string & str)
// ord('a') = 97, ord('z') = 122, ord('A') = 65, ord('Z') = 90
{
    int len = str.length() ;
    if (len <= 0) return string() ;
    char sbuf[len+1] ;
    sbuf[0] = str[0] ;
    if (97 <= str[0] && str[0] <= 122)
        sbuf[0] = str[0] - 32 ;
    for (int i = 1; i < len; i++) {
        if (65 <= str[i] && str[i] <= 90) sbuf[i] = str[i] + 32 ;
        else sbuf[i] = str[i] ;
    }
    sbuf[len] = '\0' ;
    return string(sbuf) ;
};

std::string lowerString(const std::string & str)
{
    int len = str.length() ;
    if (len <= 0) return string() ;
    char sbuf[len+1] ;
    for (int i = 0; i < len; i++) {
        if (65 <= str[i] && str[i] <= 90) sbuf[i] = str[i] + 32 ;
        else sbuf[i] = str[i] ;
    }
    sbuf[len] = '\0' ;
    return string(sbuf) ;
};

std::string upperString(const std::string & str)
{
    int len = str.length() ;
    if (len <= 0) return string() ;
    char sbuf[len+1] ;
    for (int i = 0; i < len; i++) {
        if (97 <= str[i] && str[i] <= 122) sbuf[i] = str[i] - 32 ;
        else sbuf[i] = str[i] ;
    }
    sbuf[len] = '\0' ;
    return string(sbuf) ;
} ;

std::string joinString(const std::list<string> & strList, std::string sep)
{
    if (strList.size() == 0) return string("") ;
    list<string>::const_iterator it = strList.begin() ;
    string res = string(*it) ;
    it++ ;
    for (; it != strList.end(); it++) res += sep + (*it) ;
    return string(res) ;
} ;

std::string joinString(const std::vector<string> & strList, string sep)
{
    if (strList.size() == 0) return string("") ;
    vector<string>::const_iterator it = strList.begin() ;
    string res = string(*it) ;
    it++ ;
    for (; it != strList.end(); it++) res += sep + (*it) ;
    return string(res) ;
};

std::string joinString(const std::list<string> & strList, char sep)
{
    if (sep == '\0') return joinString(strList, string("")) ;
    else return joinString(strList, string(1, sep)) ;
} ;

std::string joinString(const std::vector<string> & strList, char sep)
{
    if (sep == '\0') return joinString(strList, string("")) ;
    else return joinString(strList, string(1, sep)) ;
} ;

bool stringStartsWith(const std::string & str, const std::string & prefix)
{
    if (str.size() < prefix.size()) return false;
    for (int i=0; i<prefix.size(); i++)
        if (str[i] != prefix[i]) return false;
    return true;
};

bool stringEndsWith(const std::string & str, const std::string & suffix)
{
    if (str.size() < suffix.size()) return false;
    int diff = str.size() - suffix.size() ;
    for (int i=suffix.size()-1; i>=0; i--)
        if (str[i+diff] != suffix[i]) return false;
    return true;
};

map<string, string> parseJson(const string & jsonString) {
    map<string, string> res;
    string x = stripString(jsonString);
    if (! (stringStartsWith(x, "{") && stringEndsWith(x, "}")) ) {
        Warn("Invalid JSON format: %s", jsonString.c_str());
        return res;
    }
    vector<string> fields = tokenizeWithStrip(x.substr(1, x.size()-2), ',');
    for (auto fstr: fields) {
        vector<string> ws = tokenizeWithStrip(fstr, ':');
        if (ws.size() != 2) { Warn("Invalid JSON field: %s", fstr.c_str()); continue; }
        if (res.find(ws[0]) != res.end()) { Warn("Overwrite JSON field with: %s", fstr.c_str()); }
        res[ws[0]] = ws[1];
    }
    return res;
}

/*
string charsetConvert(const string & srcCharset, const string & desCharset, const char* srcStr) {
    // size_t len = strlen(srcStr) ;
    // char ibuf[len+1], obuf[len+1]  ;
    // memset(obuf, 0, len+1);
    // memset(obuf, 0, len+1);

    // strcpy(ibuf, srcStr) ;
    // char* pin = ibuf ;
    // char* pout = obuf ;

    // iconv_t cd=iconv_open(desCharset.c_str(), srcCharset.c_str());
    // if(cd==(iconv_t)-1) return string();
    // iconv(cd, &pin, &len, &pout, &len) ;
    // iconv_close(cd) ;

    // return string(obuf) ;
    return string(srcStr) ;
} ;


string charsetConvert(const string & srcCharset, const string & desCharset, const string & srcString)
{
    string res = charsetConvert(srcCharset, desCharset, srcString.c_str()) ;
    return string(res) ;
} ;

Int64 stringToInt64(const char* str) {
    int i = strlen(str) -1  ;
    Int64 base = 1;
    Int64 res = 0 ;
    for (; i >= 0; i--) {
        if ('0' <= str[i] && str[i] <= '9') { res += (str[i] - '0') * base ; base *= 10 ; }
        else if (str[i] == '-') return (0-res) ;
        else return res ;
    }
    return res ;
};

Int64 stringToInt64(const std::string & str)
{ return stringToInt64(str.c_str()) ; } ;

unsigned int stringToUnsignedInt(const char* str) {
    int i = strlen(str) -1  ;
    Int64 base = 1;
    Int64 res = 0 ;
    for (; i >= 0; i--) {
        if ('0' <= str[i] && str[i] <= '9') { res += (str[i] - '0') * base ; base *= 10 ; }
        else return res ;
    }
    return res ;
};

unsigned int stringToUnsignedInt(const std::string & str)
{ return stringToUnsignedInt(str.c_str()) ; } ;
*/

string doubleToString(double d, unsigned precision) {
    if (precision > 100) Error("doubleToString: too large precision %d", precision);
    char tmp[256], format[16];
    if (abs(d - static_cast<Int64>(d)) < PEPS) {
        snprintf(tmp, 256, "%.0f", d);
    }
    else {
        sprintf(format, "%%.%df", precision);
        snprintf(tmp, 256, format, d);
    }
    return string(tmp);
};

std::string intToString(int x, unsigned width) {
    if (width > 30) Error("intToString: too large width %d", width);
    char tmp[32], format[16];
    sprintf(format, "%%0%dd", width);
    snprintf(tmp, 32, format, x);
    return string(tmp);
} ;

std::string uintToString(unsigned x, unsigned width) {
    if (width > 30) Error("uintToString: too large width %d", width);
    char tmp[32], format[16];
    sprintf(format, "%%0%du", width);
    snprintf(tmp, 32, format, x);
    return string(tmp);
};

std::string int64ToString(int x, unsigned width) {
    if (width > 30) Error("int64ToString: too large width %d", width);
    char tmp[32], format[16];
    sprintf(format, "%%0%dlld", width);
    snprintf(tmp, 32, format, x);
    return string(tmp);
};

std::string uint64ToString(unsigned x, unsigned width) {
    if (width > 30) Error("uint64ToString: too large width %d", width);
    char tmp[32], format[16];
    sprintf(format, "%%0%dllu", width);
    snprintf(tmp, 32, format, x);
    return string(tmp);
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

string ptu::get_env_variable(const string & key)
{
    char * val = getenv(key.c_str()) ;
    if (val == NULL) throw PostError("Environment variable is not defined: " + key) ;
    return string(val) ;
} ;

string ptu::get_hostname(bool getFullname) {
    char hostname[512];
    gethostname(hostname, 512);
    hostname[511] = '\0';
    if (! getFullname) {
        for (int i=0; i<512; i++) {
            if (hostname[i] == '.') { hostname[i] = '\0'; break;}
            else if (hostname[i] == '.') break;
        }
    }
    return string(hostname);
};

string ptu::get_username() {
    // char username[512];
    // getlogin_r(username, 512);
    // username[511] = '\0';
    // return string(username);
    return get_env_variable("USER") ;
};

int ptu::get_max_pid()
{
#ifdef __MACH__ // OS X does not have /proc/sys/kernel/pid_max
    return 99999;
#else
    string maxpidFile = "/proc/sys/kernel/pid_max";
    if (! ptu::exists(maxpidFile))
        { Warn("Cannot find pid_max file %s", maxpidFile.c_str()); return 99999; }
    ifstream f ;
    string line ;
    f.open(maxpidFile.c_str()) ;
    getline(f, line) ;
    return atoi(line.c_str()) ;
#endif
} ;

bool ptu::exists(const char * p) {
    struct stat buf;
    return (stat(p, &buf) == 0);
};

bool ptu::is_directory(const char * p) {
    struct stat buf;
    return (stat(p, &buf) == 0 && S_ISDIR(buf.st_mode));
};

bool ptu::is_regular_file(const char * p) {
    struct stat buf;
    return (stat(p, &buf) == 0 && S_ISREG(buf.st_mode));
};

vector<string> ptu::get_files_in_directory(const char * dirstr) {
    vector<string> fns;
    DIR *dir = opendir(dirstr);
    if (dir != NULL) {
        // debugging("dir is not NULL");
        struct dirent *ent = readdir(dir) ;
        while (ent != NULL) {
            fns.push_back(string(ent->d_name));
            ent = readdir(dir);
        }
        closedir (dir);
    } else {
        Warn("Open directory failed: %s", dirstr);
    }
    return vector<string>(fns);
};

string ptu::get_parent_directory(const char * p) {
    if (sizeof(p) == 0)  return string();
    if (sizeof(p) == 1 && p[0] == '/') return string();
    vector<string> dirs = splitString(p, '/', false) ;
    if (dirs.size() == 0) return string() ;
    vector<string>::const_iterator first = dirs.begin();
    vector<string>::const_iterator last = dirs.begin() + dirs.size() - 1;
    vector<string> newDirs(first, last);
    if (p[0] == '/')  return ("/" + joinString(newDirs, '/'));
    else return joinString(newDirs, '/');
};

void ptu::create_directories(const char * pstr)
{
    if (pstr == "") return;
    vector<string> dirs = splitString(pstr, '/');
    string path = (pstr[0] == '/' ? "/" : ".");
    for (string d : dirs) {
        if (d != "") {
            path = path + "/" + d;
            if (! exists(path))
                if (mkdir(path.c_str(), 0755) != 0)
                    PostError("Failed of creating directory " + path) ;
        }
    }
    return;
} ;

void ptu::create_file(const char * p) {
    if (exists(p)) return;
    std::ofstream ofs ;
    ofs.open(p, ofstream::out | ofstream::app);
    ofs.close();
    return;
};

int ptu::set_file_owner(const char * p, uid_t uid, gid_t gid) {
    if (! exists(p)) { Warn("%s: file does not exist %s", __func__, p); return -1; }
    int res = chown(p, uid, gid);
    if (res != 0) Warn("Cannot change owner of %s. Error: %d, %s", p, errno, strerror(errno));
    return res;
};

// upper 32 bits are seconds since epoch;
Uint64 ptu::get_realtime() {
    struct timespec ts ;
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts.tv_sec = mts.tv_sec;
    ts.tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, &ts) ;
#endif
    // printf("timespec: %ld.%09ld\n", ts.tv_sec, ts.tv_nsec);
    return ( (((Uint64) ts.tv_sec) << 32) | ((Uint64) ts.tv_nsec)) ;
};

// Uint64 ptu::get_rdtsc() {
//   unsigned a, d;
//   asm volatile("rdtsc" : "=a" (a), "=d" (d) : : "%rbx", "%rcx");
//   // return ((Uint64) a) | (((uint64_t) d) << 32);
//   return ((Uint64) d) | (((uint64_t) a) << 32);
// }

unsigned ptu::get_int_date() {

};

unsigned ptu::binary_roundup_shift(unsigned x) {
    if (x == 0 || x > (1<<31)) return 0;
    unsigned shift=0, v=x;
    while (v > 0) { v = (v >> 1); shift += 1; }
    if (x == (1 << (shift-1)) ) return (shift-1) ;
    else return shift;
};

unsigned ptu::binary_roundup(unsigned x) {
    if (x == 0 || x > (1<<31)) return 0;
    else return ( 1 << (binary_roundup_shift(x)) );
};

#include "Postman/PostTime.cc"
