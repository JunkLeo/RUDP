// #include "PostTime.H"
// #include "Postman/Postman.H"
#include "Postman/PostUtility.H"

#include <sys/time.h>
#include <ctime>
#include <cassert>
#include <string>

using namespace std ;

////////////////////////////////////////////////////////////////////////////////
//  NanoTime
////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(ostream& os, const NanoTime & ts) { os << ts.toString(); return os; };

NanoTime::NanoTime() { value_.vint64 = 0; } ;

NanoTime::NanoTime(const NanoTime & nts) { value_.vint64 = nts.rawValue(); };

NanoTime::NanoTime(unsigned hh, unsigned mm, unsigned sec, unsigned nsec)
{ value_.vint[1] = 3600*hh + 60*mm + sec; value_.vint[0] = nsec; };

NanoTime::NanoTime(unsigned hh, unsigned mm, unsigned sec)
{ value_.vint[1] = 3600*hh + 60*mm + sec; value_.vint[0] = 0; };

NanoTime::NanoTime(unsigned sec, unsigned nsec)
{ value_.vint[1] = sec; value_.vint[0] = nsec; };

// NanoTime::NanoTime(const std::string & str)
// { value_.vint64 = fromString(str).rawValue(); };

NanoTime NanoTime::FromOldRawValue(unsigned rawval)
{ return NanoTime( rawval/DMS_IN_SEC, (rawval%DMS_IN_SEC)*NS_IN_DMS ); };

unsigned NanoTime::oldRawValue() const
{ return ( secValue()*DMS_IN_SEC + nsValue()/NS_IN_DMS ); };

NanoTime NanoTime::UniversalAbsTime() { return NanoTime(ptu::get_realtime()); };
NanoTime NanoTime::LocalAbsTime() { return NanoTime(ptu::get_realtime()+ TimeZoneOffset_ ); };

NanoTime NanoTime::UniversalIndayTime() {
    Uint64 ts = ptu::get_realtime();
    SecNsec * pts = (SecNsec*) (&ts);
    return NanoTime(pts->vint[1]%SEC_PER_DAY, pts->vint[0]);
};

NanoTime NanoTime::LocalIndayTime(){
    Uint64 ts = ptu::get_realtime()+TimeZoneOffset_;
    SecNsec * pts = (SecNsec*) (&ts);
    return NanoTime(pts->vint[1]%SEC_PER_DAY, pts->vint[0]);
};

// NanoTime NanoTime::LocalTimeFromRawValue(Uint64 tsRaw, Int64 offsetTimeZone)
// { return NanoTime(tsRaw+offsetTimeZone); }

NanoTime NanoTime::LocalTimeFromUniversal(const NanoTime & utm)
{ return NanoTime(utm.rawValue()+TimeZoneOffset_); }

NanoTime NanoTime::UniversalDayStart() {
    unsigned sval = ( ptu::get_realtime()  >> 32 );
    return NanoTime((sval - sval%SEC_PER_DAY), 0);
};
NanoTime NanoTime::LocalDayStart() {
    Uint64 ts = ptu::get_realtime() + TimeZoneOffset_;
    unsigned sval = ( ts >> 32) ;
    // debugging("%u %u %u %u", (ts>>32), sval, sval % SEC_PER_DAY, sval-sval % SEC_PER_DAY);
    return NanoTime((sval - sval%SEC_PER_DAY), 0);
};

NanoTime NanoTime::toIndayTime() const
{ return NanoTime(value_.vint[1] % SEC_PER_DAY, value_.vint[0]); };

