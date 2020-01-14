#include "Security/SecDef.H"

#include <string>
#include <list>
#include <vector>
#include <set>
#include <utility>
#include <iostream>
#include <assert.h>
#include <cstdio>

using namespace std;

map<string, string> SecDef::ExSymbolMap_ = map<string, string>();

int SecDef::GetSessionId() {
    int ssId = 0;
    Timestamp now = Timestamp::LocalIndayTime();
    if (Timestamp(6,0,0) <= now && now < Timestamp(12,0,0)) ssId = 1;
    else if (Timestamp(12,0,0) <= now && now < Timestamp(18,0,0)) ssId = 2;
    return ssId;
};

Datestamp SecDef::GetTradingDay() {
    Datestamp d = Datestamp::Today();
    int yy = d.year();
    string fpath = ptu::get_env_variable("TRADING_PATH") + "/Symbols/closeday_" + to_string(yy) + ".csv";
    ifstream ifs;
    ifs.open(fpath.c_str(), ifstream::in);
    string line;
    getline(ifs, line);
    vector<string> ws = tokenizeWithStrip(line, ',', false);
    std::set<int> closeDays;
    for (const string & s : ws) closeDays.insert(yy*10000+stoi(s));
    int ordin = d.ordinal();
    if (Timestamp::LocalIndayTime() > Timestamp(18,0,0)) ordin += 1;
    Datestamp td = Datestamp::FromOrdinal(ordin);
    while (td.weekday() == 6 || td.weekday() == 0 || closeDays.find(td.toIntDate()) != closeDays.end()) {
        ordin += 1;
        td = Datestamp::FromOrdinal(ordin);
    }
    return td;
};

SecDef::SecDef(const std::string & gid, const std::string & sym, const std::string & nm, int idx, int month
               ,const std::string & sunit, const std::string & direction,const Datestamp & date
               , ExchangeId ex, SecType st, DataSource src, int level, const std::string & canonicalId
               , double tick, double invTick, double commRate, double comm, double fxrate
               , double lev, double rebate, double unit, int offset,double strikepr,int maturity, const std::vector<SessionType> & times)
    : genericId_(gid), symbol_(sym), name_(nm), index_(idx), month_(month),sunit_(sunit),direction_(direction),date_(date)
    , exchangeId_(ex), secType_(st), dataSource_(src), dataLevel_(level), canonicalId_(canonicalId)
    , tickSize_(tick), inverseTickSize_(invTick), commissionRate_(commRate), fixedCommission_(comm)
    , fxRate_(fxrate), leverage_(lev), rebate_(rebate), unitPrice_(unit), timeOffset_(offset),strikepr_(strikepr),maturity_(maturity)
    , trdSessions_(times) {};

SecDef::SecDef(const SecDef & sd)
    : genericId_(sd.genericId_), symbol_(sd.symbol_), name_(sd.name_), index_(sd.index_), month_(sd.month_), sunit_(sd.sunit_),direction_(sd.direction_), date_(sd.date_)
    , exchangeId_(sd.exchangeId_), secType_(sd.secType_), dataSource_(sd.dataSource_), dataLevel_(sd.dataLevel_)
    , fxRate_(sd.fxRate_), tickSize_(sd.tickSize_), inverseTickSize_(sd.inverseTickSize_)
    , commissionRate_(sd.commissionRate_), fixedCommission_(sd.fixedCommission_)
    , leverage_(sd.leverage_), rebate_(sd.rebate_), unitPrice_(sd.unitPrice_), timeOffset_(sd.timeOffset_),strikepr_(sd.strikepr_),maturity_(sd.maturity_)
    , trdSessions_(sd.trdSessions_) {} ;

