#include "Security/Definitions.H"

#include "Postman/PostUtility.H"

#include <string>

using namespace std ;

string secTagToString(unsigned short x) {
    if (x < 0x1000) return postTagToString(x);
    switch (x) {

    case SEC_Future: return string("SEC_Future");
    case SEC_Stock: return string("SEC_Stock");
    case SEC_Index: return string("SEC_Index");
    case SEC_ETF: return string("SEC_ETF");
    case SEC_Option: return string("SEC_Option");
    case SEC_Gold: return string("SEC_Gold");

    case BOOK_None: return string("BOOK_None");
    case BOOK_New: return string("BOOK_New");
    case BOOK_Modify: return string("BOOK_Modify");
    case BOOK_Fill: return string("BOOK_Fill");
    case BOOK_Cancel: return string("BOOK_Cancel");

    case MD_Unknown : return string("MD_Unknown") ;
    case MD_Index : return string("MD_Index") ;
    case MD_Level1 : return string("MD_Level1") ;
    case MD_Trans : return string("MD_Trans") ;
    case MD_Order : return string("MD_Order");
    case MD_BookUpdate : return string("MD_BookUpdate") ;
    case MD_TopUpdate : return string("MD_TopUpdate") ;
    case MD_OrdQ10 : return string("MD_OrdQ10");
    case MD_OrdQ : return string("MD_OrdQ") ;
    case MD_Order10 : return string("MD_Order10");
    case MD_OrderQueue : return string("MD_OrderQueue") ;
    case MD_L5Book : return string("MD_L5Book") ;
    case MD_L10Book : return string("MD_L10Book") ;
    case MD_L16Book : return string("MD_L16Book") ;
    case MD_Short : return string("MD_Short") ;
    case MD_DlArbi : return string("MD_DlArbi");
    case MD_DlRealtime : return string("MD_DlRealtime");
    case MD_DlOrderStats : return string("MD_DlOrderStats");
    case MD_DlMarchPrice : return string("MD_DlMarchPrice");
    case MD_Ex : return string("MD_Ex");
    case MD_Level0 : return string("MD_Level0") ;
    case MD_L5BookX : return string("MD_L5BookX") ;

    case EX_Unknown: return string("EX_X") ;
    case EX_CNCFF: return string("CNCFF") ;
    case EX_CNSHF: return string("CNSHF") ;
    case EX_CNDLF: return string("CNDLF") ;
    case EX_CNZZF: return string("CNZZF") ;
    case EX_CNSHS: return string("CNSHS") ;
    case EX_CNSZS: return string("CNSZS") ;
    case EX_CNSGE: return string("CNSGE") ;
    case EX_CNSHO: return string("CNSHO") ;
    case EX_CNINE: return string("CNINE") ;
    case EX_COMEX: return string("COMEX") ;
    case EX_NYMEX: return string("NYMEX") ;
    case EX_USCBT: return string("USCBT") ;
    case EX_USICE: return string("USICE") ;
    case EX_UKLME: return string("UKLME") ;
    case EX_JPTCE: return string("JPTCE") ;
    case EX_SGSGX: return string("SGSGX") ;
    case EX_CNBXG: return string("CNBXG") ;

    case SRC_Unknown : return string("SRC_X") ;
    case SRC_UDP : return string("UDP") ;
    case SRC_UDPX : return string("UDPX") ;
    case SRC_TCP : return string("TCP") ;
    case SRC_CTP : return string("CTP") ;
    case SRC_CTPM : return string("CTPM") ;
    case SRC_FEMAS : return string("FEMAS") ;
    case SRC_FEMD: return string("FEMD") ;
    case SRC_XSPEED : return string("XSP") ;
    case SRC_XSPU : return string("XSPU") ;
    case SRC_XPL2 : return string("XPL2") ;
    case SRC_XQN : return string("XQN") ;
    case SRC_XQNL1 : return string("XQNL1") ;
    case SRC_ESUN : return string("ESUN") ;
    case SRC_ESL2 : return string("ESL2") ;
    case SRC_ESMDP : return string("ESMDP") ;
    case SRC_SHBOOK : return string("SHBOOK") ;

    case SRC_GTA : return string("GTA") ;
    case SRC_TDF : return string("TDF") ;
    case SRC_XELE2 : return string("XELE2") ;
    case SRC_HTUDP : return string("HTUDP") ;
    case SRC_HTDCE : return string("HTDCE") ;
    case SRC_HTDEPTH : return string("HTDEP") ;
    case SRC_XELE : return string("XELE") ;
    case SRC_XEL2 : return string("XEL2") ;
    case SRC_SHMD : return string("SHMD") ;
    case SRC_EVA : return string("EVA") ;
    case SRC_EVAX : return string("EVAX") ;
    case SRC_ESEX : return string("ESEX") ;

    case SRC_BLP : return string("BLP") ;
    // case SRC_SGIT : return string("SGIT") ;
    // case SRC_FSEXP : return string("FSEXP") ;
    case SRC_EFH : return string("EFH") ;
    case SRC_ZX : return string("ZX") ;
    case SRC_MIRROR : return string("MIRROR") ;
    case SRC_RFA : return string("RFA") ;
    case SRC_HTCFF : return string("HTCFF") ;
    case SRC_STOCK : return string("STOCK") ;
    case SRC_SL1: return string("SL1") ;

    case SRC_XTP : return string("XTP") ;
    case SRC_HTOPT : return string("HTOPT") ;
    case SRC_XEOPT : return string("XEOPT") ;
    case SRC_OES :   return string("OES");
    case SRC_SHANCE :   return string("SHANCE");
    case SRC_ZZUDP1:   return string("ZZUDP1");
    case SRC_ZZUDP2:   return string("ZZUDP2");

        // Message tag of strategy agent's message to CtpTrader
    case ORDER_Unknown : return string("ORDER_Unknown") ;
    case ORDER_Cancel : return string("ORDER_Cancel") ;// Order get in queue
    case ORDER_Ioc : return string("ORDER_Ioc") ;// Immediate trade, o.w. cancel
    case ORDER_AnyPrice : return string("ORDER_AnyPr") ;// Immediate trade, o.w. cancel
    case ORDER_LimitPrice : return string("ORDER_LimPr") ;
    case ORDER_Fok : return string("ORDER_Fok");
    case ORDER_FokBest: return string("ORDER_FokBest");
    case ORDER_FokFive: return string("ORDER_FokFive");
    case ORDER_Fak: return string("ORDER_Fak");
    case ORDER_FakBest: return string("ORDER_FakBest");
    case ORDER_FakFive: return string("ORDER_FakFive");

    case ORDER_Ask : return string("ORDER_Ask") ;
    case ORDER_Bid : return string("ORDER_Bid") ;

    case REQ_NewOrder : return string("REQ_NewOrder") ;// Cancel the order in queue
    case REQ_CancelOrder : return string("REQ_CancelOrder") ;// Cancel the order in queue
    case REQ_CancelAll : return string("REQ_CancelAll") ;// Cancel the order in queue
    case REQ_CancelRawOrders : return string("REQ_CancelRawOrders") ;
    case REQ_CloseAll : return string("REQ_CloseAll") ;// Close all open position

    case REQ_QueryOrder : return string("REQ_QueryOrder") ;
    case REQ_QueryPosition : return string("REQ_QueryPosition") ;
    case REQ_QueryAccount : return string("REQ_QueryAccount") ;
    case REQ_QuerySecurity : return string("REQ_QuerySecurity") ;
    case REQ_QueryTrade : return string("REQ_QueryTrade") ;

    case REQ_ChangeVolume : return string("REQ_ChangeVolume") ;// Not in use
    case REQ_ChangePrice : return string("REQ_ChangePrice") ;// Not in use

    case TRADE_Unknown : return string("TRADE_Unknown") ;
    case TRADE_Buy : return string("TRADE_Buy") ;
    case TRADE_Sell : return string("TRADE_Sell") ;
    case TRADE_Open : return string("TRADE_Open") ;
    case TRADE_Close : return string("TRADE_Close") ;
    case TRADE_YdClose : return string("TRADE_YdClose") ;
    // case TRADE_BuyOpen : return string("TRADE_BuyOpen") ;
    // case TRADE_SellOpen : return string("TRADE_SellOpen") ;
    // case TRADE_BuyClose : return string("TRADE_BuyClose") ;
    // case TRADE_SellClose : return string("TRADE_SellClose") ;
    // case TRADE_BuyCloseYesterday : return string("TRADE_BuyCloseYesterday") ;
    // case TRADE_SellCloseYesterday : return string("TRADE_SellCloseYesterday") ;
    // case TRADE_ForceOpen : return string("TRADE_ForceOpen") ;
    // case TRADE_ForceOff: return string("TRADE_ForceOff") ;
    case TRADE_Borrow : return string("TRADE_Borrow") ;
    case TRADE_Lend : return string("TRADE_Lend") ;
    case TRADE_Simulate: return string("TRADE_Simulate") ;

    case BrokerAck_Pause : return std::string("BrokerAck_Pause");
    case BrokerAck_TradeReturn : return std::string("BrokerAck_TradeReturn");
    case BrokerAck_OrderReturn : return std::string("BrokerAck_OrderReturn");
    case BrokerAck_OrderCanceled : return std::string("BrokerAck_OrderCanceled");
    case BrokerAck_CancelFailed : return std::string("BrokerAck_CancelFailed");
    case BrokerAck_Position : return std::string("BrokerAck_Position");
    case BrokerAck_MarketLimit : return std::string("BrokerAck_MarketLimit");
    case BrokerAck_BrokerReady : return std::string("BrokerAck_BrokerReady");

    case CTL_RiskLimit : return string("CTL_RiskLimit");
    case CTL_Chi : return string("CTL_Chi") ;
    case CTL_StopWin : return string("CTL_StopWin") ;
    case CTL_RetryLimit : return string("CTL_RetryLimit") ;
    case CTL_Closing : return string("CTL_Closing") ;
    case CTL_Tier: return string("CTL_Tier");
    case CTL_TierRisk: return string("CTL_TierRisk");
    case CTL_TipDepth: return string("CTL_TipDepth");
    case CTL_Offtop: return string("CTL_Offtop");
    case CTL_OrderFreeze: return string("CTL_OrderFreeze");
    case CTL_HeartBeat : return string("CTL_HeartBeat") ;
    case CTL_Switch : return string("CTL_Switch") ;
    case CTL_BuySellMask : return string("CTL_BuySellMask") ;
    case CTL_UniOpenLimit : return string("CTL_UniOpenLimit") ;
    case CTL_ExFront : return string("CTL_ExFront") ;

    case DRV_Ready: return string("DRV_Ready") ;
    case DRV_Disconnected: return string("DRV_Disconnected") ;
    case DRV_SymbolMap: return string("DRV_SymbolMap") ;

    case POS_Unknown : return string("POS_Unknown") ;
    case POS_Open : return string("POS_Open") ;
    case POS_Close : return string("POS_Close") ;
    case POS_Long : return string("POS_Long") ;
    case POS_Short : return string("POS_Short") ;

    default: { char s[16]; sprintf(s, "SEC_%04x", x); return std::string(s); };
    }
} ;

