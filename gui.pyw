from tkinter import *
import tkinter as tk
from tkinter.filedialog import askopenfilename
from tkinter.filedialog import asksaveasfilename
from tkinter import ttk
import os

class LogFormatSettings:
    def __init__(self, sizeX, sizeY, WinName, path):
        self.sizeX = sizeX
        self.sizeY = sizeY
        self.WinName = WinName
        self.path = path
        
        self.AskWin = None
        self.DividionSymbol = None
        self.TimePos = None
        self.DataPos = None
        self.DataLenPos = None
        self.IDPos = None

        self.settings = None
        self.SampleList = None

    def __repr__(self):
        return repr([self.DividionSymbol, self.TimePos, self.IDPos, self.DataLenPos, self.DataPos])
    
    def CreateAskWindow(self):
        self.AskWin = Tk()
        self.AskWin.title(self.WinName)
        self.AskWin.geometry(self.sizeX + 'x' + self.sizeY + '+800+200')
        self.AskWin.resizable(False, False)

        InputFile = open(self.path, 'r')
        SampleString = InputFile.readline()
        self.SampleList = SampleString.split()

        def ReadString():
            global SampleString
            SampleString = InputFile.readline()
            self.SampleList = SampleString.split()

            DataPosBox['values'] = self.SampleList
            DataLenPosBox['values'] = self.SampleList
            IDPosBox['values'] = self.SampleList
            TimePosBox['values'] = self.SampleList
            
            DataPosBox.set("-Data Position-")
            DataLenPosBox.set("-Data Length Position-")
            IDPosBox.set("-ID Position-")
            TimePosBox.set("-Time Position-")
            
        
        AnoterStringButton = Button(self.AskWin, text = 'Refresh String', command = ReadString)
        AnoterStringButton.grid(column = 1, row = 0, padx = 20)

        DataPosBox = ttk.Combobox(self.AskWin, values = self.SampleList, state = "readonly")
        DataPosBox.grid(column = 0, row = 0, padx = 5, pady = 5)
        DataPosBox.set("-Data Position-")

        DataLenPosBox = ttk.Combobox(self.AskWin, values = self.SampleList, state = "readonly")
        DataLenPosBox.grid(column = 0, row = 1, padx = 5, pady = 5)
        DataLenPosBox.set("-Data Length Position-")

        IDPosBox = ttk.Combobox(self.AskWin, values = self.SampleList, state = "readonly")
        IDPosBox.grid(column = 0, row = 2, padx = 5, pady = 5)
        IDPosBox.set("-ID Position-")

        TimePosBox = ttk.Combobox(self.AskWin, values = self.SampleList, state = "readonly")
        TimePosBox.grid(column = 0, row = 3, padx = 5, pady = 5)
        TimePosBox.set("-Time Position-")

        DividionSymbols = ['Spaces', 'Tabs', 'Other...']
        DividionSymbolsBox = ttk.Combobox(self.AskWin, values = DividionSymbols, state = "readonly")
        DividionSymbolsBox.grid(column = 0, row = 4, padx = 5, pady = 5)
        DividionSymbolsBox.set('-Dividion Symbols-')

        NameText = Text(self.AskWin, width = 20, height = 1)
        NameText.grid(column = 0, row = 5, padx = 5, pady = 5)
        NameText.config(state = DISABLED)

        def ChangeData(event):
            self.DataPos = self.SampleList.index(DataPosBox.get())
        def ChangeDataLength(event):
            self.DataLenPos = self.SampleList.index(DataLenPosBox.get())
        def ChangeID(event):
            self.IDPos = self.SampleList.index(IDPosBox.get())
        def ChangeTime(event):
            self.TimePos = self.SampleList.index(TimePosBox.get())
        def ChangeDividionSymbol(event):
            if(DividionSymbolsBox.get() == DividionSymbols[0]):
                self.DividionSymbol = 32
            elif (DividionSymbolsBox.get() == DividionSymbols[1]):
                self.DividionSymbol = 9
            else:
                self.DividionSymbol = int(NameText.get(1.0, END)[0:1])

        DataPosBox.bind("<<ComboboxSelected>>", ChangeData)
        DataLenPosBox.bind("<<ComboboxSelected>>", ChangeDataLength)
        IDPosBox.bind("<<ComboboxSelected>>", ChangeID)
        TimePosBox.bind("<<ComboboxSelected>>", ChangeTime)
        DividionSymbolsBox.bind("<<ComboboxSelected>>", ChangeDividionSymbol)

        ContinueButton = Button(self.AskWin, text = 'Continue...')
        ContinueButton.grid(column = 2, row = 2, padx = 20)

        def CloseAndPrint(event):
            OutputFilePath = asksaveasfilename(filetypes=[("Txt File", "*.txt")])
            if (OutputFilePath != ''):
                OutputFile = open(OutputFilePath, 'w')
                OutputFile.write(str(self.DividionSymbol) + '\n' + str(self.IDPos)  + '\n' + str(self.TimePos) + '\n' + str(self.DataLenPos) + '\n' + str(self.DataPos) + '\n')
                self.AskWin.quit()
                self.AskWin.destroy()
        
        ContinueButton.bind('<Button-1>', CloseAndPrint) # ЛКМ
        self.AskWin.bind('<Return>', CloseAndPrint)
        
        self.AskWin.mainloop()