vector<SessionType> getTradingTimeByExchange(ExchangeId ex) {
    vector<SessionType> tt ;
    switch (ex) {
    case EX_CNCFF: {
        tt.push_back( make_pair(Timestamp(9, 15, 0), Timestamp(11, 30, 0)) ) ;
        tt.push_back( make_pair(Timestamp(13, 0, 0), Timestamp(15, 15, 0)) );
    } break ;
    case EX_CNSHF: case EX_CNDLF: case EX_CNZZF: case EX_CNINE: {
        tt.push_back( make_pair(Timestamp(9, 0, 0), Timestamp(10, 15, 0)) );
        tt.push_back( make_pair(Timestamp(10, 30, 0), Timestamp(11, 30, 0)) );
        tt.push_back( make_pair(Timestamp(13, 30, 0), Timestamp(15, 0, 0)) );
    } break ;
    case EX_CNSHS: case EX_CNSZS: case EX_CNSHO: case EX_CNSZO: {
        tt.push_back( make_pair(Timestamp(9, 30, 0), Timestamp(11, 30, 0)) );
        tt.push_back( make_pair(Timestamp(13, 0, 0), Timestamp(15, 0, 0)) );
    } break;
    default:
        tt.push_back( make_pair(Timestamp(9, 0, 0), Timestamp(11, 30, 0)) );
        tt.push_back( make_pair(Timestamp(13, 0, 0), Timestamp(15, 0, 0)) );
    }
    return vector<SessionType>(tt) ;
} ;

bool SecDef::IsValidGid(const std::string & gid)
{ GidFields gf=GetGidFields(gid); return (gf.name_ != "" && gf.exchange_ != EX_Unknown); };

// Future: Ex, Name, FutureContract[, Level, Source]
// Stock: Ex, Name[, Level, Source, Type]
GidFields SecDef::GetGidFields(const string & gid) {
    vector<string> id_ex = tokenizeWithStrip(gid, '.', false) ; // do NOT inclue empty token
    if (2 != id_ex.size()) {
        Warn( "<SecDef::%s> Bad GID %s with insufficient fields!", __func__, gid.c_str() );
        return GidFields();
    }
    vector<string> gidTokens = tokenizeWithStrip(id_ex[0], '_', false) ;
    gidTokens.insert(gidTokens.begin(), id_ex[1]) ;
    //if(gid[0] == "_" ) ExchangeId ex = exchangeIdFromString(gidTokens[1]) ;
    ExchangeId ex = exchangeIdFromString(gidTokens[0]) ;

    if (EX_Unknown == ex) {
        Warn( "<SecDef::%s> BAD GID % with unknown exchange!", __func__, gid.c_str() );
        return GidFields();
    }

    int minGidFields = 3;
    if (EX_CNSHS == ex || EX_CNSZS == ex || EX_CNSGE == ex) minGidFields = 2;
    if (gidTokens.size() < minGidFields) {
        Warn( "<SecDef::%s> Bad GID [%s] with insufficient fields!", __func__, gid.c_str() );
        return GidFields();
    }
    string name = gidTokens[1] ;
    int level = 0 ;
    DataSource src = SRC_X;
    switch (ex) {
    case EX_CNCFF: case EX_CNSHF: case EX_CNDLF: case EX_CNZZF: case EX_CNINE:
    case EX_COMEX: case EX_NYMEX: case EX_USCBT: case EX_USICE:
    case EX_UKLME: case EX_JPTCE: case EX_SGSGX: {
        int idx = 0;
        idx = stoi(gidTokens[2]);
        if (gidTokens.size() > 3){
            level = stoi(gidTokens[3]) ;
            if (gidTokens.size() > 4) src = dataSourceFromString(gidTokens[4]) ;
        }
        return GidFields(name, ex, src, level, idx) ;
    } break ;
    case EX_CNSHS: case EX_CNSZS: case EX_CNSGE:{
        level = 0;
        if (gidTokens.size() > 2) level = stoi(gidTokens[2]);
        if (gidTokens.size() > 3) src = dataSourceFromString(gidTokens[3]) ;
        // debugging("SecDef: %s L%d", name.c_str(), level);
        if (gidTokens.size() > 4)

            return GidFields(name, ex, src, level, gidTokens[4]) ;
        else
            return GidFields(name, ex, src, level, "") ;
    } break ;
    case EX_CNSHO: case EX_CNSZO: case EX_CNCFO: case EX_CNSFO: case EX_CNDFO: case EX_CNZFO:{
         string name = "_"+gidTokens[1] ;
         string direction = "S";
         int month = 0;
         string sunit = "M";
         int idx = 0;
         if(gidTokens.size() == 6){
            level =  stoi(gidTokens[4]);
            sunit = gidTokens[5];
            idx = gidTokens[2][1] - '0';
            direction[0] = gidTokens[2][0];
            month = stoi(gidTokens[3]);
         }
         return GidFields(name, ex, src, level, idx, direction ,month, sunit) ;
      }break;
    default :
        Warn("<SecDef::%s> Bad GID: %s", __func__, gid.c_str()) ;
        return GidFields() ;
    }
} ;