ExchangeId exchangeIdFromString(const string & exName) {
    if (upperString(exName) == "CNCFF") return EX_CNCFF ;
    else if (upperString(exName) == "CNSHF") return EX_CNSHF ;
    else if (upperString(exName) == "CNDLF") return EX_CNDLF ;
    else if (upperString(exName) == "CNZZF") return EX_CNZZF ;
    else if (upperString(exName) == "CNSHS") return EX_CNSHS ;
    else if (upperString(exName) == "CNSZS") return EX_CNSZS ;
    else if (upperString(exName) == "CNSGE") return EX_CNSGE ;
    else if (upperString(exName) == "CNSHO") return EX_CNSHO ;
    else if (upperString(exName) == "CNINE") return EX_CNINE ;

    else if (upperString(exName) == "COMEX") return EX_COMEX ;
    else if (upperString(exName) == "NYMEX") return EX_NYMEX ;
    else if (upperString(exName) == "USCBT") return EX_USCBT ;
    else if (upperString(exName) == "CBOT")  return EX_USCBT ;
    else if (upperString(exName) == "USICE") return EX_USICE ;
    else if (upperString(exName) == "UKLME") return EX_UKLME ;
    else if (upperString(exName) == "LME")   return EX_UKLME ;
    else if (upperString(exName) == "JPTCE") return EX_JPTCE ;
    else if (upperString(exName) == "TOCOM") return EX_JPTCE ;
    else if (upperString(exName) == "SGSGX") return EX_SGSGX ;
    else Warn("Invalid exchange name: " + exName) ;
    return EX_Unknown ;
} ;

