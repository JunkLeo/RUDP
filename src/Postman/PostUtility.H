#ifndef _PostUtility_H_
#define _PostUtility_H_

#include "PostDefinitions.H"

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <exception>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <list>

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

class NanoTime;
typedef NanoTime Timestamp;

class PostError: public std::exception {
protected:
    std::string msg_;
public:
    PostError(const std::string & m): msg_(m) {} ;
    // PostError(const char * m): msg_(m) {} ;
    PostError(const char * fmt, ...) ;
    virtual ~PostError() throw (){}
    virtual const char* what() const throw() { std::string x="!!!!!!!!!!"+msg_; return x.c_str(); } ;
} ;

void println(const char * fmt, ...);
void debugging(const char * fmt, ...);

void Error(const char * fmt, ...);
void Warn(const char * fmt, ...);
inline void Error(const std::string & msg) { Error(msg.c_str()); };
inline void Warn(const std::string & msg) { Warn(msg.c_str()); };

// void error(const std::string & msg);
// void warn(const std::string & msg);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline bool doublePriceEqualCompare(const double & d1, const double & d2) { return (fabs(d1 - d2) <= PEPS); };

inline int signOfDouble(double d) { return d < - PEPS ? -1 : d > PEPS; };

// To be moved into Signal class (as static member)
std::vector<std::string> parseSignalName(const std::string & line, char delim) ;

double getNumber(const std::string & ss, char flag) ;

std::vector<int> parseNumbetList(const std::string & s);

std::vector<std::string> tokenize(const char * line, char delim, bool withEmptyToken=true) ;
std::vector<std::string> tokenize(const std::string & line, char delim, bool withEmptyToken=true) ;
std::vector<std::string> tokenizeWithStrip(const char * line, char delim, bool withEmptyToken=true) ;
std::vector<std::string> tokenizeWithStrip(const std::string & line, char delim, bool withEmptyToken=true) ;

inline std::vector<std::string> splitString(const char * line, char delim, bool withEmptyToken=true)
{ return tokenize(line, delim, withEmptyToken); } ;
inline std::vector<std::string> splitString(const std::string & line, char delim, bool withEmptyToken=true)
{ return tokenize(line, delim, withEmptyToken); } ;
// remove all leading and ending space, tab, newline, return and the character by strip
std::string stripString(const std::string & str, char strip=' ') ;
std::string removeEndSpaces(const std::string &s) ;
std::string capitalizeString(const std::string & str) ;
std::string lowerString(const std::string & str) ;
std::string upperString(const std::string & str) ;
std::string joinString(const std::list<std::string> & strList, std::string sep="") ;
std::string joinString(const std::vector<std::string> & strList, std::string sep="") ;
std::string joinString(const std::list<std::string> & strList, char sep='\0') ;
std::string joinString(const std::vector<std::string> & strList, char sep='\0') ;

bool stringStartsWith(const std::string & str, const std::string & prefix);
bool stringEndsWith(const std::string & str, const std::string & suffix);

std::string doubleToString(double d, unsigned maxPrecision);
std::string intToString(int x, unsigned zeroPaddingWidth);
std::string uintToString(unsigned x, unsigned zeroPaddingWidth);
std::string int64ToString(int x, unsigned zeroPaddingWidth);
std::string uint64ToString(unsigned x, unsigned zeroPaddingWidth);

std::map<std::string, std::string> parseJson(const std::string & jsonString);

namespace ptu {
    // throw PostError when key is not defined
    std::string get_env_variable(const std::string & key) ;
    std::string get_hostname(bool fullname=true);
    std::string get_username();
    int get_max_pid() ;

    bool exists(const char * p);
    inline bool exists(const std::string & p) { return exists(p.c_str()); };
    bool is_directory(const char * p);
    inline bool is_directory(const std::string & p) { return is_directory(p.c_str()); };
    bool is_regular_file(const char * p);
    inline bool is_regular_file(const std::string & p) { return is_regular_file(p.c_str()); };
    std::vector<std::string> get_files_in_directory(const char * p);
    inline std::vector<std::string> get_files_in_directory(const std::string & p)
    { return get_files_in_directory(p.c_str()); };
    std::string get_parent_directory(const char * p);
    inline std::string get_parent_directory(const std::string & p) { return get_parent_directory(p.c_str()); };
    void create_directories(const char * p) ;
    inline void create_directories(const std::string & p) { create_directories(p.c_str()); };
    void create_file(const char * p);
    inline void create_file(const std::string & p) { create_file(p.c_str()); };
    int set_file_owner(const char * p, uid_t uid, gid_t gid);
    inline int set_file_owner(const std::string & p, uid_t uid, gid_t gid) { return set_file_owner(p.c_str(), uid, gid); };

