#ifndef _SecDef_H_
#define _SecDef_H_

#include "Postman/PostUtility.H"
#include "Security/Definitions.H"

#include <string>
#include <vector>

typedef std::pair<Timestamp, Timestamp> SessionType;

class SecDef {
protected:
    SecDef(const std::string & gid, const std::string & sym, const std::string & nm, int idx
           , int month,const std::string & sunit, const std::string & direction , const Datestamp & date
           , ExchangeId ex, SecType st, DataSource src, int level, const std::string & canonicalId
           , double tick, double invTick, double commRate, double comm, double fxrate
           , double lev, double stoprate, double unit, int offset,double strikepr,int maturity
           , const std::vector<SessionType> & times);

    const Datestamp date_ ;
    const DataSource dataSource_ ; //  Can be removed

    const std::string direction_; // S,N
    const std::string sunit_; // M,A
    const int month_; //0,1,2,3,4
    // const double strikepr_;
    // const int maturity_;

public:
    /*
      GID examples:
      IF_0_1.CNCFF
      IF_1_2.CNCFF
      000300.CNSHS
      510300_2.CNSHS
      000002_3.CNSZS
    */
    const std::string genericId_ ;
    const std::string symbol_ ; // E.g., "IF1307", "000300",
    const std::string name_ ; // E.g., "IF", "000300",
    const std::string canonicalId_; // IF_0.CNCFF, AU_1.CNSHF, 000300.CNSHS, 510300.CNSHS,
    const ExchangeId exchangeId_ ;
    const SecType secType_ ;
    // default 0: only price and volume, transactions are also sent
    // 1 : with top Bid/Ask and other info (turnover, position, etc)
    // >=2 : IF has 5-level Bid/Ask; SHS has 10-level Bid/Ask
    const int dataLevel_ ;
    const int index_;

    const double tickSize_ ;
    const double inverseTickSize_ ;
    const double commissionRate_ ;
    const double fixedCommission_ ;
    const double  fxRate_  ;
    const double leverage_ ;
    const double unitPrice_ ;
    const double rebate_ ;
    const int timeOffset_ ;
    const double strikepr_;
    const int maturity_;
    const std::vector<SessionType> trdSessions_ ;

    static std::map<std::string, std::string> ExSymbolMap_;

    // parsing error will quit the program
    SecDef(const SecDef & sd);

    static SecDef GetSecDef(const std::string & gid, const Datestamp & date, bool debug=false);

    static Datestamp GetTradingDay() ;
    // static Datestamp TestTradingDay(Datestamp d) ;
    static int GetSessionId();

    // Invalid: GidFields.name_ == "" || GidFields.exchange_ == EX_Unknown
    static bool IsValidGid(const std::string & gid);
    static GidFields GetGidFields(const std::string & genericId) ;

    bool isSymbolOK() const ;
    SessionType getTradingSession(const Timestamp & ts) const;
    // bool isTradingTime(const Timestamp & ts) const;
    bool isTradingClosingTime(const Timestamp & ts, const Timestamp & closeTime) const;

    static std::string FindSecDefFile(ExchangeId ex, const Datestamp & date);
    static void LoadExSymbolMap();

} ;

#endif
