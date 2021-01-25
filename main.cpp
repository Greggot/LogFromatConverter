#include "Parser.h"
#include "LogConverter.h"

std::string _Instruction = "tx = transmitted standard frame.  rx = received standard frame\nTX = transmitted Extended frame.  RX = received Extended frame \nHexadecimal values except Time Stamp (Sec) and N (0-99 Counts)\n(In Notepad replace ; with ,0 x to show hex values correctly.)\n\n   ,CAN ID   ,DL,D0,D1,D2,D3,D4,D5,D6,D7,T Stamp,N,But 1,But 2 \n";

int main(int argc, char* argv[])
{   
    std::fstream File;
    File.open("settingsFiles/command.txt");

    std::vector <std::string> IDs;
    for (uint8_t  i = 1; i < argc; i++)
        IDs.push_back(argv[i]);

    char* SettingsFile = new char [1024] {0};
    char* SourceFile = new char [1024] {0};
    char* OutputFile = new char [1024] {0};

    uint32_t RelativeSeconds = 0;
    uint64_t StringNumber = 0;

    File >> SettingsFile >> SourceFile >> OutputFile;
    
    uint8_t* Settings = Parser::ReadSettingsFile(SettingsFile);
    std::fstream Input;
    Input.open(SourceFile);
    
    std::ofstream Output;
    Output.open(OutputFile);

    #ifdef HEADER_IS_NEEDED
        Output << _Instruction;
    #endif

    char* TempBuffer = new char [MAX_STRING_LENGTH_ASCII] {0};

    std::string lastTime = "00.00.00.000";

    while (Input.getline(TempBuffer, MAX_STRING_LENGTH_ASCII, '\n'))
    {
        dataString temp = Parser::StringToDataString(TempBuffer, (char)Settings[DIVIDION_SYMBOL],
                                                    Settings[TIME_POS], Settings[ID_POS], Settings[DATA_LEN_POS], Settings[DATA_POS]);

        Output << LogConveter::ConvertAStringToArduino(temp, lastTime, IDs, RelativeSeconds, StringNumber);
        lastTime = temp.TIME;
    }
    Input.close();

    Output << "Received " << StringNumber << "    " << "Sent 0    " << "Total " << StringNumber << ".                        \n"; 

    Output.close();
    /*
    uint64_t StringCounter = 0;

    dataString *Buffer = Parser::ReadDataString(SourceFile, (char)Settings[DIVIDION_SYMBOL],
                                                Settings[TIME_POS], Settings[ID_POS], Settings[DATA_LEN_POS], Settings[DATA_POS], StringCounter);

    Parser::dataStringOut(Buffer[0]);
    std::cout << "...\n";
    Parser::dataStringOut(Buffer[StringCounter - 1]);
    
    File >> Temp;
    LogConveter::ConvertToArduino(Temp, Buffer, StringCounter, IDs);*/

    system("pause");

    return 0;
}