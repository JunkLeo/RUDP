#ifndef _PostDefinitions_H_
#define _PostDefinitions_H_

#include <string>

#define KILO 1024
#define MEGA 1048576
#define GIGA 1073741824

#define K_SIZE(x) ( (x) * 1024 )
#define M_SIZE(x) ( (x) * 1048576 )
#define G_SIZE(x) ( (x) * 1073741824 )

// #define boostfor BOOST_FOREACH
#define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))

#define CACHELINE_SIZE 64

// deprecated #define TIMESCALE 10 //current base time is decimillisec, strategy, signal and trading configuration use millisecond, therefore a scale of 10 is used when constructing Timestamp
#define MIN_DOUBLE_DIFF 1e-10
#define EPS 1e-10 //minimum difference
#define PEPS 1e-10 //minimum price difference
// #define PEPS 1e-12 //minimum price difference
#define SUPSIZE 1e+6 //trade size upper bound
// #define SUPSIZE 1e+10 //trade size upper bound

typedef unsigned long long WORD8;
const unsigned WORD8_SHIFT = 3;
const unsigned WORD8_SIZE = (1 << WORD8_SHIFT);

typedef long long Int64 ;
typedef unsigned long long Uint64 ;
typedef unsigned short MessageType;
typedef unsigned short AgentId;
typedef unsigned short AgentType ;
typedef unsigned short AgentState ;

typedef struct sockaddr_un SocketStruct ;
typedef struct socksddr SocketAddr ;

const Uint64 TIMEZONE = (28800 << 32) ; // 3600 * 8hours

const int LOG_CRITICAL = 0;
const int LOG_ERROR = 1;
const int LOG_WARN = 2;
const int LOG_INFO = 5;
const int LOG_DEBUG = 9;

const unsigned MINUTE_PER_DAY = 1440 ;
const unsigned SEC_PER_DAY   = 86400 ;
const unsigned MS_PER_DAY    = 86400000 ;
const unsigned DMS_PER_DAY   = 864000000 ;
const unsigned MS_IN_HOUR    = 3600000 ;
const unsigned MS_IN_MINUTE  = 60000 ;
const unsigned MS_IN_SEC     = 1000 ;
const unsigned DMS_IN_HOUR   = 36000000 ;
const unsigned DMS_IN_MINUTE = 600000 ;
const unsigned DMS_IN_SEC    = 10000 ;
const unsigned DMS_IN_MS     = 10 ;
const unsigned US_IN_SEC     = 1000000 ;
const unsigned US_IN_MS      = 1000 ;
const unsigned US_IN_DMS     = 100 ;
const unsigned NS_IN_SEC     = 1000000000 ;
const unsigned NS_IN_MS      = 1000000 ;
const unsigned NS_IN_DMS     = 100000 ;
const unsigned NS_IN_US      = 1000 ;

// All values smaller than 0x1000 are preserved for PostMan system
// User const values should be lager than 0x1000
const unsigned short Tag_Null = 0;
const AgentType AGENT_X = 0x010F ;  // Normal agent of no particular type 
const AgentType AGENT_Root = 0x0100 ;
const AgentType AGENT_Timer = 0x0101 ;
const AgentType AGENT_Control = 0x0102 ;
const AgentType AGENT_PostLogger = 0x0103 ;
const AgentType AGENT_ExtReceive = 0x0104 ;
const AgentType AGENT_ExtSend = 0x0105 ;
const AgentType AGENT_NetReceive = 0x0106 ;
const AgentType AGENT_NetSend = 0x0107 ;
const AgentType AGENT_Logger = 0x0108 ;

const AgentType AGENT_Test = 0x01FF ;

const AgentState AGENT_Ready = 0x0201 ; // Agent is in use and is ready for running 
const AgentState AGENT_Inactive = 0x0202 ; // Agent not in use 
const AgentState AGENT_Running = 0x0203 ; // Agent thread is running 

const unsigned short MSG_NULL = 0x0300 ;
const unsigned short MSG_HeartBeat = 0x0301 ;
const unsigned short MSG_Stop = 0x0302 ;
const unsigned short MSG_STOP = 0x0302 ;
const unsigned short MSG_Halt = 0x0302 ; // Same as MSG_STOP
const unsigned short MSG_HALT = 0x0302 ; // Same as MSG_STOP
const unsigned short MSG_Timeup = 0x0303 ; 
const unsigned short MSG_TIMEUP = 0x0303 ; 
const unsigned short MSG_Control = 0x0304 ;
const unsigned short MSG_CONTROL = 0x0304 ;
const unsigned short MSG_Test = 0x0305 ;
const unsigned short MSG_String = 0x0306 ;
const unsigned short MSG_Object = 0x0307 ;
const unsigned short MSG_Probe = 0x030A ;

