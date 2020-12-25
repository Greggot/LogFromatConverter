#include "Parser.h"

#define EXTENDED_RX_ID_LEN 8
#define RESTRICTED_RX_ID_LEN 3

#define START_TIME 0
#define SECONDS_SUBSTRING_LEN 5

std::string Instruction = "tx = transmitted standard frame.  rx = received standard frame\nTX = transmitted Extended frame.  RX = received Extended frame \nHexadecimal values except Time Stamp (Sec) and N (0-99 Counts)\n(In Notepad replace ; with ,0 x to show hex values correctly.)\n\n   ,CAN ID   ,DL,D0,D1,D2,D3,D4,D5,D6,D7,T Stamp,N,But 1,But 2 \n";
void ConvertToArduino(const char* path, dataString* Buffer, uint32_t BufferLength);
std::string LogTimeToArduinoTime(std::string time, uint32_t second);

#define HEADER_IS_NEEDED

int main(int argc, char* argv[])
{
    char* path = argv[PATH_POS];       // Log file Path

    char DivSymb = (char) (std::stoi(argv[DIV_SYMB_POS], 0, 10));  // Divivdion Symbol code translates to char

    uint8_t IDPos, TimePos, DataLenPos, DataPos;

    IDPos = (std::stoi(argv[ID_POS], 0, 10));
    TimePos = (std::stoi(argv[TIME_POS], 0, 10));
    DataLenPos = (std::stoi(argv[DATA_LEN_POS], 0, 10));
    DataPos = (std::stoi(argv[DATA_POS], 0, 10));

    uint64_t StringCounter = 0;
    dataString *Buffer = Parser::ReadDataString (path, DivSymb, TimePos, IDPos, DataLenPos, DataPos, StringCounter);

    Parser::dataStringOut(Buffer[0]);
    std::cout << "...\n";
    Parser::dataStringOut(Buffer[StringCounter - 1]);

    ConvertToArduino ("a.txt", Buffer, StringCounter);

    system("pause");

    return 0;
}

std::string LogTimeToArduinoTime(std::string time, uint32_t second)
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

void ConvertToArduino(const char* path, dataString* Buffer, uint32_t BufferLength)
{
    std::ofstream File;
    File.open(path);

    std::string ID = "";
    uint8_t Number = 0;
    uint32_t RelativeSeconds = 0;

    #ifdef HEADER_IS_NEEDED
        File << Instruction;
    #endif

    for (uint32_t i = 0; i < BufferLength; i++)
    {
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
        
        if (i > 1 && Buffer[i - 1].TIME[9] == '9' && Buffer[i].TIME[9] == '0')
            RelativeSeconds++;

        File << LogTimeToArduinoTime(Buffer[i].TIME, RelativeSeconds + START_TIME) << ',';
        if (Number < 10)
            File << '0';
        
        File << std::dec << (int) Number << ';';
        File << "00;00\n";

    }

    File.close();
}