string getSecuritySymbol(const string & name, int idx, ExchangeId ex, Datestamp d) {
    if (SecDef::ExSymbolMap_.size() == 0) SecDef::LoadExSymbolMap();
    if (ex == EX_UKLME) {
        char sym[8];
        sprintf(sym, "%2s%02d%02d", SecDef::ExSymbolMap_[name].c_str(), d.year() % 100, d.month() % 100);
        return string(sym);
    }
    string exName = secTagToString(ex) ;
    char symFile[256];
    snprintf(symFile, 255, "%s/Symbols/%s.%s/%04d/%s-%04d%02d%02d.sym"
             , ptu::get_env_variable("TRADING_PATH").c_str()
             , name.c_str(), exName.c_str(), d.year(), name.c_str(), d.year(), d.month(), d.day() );
    symFile[255] = '\0';
    if (! ptu::exists(symFile)) {
        Warn("<SecDef::%s> Cannot open symbol file %s", __func__, symFile) ;
        return string(name);
    }
    ifstream sfs(symFile, ifstream::in) ;
    string line ;
    getline(sfs, line) ;
    vector<string> symbols = tokenizeWithStrip(line, ',', false) ; // do Not include empty token
    sfs.close() ;
    if (0 == symbols.size()) {
        Warn("<SecDef::%s> No symbols found in %s", __func__, symFile) ;
        return string(name) ;
    }
    else if (symbols.size() <= idx) {
        Warn("<SecDef::%s> No symbol of index %d is found for %s.%s", __func__, idx, name.c_str(), exName.c_str() ) ;
        return string(name) ;
    }
    else return string(symbols[idx]) ;
}

std::string getOptionSymbolUnit(const string & name, int idx, ExchangeId ex, Datestamp d, const string & direction,const string & sunit, int month) {
    string exName = secTagToString(ex) ;
    char symFile[256];
    snprintf(symFile, 255, "%s/Symbols/%s.%s/%04d/%d.%s/%s.%s/%04d%02d%02d.sym"
             , ptu::get_env_variable("TRADING_PATH").c_str()
             , name.c_str(), exName.c_str(), d.year(), month, exName.c_str(), sunit.c_str(),  exName.c_str(), d.year(), d.month(), d.day());
    symFile[255] = '\0';
    if (! ptu::exists(symFile)) {
        Error("<SecDef::%s> Cannot open symbol file %s", __func__, symFile) ;
        return name;
    }
    ifstream sfs(symFile, ifstream::in) ;
    string line ;
    vector <string> symunits;
    int linecount = 0;
    while (! sfs.eof()) {
        getline(sfs, line) ;
        line = stripString(line);
        linecount += 1;
        if (line == "") continue;
        if (line[0] == '#') continue;
        vector<string> ws = splitString(line, ',');
        if (ws.size() < 3) { Warn("<SecDef::%s> Line #%d in %s is BAD: no sufficient fields!", __func__, linecount, symFile); continue; }
        try {
            symunits.push_back(line);
            double unitpr = stoi(ws[1]);
            double strikepr = stod(ws[3]);
            int maturity = stoi(ws[4]);
        } catch (std::exception &) {
            Warn("<SecDef::%s> Line #%d in %s is BAD: (sym:%s, unitpr:%s)!", __func__, linecount, symFile, ws[0].c_str(), ws[3].c_str());
        }
    }
    sfs.close() ;
    if (0 == symunits.size()) {
        Error("<SecDef::%s> No symbols found in %s", __func__, symFile);  return name;
    }
    else if (symunits.size() <= idx) {
        Error("<SecDef::%s> No symbol of index %d is found for %s.%s", __func__, idx, name.c_str(), exName.c_str() ) ;
        return  name;
    }
    if(direction[0] == 'S'){return symunits[idx];}
    else {return symunits[symunits.size()-1-idx];}
}

struct SdValues { double tickPrice, unitPrice, commRate, fixedComm, leverage, rebate; unsigned short sectype; } ;