const unsigned short MSG_Subscribe = 0x0311 ;
const unsigned short MSG_Unsubscribe = 0x0312 ;
const unsigned short MSG_ExtSubscribe = 0x0313 ;
const unsigned short MSG_ExtUnsubscribe = 0x0314 ;
const unsigned short MSG_NetSubscribe = 0x0315 ;
const unsigned short MSG_NetUnsubscribe = 0x0316 ;
const unsigned short MSG_BufferSize = 0x0317;
const unsigned short MSG_RegisterDeadline = 0x0318;

const unsigned short MSG_MdSubscribe = 0x0321 ; // Application to data factory
const unsigned short MSG_MdUnsubscribe = 0x0322 ; // Application to data factory
const unsigned short MSG_DriverSubscribe = 0x0323 ; // data factory  to driver
const unsigned short MSG_Log = 0x0325 ;
const unsigned short MSG_Driver = 0x0328 ;

const unsigned short MSG_MDS = 0x0331 ; 
const unsigned short MSG_SecurityTime = 0x0332 ; 
const unsigned short MSG_Signal = 0x0333 ; 
const unsigned short MSG_MarketData = 0x0334 ; 

const unsigned short MSG_Request = 0x0340;
const unsigned short MSG_Order = 0x0341 ;
const unsigned short MSG_TraderAck = 0x0342 ;
const unsigned short MSG_BrokerAck = 0x0343 ;
const unsigned short MSG_DriverApi = 0x0344 ;

inline bool isValidMsgType(unsigned short x) 
{ return (0x0300 <= x && x <= 0x03FF); };

inline std::string postTagToString(unsigned short x) {
    switch (x) {
    case AGENT_X : return std::string("AGENT_X") ; // Normal agent of no particular type 
    case AGENT_Root : return std::string("AGENT_Root") ;
    case AGENT_Timer : return std::string("AGENT_Timer") ;
    case AGENT_Control : return std::string("AGENT_Control") ;
    case AGENT_Logger : return std::string("AGENT_Logger") ;
    case AGENT_PostLogger : return std::string("AGENT_PostLogger") ;
    case AGENT_ExtReceive : return std::string("AGENT_ExtReceive") ;
    case AGENT_ExtSend : return std::string("AGENT_ExtSend") ;
    case AGENT_NetReceive : return std::string("AGENT_NetReceive") ;
    case AGENT_NetSend : return std::string("AGENT_NetSend") ;
    case AGENT_Test : return std::string("AGENT_Test") ;

    case AGENT_Ready : return std::string("AGENT_Ready") ;// Agent is in use and is ready for running 
    case AGENT_Inactive : return std::string("AGENT_Inactive") ;// Agent not in use (typically SecurityAgent or DataDriver without listeners)
    case AGENT_Running : return std::string("AGENT_Running") ;// Agent thread is running 

    case MSG_NULL : return std::string("MSG_Null") ;
    case MSG_HeartBeat : return std::string("MSG_HeartBeat") ;
    case MSG_Stop : return std::string("MSG_Stop") ;
    case MSG_Timeup : return std::string("MSG_Timeup") ;
    case MSG_Control : return std::string("MSG_Control") ;
    case MSG_Test : return std::string("MSG_Test") ;
    case MSG_String : return std::string("MSG_String") ;
    case MSG_Object : return std::string("MSG_Object") ;
    case MSG_Probe : return std::string("MSG_Probe") ;

    case MSG_Subscribe : return std::string("MSG_Subscribe") ;
    case MSG_Unsubscribe : return std::string("MSG_Unsubscribe") ;
    case MSG_ExtSubscribe : return std::string("MSG_ExtSubscribe") ;
    case MSG_ExtUnsubscribe : return std::string("MSG_ExtUnsubscribe") ;
    case MSG_NetSubscribe : return std::string("MSG_NetSubscribe") ;
    case MSG_NetUnsubscribe : return std::string("MSG_NetUnsubscribe") ;
    case MSG_BufferSize : return std::string("MSG_BufferSize") ;
    case MSG_RegisterDeadline : return std::string("MSG_RegisterDeadline") ;

    case MSG_MdSubscribe : return std::string("MSG_MdSubscribe") ;
    case MSG_MdUnsubscribe : return std::string("MSG_MdUnsubscribe") ;
    case MSG_DriverSubscribe : return std::string("MSG_DriverSubscribe") ;
    case MSG_Log : return std::string("MSG_Log") ;
    case MSG_Driver : return std::string("MSG_Driver") ;

    case MSG_MDS : return std::string("MSG_MDS") ;
    case MSG_SecurityTime : return std::string("MSG_SecurityTime") ;
    case MSG_Signal : return std::string("MSG_Signal") ;
    case MSG_MarketData : return std::string("MSG_MarketData") ;

    case MSG_Request : return std::string("MSG_Request") ;
    case MSG_Order : return std::string("MSG_Order") ;
    case MSG_TraderAck : return std::string("MSG_TraderAck") ;
    case MSG_BrokerAck : return std::string("MSG_BrokerAck") ;
    case MSG_DriverApi : return std::string("MSG_DriverApi") ;

    default: { char s[16]; sprintf(s, "POST_%04x", x); return std::string(s); };
    }
} ;

#endif