string NanoTime::rawString() const {
    char buf[32];
    sprintf(buf, "%u.%09u", value_.vint[1], value_.vint[0]);
    return string(buf);
};
std::string NanoTime::toString() const {
    char buf[32];
    sprintf(buf, "%02d:%02d:%02d.%06u", hmsHour(), hmsMinute(), hmsSec(), microSec());
    return string(buf);
};
std::string NanoTime::hmsString() const {
    char buf[32];
    sprintf(buf, "%02d:%02d:%02d", hmsHour(), hmsMinute(), hmsSec());
    return string(buf);
};
std::string NanoTime::shortString() const {
    char buf[32];
    sprintf(buf, "%02d:%02d:%02d.%03u", hmsHour(), hmsMinute(), hmsSec(), milliSec());
    return string(buf);
};
std::string NanoTime::fullString() const {
    char buf[32];
    sprintf(buf, "%02d:%02d:%02d.%09u", hmsHour(), hmsMinute(), hmsSec(), nanoSec());
    return string(buf);
};
// std::string NanoTime::fullStringWithDay() const {
//     char buf[32];
//     sprintf(buf, "%02d:%02d:%02d.%09u", hmsHour(), hmsMinute(), hmsSec(), nanoSec());
//     return string(buf);
// };

bool NanoTime::operator==(const NanoTime & ts) const { return (value_.vint64 == ts.value_.vint64) ;} ;

bool NanoTime::operator<=(const NanoTime & ts) const
{ return (secValue() < ts.secValue()) || (secValue() == ts.secValue() && nsValue() <= ts.nsValue()) ;} ;

bool NanoTime::operator>=(const NanoTime & ts) const
{ return (secValue() > ts.secValue()) || (secValue() == ts.secValue() && nsValue() >= ts.nsValue()) ;} ;

bool NanoTime::operator<(const NanoTime & ts) const
{ return (secValue() < ts.secValue()) || (secValue() == ts.secValue() && nsValue() < ts.nsValue()) ;} ;

bool NanoTime::operator>(const NanoTime & ts) const
{ return (secValue() > ts.secValue()) || (secValue() == ts.secValue() && nsValue() > ts.nsValue()) ;} ;

NanoTime NanoTime::operator+(const NanoTime & ts) const {
    unsigned ns = nsValue() + ts.nsValue() ;
    unsigned ss = secValue() + ts.secValue() ;
    return ( ns < NS_IN_SEC ? NanoTime(ss, ns) : NanoTime(ss+1, ns-NS_IN_SEC) ) ;
} ;

// #if defined __NEW_TIME_SUB__
// #else
NanoTime NanoTime::operator-(const NanoTime & ts) const {
    if ( secValue() < ts.secValue() || (secValue() == ts.secValue() && nsValue() < ts.nsValue()) ) {
        Warn("Timestamp sub results in neg time: %s(%s) - %s(%s)", fullString().c_str(), rawString().c_str(), ts.fullString().c_str(), ts.rawString().c_str());
        return NanoTime();
    }
    if (nsValue() < ts.nsValue())
        return NanoTime( secValue()-ts.secValue()-1, NS_IN_SEC+nsValue()-ts.nsValue() );
    else
        return NanoTime( secValue()-ts.secValue(), nsValue()-ts.nsValue() );
} ;
// #endif

// [ToBeOptim]
//NanoTime NanoTime::operator*(unsigned x) const {
//    Error("Timestamp multiplication is deprecated!");
//    Uint64 allns = ( ((Uint64) secValue())*NS_IN_SEC + nsValue() ) * x;
//    return NanoTime(allns/NS_IN_SEC, allns%NS_IN_SEC) ;
//};
//
//NanoTime NanoTime::operator/(unsigned x) const {
//    Error("Timestamp division is deprecated!");
//    Uint64 allns = ( ((Uint64) secValue())*NS_IN_SEC + nsValue() ) / x;
//    return NanoTime(allns/NS_IN_SEC, allns%NS_IN_SEC) ;
//};

NanoTime NanoTime::FromString(const char * cstr) {
    if (cstr[0] == '+') return NanoTime(fromCstring(&(cstr[1]), 86400)); // 3600*24
    else return NanoTime(fromCstring(cstr, 0));
};

