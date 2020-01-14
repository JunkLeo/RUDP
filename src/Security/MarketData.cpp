#include "Security/MarketData.H"
#include "Postman/PostUtility.H"

#include <string>
#include <list>
#include <string.h>

using namespace std;

const unsigned SIZE_MdOrderQueue = sizeof(MdOrderQueue);
const unsigned SIZE_MdOrdQ = sizeof(MdOrdQ);

string orderSideToString(TagType s) {
    switch (s) {
    case ORDER_Ask: return string("A");
    case ORDER_Bid: return string("B");
    default: return string("X");
    }
};

string bookActionToString(TagType s) {
    switch (s) {
    case BOOK_New: return string("N");
    case BOOK_Fill: return string("F");
    case BOOK_Modify: return string("M");
    case BOOK_Cancel: return string("C");
    default: return string("X");
    }
};

string MD::mdToString(const MdHeadCore * mdhead) {
    char mts[16];
    strncpy(mts, mdhead->marketTime, 15);
    if (SRC_ESL2 == mdhead->source) {
        int x = atoi(mts);
        int hb=10000000, mb=100000, sb=1000;
        sprintf(mts, "%02d:%02d:%02d.%03d", x/hb, (x%hb)/mb, (x%mb)/sb, x%sb);
    };

    string mdstr
        = Timestamp::LocalTimeFromUniversal(Timestamp::FromRawValue(mdhead->tsApiUniversal)).fullString() \
        + "," + string(mts) + "," + secTagToString(mdhead->source) + "," + string(mdhead->symbol) ;

    switch (mdhead->mdType) {
    case MD_Short: {
        const MdShortCore * p = reinterpret_cast<const MdShortCore*>(mdhead) ;
        mdstr += ( ",SHORT," + doubleToString(p->price, 4) + "," + to_string(p->volume) );
    } break ;
    case MD_Index: {
        const MdIndexCore * p = reinterpret_cast<const MdIndexCore *>(mdhead) ;
        mdstr += ( ",IDX," + doubleToString(p->price, 4)
                   + "," + to_string(p->volume)                         \
                   + "," + doubleToString(p->accumTurnover, 4) );
    } break ;
    case MD_Ex: {
        const MdL1Core * p = reinterpret_cast<const MdL1Core *>(mdhead) ;
        mdstr += ( ",EX," + doubleToString(p->price, 4) \
                   + "," + to_string(p->volume) \
                   + "," + doubleToString(p->accumTurnover, 4) \
                   + "," + doubleToString(p->accumPosition, 0) \
                   + "," + doubleToString(p->askPrice, 4) \
                   + ":" + to_string(p->askVolume) \
                   + "," + doubleToString(p->bidPrice, 4) \
                   + ":" + to_string(p->bidVolume) \
                   );

    } break ;
    case MD_Level0: {
        const MdL0Core * p = reinterpret_cast<const MdL0Core *>(mdhead) ;
        mdstr += ( ",L0," + doubleToString(p->price, 4) \
                   + "," + to_string(p->volume) \
                   + "," + doubleToString(p->accumTurnover, 4) \
                   + "," + doubleToString(p->accumPosition, 0) \
                   );

    } break ;
    case MD_Level1: {
        const MdL1Core * p = reinterpret_cast<const MdL1Core *>(mdhead) ;
        mdstr += ( ",L1," + doubleToString(p->price, 4) \
                   + "," + to_string(p->volume) \
                   + "," + doubleToString(p->accumTurnover, 4) \
                   + "," + doubleToString(p->accumPosition, 0) \
                   + "," + doubleToString(p->askPrice, 4) \
                   + ":" + to_string(p->askVolume) \
                   + "," + doubleToString(p->bidPrice, 4) \
                   + ":" + to_string(p->bidVolume) \
                   );

    } break ;
    case MD_L5Book: {
        const MdL5Book * p = reinterpret_cast<const MdL5Book *>(mdhead) ;
        mdstr += (",L5," + doubleToString(p->price, 4) + "," + to_string(p->volume) \
                  + "," + doubleToString(p->accumTurnover, 4) + "," + doubleToString(p->accumPosition, 0));
        for (int i=0; i<10; i++)
            mdstr += ("," + doubleToString(p->bookPrice[i], 4) + ":" + to_string(p->bookVolume[i]));

    } break;
    case MD_L5BookX: {
        const MdL5BookX * p = reinterpret_cast<const MdL5BookX *>(mdhead) ;
        mdstr += (",L5X," + doubleToString(p->price, 4) + "," + to_string(p->volume) \
                  + "," + doubleToString(p->accumTurnover, 4) + "," + doubleToString(p->accumPosition, 0));
        for (int i=0; i<10; i++)
            mdstr += ("," + doubleToString(p->bookPrice[i], 4) + ":" + to_string(p->bookVolume[i])+ ":" + to_string(p->bookCount[i]));

    } break;
    case MD_L10Book: {
        const MdL10Book * p = reinterpret_cast<const MdL10Book *>(mdhead) ;
        mdstr += (",L10," + doubleToString(p->price, 4) + "," + to_string(p->volume) \
                  + "," + doubleToString(p->accumTurnover, 4) + "," + doubleToString(p->accumPosition, 0));
        for (int i=0; i<20; i++)
            mdstr += ("," + doubleToString(p->bookPrice[i], 4) + ":" + to_string(p->bookVolume[i]));

    } break;
    case MD_L16Book: {
        const MdL16Book * p = reinterpret_cast<const MdL16Book *>(mdhead) ;
        mdstr += (",L16," + doubleToString(p->price, 4) + "," + to_string(p->volume) \
                  + "," + doubleToString(p->accumTurnover, 4) + "," + doubleToString(p->accumPosition, 0));
        for (int i=0; i<32; i++)
            mdstr += ("," + doubleToString(p->bookPrice[i], 4) + ":" + to_string(p->bookVolume[i]));

    } break;
    case MD_Trans: {
        const MdTrans * p = reinterpret_cast<const MdTrans *>(mdhead) ;
        mdstr += (",TRANS," + doubleToString(p->price, 4) + "," + to_string(p->volume) + "," + doubleToString(p->turnover, 4));
        if (TRADE_Buy == p->dir)
            mdstr += ",B";
        else if (TRADE_Sell == p->dir)
            mdstr += ",S";
        else if ('Y' == p->dir)
            mdstr += ",Y";
        else
            mdstr += ",X";
        mdstr += "," + to_string(p->orderIdBid) + "," + to_string(p->orderIdAsk);
        // + "," + to_string(p->tdfCode[0]) + "," + to_string(p->tdfCode[1]));

    } break;
    case MD_TopUpdate: {
        const MdTopUpdate * p = reinterpret_cast<const MdTopUpdate *>(mdhead) ;
        mdstr += (",TOP," + doubleToString(p->priceAsk, 4) + ":" + to_string(p->volumeAsk) + ","  + doubleToString(p->priceBid, 4) + ":" + to_string(p->volumeBid) );

    } break;
    case MD_BookUpdate: {
        const MdBookUpdate * p = reinterpret_cast<const MdBookUpdate *>(mdhead) ;
        mdstr += (",BUPD," + to_string(p->entryCount));
        for (int i=0; i<p->entryCount; i++)
            mdstr += ("," + doubleToString(p->entries[i].price, 4) + ":" + to_string(p->entries[i].volume) + ":" + orderSideToString(p->entries[i].side)
                      + ":" + bookActionToString(p->entries[i].action) + ":" + to_string(p->entries[i].orderid));

    } break;
    case MD_Order: {
        const MdOrder * p = reinterpret_cast<const MdOrder *>(mdhead) ;
        mdstr += (",ORD," + doubleToString(p->price, 4) + "," + to_string(p->volume) + ",") ;
        if (ORDER_Cancel == p->orderType)
            mdstr += "0";
        else if (ORDER_AnyPrice == p->orderType)
            mdstr += "1";
        else if (ORDER_LimitPrice == p->orderType)
            mdstr += "0";
        else if (ORDER_Best == p->orderType)
            mdstr += "U";

        if (ORDER_Cancel != p->orderType){
            if ( TRADE_Buy == p->dir || ORDER_Bid == p->dir)
                mdstr += "B";
            else if (TRADE_Sell == p->dir || ORDER_Ask == p->dir)
                mdstr += "S";
        }
        else
            mdstr += "X";
        mdstr += "," + to_string(p->orderSeqNum);
        // + "," + to_string(p->tdfCode[0]) + "," + to_string(p->tdfCode[1]));

    } break;
    case MD_OrdQ10: {
        const MdOrdQ10 * p = reinterpret_cast<const MdOrdQ10 *>(mdhead) ;
        mdstr += (",Q10," + doubleToString(p->askPrice, 4) + ",");
        for (int i=0; i<10; i++)  { mdstr += ((i==0?"":":")  + to_string(p->vols[i])); }
        mdstr += ("," + doubleToString(p->bidPrice, 4) + ",");
        for (int i=10; i<20; i++) { mdstr += ((i==10?"":":") + to_string(p->vols[i])); }

    } break;
    case MD_OrdQ: {
        const MdOrdQ * p = reinterpret_cast<const MdOrdQ *>(mdhead) ;
        int * q = (int *) ( ((char *)p) + SIZE_MdOrdQ);
        int i = 0;
        mdstr += (",QXX," + doubleToString(p->askPrice, 4) + "," + to_string(p->askCount) );
        for (; i<p->askCount; i++) { mdstr += (":" + to_string(q[i])); }
        mdstr += ("," + doubleToString(p->bidPrice, 4) + "," + to_string(p->bidCount) );
        for (; i<(p->askCount+p->bidCount); i++) { mdstr += (":" + to_string(q[i])); }

    } break;
    case MD_Order10: {
        const MdOrder10 * p = reinterpret_cast<const MdOrder10 *>(mdhead) ;
        mdstr += (",O10," + doubleToString(p->price, 4));
        for (int i=0; i<10; i++) { mdstr += ("," + to_string(p->orderVolume[i])); }

    } break;
    case MD_OrderQueue: {
        const MdOrderQueue * p = reinterpret_cast<const MdOrderQueue *>(mdhead) ;
        mdstr += (",ORDQ," + doubleToString(p->price, 4) + "," + to_string(p->nOrders) + "," + to_string(p->qCount)
                  + (p->dir == TRADE_Sell ? ",S" : ",B"));
        int * q = (int *) ( ((char *)p) + SIZE_MdOrderQueue);
        for (int i=0; i<p->qCount; i++) { mdstr += ("," + to_string(q[i])); }

    } break;
    case MD_DlArbi: {
        const MdDlArbi * p = reinterpret_cast<const MdDlArbi *>(mdhead) ;
        mdstr += (",DLA," + doubleToString(p->price, 4) + "," + to_string(p->volume) \
                  + "," + doubleToString(p->accumTurnover, 2) + "," + doubleToString(p->accumPosition, 2) \
                   + "," + doubleToString(p->askPrice, 4) + ":" + to_string(p->askVolume) \
                   + "," + doubleToString(p->bidPrice, 4) + ":" + to_string(p->bidVolume) \
                  );

    } break;
    case MD_DlRealtime: {
        const MdDlRealtime * p = reinterpret_cast<const MdDlRealtime *>(mdhead) ;
        mdstr += (",DLR," + doubleToString(p->price, 4) );

    } break;
    case MD_DlOrderStats: {
        const MdDlOrderStats * p = reinterpret_cast<const MdDlOrderStats *>(mdhead) ;
        mdstr += (",DLS," + doubleToString(p->avBuyPrice, 4) + "," + to_string(p->nBuyOrders) \
                  + "," + doubleToString(p->avSellPrice, 4) + "," + to_string(p->nSellOrders) );

    } break;
    case MD_DlMarchPrice: {
        const MdDlMarchPrice * p = reinterpret_cast<const MdDlMarchPrice *>(mdhead) ;
        mdstr += ",DLM";
        for (int i=0; i<4; i++)
            mdstr += ( "," + doubleToString(p->mPrice[i], 4) + ":" + to_string(p->nBuyOpen[i]) + ":" + to_string(p->nBuyClose[i]) \
                       + ":" + to_string(p->nSellOpen[i]) + ":" + to_string(p->nSellClose[i]) );

    } break;
    default :
        return ("MD:" + secTagToString(mdhead->mdType));
    }

    return string(mdstr);
}