DataSource dataSourceFromString(const string & srcName) {
    if (lowerString(srcName) == "eva") return SRC_EVA ;
    else if (lowerString(srcName) == "ctp") return SRC_CTP ;
    else if (lowerString(srcName) == "femas") return SRC_FEMAS ;
    else if (lowerString(srcName) == "xspeed") return SRC_XSPEED ;
    else if (lowerString(srcName) == "tdf") return SRC_TDF ;
    else if (lowerString(srcName) == "gta") return SRC_GTA ;
    else if (lowerString(srcName) == "htudp") return SRC_HTUDP ;
    else Warn("Invalid data source: " + srcName) ;
    return SRC_Unknown ;
}

string stdToFutureDriverSymbol(const std::string & symbol, ExchangeId ex) {
    switch (ex) {
    case EX_CNSHF: case EX_CNDLF: case EX_CNINE: {
        return lowerString(symbol) ;
    }
    case EX_CNZZF: {// FG1406 --> FG406
        int len = symbol.length() ;
        if (len == 0) return std::string() ;
        int i ;
        for (i = 0; i < len; i++)
            if (symbol[i] < 65 || (symbol[i] > 90 && symbol[i] < 97) || symbol[i] > 122) break ;
        if (len - i == 4)
            return std::string(symbol.substr(0, i) + symbol.substr(i+1)) ;
        else
            return std::string(symbol) ;
    }
    default:
        return std::string(symbol) ;
    }
};

string stdToExchangeSymbol(const std::string & symbol, ExchangeId ex) {
    switch (ex) {
    case EX_CNSHF: case EX_CNDLF: case EX_CNZZF: case EX_CNCFF : case EX_CNINE: {
        return stdToFutureDriverSymbol(symbol, ex) ;
    }
    case EX_CNSGE: {
        if (symbol == "AUD") return "Au(T+D)";
        if (symbol == "MAUD") return "mAu(T+D)";
        if (symbol == "AUN1") return "Au(T+N1)";
        if (symbol == "AUN2") return "Au(T+N2)";
        if (symbol == "AU100") return "Au100g";
        if (symbol == "AU50") return "Au50g";
        if (symbol == "AU100") return "Au100g";
        if (symbol == "AU995") return "Au99.5";
        if (symbol == "AU9995") return "Au99.95";
        if (symbol == "AU9999") return "Au99.99";
        if (symbol == "AGD") return "Ag(T+D)";
        if (symbol == "AG999") return "Ag99.9";
        if (symbol == "AG9999") return "Ag99.99";
        return std::string(symbol);
    }
    default:
        return std::string(symbol) ;
    }
};
