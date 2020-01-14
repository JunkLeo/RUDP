#ifndef _MdHead_H_
#define _MdHead_H_

#include "Postman/PostDefinitions.H"

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

struct MdHeadCore {
    char bReady; unsigned char bBad;
    unsigned short nBytes;
    unsigned short source, mdType ; // Also interpreted as buffer entry head
    char symbol[16];
    char marketTime[16]; // Uint64 marketTimeValue;
    Uint64 tsApiUniversal;
};

struct MdHeadOffline {
    unsigned sequenceNo;
    unsigned short nBytes, mdType ; // Also interpreted as buffer entry head
    char symbol[16];
    char marketTime[16]; // Uint64 marketTimeValue;
    Uint64 tsApiUniversal;
};

#endif