    // upper 32 bits are seconds since epoch; lower 32 bits are nanoseconds
    Uint64 get_realtime() ;
    Uint64 get_rdtsc() ;

    unsigned get_int_date() ;

    unsigned binary_roundup_shift(unsigned x);
    unsigned binary_roundup(unsigned x);
};

////////////////////////////////////////////////////////////////////////////////
//  NanoTime
////////////////////////////////////////////////////////////////////////////////

class NanoTime {
protected:
    // vint[1]: seconds; vint[0]: nanoseconds
    union SecNsec { unsigned vint[2]; Uint64 vint64; } value_;
    static Uint64 fromCstring(const char * cstr, unsigned offsetSeconds);

    NanoTime(Uint64 rawval) { value_.vint64 = rawval; };

public:
    static const unsigned TimeZoneSeconds_ = 28800 ; // 3600 * 8hours
    static const Int64 TimeZoneOffset_ = ( ((Uint64) TimeZoneSeconds_) << 32) ; // 3600 * 8hours

    NanoTime();
    NanoTime(const NanoTime & nts);
    NanoTime(unsigned hh, unsigned mm, unsigned sec, unsigned nsec);
    NanoTime(unsigned hh, unsigned mm, unsigned sec);
    NanoTime(unsigned sec, unsigned nsec);
    // NanoTime(const std::string & str);

    static NanoTime UniversalIndayTime();
    static NanoTime LocalIndayTime();
    static NanoTime UniversalAbsTime();
    static NanoTime LocalAbsTime();
    static NanoTime UniversalDayStart(); // return timestamp 00:00:00 of the current universal day
    static NanoTime LocalDayStart(); // return timestamp 00:00:00 of the current day
    static NanoTime LocalTimeFromUniversal(const NanoTime & utm);

    static unsigned GetRawTsSeconds(Uint64 rawts) { return (rawts >> 32); };

    static NanoTime FromRawValue(Uint64 rawval) { return NanoTime(rawval); };
    // static NanoTime LocalTimeFromRawValue(Uint64 tsRaw, Int64 offsetTimeZone);
    static NanoTime FromOldRawValue(unsigned rawval);

    static inline NanoTime FromStr(const std::string & tstr) { return FromString(tstr.c_str()); };
    static inline NanoTime FromString(const std::string & tstr) { return FromString(tstr.c_str()); };
    static NanoTime FromString(const char * cstr) ;

    NanoTime toIndayTime() const;

    std::string rawString() const;   // 12345678.123456789
    std::string toString() const;    // 12:34:56.123456
    std::string hmsString() const;   // 12:34:56
    std::string shortString() const; // 12:34:56.123
    std::string fullString() const;  // 12:34:56.123456789

    inline Uint64 rawValue() const   { return value_.vint64; };
    inline unsigned secValue() const { return value_.vint[1]; };
    inline unsigned nsValue() const  { return value_.vint[0]; };
    unsigned oldRawValue() const;

    inline int days() const { return (secValue()/SEC_PER_DAY); }

    inline int allHours() const   { return (secValue() / 3600); };
    inline int allMinutes() const { return (secValue() / 60); };
    inline int allSeconds() const { return  secValue(); };
    inline Int64 allMilliSeconds() const     { return ( ((Int64) secValue())*MS_IN_SEC + nsValue()/NS_IN_MS ); };
    inline Int64 allMicroSeconds() const     { return ( ((Int64) secValue())*US_IN_SEC + nsValue()/NS_IN_US ); };
    inline Int64 allDeciMilliSeconds() const { return ( ((Int64) secValue())*DMS_IN_SEC + nsValue()/NS_IN_DMS ); };
    // inline Int64 allNanoSeconds() const { return ( (((Int64)secValue())%SEC_PER_DAY)*NS_IN_SEC + nsValue() ); };

