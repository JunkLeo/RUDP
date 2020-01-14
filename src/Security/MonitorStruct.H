#ifndef _MonitorStruct_H_
#define _MonitorStruct_H_

#include "Postman/PostUtility.H"
#include "Postman/PostSync.H"

const unsigned SHM_SLOT_NUM = 4000;

struct ControlParam {
    AgentId trader_ ;
    int riskLimit_, tier_, chi_, tipDepth_, offtop_;
    int freezeSeconds_, stopWin_, retryLimit_, uniOpenLimit_;
    int closing_, accSwitch_;
    unsigned bsMask_;
    int exFrontId_;
};

struct CommRebate {
    float commOpenByPrice, commOpenByVolume, commCloseByPrice, commCloseByVolume;
    float rebateRate, openRebateByPrice, openRebateByVolume;
};

// Change this structure will affect the correctness of webmonitor
struct SecurityStats {
    Uint64 updateCount;
    int secIndex; // 1 for Account, 2 for Security
    char symIndex[20];
    char tsLastTrade[16];
    char tsLastOrder[16];
    int nCancel, nDayCancel, nCross, nDayCross;
    int nOrder, nTrade, nDayTrade, orderedVolume;
    int nLong, nShort; // Total position (today + yesterday)
    double pnlMid, pnlFloat, commission, maxPnl, minPnl, fillRatio ;
    double dayPnl, dayCommission;
    double longPrice, shortPrice;
    int nPosition, nYdLong, nYdShort;
    float rebate, dayRebate;
    float unitPrice, openCommission, closeCommission;
    float commOpenByPrice, commOpenByVolume, commCloseByPrice, commCloseByVolume;
    char stdsym[12];
    int orderSysId;
    // char padding[12];
    float openRebateByPrice, openRebateByVolume, rebateRate;
};

struct AccountState {
    Uint64 updateCount;
    int secIndex; // 1 for Account, 2 for Security
    char symIndex[20];
    double balance, available, commission, margin, netPnl, cashin, cashout, preBalance;
    int dayOrder, dayTrade, dayCancel, dayPad;
    double dayPnl, dayComm, dayRebate, dayRepnl;
    char pad[120];
};

inline int get_monitor_session(const Timestamp & ts) {
    if (ts > Timestamp(6,0,0) && ts <= Timestamp(12,0,0)) return 1;
    if (ts > Timestamp(12,0,0) && ts <= Timestamp(18,0,0)) return 2;
    return 0;
};

inline std::string readParamsFromFile(const std::string & fpath, ControlParam & param) {
    std::ifstream ifs ;
    FileLock flock;
    bool locked = flock.readLock(fpath) ;
    std::string op = "";
    if (locked) {
        ifs.open(fpath.c_str()) ;
        if (ifs.fail()) {
            Error("TradingMonitor: open param file failed: " + fpath) ;
        }
        std::string line ;
        while( getline(ifs, line) ) {
            std::vector<std::string> ws = tokenizeWithStrip(line, ',', false) ;
            if (ws.size() < 2) continue ;
            if (ws[0] == "Operator") { op = ws[1]; continue; }
            int x = 0 ;
            try { x = stoi(ws[1]) ; }
            catch (...) { continue; }
            if (ws[0] == "RiskLimit") param.riskLimit_ = x ;
            else if (ws[0] == "Tier") param.tier_ = x ;
            else if (ws[0] == "Chi") param.chi_ = x ;
            else if (ws[0] == "TipDepth") param.tipDepth_ = x ;
            else if (ws[0] == "Offtop") param.offtop_ = x ;
            else if (ws[0] == "OrderFreeze") param.freezeSeconds_ = x ;
            else if (ws[0] == "StopWin") param.stopWin_ = x ;
            else if (ws[0] == "RetryLimit") param.retryLimit_ = x ;
            else if (ws[0] == "UniOpenLimit") param.uniOpenLimit_ = x ;
            else if (ws[0] == "Closing") param.closing_ = x ;
            else if (ws[0] == "AccSwitch") param.accSwitch_ = x ;
            else if (ws[0] == "BuySellSwitch") param.bsMask_ = x ;
            else if (ws[0] == "Front") param.exFrontId_ = x ;
        }
        ifs.close() ;
        flock.unlock(fpath) ;
    }
    return op;
};

inline std::string readParams(const std::string & stdsym, ControlParam & param) {
    std::string fpath = ptu::get_env_variable("TRADING_PATH") + "/StratConf/ctl-" + stdsym + ".csv";
    std::string op  = readParamsFromFile(fpath, param);
    return op;
};

inline void writeParamsToFile(const std::string & fpath, const ControlParam & param, const std::string & op="console") {
    FileLock flock;
    std::ofstream ofs ;
    flock.writeLock(fpath);
    ofs.open(fpath.c_str(), std::ofstream::out) ;
    ofs << "RiskLimit," <<  param.riskLimit_ << std::endl ;
    ofs << "Tier," <<  param.tier_ << std::endl ;
    ofs << "Chi," <<  param.chi_ << std::endl ;
    ofs << "TipDepth," <<  param.tipDepth_ << std::endl ;
    ofs << "Offtop," <<  param.offtop_ << std::endl ;
    ofs << "OrderFreeze," <<  param.freezeSeconds_ << std::endl ;
    ofs << "StopWin," <<  param.stopWin_ << std::endl ;
    ofs << "RetryLimit," <<  param.retryLimit_ << std::endl ;
    ofs << "UniOpenLimit," <<  param.uniOpenLimit_ << std::endl ;
    ofs << "Closing," <<  param.closing_ << std::endl ;
    ofs << "AccSwitch," <<  param.accSwitch_ << std::endl ;
    ofs << "BuySellSwitch," << param.bsMask_ << std::endl ;
    ofs << "Front," << param.exFrontId_ << std::endl ;
    ofs << "Operator," << op << std::endl ;
    ofs.close() ;
    flock.unlock(fpath) ;
    return;
}

inline void writeParams(const std::string & stdsym, const ControlParam & param) {
    std::string fpath = ptu::get_env_variable("TRADING_PATH") + "/StratConf/ctl-" + stdsym + ".csv";
    writeParamsToFile(fpath, param);
    return;
}

#endif