SdValues readFutureDefinition(const string & secName, int idx, const string & sdefPath, vector<SessionType> & sessions) {
    if (!ptu::exists(sdefPath)) Error("<SecDef::%s> cannot find security definition file: %s", __func__, sdefPath.c_str()) ;

    SdValues sdval = { -1, -1, -1, -1, -1, 0, SEC_Future } ;

    ifstream sdfs(sdefPath.c_str(), ifstream::in) ;
    string line, defLine="", secLine="" ;
    int lineCount = 0;
    while ( !sdfs.eof() ) {
        lineCount += 1;
        getline(sdfs, line) ;
        vector<string> contents = tokenize(line, '#') ;
        if (contents.size() <= 0) continue ;
        vector<string> fields = tokenize(contents[0], ',') ;
        if (stripString(fields[0]) == secName)  defLine = contents[0];
        else if (stripString(fields[0]) == secName+"1")  secLine = contents[0];
    };

    if (defLine == "") Error( "<SecDef::%s> Find no definition for %s in file %s", __func__, secName.c_str(), sdefPath.c_str());

    vector<string> fields;
    if (idx == 0 || secLine == "") fields = tokenize(defLine, ',');
    else fields = tokenize(secLine, ',');
    int findex = 1 ;
    try {
        sdval.tickPrice = stod(stripString(fields[++findex])) ;
        sdval.unitPrice = stod(stripString(fields[++findex])) ;

        double commission = stod(stripString(fields[++findex], '%')) ;
        if (stripString(fields[4]).find("%%") != string::npos)
            sdval.commRate = commission / 10000 ;
        else if (stripString(fields[4]).find("%") != string::npos)
            sdval.commRate = commission / 100 ;
        else
            sdval.commRate = commission;

        sdval.fixedComm = stod(stripString(fields[++findex])) ;
        sdval.leverage = stod(stripString(fields[++findex])) ;
        sdval.rebate = stod(stripString(fields[++findex])) ;
    }
    catch (std::exception &) {
        Error( "<SecDef::%s> reads bad field [L%d, %d]: %s, %s", __func__, lineCount, findex, secName.c_str(), sdefPath.c_str() );
    }

    for(int idx = 8; idx < fields.size(); idx++) {
        vector<string> tss = tokenize(stripString(fields[idx]), '-') ;
        if (tss.size() != 2) continue ;
        Timestamp ssStart = Timestamp::FromString(stripString(tss[0])) ;
        Timestamp ssEnd = Timestamp::FromString(stripString(tss[1])) ;
        sessions.push_back( make_pair(ssStart, ssEnd) );
    }
    sdfs.close();
    return sdval;
};

SdValues readStockDefinition(const string & secName, const string & sdefPath, vector<SessionType> & sessions) {
    if (!ptu::exists(sdefPath)) Error("<SecDef::%s> cannot find security definition file: %s", __func__, sdefPath.c_str()) ;

    SdValues sdval = { 0.01, 100, 0, 0, 1, 0, SEC_Stock } ;

    ifstream sdfs(sdefPath.c_str(), ifstream::in) ;
    string line, defLine="", secLine="" ;
    int lineCount = 0;
    while ( !sdfs.eof() ) {
        lineCount += 1;
        getline(sdfs, line) ;
        vector<string> contents = tokenize(line, '#') ;
        if (contents.size() <= 0) continue ;
        string content = stripString(contents[0]);
        vector<string> fields = tokenize(content, ',') ;
        if (fields.size() <= 0) continue;
        if      (stripString(fields[0]) == secName)  { defLine = content; break; }
        else if (stripString(fields[0]) == "stock")  { defLine = content; }
    };

    if (defLine == "") return sdval;

    vector<string> fields = tokenize(defLine, ',');
    int findex = 1 ;
    if       (stripString(fields[findex]) == "index") sdval.sectype = SEC_Index;
    else if  (stripString(fields[findex]) == "etf")   sdval.sectype = SEC_ETF;
    else if  (stripString(fields[findex]) == "option") sdval.sectype = SEC_Option;
    try {
        sdval.tickPrice = stod(stripString(fields[++findex])) ;
        sdval.unitPrice = stod(stripString(fields[++findex])) ;

        double commission = stod(stripString(fields[++findex], '%')) ;
        if (stripString(fields[4]).find("%%") != string::npos)
            sdval.commRate = commission / 10000 ;
        else if (stripString(fields[4]).find("%") != string::npos)
            sdval.commRate = commission / 100 ;
        else
            sdval.commRate = commission;

        sdval.fixedComm = stod(stripString(fields[++findex])) ;
        sdval.leverage = stod(stripString(fields[++findex])) ;
        sdval.rebate = stod(stripString(fields[++findex])) ;
    }
    catch (std::exception &) {
        Error( "<SecDef::%s> reads bad field [L%d, %d]: %s, %s", __func__, lineCount, findex, secName.c_str(), sdefPath.c_str() );
    }

    for(int idx = 8; idx < fields.size(); idx++) {
        vector<string> tss = tokenize(stripString(fields[idx]), '-') ;
        if (tss.size() != 2) continue ;
        Timestamp ssStart = Timestamp::FromString(stripString(tss[0])) ;
        Timestamp ssEnd = Timestamp::FromString(stripString(tss[1])) ;
        sessions.push_back( make_pair(ssStart, ssEnd) );
    }
    sdfs.close();
    return sdval;
} ;

