#ifndef _Definitions_H_
#define _Definitions_H_

#include <string>

const unsigned MAX_TS_DIFF_SECS = 300 ;

const unsigned short MDS_MASK = 0x00FF;
const int SEC_GID_SIZE = 26;

const int MAX_STOCK_INT = 999999;
const int MAX_SHOPT_INT = 9999; // SH option

typedef unsigned short TradeDirEnum ;
typedef unsigned short OrderTagEnum ;
typedef unsigned short StrategyOrderStateEnum ;
typedef unsigned short TradingControlTagEnum ;

typedef unsigned short TagType;

typedef unsigned short SecType ;
typedef unsigned short ExchangeId ;
typedef unsigned short DataSource ;
typedef unsigned short MarketDataType ;

// unsigned short : 0x1100 ~~ 0x117F
const unsigned short SEC_Future = 0x1101 ;
const unsigned short SEC_Stock = 0x1102 ;
const unsigned short SEC_Index = 0x1103 ;
const unsigned short SEC_ETF = 0x1104 ;
const unsigned short SEC_Option = 0x1105 ;
const unsigned short SEC_Gold = 0x110A ;

const unsigned short BOOK_None = 0x11A0 ;
const unsigned short BOOK_New = 0x11A1 ;
const unsigned short BOOK_Modify = 0x11A2 ;
const unsigned short BOOK_Fill = 0x11A3 ;
const unsigned short BOOK_Cancel = 0x11A4 ;

//!!!!!! Change MD definition can cause serious damage !!!!!!
// MarketDataType :
const TagType MD_Unknown = 0xF000;
const TagType MD_Level1 = 0xF001 ;      // 0000 0000 0001
const TagType MD_L5Book = 0xF002 ;      // 0000 0000 0010
const TagType MD_L10Book = 0xF004 ;     // 0000 0000 0100
const TagType MD_L16Book = 0xF006  ;    // 0000 0000 0110
// const TagType MD_L5BookX = 0xF008 ;     // 0000 0000 1000
const TagType MD_L5BookX = 0xF600 ;     // 0000 0000 1000
const TagType MD_Index = 0xF010 ;       // 0000 0001 0000
const TagType MD_Short = 0xF020 ;       // 0000 0010 0000
const TagType MD_OrdQ10 = 0xF040;       // 0000 0100 0000
const TagType MD_OrdQ = 0xF080 ;        // 0000 1000 0000
const TagType MD_BookUpdate = 0xFF01;   // 1111 0000 0001
const TagType MD_TopUpdate = 0xFF02;    // 1111 0000 0010
const TagType MD_Order = 0xFF04;        // 1111 0000 0100
const TagType MD_Trans = 0xFF08;        // 1111 0000 1000

const TagType MD_DlArbi = 0xF100;       // 0001 0000 0000
const TagType MD_DlRealtime = 0xF200;   // 0010 0000 0000
const TagType MD_DlOrderStats = 0xF300; // 0011 0000 0000
const TagType MD_DlMarchPrice = 0xF400; // 0100 0000 0000
const TagType MD_Order10 = 0xF500;      // 0101 0000 0000
// const TagType MD_OrderQueue = 0xF600;   // 0110 0000 0000
const TagType MD_Level0 = 0xF700;       // 0111 0000 0000
const TagType MD_Ex = 0xF800;           // 1000 0000 0000
const TagType MD_OrderQueue = 0xF900;   // 0110 0000 0000

const ExchangeId EX_Unknown = 0x0000 ;
const ExchangeId EX_CNCFF = 0x12A1 ; // China Financial Future
const ExchangeId EX_CNSHF = 0x12A2 ; // Shanghai Commodity Future
const ExchangeId EX_CNDLF = 0x12A3 ; // Dalian Commodity Future
const ExchangeId EX_CNZZF = 0x12A4 ; // Zhengzhou Commodity Future
const ExchangeId EX_CNSHS = 0x12A5 ; // Shanghai Stock
const ExchangeId EX_CNSZS = 0x12A6 ; // Shenzhen Stock
const ExchangeId EX_CNSGE = 0x12A7 ; // Shanghai Gold Exchange
const ExchangeId EX_CNSHO = 0x12A8 ; // Shanghai Stock Option
const ExchangeId EX_UKLME = 0x12A9 ;
const ExchangeId EX_COMEX = 0x12AA ;
const ExchangeId EX_NYMEX = 0x12AB ;
const ExchangeId EX_USCBT = 0x12AC ;
const ExchangeId EX_USICE = 0x12AD ;
const ExchangeId EX_CNINE = 0x12AE ;
const ExchangeId EX_JPTCE = 0x12AF ;
const ExchangeId EX_SGSGX = 0x1291 ;
const ExchangeId EX_CNSZO = 0x1292 ; // Shenzhen Stock Option
const ExchangeId EX_CNCFO = 0x1293 ; // China Financial Future Option
const ExchangeId EX_CNDFO = 0x1294 ; // Dalian Commodity Future Option
const ExchangeId EX_CNZFO = 0x1295 ; // Zhengzhou Commodity Future Option
const ExchangeId EX_CNSFO = 0x1296 ; // Shanghai Commodity Future Option
const ExchangeId EX_CNBXG = 0x1297 ; // Wuxi Stainless Exchange

