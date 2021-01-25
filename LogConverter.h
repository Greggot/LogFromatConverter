#ifndef H_LOG_CONVERTER
#define H_LOG_CONVERTER

#include <stdint.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "Parser.h"

#define EXTENDED_RX_ID_LEN 8
#define RESTRICTED_RX_ID_LEN 3

#define START_TIME 0
#define SECONDS_SUBSTRING_LEN 5

class LogConveter
{
    private:
        
    public:
        static std::string LogTimeToArduinoTime(std::string time, uint32_t second);
        static std::string ConvertAStringToArduino(dataString source, std::string lastTime, std::vector<std::string> IDs, uint32_t& RelativeSeconds, uint64_t& StringCounter);
        static void ConvertToArduino(const char* path, dataString* Buffer, uint64_t BufferLength, std::vector<std::string> IDs);
};

#endif