int MD::parseMdLine(const string & line, MdHeadCore * mdhead, unsigned bufsize) {
    vector<string> ws = tokenize(line, ',') ;
    // debugging(line.c_str());
    if (ws.size() < 7) {
        throw PostError("Too few fields: %d, (at least 8)", ws.size()) ;
        return -1;
    }

    if (ws[5] == "EX") {
        return 1;
    }
    else if (ws[5] == "DLS" || ws[5] == "DLM" || ws[5] == "DLR") {
        mdhead->symbol[0] = '\0'; mdhead->marketTime[0] = '\0'; mdhead->mdType = MD_Unknown;
        return 2;
    }
    // else if (ws[5] == "TRANS" || ws[5] == "ORD" || ws[5] == "ORDQ") {
    //     mdhead->symbol[0] = '\0'; mdhead->marketTime[0] = '\0'; mdhead->mdType = MD_Unknown;
    //     return 3;
    // }

    memset(mdhead, 0, bufsize);

    NanoTime apiTime = NanoTime::FromString(ws[1]);
    if (apiTime < NanoTime(3,0,0)) apiTime = apiTime+TsHour(24);
    mdhead->tsApiUniversal = apiTime.rawValue();

    strncpy(mdhead->marketTime, ws[2].c_str(), 15);   mdhead->marketTime[15] = '\0';
    strncpy(mdhead->symbol, ws[4].c_str(), 15);       mdhead->symbol[15] = '\0';

    if (ws[3] == "ESMDP") mdhead->source = SRC_ESMDP;
    else                  mdhead->source = SRC_X;

    if (ws[5] == "SHORT") {
        mdhead->mdType = MD_Short;
        mdhead->nBytes = sizeof(MdShortCore);
        MdShortCore * mdShort = (MdShortCore*) mdhead;
        mdShort->price = stod(ws[6]);   mdShort->volume = stoll(ws[7]);
        return 0;
    }
    else if (ws[5] == "IDX") {
        if (ws.size() < 9) throw PostError("MdIndex fields < 9!");
        mdhead->mdType = MD_Index;
        mdhead->nBytes = sizeof(MdIndexCore);
        MdIndexCore * mdIndex = (MdIndexCore*) mdhead;
        mdIndex->price = stod(ws[6]);   mdIndex->volume = stoll(ws[7]);
        mdIndex->accumTurnover = stod(ws[8]);
        return 0;
    } // IDX
    else if (ws[5] == "L0") {
        if (ws.size() < 10) throw PostError("MdL0 fields < 10!");
        mdhead->mdType = MD_Level0;
        mdhead->nBytes = sizeof(MdL0Core);
        MdL0Core * pmd = (MdL0Core*) mdhead;
        pmd->price = stod(ws[6]);          pmd->volume = stoll(ws[7]);
        pmd->accumTurnover = stod(ws[8]);  pmd->accumPosition = stod(ws[9]);
        return 0;
    } // L1
    else if (ws[5] == "L1") {
        if (ws.size() < 12) throw PostError("MdL1 fields < 12!");
        vector<string> topask = tokenize(ws[10], ':') ;
        if (topask.size() != 2) throw PostError("MdL1: bad top ask: %s", ws[10].c_str() );
        vector<string> topbid = tokenize(ws[11], ':') ;
        if (topbid.size() != 2) throw PostError("MdL1: bad top bid: %s", ws[11].c_str() );

        mdhead->mdType = MD_Level1;
        mdhead->nBytes = sizeof(MdL1Core);
        MdL1Core * mdL1 = (MdL1Core*) mdhead;
        mdL1->price = stod(ws[6]);          mdL1->volume = stoll(ws[7]);
        mdL1->accumTurnover = stod(ws[8]);  mdL1->accumPosition = stod(ws[9]);
        mdL1->askPrice = stod(topask[0]);   mdL1->askVolume = stoi(topask[1]);
        mdL1->bidPrice = stod(topbid[0]);   mdL1->bidVolume = stoi(topbid[1]);
        return 0;
    } // L1
    else if (ws[5] == "L5") {
        int level = 5;
        if (ws.size() < 10+2*level) throw PostError("MdL5 fields < %d!", (10+level*2));
        mdhead->mdType = MD_L5Book;
        mdhead->nBytes = sizeof(MdL5Book);
        MdL5Book * mdL5 = (MdL5Book*) mdhead;
        mdL5->price = stod(ws[6]);          mdL5->volume = stoll(ws[7]);
        mdL5->accumTurnover = stod(ws[8]);  mdL5->accumPosition = stod(ws[9]);
        for (int i=0; i<2*level; i++) {
            vector<string> ord = tokenize(ws[10+i], ':') ;
            if (ord.size() != 2) throw PostError("MdL5: bad book[%d]: %s", i, ws[10+i].c_str() );
            mdL5->bookPrice[i] = stod(ord[0]);
            mdL5->bookVolume[i] = stoi(ord[1]);
        }
        return 0;
    }
    else if (ws[5] == "L5X") {
        int level = 5;
        if (ws.size() < 10+2*level) throw PostError("MdL5 fields < %d!", (10+level*2));
        mdhead->mdType = MD_L5BookX;
        mdhead->nBytes = sizeof(MdL5BookX);
        MdL5BookX * pmd = (MdL5BookX*) mdhead;
        pmd->price = stod(ws[6]);          pmd->volume = stoll(ws[7]);
        pmd->accumTurnover = stod(ws[8]);  pmd->accumPosition = stod(ws[9]);
        for (int i=0; i<2*level; i++) {
            vector<string> ord = tokenize(ws[10+i], ':') ;
            if (ord.size() != 3) throw PostError("MdL5: bad x-book[%d]: %s", i, ws[10+i].c_str() );
            pmd->bookPrice[i] = stod(ord[0]); pmd->bookVolume[i] = stoi(ord[1]); pmd->bookCount[i] = stoi(ord[2]);
        }
        return 0;
    }
    else if (ws[5] == "L10") {
        int level = 10;
        if (ws.size() < 10+2*level) throw PostError("MdL10 fields < %d!", (10+level*2));
        mdhead->mdType = MD_L10Book;
        mdhead->nBytes = sizeof(MdL10Book);
        MdL10Book * mdL10 = (MdL10Book*) mdhead;
        mdL10->price = stod(ws[6]);          mdL10->volume = stoll(ws[7]);
        mdL10->accumTurnover = stod(ws[8]);  mdL10->accumPosition = stod(ws[9]);
        for (int i=0; i<2*level; i++) {
            vector<string> ord = tokenize(ws[10+i], ':') ;
            if (ord.size() != 2) throw PostError("MdL10: bad book[%d]: %s", i, ws[10+i].c_str() );
            mdL10->bookPrice[i] = stod(ord[0]);
            mdL10->bookVolume[i] = stoi(ord[1]);
        }
        return 0;
    }
    else if (ws[5] == "L16") {
        int level = 16;
        if (ws.size() < 10+2*level) throw PostError("MdL16 fields < %d!", (10+level*2));
        mdhead->mdType = MD_L16Book;
        mdhead->nBytes = sizeof(MdL16Book);
        MdL16Book * mdb = (MdL16Book*) mdhead;
        mdb->price = stod(ws[6]);          mdb->volume = stoll(ws[7]);
        mdb->accumTurnover = stod(ws[8]);  mdb->accumPosition = stod(ws[9]);
        for (int i=0; i<2*level; i++) {
            vector<string> ord = tokenize(ws[10+i], ':') ;
            if (ord.size() != 2) throw PostError("MdL16: bad book[%d]: %s", i, ws[10+i].c_str() );
            mdb->bookPrice[i] = stod(ord[0]);
            mdb->bookVolume[i] = stoi(ord[1]);
        }
        return 0;
    }
    else if (ws[5] == "TRANS") {
        if (ws.size() < 10) throw PostError("MdTrans fields < 10!");
        mdhead->mdType = MD_Trans;
        mdhead->nBytes = sizeof(MdTrans);
        MdTrans * pmd = (MdTrans *) mdhead;
        switch (ws[9][0]) {
        case 'B': pmd->dir = TRADE_Buy; break;
        case 'S': pmd->dir = TRADE_Sell; break;
        default: pmd->dir = TRADE_Unknown; break;
        }
        // debugging("MD: %s; %s", ws[6].c_str(), ws[7].c_str());
        pmd->price = stod(ws[6]);
        pmd->volume = stoi(ws[7]);
        if (ws[3] == "TDF") pmd->turnover = 10000*stod(ws[8]);
        if (ws.size() == 12) { pmd->orderIdBid = stoll(ws[10]); pmd->orderIdAsk = stoll(ws[11]); }
        return 0;
    }
    else if (ws[5] == "ORD") {
        if (ws.size() < 9) throw PostError("MdOrder fields < 9!");
        mdhead->mdType = MD_Order;
        mdhead->nBytes = sizeof(MdOrder);
        MdOrder * pmd = (MdOrder *) mdhead;
        switch (ws[8][0]) {
        case '0': { pmd->orderType = ORDER_LimitPrice; } break;
        case '1': { pmd->orderType = ORDER_AnyPrice; }   break;
        case 'U': { pmd->orderType = ORDER_Best; }       break;
        default : {
            // debugging("%s %s %s", mdhead->marketTime, mdhead->symbol, ws[8].c_str());
            return 801;
        }
        }
        switch (ws[8][1]) {
        case 'B': { pmd->dir = ORDER_Bid; } break;
        case 'S': { pmd->dir = ORDER_Ask; } break;
        case 'X': case 'C': { pmd->orderType = ORDER_Cancel; } break;
        default: return 801;
        }
        pmd->price = stod(ws[6]);
        pmd->volume = stoi(ws[7]);
        if (ws.size() > 9) pmd->orderSeqNum = stoll(ws[9]);
        return 0;
    }
    else if (ws[5] == "Q10") {
        if (ws.size() < 10) Error("MdOrdQ10 fields < 10! "+ line);
        mdhead->mdType = MD_OrdQ10;
        mdhead->nBytes = sizeof(MdOrdQ10);
        MdOrdQ10 * pmd = (MdOrdQ10 *) mdhead;
        vector<string> asklist = splitString(ws[7], ':');
        if (asklist.size() < 10) Error("MdOrdQ10 ask queue size < 10! "+ line);
        vector<string> bidlist = splitString(ws[9], ':');
        if (bidlist.size() < 10) Error("MdOrdQ10 bid queue size < 10! "+ line);
        pmd->askPrice = stod(ws[6]);
        for (int i=0; i<10; i++) { if (asklist[i] == "0")  break; pmd->vols[i] = stoi(asklist[i]); }
        pmd->bidPrice = stod(ws[8]);
        for (int i=0; i<10; i++) { if (bidlist[i] == "0")  break; pmd->vols[10+i] = stoi(bidlist[i]); }
        return 0;
    }
    else if (ws[5] == "QXX") {
        if (ws.size() < 10) Error("MdOrdQ fields < 10! " + line);
        mdhead->mdType = MD_OrdQ;
        MdOrdQ * pmd = (MdOrdQ *) mdhead;
        vector<string> asklist = splitString(ws[7], ':');
        pmd->askCount = stoi(asklist[0]);
        if (asklist.size() < 1+pmd->askCount) Error("MdOrdQ ask queue size not match: %d/%d! %s", asklist.size(), pmd->askCount, line.c_str());
        vector<string> bidlist = splitString(ws[9], ':');
        pmd->bidCount = stoi(bidlist[0]);
        if (bidlist.size() < 1+pmd->bidCount) Error("MdOrdQ bid queue size not match: %d/%d! %s", bidlist.size(), pmd->bidCount, line.c_str());
        pmd->askPrice = stod(ws[6]);
        for (int i=1; i<=pmd->askCount; i++) { pmd->vols[i-1] = stoi(asklist[i]); }
        pmd->bidPrice = stod(ws[8]);
        for (int i=1; i<=pmd->bidCount; i++) { pmd->vols[i-1+pmd->askCount] = stoi(bidlist[i]); }
        mdhead->nBytes = sizeof(MdOrdQ)+(pmd->askCount+pmd->bidCount)*sizeof(int);
        return 0;
    }
    else if (ws[5] == "O10") {
        if (ws.size() < 17) throw PostError("MdOrder10 fields < 17!");
        mdhead->mdType = MD_Order10;
        mdhead->nBytes = sizeof(MdOrder10);
        MdOrder10 * pmd = (MdOrder10 *) mdhead;
        pmd->price = stod(ws[6]);
        for (int i=0; i<10; i++) {
            if (ws[i+7] == "0")  break;
            pmd->orderVolume[i] = stoi(ws[i+7]);
        }
        return 0;
    }
    else if (ws[5] == "ORDQ") {
        if (ws.size() < 10) throw PostError("MdOrderQueue fields < 11!");
        mdhead->mdType = MD_OrderQueue;
        mdhead->nBytes = sizeof(MdOrderQueue);
        MdOrderQueue * pmd = (MdOrderQueue *) mdhead;
        switch (ws[9][0]) {
        case 'B': pmd->dir = TRADE_Buy; break;
        case 'S': pmd->dir = TRADE_Sell; break;
        default: pmd->dir = TRADE_Unknown; return 901;
        }
        pmd->price = stod(ws[6]);
        pmd->nOrders = stoi(ws[7]);
        for (int i=0; i<50; i++) { if (10+i >= ws.size()) break; pmd->vols[i] = stoi(ws[10+i]); }
        return 0;
    }
    else
        throw PostError("Unknown type %s", ws[5].c_str() );
    return -1 ;
};
