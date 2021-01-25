#include "LogConverter.h"

std::string Instruction = "tx = transmitted standard frame.  rx = received standard frame\nTX = transmitted Extended frame.  RX = received Extended frame \nHexadecimal values except Time Stamp (Sec) and N (0-99 Counts)\n(In Notepad replace ; with ,0 x to show hex values correctly.)\n\n   ,CAN ID   ,DL,D0,D1,D2,D3,D4,D5,D6,D7,T Stamp,N,But 1,But 2 \n";

std::string LogConveter::LogTimeToArduinoTime(std::string time, uint32_t second)
{
    std::string Seconds = std::to_string(second);
    uint8_t secondsSize = Seconds.size();
    
    if (secondsSize < SECONDS_SUBSTRING_LEN)
    {
        Seconds = "";
        for (uint8_t i = secondsSize; i < SECONDS_SUBSTRING_LEN; i++)
        {
            Seconds += '0';
        }
        Seconds += std::to_string(second);
    }

    std::string Milliseconds = "";
    uint8_t strLen = time.size();
    Milliseconds += time[strLen - 3];
    Milliseconds += time[strLen - 2];
    Milliseconds += time[strLen - 1];
    Milliseconds += "00";

    return (Seconds + "." + Milliseconds);
}

std::string LogConveter::ConvertAStringToArduino(dataString source, std::string lastTime, std::vector<std::string> IDs,
                                                 uint32_t& RelativeSeconds, uint64_t& StringCounter)
{
    std::string output = "";
    std::stringstream dataBytes;

    uint8_t TimeStrLen = source.TIME.size();
    uint8_t IDsNumber = IDs.size();
    uint8_t Number = 0;

    for (uint8_t k = 0; k < IDsNumber; k++)
    {
        if (source.ID == IDs[k])
        {   
            StringCounter++;

            if(lastTime[TimeStrLen - 3] == '9' && source.TIME[TimeStrLen - 3] == '0')
                RelativeSeconds++;

            Number++;
            if(Number >= 100)
                Number = 0;

            uint8_t IDLen = source.ID.size();
            switch (IDLen)
            {
                case RESTRICTED_RX_ID_LEN:
                    output += "rx,";
                    for (uint8_t j = RESTRICTED_RX_ID_LEN; j < EXTENDED_RX_ID_LEN; j++)
                        output += ' ';
                    output += "0x";
                    output += source.ID;
                    output += ',';
                    break;
                case EXTENDED_RX_ID_LEN:
                    output += "RX,0x";
                    output += source.ID;
                    output += ',';
                    break;
                default:
                    break;
            }

            output += std::to_string(source.DataLen);
            output += ',';

            for (uint8_t j = 0; j < STRING_LENGTH_BYTE; j++)
            {   
                if (source.Data.bytes[j] > 15)
                {
                    dataBytes << std::uppercase << std::hex << (int) source.Data.bytes[j];
                    output+= dataBytes.str();
                    dataBytes.str(std::string());
                }
                else
                {
                    output+= '0';
                    dataBytes << std::uppercase << std::hex << (int) source.Data.bytes[j];
                    output+= dataBytes.str();
                    dataBytes.str(std::string());
                }

                if (j != STRING_LENGTH_BYTE - 1)
                    output+= ';';
                else
                    output+= ',';
                
            }
            output += LogConveter::LogTimeToArduinoTime (source.TIME, RelativeSeconds + START_TIME);
            output += ',';
            if (Number < 10)
                output+= '0';
            
            dataBytes << std::dec << (int) Number << ';';
            output += dataBytes.str();

            output += "00;00\n";
        }
    }
    return output;
}

void LogConveter::ConvertToArduino(const char* path, dataString* Buffer, uint64_t BufferLength, std::vector<std::string> IDs)
{
    std::ofstream File;
    File.open(path);

    uint8_t TimeStrLen = Buffer[0].TIME.size();

    std::string ID = "";
    uint8_t Number = 0;
    uint32_t RelativeSeconds = 0;
    uint32_t SendCounter = 0;

    #ifdef HEADER_IS_NEEDED
        File << Instruction;
    #endif

    uint8_t IDsNumber = IDs.size();

    for (uint32_t i = 0; i < BufferLength; i++)
    {
        for (uint8_t k = 0; k < IDsNumber; k++)
        {
            if (Buffer[i].ID == IDs[k])
            {   
                SendCounter++;

                Number++;
                if(Number >= 100)
                    Number = 0;

                uint8_t IDLen = Buffer[i].ID.size();
                switch (IDLen)
                {
                    case RESTRICTED_RX_ID_LEN:
                        File << "rx,";
                        for (uint8_t j = RESTRICTED_RX_ID_LEN; j < EXTENDED_RX_ID_LEN; j++)
                            File << ' ';
                        File << "0x" << Buffer[i].ID << ',';
                        break;
                    case EXTENDED_RX_ID_LEN:
                        File << "RX,0x" << Buffer[i].ID << ',';
                        break;
                    default:
                        break;
                }

                File << (int) Buffer[i].DataLen << ',';

                for (uint8_t j = 0; j < STRING_LENGTH_BYTE; j++)
                {   
                    if (Buffer[i].Data.bytes[j] > 15)
                    {
                        File << std::uppercase << std::hex << (int) Buffer[i].Data.bytes[j];
                    }
                    else
                    {
                        File << '0';
                        File << std::uppercase << std::hex << (int) Buffer[i].Data.bytes[j];
                    }

                    if (j != STRING_LENGTH_BYTE - 1)
                        File << ';';
                    else
                        File << ',';
                    
                }

                File << LogTimeToArduinoTime(Buffer[i].TIME, RelativeSeconds + START_TIME) << ',';
                if (Number < 10)
                    File << '0';
                
                File << std::dec << (int) Number << ';';
                File << "00;00\n";
            }
        }
        
        if(Buffer[i].TIME[TimeStrLen - 3] == '9' && Buffer[i + 1].TIME[TimeStrLen - 3] == '0')
            RelativeSeconds++;
    }

    File << "Received " << SendCounter << "    " << "Sent 0    " << "Total " << SendCounter << ".                        \n"; 

    File.close();
}

