/**
  ******************************************************************************
  * @file    Parser.h
  * @author  Tochilin
  * @version V 0.1
  * @date    25-December-2020
  * @brief   Header file for Parse.cpp
  ******************************************************************************
  */ 

//**************************************************************************************************
// Includes
//**************************************************************************************************
#include <fstream>      // Files
#include <stdint.h>     // Unsigned integers
#include <string>       // String works
#include <iostream>     // Standat data-in data-out
#include <cstring>      
#include <vector>
#include <sstream>
//**************************************************************************************************
// Definitions
//**************************************************************************************************

#define MAX_BUFFER_SIZE 0x2FFFFFFF   // 767 MB - Maximum buffer (input file) size
#define STRING_LENGTH_BYTE 8        // Average number of bytes in a string

#define MAX_SUBSTRING_NUMBER 0x20       // Maximum substring's number 
#define MAX_SUBSTRING_LENGTH 0x20       // MAximum string element's length
#define MAX_STRING_LENGTH_ASCII 0xFF    // Maximum string length - 255 symbols

enum ARGS
{
    PROGRAM_NAME = 0,
    PATH_POS,
    DIV_SYMB_POS,
    ID_POS,
    TIME_POS,
    DATA_LEN_POS,
    DATA_POS,
};

const uint8_t PLUS = 43;        // (int) '+'
const uint8_t MINUS = 45;       // (int) '-'
const uint8_t  MULTIPLY = 42;   // (int) '*'
const uint8_t  DIVIDE = 47;     // (int) '/'
const uint8_t _DIVIDE = 92;     // (int) '\\'

//**************************************************************************************************
// Structures and Data Types
//**************************************************************************************************

struct byteString           
{
    uint8_t bytes[STRING_LENGTH_BYTE];
};


struct dataString          // Useful data structure ID-DATA-TIME
{
    uint8_t DataLen;
    std::string ID;
    byteString Data;
    std::string TIME;
};

//**************************************************************************************************
// Declarations
//**************************************************************************************************

class Parser        // Class that is inherited by other Parse classes
{
    public:       
        static void byteStringOut(byteString Out);  // Console outputs
        static void dataStringOut(dataString Out);

        static char* ReadFile(const char* path, uint64_t& fileLength);  // text file input
        static dataString* ReadDataString(const char* path, char DividionSymbol, uint8_t timePos, uint8_t IDPos, uint8_t dataLenPos, uint8_t dataPos, uint64_t& size);
        
        static byteString StringToByte(std::string Input);      // Strings with data only convertation
        static dataString StringToDataString(std::string BuffPtr, char DividionSymbol, const uint8_t timePos, const uint8_t IDPos, const uint8_t dataLen, const uint8_t dataPos);
};