    inline int hmsHour() const   { return ((value_.vint[1] / 3600) % 24); };
    inline int hmsMinute() const { return ( ((value_.vint[1] % SEC_PER_DAY) % 3600) / 60 ); };
    inline int hmsSec() const    { return ( value_.vint[1] % 60 ); };

    inline int nanoSec() const  { return nsValue(); };
    inline int microSec() const { return nsValue()/NS_IN_US; };
    inline int milliSec() const { return nsValue()/NS_IN_MS; };

    bool operator==(const NanoTime & ts) const ;
    bool operator<=(const NanoTime & ts) const ;
    bool operator>=(const NanoTime & ts) const ;
    bool operator<(const NanoTime & ts) const ;
    bool operator>(const NanoTime & ts) const ;

    NanoTime operator+(const NanoTime & ts) const ;
    // No negative time
    NanoTime operator-(const NanoTime & ts) const ;
//    NanoTime operator*(unsigned x) const ;
//    NanoTime operator/(unsigned x) const ;
} ;

class TsHour: public NanoTime {
public:
    TsHour() : NanoTime() {} ;
    TsHour(unsigned hh) : NanoTime(hh*3600, 0) {} ;
};

class TsMinute: public NanoTime {
public:
    TsMinute() : NanoTime() {} ;
    TsMinute(unsigned mm) : NanoTime(mm*60, 0) {} ;
};

class TsSecond: public NanoTime {
public:
    TsSecond() : NanoTime() {} ;
    TsSecond(unsigned ss) : NanoTime(ss, 0) {} ;
};

class TsMilliSec: public NanoTime {
public:
    TsMilliSec() : NanoTime() {} ;
    TsMilliSec(unsigned ms) : NanoTime(ms/MS_IN_SEC, (ms%MS_IN_SEC)*NS_IN_MS) {} ;
};

class TsMicroSec: public NanoTime {
public:
    TsMicroSec() : NanoTime() {}
    TsMicroSec(Uint64 us): NanoTime(us/US_IN_SEC, (us%US_IN_SEC)*NS_IN_US) {};
};

class TsNanoSec: public NanoTime {
public:
    TsNanoSec() : NanoTime() {}
    TsNanoSec(Uint64 ns): NanoTime(ns/NS_IN_SEC, ns%NS_IN_SEC) {};
};

////////////////////////////////////////////////////////////////////////////////
//  Datestamp
////////////////////////////////////////////////////////////////////////////////

const unsigned EPOCH_YEAR = 1970;
const unsigned EPOCH_ORDINAL = 719163;

class Datestamp {
protected:
    struct tm xtm_;
    time_t xtt_;

    Datestamp(const time_t * ptt);
    void constructFromYMD(unsigned year, unsigned month, unsigned day) ;

public :
    Datestamp();
    Datestamp(const Datestamp & d) : xtt_(d.xtt_) { memcpy(&xtm_, &(d.xtm_), sizeof(struct tm)); };
    Datestamp(unsigned year, unsigned month, unsigned day) { constructFromYMD(year, month, day); };
    Datestamp(unsigned intDate) { constructFromYMD(intDate/10000, (intDate%10000)/100, intDate%100); };
    Datestamp(const std::string & strdate)
    { unsigned iDate = stoi(strdate); constructFromYMD(iDate/10000, (iDate%10000)/100, iDate%100); };

    static inline Datestamp Today() { return Datestamp() ; } ;
    static inline Datestamp LocalDate() { return Datestamp() ; } ;
    static Datestamp UniversalDate() ;

    static Datestamp FromNanoTime(const NanoTime & ts);
    static Datestamp FromOrdinal(unsigned ordin);
    // static inline Datestamp FromString(const std::string & dstr) { return Datestamp(dstr); };
    // static inline Datestamp FromString(const char * cstr) { return Datestamp(std::string(cstr)); };

    inline unsigned ordinal() const { return (EPOCH_ORDINAL + seconds() / SEC_PER_DAY); };

    inline unsigned weekday() const { return xtm_.tm_wday; };
    inline unsigned year() const { return 1900+xtm_.tm_year; };
    inline unsigned month() const { return 1+xtm_.tm_mon; };
    inline unsigned day() const { return xtm_.tm_mday; };
    inline unsigned seconds() const { return xtt_; };

    inline std::string isoFormat() const
    { char s[16]; sprintf(s, "%04d-%02d-%02d", year(), month(), day()); return std::string(s); };