const DataSource SRC_Unknown = 0x12B0 ;
const DataSource SRC_X       = 0x12B0 ;
const DataSource SRC_CTP     = 0x12B2 ;
const DataSource SRC_CTPM    = 0x12B3 ;
const DataSource SRC_FEMAS   = 0x12B4 ;
const DataSource SRC_FEMD    = 0x12B5 ;
const DataSource SRC_XSPEED  = 0x12B6 ;
const DataSource SRC_XPL2    = 0x12B7 ;
const DataSource SRC_XQN     = 0x12B8 ;
const DataSource SRC_XQNL1   = 0x12B9 ;
const DataSource SRC_ESUN    = 0x12BA;
const DataSource SRC_ESMDP   = 0x12BB;
const DataSource SRC_ESL2    = 0x12BC;
const DataSource SRC_XSPU    = 0x12BD;
const DataSource SRC_SHBOOK  = 0x12BF;

const DataSource SRC_GTA     = 0x12C0 ;
const DataSource SRC_TDF     = 0x12C2 ;
const DataSource SRC_XELE2   = 0x12C3 ;
const DataSource SRC_HTUDP   = 0x12C4 ;
const DataSource SRC_HTDCE   = 0x12C5 ;
const DataSource SRC_XELE    = 0x12C6 ;
const DataSource SRC_SHMD    = 0x12C7 ;
const DataSource SRC_XEL2    = 0x12C8 ;
const DataSource SRC_HTDEPTH = 0x12C9 ;
const DataSource SRC_EVA     = 0x12CA ;
const DataSource SRC_EVAX    = 0x12CB ;
const DataSource SRC_ESEX    = 0x12CC ;
const DataSource SRC_UDP     = 0x12CD ;
const DataSource SRC_UDPX    = 0x12CE ;
const DataSource SRC_TCP     = 0x12CF ;

const DataSource SRC_BLP    = 0x12D1;
// const DataSource SRC_SGIT   = 0x12D2;
// const DataSource SRC_FSEXP  = 0x12D3;
const DataSource SRC_EFH    = 0x12D4;
const DataSource SRC_ZX     = 0x12D5;
const DataSource SRC_MIRROR = 0x12D6;
const DataSource SRC_RFA    = 0x12D7;
const DataSource SRC_HTCFF  = 0x12D8;
const DataSource SRC_STOCK  = 0x12D9;
const DataSource SRC_SL1    = 0x12DA; // Shangliang udp level1

const DataSource SRC_XTP   = 0x12E1;
const DataSource SRC_HTOPT = 0x12E2;
const DataSource SRC_XEOPT = 0x12E3;
const DataSource SRC_OES   = 0x12E4;
const DataSource SRC_SHANCE= 0x12E5;
const DataSource SRC_ZZUDP1 = 0x12E6;
const DataSource SRC_ZZUDP2 = 0x12E7;

// Message tag of strategy agent's message to CtpTrader
const TagType ORDER_Unknown = 0x1500 ;
const TagType ORDER_Cancel = 0x1501;
// const TagType ORDER_RegularOrder = 0x1501 ;//
// const TagType ORDER_Regular = 0x1501 ;//
const TagType ORDER_IocOrder = 0x1502 ;// Immediate trade, o.w. cancel
const TagType ORDER_Ioc = 0x1502 ;// Immediate trade, o.w. cancel
const TagType ORDER_AnyPriceOrder = 0x1503 ;//
const TagType ORDER_AnyPrice = 0x1503 ;//
const TagType ORDER_LimitPrice = 0x1504 ;//
const TagType ORDER_Fok = 0x1505 ;//
const TagType ORDER_FokBest = 0x1506 ;//
const TagType ORDER_FokFive = 0x1507 ;//
const TagType ORDER_Fak = 0x1508 ;//
const TagType ORDER_FakBest = 0x1509 ;//
const TagType ORDER_FakFive = 0x150A ;//
const TagType ORDER_Best = 0x150B; // the best price on the same side, used in stock

const TagType ORDER_Ask = 0x1681 ;
const TagType ORDER_Bid = 0x1682 ;

const TagType REQ_NewOrder = 0x1580 ;//
const TagType REQ_CancelOrder = 0x1581 ;//
const TagType REQ_CancelAll = 0x1582 ;//
const TagType REQ_CancelRawOrders = 0x153 ;
const TagType REQ_CloseAll = 0x1587 ;//

const TagType REQ_QueryOrder = 0x1588 ;
const TagType REQ_QueryTrade = 0x1589 ;
const TagType REQ_QuerySecurity = 0x158A ;
const TagType REQ_QueryAccount = 0x158B ;
const TagType REQ_QueryPosition = 0x158C ;