void SecDef::LoadExSymbolMap() {
    string fpath = ptu::get_env_variable("TRADING_PATH") + "/Symbols/ex-symbols.csv";
    if (! ptu::exists(fpath) ) {
        Warn("<SecDef::%s> Ex symbol map file %s does not exist! Will not generate any ex symbol.", __func__, fpath.c_str());
        return ;
    }
    ifstream f(fpath.c_str(), ifstream::in);
    string line ;
    while (! f.eof()) {
        getline(f, line) ;
        if (stripString(line).size() == 0 || stripString(line)[0] == '#') continue;;
        vector<string> ws = splitString(line, ',');
        if (ws.size() < 4 || stripString(ws[0]) == "") continue;
        if (stripString(ws[1]) == "" || stripString(ws[1])[0] == '.') continue;
        vector<string> fs = splitString(stripString(ws[1]), '.');
        // if (stripString(ws[2]) == "") continue;
        ExSymbolMap_[fs[0]] = stripString(ws[0]);
    }
    f.close() ;
    return;
};

SecDef SecDef::GetSecDef(const string & gid, const Datestamp & date, bool debug) {
    try {
        GidFields gidFields = GetGidFields(gid) ;
        if (gidFields.name_ == "" || gidFields.exchange_ == EX_Unknown) Error("SecDef Bad GID: " + gid);

        string nm = gidFields.name_ ;
        int idx = gidFields.index_;
        ExchangeId ex = gidFields.exchange_ ;
        int level = gidFields.level_ ;
        DataSource src = gidFields.source_ ;

        // debugging("GidFields: %s L%d", nm.c_str(), level);

        string exSecDefPath = FindSecDefFile(ex, date);
        SecType st = SEC_Stock ;
        vector<SessionType> sessions ;
        SdValues sdval;

        switch (ex) {
        case EX_CNCFF: case EX_CNSHF: case EX_CNDLF: case EX_CNZZF: case EX_CNINE:
        case EX_COMEX: case EX_NYMEX: case EX_USCBT: case EX_USICE:
        case EX_UKLME: case EX_JPTCE: case EX_SGSGX: {
            sdval = readFutureDefinition(nm, idx, exSecDefPath, sessions);
            st = SEC_Future ;
        } break ;
        case EX_CNSHS: case EX_CNSZS: case EX_CNSHO: case EX_CNSZO: {
            sdval = readStockDefinition(nm, exSecDefPath, sessions) ;
            // if (sdval.unitPrice < PEPS) st = SEC_Index;
            st = sdval.sectype;
        } break ;
        case EX_CNSGE: {
            st = SEC_Gold;
            sdval = readFutureDefinition(nm, 0, exSecDefPath, sessions);
        } break;
        default:
            Error( "<SecDef::%s> %s has unknown exchange: ", __func__, gid.c_str() );
        }

        double tick = sdval.tickPrice;
        if (tick < PEPS && st != SEC_Index)
            Error( "<SecDef::%s> %s with invalid tick size %.4f!", __func__, gid.c_str(), tick );
        double invTick = 1 / tick ;
        double unit = sdval.unitPrice;
        // if (unit < PEPS && st != SEC_Index)
        //     Error( "<SecDef::%s> %s with invalid unitprice: %.4f!", __func__, gid.c_str(), unit );
        double commRate = (sdval.commRate < 0 ? 0 : sdval.commRate);
        double fixComm = (sdval.fixedComm < 0 ? 0 : sdval.fixedComm);
        double rebate = sdval.rebate;
        double fxrate=1;
        double strikepr = 0;
        int maturity = 0;
        if (sessions.size() == 0)  sessions = getTradingTimeByExchange(ex) ;

        string sym = nm;
        string canonicalId = nm;
        double leverage = 1;
        int offset=0;
        switch (ex) {
        case EX_CNCFF: case EX_CNSHF: case EX_CNDLF: case EX_CNZZF: case EX_CNINE:
        case EX_COMEX: case EX_NYMEX: case EX_USCBT: case EX_USICE:
        case EX_UKLME: case EX_JPTCE: case EX_SGSGX: {
            sym = getSecuritySymbol(nm, gidFields.index_, ex, date) ;
            canonicalId += "_" + to_string(gidFields.index_) + "." + secTagToString(ex);
            leverage = sdval.leverage;
            if (leverage <= 0 || leverage > 1) leverage = 1 ;
        } break;
        case EX_CNSHS: case EX_CNSZS: case EX_CNSGE: {
            sym = nm ;
            canonicalId += "." + secTagToString(ex);
        } break;
        case EX_CNSHO: case EX_CNSZO:{
            std::string x = getOptionSymbolUnit(nm, gidFields.index_, ex, date, gidFields.direction_, gidFields.sunit_,gidFields.month_);
            vector<string> ws = splitString(x, ',');
            sym = ws[0];
            unit = stoi(ws[1]);
            strikepr = stod(ws[3]);
            maturity = stoi(ws[4]);
            canonicalId += "_" + to_string(gidFields.index_) + "." + secTagToString(ex);
            leverage = sdval.leverage;
            return SecDef(gid, sym, nm, idx, gidFields.month_,gidFields.sunit_,gidFields.direction_,date, ex, st, src, level, canonicalId
                         , tick, invTick, commRate, fixComm, fxrate
                         , leverage, rebate, unit, offset, strikepr,maturity ,sessions);

         }break;
        default:
            Error( "<SecDef::%s> %s with unknown exchange", __func__, gid.c_str() );
        }
        return SecDef(gid, sym, nm, idx,0, "M","S",date, ex, st, src, level, canonicalId
                      , tick, invTick, commRate, fixComm, fxrate
                      , leverage, rebate, unit, offset, strikepr,maturity,sessions);
    }
    catch (...) { Error("<SecDef::%s> Bad GID: %s", __func__, gid.c_str()); }
} ;