    inline std::string toString() const // Example: 20131127
    { char s[16]; sprintf(s, "%04d%02d%02d", year(), month(), day()); return std::string(s); };

    inline std::string ascTimeString() const { return std::string(asctime(&xtm_)); };

    inline int toIntDate() const { return 10000 * year() + 100 * month() + day() ; } ;
};

////////////////////////////////////////////////////////////////////////////////
// Old datestamp
////////////////////////////////////////////////////////////////////////////////

class Dstamp {
protected:
    unsigned ordinal_, hexdate_ ;
    // construct from ordinal (repeat argument to avoid overloading conflict)
    Dstamp(unsigned ordinal, unsigned x);

    static unsigned intdate_to_hexdate(int intdate);
    static unsigned intdate_to_ordinal(int intdate);
    static unsigned string_to_hexdate(const std::string & dstr);
    static unsigned string_to_ordinal(const std::string & dstr);

public:
    static unsigned hexdate_to_ordinal(unsigned hex) ;
    static unsigned ordinal_to_hexdate(unsigned ordin) ;
    static unsigned ymd_to_ordinal(unsigned yy, unsigned mm, unsigned dd) ;
    static unsigned ymd_to_hexdate(unsigned yy, unsigned mm, unsigned dd) ;

    Dstamp() ; // Dstamp for today
    Dstamp(const Dstamp & d) : ordinal_(d.ordinal_), hexdate_(d.hexdate_) {}; // copy constructor
    Dstamp(unsigned year, unsigned month, unsigned day) ;
    Dstamp(unsigned intDate) ; // construct from int date of the form yyyymmdd
    Dstamp(const std::string & strdate) ; // "20160101"

    static inline Dstamp today() { return Dstamp() ; } ;
    static inline Dstamp localDay() { return Dstamp() ; } ;
    static  Dstamp universalDay() ;

    static  Dstamp fromNanoTime(const NanoTime & ts);
    static  Dstamp fromOrdinal(unsigned ordin);
    static  Dstamp fromEpoch(unsigned epochdays);

    static inline Dstamp fromString(const std::string & dstr) { return Dstamp(dstr); };
    static inline Dstamp fromString(const char * cstr) { return Dstamp(std::string(cstr)); };

    static Dstamp nthWeekdayOfMonth(short year, short month, short nth, short weekday) ;
    static bool isLeapYear(int yy) { return ((yy % 4 == 0 && yy % 100 != 0) || (yy % 400 == 0)); }

    inline unsigned year() const { return (hexdate_ >> 16); };
    inline unsigned month() const { return ((hexdate_ & 0x0000FF00) >> 8); };
    inline unsigned day() const { return (hexdate_ & 0x000000FF); };

    inline unsigned ordinal() const { return ordinal_; };
    inline unsigned epoch() const
    { if (ordinal_<EPOCH_ORDINAL) return 0; else return ordinal_-EPOCH_ORDINAL; };
    inline unsigned weekday() const { return (ordinal_ % 7); } ;

    inline std::string isoFormat() const // Example: 2013-11-27
    { char s[16]; sprintf(s, "%04d-%02d-%02d", year(), month(), day()); return std::string(s); };

    inline std::string toString() const // Example: 20131127
    { char s[16]; sprintf(s, "%04d%02d%02d", year(), month(), day()); return std::string(s); };

    inline int toIntDate() const // Example: 20131127
    { return 10000 * year() + 100 * month() + day() ; } ;

    inline bool operator<(const Dstamp & ds) const { return (ordinal_ < ds.ordinal_); };
    inline bool operator<=(const Dstamp &  ds) const { return (ordinal_ <= ds.ordinal_); };
    inline bool operator>(const Dstamp & ds) const { return (ordinal_ > ds.ordinal_); };
    inline bool operator>=(const Dstamp & ds) const { return (ordinal_ >= ds.ordinal_); };
    inline bool operator==(const Dstamp & ds) const { return (ordinal_ == ds.ordinal_); };

    inline Dstamp operator+(unsigned dd) const { return Dstamp(ordinal_+dd, ordinal_+dd); };
    inline Dstamp operator-(unsigned dd) const { return Dstamp(ordinal_-dd, ordinal_-dd); };
} ;
#endif
