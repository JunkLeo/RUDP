#ifndef _MarketData_H_
#define _MarketData_H_

#ifdef __MDOPT__
#include "Security/OptMdHead.H"
#else
#include "Security/MdHead.H"
#endif

//#include "Postman/PostUtility.H"
//#include "Postman/PostSync.H"
#include "Security/Definitions.H"

// #include <list>
// #include <string>

// MdHead / MdShort
//   0    1    2    3    4    5    6    7    8
//   -----------------------------------------
// 1 |              |Bad | SRC     | MdType  |
//   -----------------------------------------
// 2 | marketTime (rawvalue)                 |
//   -----------------------------------------
// 3 | apiTime (rawValue)                    |
//   -----------------------------------------
// 4 | price                                 |
//   -----------------------------------------
// 5 | volume                                |
//   -----------------------------------------
// 6 | symbol                                |
//   -----------------------------------------
// 7 |                                       |
//   -----------------------------------------
// 8 |                                       |
//   -----------------------------------------

// MdIndex
//   0    1    2    3    4    5    6    7    8
//   -----------------------------------------
// 1 |                   | SRC     | MdType  |
//   -----------------------------------------
// 2 | marketTime (rawvalue)                 |
//   -----------------------------------------
// 3 | apiTime (rawValue)                    |
//   -----------------------------------------
// 4 | price                                 |
//   -----------------------------------------
// 5 | volume                                |
//   -----------------------------------------
// 6 | symbol                                |
//   -----------------------------------------
// 7 |                                       |
//   -----------------------------------------
// 8 |                                       |
//   -----------------------------------------

struct MdCast {
    unsigned short tag;
    unsigned short nBytes;
    unsigned short source, mdType;
    char symbol[16];
    char marketTime[16];
    unsigned long long localTimestamp;
    double price;
    long long volume;
    double  accumTurnover;
    double accumPosition ;
    double askPrice ;
    double bidPrice ;
    int askVolume ; int bidVolume ;
} ;

//struct MdHeadCore {
//    char bReady; unsigned char bBad;
//    unsigned short nBytes;
//    unsigned short source, mdType ; // Also interpreted as buffer entry head
//    char symbol[16];
//    char marketTime[16]; // Uint64 marketTimeValue;
//    Uint64 tsApiUniversal;
//};
//
//struct MdHeadOffline {
//    unsigned sequenceNo;
//    unsigned short nBytes, mdType ; // Also interpreted as buffer entry head
//    char symbol[16];
//    char marketTime[16]; // Uint64 marketTimeValue;
//    Uint64 tsApiUniversal;
//};

struct MdShortCore: public MdHeadCore {
    // MdHeadCore head;
    double price;
    Int64 volume;
};

struct MdIndexCore: public MdHeadCore {
    // MdHeadCore head;
    double price;
    Int64 volume;
    double accumTurnover;
} ;

struct MdTrans: public MdHeadCore {
    // MdHeadCore head;
    double price, turnover;
    int volume;
    TagType dir;
    char tdfCode[2];
    Int64 orderIdBid;
    Int64 orderIdAsk;
};

struct MdTopUpdate: public MdHeadCore {
    // MdHeadCore head;
    double priceBid;
    double priceAsk;
    int volumeBid;
    int volumeAsk;
};

struct MdBookUpdateEntry {
    double price;
    unsigned int volume;
    TagType side;//bid ask
    TagType action;//insert change cancel fill
    Uint64 orderid;
    int changedVol;
};

struct MdBookUpdate: public MdHeadCore {
    // MdHeadCore head;
    unsigned int entryCount;
    MdBookUpdateEntry entries[0];
};

struct MdL0Core: public MdHeadCore {
    // MdHeadCore head;
    double price;
    Int64 volume;
    double accumTurnover;
    double accumPosition;
} ;

struct MdL1Core: MdHeadCore {
    // MdHeadCore head;
    double price;
    Int64 volume;
    double accumTurnover;
    double accumPosition;
    double askPrice ;
    double bidPrice ;
    int askVolume ; int bidVolume ;
} ;

struct MdL5Book: public MdHeadCore {
    // MdHeadCore head;
    double price;
    Int64 volume;
    double  accumTurnover;
    double accumPosition ;
    double bookPrice[10]; // first 5 ask, second 5 bid
    int bookVolume[10];
} ;

struct MdL5BookX: public MdHeadCore {
    // MdHeadCore head;
    double price;
    Int64 volume;
    double  accumTurnover;
    double accumPosition ;
    double bookPrice[10]; // first 5 ask, second 5 bid
    int bookVolume[10];
    int bookCount[10];
} ;

struct MdL10Book: public MdHeadCore {
    // MdHeadCore head;
    double price;
    Int64 volume;
    double  accumTurnover;
    double accumPosition ;
    double bookPrice[20]; // first 10 ask, second 10 bid
    int bookVolume[20];
} ;

struct MdL16Book: public MdHeadCore {
    // MdHeadCore head;
    double price;
    Int64 volume;
    double  accumTurnover;
    double accumPosition ;
    double bookPrice[32]; 
    int bookVolume[32];
} ;

struct MdOrder: public MdHeadCore {
    // MdHeadCore head;
    double price;
    int volume;
    TagType dir;
    TagType orderType; //any price order, limited price order, best price order
    Int64 orderSeqNum;
    // int pad;
};

struct MdOrdQ10: public MdHeadCore {
    // MdHeadCore head;
    double askPrice, bidPrice;
    int vols[20]; // first 10 ask, second 10 bid
} ;

struct MdOrdQ: public MdHeadCore {
    // MdHeadCore head;
    double askPrice, bidPrice;
    int askCount, bidCount;
    int vols[0]; // first ask, then bid
} ;

struct MdOrder10: public MdHeadCore {
    // MdHeadCore head;
    double price;
    int orderVolume[10]; // top 10 orders at price
} ;

struct MdOrderQueue: public MdHeadCore {
    // MdHeadCore head;
    double price;
    int nOrders;
    TagType dir, qCount;
    int vols[50];
};

struct MdDlArbi: public MdHeadCore {
    // MdHeadCore head;
    double price;
    Int64 volume;
    double  accumTurnover;
    double accumPosition ;
    double askPrice ;
    double bidPrice ;
    int askVolume ; int bidVolume ;
};

struct MdDlRealtime: public MdHeadCore {
    // MdHeadCore head;
    double price;
};

struct MdDlOrderStats: public MdHeadCore {
    // MdHeadCore head;
    double avBuyPrice, avSellPrice;
    int nBuyOrders, nSellOrders;
};

struct MdDlMarchPrice: public MdHeadCore {
    // MdHeadCore head;
    double mPrice[4];
    int nBuyOpen[4];
    int nBuyClose[4];
    int nSellOpen[4];
    int nSellClose[4];
};

namespace MD{
    int parseMdLine(const std::string & line, MdHeadCore * mdhead, unsigned bufsize);
    // NanoTime getMarketTime(MdHeadCore * mdhead);
    std::string mdToString(const MdHeadCore * mdhead);
    // NanoTime getApiLocalTimeInday(MdHeadCore * mdhead);
    // int getSessionId();
} ;
#endif