Uint64 NanoTime::fromCstring(const char * str, unsigned offsetSeconds) {
    int nspos = 0;
    for(; nspos<strlen(str); nspos++) {
        if (str[nspos] == '.') { break ;}
        if (str[nspos] != ':' && (str[nspos] < '0' || str[nspos] > '9') )
            throw PostError("Invalid timestamp string: %s", str) ;
    }
    nspos += 1;
    // printf("NS pos: %d\n", nspos);
    int hhStart=0, segEnd=1, hh=-1, mm=-1, ss=-1;
    int hhpos=0, mmpos=0, sspos=0;
    if (str[hhpos+1] == ':') { hh = str[hhpos] - '0'; mmpos = hhpos+2; }
    else if (str[hhpos+2] == ':') { hh = 10*(str[hhpos]-'0') + (str[hhpos+1]-'0'); mmpos = hhpos+3; }
    else throw PostError("Invalid timestamp string: %s", str) ;

    if (str[mmpos+1] == ':') { mm = str[mmpos] - '0'; sspos = mmpos+2; }
    else if (str[mmpos+2] == ':') { mm = 10*(str[mmpos]-'0') + (str[mmpos+1]-'0'); sspos = mmpos+3; }
    else throw PostError("Invalid timestamp string: %s", str) ;

    if (sspos+1 == nspos-1) { ss = str[sspos] - '0'; }
    else if (sspos+2 == nspos-1) { ss = 10*(str[sspos]-'0') + (str[sspos+1]-'0'); }
    else throw PostError("Invalid timestamp string: %s", str) ;

    if (hh < 0 || mm < 0 || ss < 0) throw PostError("Invalid timestamp string: %s", str) ;
    // printf("NanoTime::fromCtring: %02d:%02d:%02d\n", hh, mm, ss);

    unsigned nsval = 0;
    unsigned base = NS_IN_SEC;
    for(int i=nspos; i<strlen(str); i++) {
        base /= 10;
        if (str[i] < '0' || str[i] > '9') throw PostError("Invalid timestamp string: %s", str) ;
        nsval = nsval+(str[i] - '0') * base;
        if (base == 0) break;
    }
    return ( (((Uint64) (3600*hh+60*mm+ss+offsetSeconds)) << 32) | nsval);
};

////////////////////////////////////////////////////////////////////////////////
//  Datestamp
////////////////////////////////////////////////////////////////////////////////


Datestamp::Datestamp(const time_t * ptt) {
    memset(&xtm_, 0, sizeof(struct tm));
    gmtime_r(ptt, &xtm_);
    xtt_ = *ptt;
};

void Datestamp::constructFromYMD(unsigned year, unsigned month, unsigned day) {
    if (year < 1970)
        throw PostError("Datestamp does not support date before EPOCH : %04d-%02d-%02d", year, month, day);
    if (month < 1 || month > 12)
        throw PostError("Datestamp with invalid month: %04d-%02d-%02d", year, month, day);
    memset(&xtm_, 0, sizeof(struct tm));
    xtm_.tm_year = year-1900; xtm_.tm_mon = month-1; xtm_.tm_mday = day;
    xtt_ = mktime(&xtm_) + Timestamp::TimeZoneSeconds_;
    gmtime_r(&xtt_, &xtm_);
    // println("%d-%d-%d %d %d:%d:%d %d", xtm_.tm_year, xtm_.tm_mon, xtm_.tm_mday, xtm_.tm_wday
    //         , xtm_.tm_hour, xtm_.tm_min, xtm_.tm_sec, xtm_.tm_zone);
    return ;
};

Datestamp::Datestamp() {
    memset(&xtm_, 0, sizeof(struct tm));
    xtt_ = Timestamp::LocalDayStart().secValue();
    gmtime_r(&xtt_, &xtm_);
    // localtime_r(&xtt_, &xtm_);
};

Datestamp Datestamp::FromNanoTime(const NanoTime & ts)
{ time_t tt = (ts.secValue()/SEC_PER_DAY)*SEC_PER_DAY;  return Datestamp(&tt); };

Datestamp Datestamp::FromOrdinal(unsigned ordin) {
    if (ordin < EPOCH_ORDINAL) throw PostError("Ordinal %d before EPOCH %d", ordin, EPOCH_ORDINAL);
    time_t tt; memset(&tt, 0, sizeof(time_t));
    tt = (ordin-EPOCH_ORDINAL) * SEC_PER_DAY;
    return Datestamp(&tt);
};