const TagType REQ_ChangeVolume = 0x1591 ;// Not in use
const TagType REQ_ChangePrice = 0x1592 ;// Not in use

const TagType TRADE_Unknown = 0x1600 ;
const TagType TRADE_Buy = 0x1601 ;
const TagType TRADE_Sell = 0x1602 ;
const TagType TRADE_Open = 0x1603 ;
const TagType TRADE_Close = 0x1604 ;
const TagType TRADE_YdClose = 0x1605 ;
// const TagType TRADE_ForceOpen = 0x160C ;
// const TagType TRADE_ForceOff = 0x160D ;
// const TagType TRADE_ForceClose = 0x160D ;
const TagType TRADE_Borrow = 0x160A ;
const TagType TRADE_Lend = 0x160B ;
const TagType TRADE_Simulate = 0x160F ;

const TagType CTL_RiskLimit = 0x1F01 ;
const TagType CTL_Chi = 0x1F02 ;
const TagType CTL_StopWin = 0x1F03 ;
const TagType CTL_RetryLimit = 0x1F04 ;
const TagType CTL_Closing = 0x1F05 ;
const TagType CTL_Tier = 0x1F06 ;
const TagType CTL_TierRisk = 0x1F07 ;
const TagType CTL_TipDepth = 0x1F08 ;
const TagType CTL_Offtop = 0x1F09 ;
const TagType CTL_OrderFreeze = 0x1F0A ;
const TagType CTL_Switch = 0x1F0B ;
const TagType CTL_BuySellMask = 0x1F0C ;
const TagType CTL_UniOpenLimit = 0x1F0D ;
const TagType CTL_ExFront = 0x1F0E ;
const TagType CTL_HeartBeat = 0x1F0F ;

const TagType DRV_Ready = 0x2001 ;
const TagType DRV_Disconnected = 0x2002 ;
const TagType DRV_SymbolMap = 0x2080;

const TagType POS_Unknown = 0x2A80 ;
const TagType POS_Open = 0x2A81 ;
const TagType POS_Close = 0x2A82 ;
const TagType POS_Long = 0x2A83 ;
const TagType POS_Short = 0x2A84 ;

const TagType BrokerAck_Pause = 0x3100;
const TagType BrokerAck_TradeReturn = 0x3101;
const TagType BrokerAck_OrderCanceled = 0x3102;
const TagType BrokerAck_CancelFailed = 0x3103;
const TagType BrokerAck_Position = 0x3104;
const TagType BrokerAck_OrderReturn = 0x3105;
const TagType BrokerAck_MarketLimit = 0x3108;
const TagType BrokerAck_BrokerReady = 0x3109;
const TagType BrokerAck_OpenLimit = 0x310A;

////////////////////////////////////////////////////////////////////////////////
// TDF tags
////////////////////////////////////////////////////////////////////////////////
const double TDF_PRICE_FACTOR = 0.0001 ;


std::string secTagToString(unsigned short x) ;

// std::string exchangeIdToString(ExchangeId ex) ;

// std::string dataSourceToString(DataSource src) ;

ExchangeId exchangeIdFromString(const std::string & exName) ;
DataSource dataSourceFromString(const std::string & srcName) ;

std::string stdToFutureDriverSymbol(const std::string & symbol, ExchangeId ex);
// std::string drvToStdSymbol(const std::string & drvsym, ExchangeId ex, DataSource drv);
std::string stdToExchangeSymbol(const std::string & drvsym, ExchangeId ex);

struct GidFields {
public:
        std::string name_ ;
        ExchangeId exchange_ ;
        DataSource source_ ;
        int index_, level_ , month_ ;
        std::string extra_ ; // contract number for futures; type for stock securities (index, etf, ...)
        std::string direction_ ;
        std::string sunit_ ;
        GidFields(const std::string & name, ExchangeId ex, DataSource src, int level, std::string extra)
                : name_(name), exchange_(ex), source_(src), index_(0), level_(level), extra_(extra), month_(1), direction_(), sunit_() {} ;
        GidFields(const std::string & name, ExchangeId ex, DataSource src, int level, int idx)
                : name_(name), exchange_(ex), source_(src), index_(idx), level_(level), extra_(), month_(1), direction_(), sunit_() {} ;
        GidFields(const std::string & name, ExchangeId ex, DataSource src, int level, int idx, const std::string & direction, int month, const std::string & sunit )
                : name_(name), exchange_(ex), source_(src), index_(idx), level_(level), extra_(), direction_(direction),month_(month),sunit_(sunit) {};
        GidFields() : name_(), exchange_(EX_Unknown), source_(0), index_(0), level_(0), extra_(), month_(1), direction_(), sunit_() {} ;

};


//inline bool doublePriceEqualCompare(const double & d1, const double & d2)
//{  return (fabs(d1 - d2) <= PEPS); };

#endif