SessionType SecDef::getTradingSession(const Timestamp & ts) const {
    for (int i=0; i<trdSessions_.size(); i++)
        if (ts < trdSessions_[i].second) return trdSessions_[i];
    return make_pair(Timestamp(), Timestamp());
};

bool SecDef::isTradingClosingTime(const Timestamp & ts, const Timestamp & closeTime) const {
    for (int i=0; i<trdSessions_.size(); i++)
        if (ts >= trdSessions_[i].first && ts + closeTime <= trdSessions_[i].second) return false ;
    return true ;
};

bool SecDef::isSymbolOK() const {
    switch (exchangeId_) {;
    case EX_CNCFF: case EX_CNSHF: case EX_CNDLF: case EX_CNZZF: case EX_CNINE:
    case EX_COMEX: case EX_NYMEX: case EX_USCBT: case EX_USICE:
    case EX_UKLME: case EX_JPTCE: case EX_SGSGX:
        return (symbol_.size() > name_.size());
    case EX_CNSHS: case EX_CNSZS: case EX_CNSGE:
        return (symbol_ == name_);
    case EX_CNSHO: case EX_CNSZO:return (stoi(symbol_) /10000 == 1000);
    default:
        return false;
    }
};

string SecDef::FindSecDefFile(ExchangeId ex, const Datestamp & d) {
    string exName = secTagToString(ex);
    string dstr = d.toString();
    string sdPath = ptu::get_env_variable("TRADING_PATH") + "/SecDef/";
    vector<string> fns = ptu::get_files_in_directory(sdPath);
    list<string> sdDays;
    for (const string & fn: fns) {
        if (stringEndsWith(fn, ".csv") && stringStartsWith(fn, exName)) {
            // cout << fn << endl;
            vector<string> ws = splitString(fn, '.');
            if (ws.size() == 3) sdDays.push_back(ws[1]);
        }
    }
    sdDays.sort();
    sdDays.reverse();
    for (const string & ds: sdDays) {
        if (dstr >= ds) return (sdPath+exName+"."+ds+".csv");
    }
    return (sdPath+exName+".csv");
};