////////////////////////////////////////////////////////////////////////////////
//  Dstamp
////////////////////////////////////////////////////////////////////////////////

unsigned mdays[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const unsigned DAYS_IN_400Y = 146097;
const unsigned DAYS_IN_100Y = 36524;
const unsigned DAYS_IN_4Y = 1461;

unsigned Dstamp::hexdate_to_ordinal(unsigned hexdate)
{ return ymd_to_ordinal( (hexdate>>16), ((hexdate & 0x0000FF00) >> 8), (hexdate & 0x000000FF) ); };

// upper 16 bits represent year, mid 8 bits represent month, lower 8 bits represent day
// YYYYYYYYYYYYYYYYMMMMMMMMDDDDDDDD
// 0       8       16      24
unsigned Dstamp::ordinal_to_hexdate(unsigned ordinval) {
    unsigned yy=0, mm=1, dd=1, ordin=0;
    // cout << "Ordinal: " << ordinval << endl;
    while (ordin+DAYS_IN_400Y <= ordinval) {
        ordin += DAYS_IN_400Y; yy += 400;
        // cout << format("Add 400Y: %d-%d-%d") % yy % mm % dd << endl;
    }
    while (ordin+DAYS_IN_100Y <= ordinval) {
        ordin += DAYS_IN_100Y; yy += 100;
        if (yy % 400 == 0 && ordin == ordinval) { // 400-12-30: will not advance
            return ((yy << 16) + (12 << 8) + 30);
        }
        // cout << format("Add 100Y: %d-%d-%d") % yy % mm % dd  << endl;
    }
    while (ordin+DAYS_IN_4Y <= ordinval) {
        ordin += DAYS_IN_4Y; yy += 4;
        if (yy % 100 == 0) {
            ordin -= DAYS_IN_4Y; yy -= 4; break;
        }
        // cout << format("Add 4Y: %d-%d-%d") % yy % mm % dd << endl;
    }
    while (ordin + (isLeapYear(yy+1)?366:365) <= ordinval) {
        ordin += (isLeapYear(yy+1)?366:365); yy += 1;
        // cout << format("Add 1Y: %d-%d-%d") % yy %mm % dd << endl;
    }
    if (ordin == ordinval) return ((yy << 16) + (12 << 8) + 31);
    yy += 1;
    // cout << "Year is " << yy << endl;
    // while (ordin + (isLeapYear(yy) ? 366 : 365) < ordinval)
    //   { yy += 1; ordin += (isLeapYear(yy-1) ? 366 : 365); }
    // cout << format("%04d: %d / %d") % yy % ordin % ordinval << endl;
    while (ordin < ordinval) {
        if (mm == 2 && isLeapYear(yy) && (ordin+29 < ordinval)) {
            ordin += 29; mm += 1;
            // cout << format("Add 1M: %d-%d-%d") % yy % mm % dd  << endl;
        }
        else if ((mm != 2 || ! isLeapYear(yy)) && ordin+mdays[mm] < ordinval) {
            ordin += mdays[mm]; mm += 1;
            // cout << format("Add 1M: %d-%d-%d") % yy % mm % dd  << endl;
        }
        else {
            dd = ordinval - ordin;
            break ;
        }
    }
    // cout << format("hexdate: %04d, %02d, %02d") % yy % mm % dd << endl;
    return ((yy << 16) + (mm << 8) + dd);
};

unsigned Dstamp::ymd_to_ordinal(unsigned yy, unsigned mm, unsigned dd) {
    // cout << format("ymd_to_ordinal: %04d, %02d, %02d") % yy % mm % dd << endl;
    unsigned ordinal = 0;
    if (yy < 1) throw PostError("Year (%d, %d, %d) out of range", yy, mm, dd);
    if (mm == 0 || mm > 12) throw PostError("Month (%d, %d, %d) out of range", yy, mm, dd);
    if ((!isLeapYear(yy) && dd > mdays[mm]) || (mm == 2 && isLeapYear(yy) && dd > 29))
        throw PostError("Day (%d, %d, %d) out of range", yy, mm, dd);
    int  y = 1;
    for(; y+400 <= yy; y+=400)  ordinal += DAYS_IN_400Y;
    for(; y+100 <= yy; y+=100)  ordinal += DAYS_IN_100Y;
    for(; y+4 <= yy; y+=4)  ordinal += DAYS_IN_4Y;
    for(; y < yy; y+=1)  ordinal += 365;
    for(int m=1; m<mm; m++) ordinal += mdays[m];
    ordinal += dd;
    if (isLeapYear(yy) && mm > 2) ordinal += 1;
    return ordinal;
};

unsigned Dstamp::ymd_to_hexdate(unsigned y, unsigned m, unsigned d)
{ return ( (y << 16) + (m << 8) + d); };

unsigned Dstamp::intdate_to_hexdate(int intdate)
{ return ymd_to_hexdate(intdate/10000, intdate%10000/100, intdate%100); };

unsigned Dstamp::intdate_to_ordinal(int intdate)
{ return ymd_to_ordinal(intdate/10000, intdate%10000/100, intdate%100); };

unsigned Dstamp::string_to_hexdate(const string & dstr)
{ return intdate_to_hexdate(stoi(dstr)); };

unsigned Dstamp::string_to_ordinal(const string & dstr)
{ return intdate_to_ordinal(stoi(dstr)); };

Dstamp Dstamp::universalDay() {
    unsigned ordin = (ptu::get_realtime() >> 32) / SEC_PER_DAY + EPOCH_ORDINAL;
    return Dstamp(ordin, ordin);
};

// Main constructor for local day
Dstamp::Dstamp() : ordinal_( NanoTime::LocalAbsTime().days()+EPOCH_ORDINAL )
{ hexdate_ = ordinal_to_hexdate(ordinal_); };

Dstamp::Dstamp(unsigned ordinal, unsigned x) : ordinal_(ordinal)
{ hexdate_ = ordinal_to_hexdate(ordinal); } ;

Dstamp::Dstamp(unsigned year, unsigned month, unsigned day)
    : ordinal_( ymd_to_ordinal(year, month, day) ), hexdate_( ymd_to_hexdate(year, month, day) )
{ if (hexdate_ != ordinal_to_hexdate(ordinal_))
        throw PostError("Inconsistent Dstamp: %d", year*10000+month*100+day); };

Dstamp::Dstamp(unsigned intdate)
    : hexdate_( intdate_to_hexdate(intdate) ), ordinal_( intdate_to_ordinal(intdate) )
{ if (hexdate_ != ordinal_to_hexdate(ordinal_))
        throw PostError("Inconsistent Dstamp: %d", intdate); };

Dstamp::Dstamp(const std::string & dstr)
    : hexdate_( string_to_hexdate(dstr) ), ordinal_( string_to_ordinal(dstr) )
{ if (hexdate_ != ordinal_to_hexdate(ordinal_))
        throw PostError("Inconsistent Dstamp: %s", dstr.c_str()); };

Dstamp Dstamp::fromNanoTime(const NanoTime & ts)
{ return Dstamp(ts.days()+EPOCH_ORDINAL, ts.days()+EPOCH_ORDINAL); };

Dstamp Dstamp::fromOrdinal(unsigned ordin) { return Dstamp(ordin, ordin); };

Dstamp Dstamp::fromEpoch(unsigned epochdays)
{ unsigned ordin = EPOCH_ORDINAL+epochdays; return Dstamp(ordin, ordin); };

Dstamp Dstamp::nthWeekdayOfMonth(short year, short month, short nth, short weekday)
{
    assert (nth > 0 && nth <= 5 && weekday >= 0 && weekday <= 6) ;
    unsigned weekdayOfMonthStart = ymd_to_ordinal(year, month, 1) % 7;
    if (weekdayOfMonthStart > weekday)
        return Dstamp(year, month, 1 + 7*nth - (weekdayOfMonthStart-weekday)) ;
    else
        return Dstamp(year, month, 1 + (weekday-weekdayOfMonthStart) + 7*(nth-1)) ;
} ;

