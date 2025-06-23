// Copyright (c) 2010-2025, Numerics88 Solutions.
// See LICENSE for details.

// See https://github.com/mdoube/BoneJ/blob/master/src/org/doube/bonej/pqct/Read_Stratec_File.java

#ifndef __qPQCTIO_pQCTIO_Types_h
#define __qPQCTIO_pQCTIO_Types_h

#include <boost/endian/arithmetic.hpp>
#include <string.h>

namespace pQCTIO
{

using namespace boost::endian;

const int CT_HEADER_LENGTH = 1609;

/** Type definitions to avoid mistakes.
 *
 * Note that there is no 6 byte floating point type, so I have used
 * a 48 bit unsigned integer as a placeholder. This is a Fortran-ism.
 *
 * Also, there is no nice way to have platform independany little
 * endian floats in Boost or otherwise. Be aware that you need to
 * convert these for your system.
 *
 * Notes on Fortran:
 *  string      : 256 byte of characters first character defines used length
 *  string[nn]  : nn+1 byte of characters first character defines used length
 */
typedef little_uint8_t  pBool_t;         // 1 byte boolean (0=false; 1=true)
typedef little_uint8_t  pByte_t;         // 1 byte unsigned
typedef little_int8_t   pShortInt_t;     // 1 byte signed
typedef little_uint16_t pWord_t;         // 2 byte unsigned
typedef little_int16_t  pInteger_t;      // 2 byte signed
typedef little_int32_t  pLongInt_t;      // 4 byte signed

typedef float           pSingle_t;       // 4 byte floating point
typedef little_int48_t  pReal_t;         // 6 byte floating point
typedef double          pDouble_t;       // 8 byte floating point
typedef char            pChar_t;         // 1 byte character
typedef little_uint32_t pPointer_t;      // 4 byte pointer

/** Defines the header prefix.
 * 
 */
typedef struct HeadPrefix {
    pLongInt_t      HeadVers;
    pLongInt_t      HeadLen;
} HeadPrefix_t;

/** Defines the HeaderData
 * 
 * Regretably, we were given an incomplete header. In a perfect
 * world, we would have the structure defined for the entire header.
 * However, we just know where certain datum is located within the
 * header. So we have to do this 'padding' trick to make reading the
 * header a little easier.
 */
#pragma pack(push)
#pragma pack(1)
typedef struct HeaderData {
    HeadPrefix_t    HeadPrefix;         // Bytes 0 -7
    pLongInt_t      MaxNumScans;        // Bytes 8 - 11
    pDouble_t       VoxelSize;          // Bytes 12 - 19        Units mm
    char            Pad1[318-20];       // Pad to 318
    pDouble_t       ObjLen;             // Bytes 318 - 325      Units mm
    char            Pad2[663-326];      // Pad to 663 
    char            MeasInfo1[80];      // Bytes 663 - 985
    char            PadChar1;
    char            MeasInfo2[80];      // Bytes 663 - 985
    char            PadChar2;
    char            MeasInfo3[80];      // Bytes 663 - 985
    char            PadChar3;
    char            MeasInfo4[80];      // Bytes 663 - 985
    pLongInt_t      MeasDate;           // Bytes 986 - 989      YYYYMMDD
    char            Pad3[1051-990];     // Pad to 1050
    char            Device[12];         // Bytes 1051 - 1062
    char            Pad4[1085-1063];    // Pad to 1085          Ends in .TYP
    pInteger_t      PatMeasNo;          // Bytes 1085 - 1086
    pLongInt_t      PatNo;              // Bytes 1087 - 1090
    pLongInt_t      PatBirth;           // Bytes 1091 - 1094
    char            Pad5[1100-1095];    // Pad to 1100
    char            PatName[40];        // Bytes 1100 - 1139
    char            Pad6[1283-1140];    // Pad to 1283
    char            PatID[12];          // Bytes 1283 - 1294
    char            Pad7[1525-1295];    // Pad to 1525
    pWord_t         PicX0;              // Bytes 1525 - 1526
    pWord_t         PicY0;              // Bytes 1527 - 1529
    pWord_t         PicMatrixX;         // Bytes 1530 - 1531
    pWord_t         PicMatrixY;         // Bytes 1531 - 1532

    // Pad the remaining bytes
    char            Pad8[CT_HEADER_LENGTH - 1533];
} HeaderData_t;
#pragma pack(pop)

/// A utility function since pascal strings are not null terminated.
std::string parse_pascal_string(const char *s, int n) {
    std::string a(s);
    if (a.size() > n) {
        a.resize(n);
    }
    return a;
}

} /* end namepsace pQCTIO */

#endif /* __qPQCTIO_pQCTIO_Types_h */
