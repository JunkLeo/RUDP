#ifndef _TradingStruct_H_
#define _TradingStruct_H_

#include "Postman/PostUtility.H"

struct TradeAck { double price; int volume; TagType tDir; TagType pDir; };

struct OrderState { 
    double price; int volume; TagType tDir, pDir; 
    unsigned ordRef; 
    bool isCanceling;
    Timestamp sendTimeUniv, nextCancelTime; 
};

#endif