MainWin = Tk()
MainWin.title("SPN Parser")                # Название окна
MainWin.geometry('530x200+200+100')    # Размеры окна и расстояние от краёв экрана
MainWin.resizable(False, False)        # Запрет на изменение размеров окна
MainWin.iconbitmap('Icon.ico')

os.system('mkdir settingsFiles')

SettingsPath = ''

NameText = Text(MainWin, width = 64, height = 1, state = DISABLED)
IDText = Text(MainWin, width = 64, height = 1, state = NORMAL)
IDText.grid(column = 0, row = 3, padx = 5, pady = 5)
IDText.insert(INSERT, 'Enter IDs...')

def CreateSettingsWin():
    InputFilePath = askopenfilename(filetypes=[("Log File", "*.log"), ("Txt File", "*.txt"), ("002 File", "*.002")])
    if(InputFilePath != ''):
        Ask = LogFormatSettings('400', '200', 'Log Format Settings', InputFilePath)
        Ask.CreateAskWindow()

def OpenSettingsFile():
    global SettingsPath
    global NameText
    SettingsFilePath = askopenfilename(filetypes=[("Txt File", "*.txt")])
    if(SettingsFilePath != ''):
        SettingsPath = SettingsFilePath
        NameText.config(state = NORMAL)
        NameText.delete('1.0', END)
        NameText.insert(INSERT, SettingsFilePath)
        NameText.config(state = DISABLED)

def Run():
    global SettingsPath
    InputFilePath = askopenfilename(filetypes=[("Log File", "*.log"), ("Txt File", "*.txt"), ("002 File", "*.002")])
    if (InputFilePath != ''):
        OutputFilePath = asksaveasfilename(filetypes=[("Text File", "*.TXT")])
        if (OutputFilePath != ''):
            CommandFile = open('settingsFiles/command.txt', 'w')
            CommandFile.write(SettingsPath + '\n' + InputFilePath + '\n' + OutputFilePath + '\n')
            CommandFile.close()
            os.system('Log2Arduino ' + IDText.get(1.0, END)[0:-1])

Button(MainWin, text = 'Create Settings from Log', command = CreateSettingsWin).grid(column = 0, row = 0, pady = 5, padx = 5)
Button(MainWin, text = 'Choose Settigs File', command = OpenSettingsFile).grid(column = 0, row = 1, pady = 5, padx = 5)

NameText.grid(column = 0, row = 2, padx = 5, pady = 5)

Button(MainWin, text = 'Convert to arduino log', command = Run).grid(column = 0, row = 4, pady = 5, padx = 5)

MainWin.mainloop()
