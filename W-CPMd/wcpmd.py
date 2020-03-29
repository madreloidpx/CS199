import sys
import os.path as Path

class WCPMD:
    def __init__(self, graphDir = None, trueCommunityDir = None, recursionLimit = 2000, nmiDir = "./Overlapping-NMI"):
        self.recursionLimit = recursionLimit
        self.nmiDir = None
        self.graphDataFile = graphDir
        self.trueCommunityFile = trueCommunityDir
        self.__graphData = None
        self.__graph = None
        self.__welcome = True
        self.__border = "+-----------------------------------------------------------------------------------------+"
        sys.setrecursionlimit(self.recursionLimit)
        if Path.exists(nmiDir):
            self.nmiDir = nmiDir
    
    def __welcomeMessage(self):
        return """
    {0}
        Welcome to W-CPM-d!
        For a quick set-up, enter 'initialize'.
        For the complete list of commands, enter 'help'.
    {0}
        """.format(self.__border)

    def menu(self):
        return """
    {4}
        Command List:                                                                         
        > help: opens the commands menu
        > initialize: runs settings                                                          
        > setNMIDir: sets the directory for NMI evaluator. Default is {0}.     
        > setRL: changes the recursion limit. Default is {1}.                               
        > setGD: input file directory for graph data. Default is {2}.                       
        > setTCD: input true community file directory. Default is {3}.
        > settings: show current settings. 
        > quit: exit the program                     
    {4}
        """.format(self.nmiDir, self.recursionLimit, self.graphDataFile, self.trueCommunityFile, self.__border)
    
    def showSettings(self):
        return """
    {4}
        Current Settings:
        > NMI Directory: {0}
        > Recursion Limit: {1}
        > Graph Data Directory: {2}
        > True Community Directory: {3}
    {4}
        """.format(self.nmiDir, self.recursionLimit, self.graphDataFile, self.trueCommunityFile, self.__border)

    def __goodbyeMessage(self):
        return """
    {0}
        Thank you for using W-CPMd!
        Insert license and copyright here laterrrr
    {0}
        """.format(self.__border)

    def getCommands(self):
        return {
            "help": self.help,
            "initialize": self.initialize,
            "setNMIDir": self.setNMI,
            "setRL": self.setRecursionLimit,
            "setGD": self.setGraph,
            "setTCD": self.setTrueCommunityDir,
            "settings": self.settings
        }
    
    def help(self):
        print(self.menu())
    
    def initialize(self):
        print("Initializing...")
        self.setRecursionLimit()
        self.setNMI()
        self.setGraph()
        self.setTrueCommunityDir()
        print("Done initializing.")
    
    def setNMI(self):
        nmiDir = input("NMI Directory [%s] :" % (self.nmiDir))
        if nmiDir == "":
            nmiDir = self.nmiDir
        validPath = Path.exists(nmiDir)
        if validPath == True:
            self.nmiDir = nmiDir
            print("NMI directory is set.")
        else:
            print("File directory doesn't exist.")
            self.setNMI()
    
    def setRecursionLimit(self):
        recursionLimit = input("Recursion Limit [%d] :" % (self.recursionLimit))
        if recursionLimit == "":
            recursionLimit = self.recursionLimit
        try:
            self.recursionLimit = int(recursionLimit)
            sys.setrecursionlimit(self.recursionLimit)
            print("Recursion limit is set to", self.recursionLimit)
        except ValueError:
            print("Invalid input.")
            self.setRecursionLimit()
    
    def setGraph(self):
        print("Setting graph data...")
    
    def setTrueCommunityDir(self):
        tcDir = input("True Community File Directory [%s] :" % (self.trueCommunityFile))
        if tcDir == "":
            tcDir = self.trueCommunityFile
        if tcDir == None:
            print("No file set.")
            return
        validPath = Path.exists(tcDir)
        if validPath == True:
            self.trueCommunityFile = tcDir
            print("True Community File directory is set.")
        else:
            print("File directory doesn't exist.")
            self.setTrueCommunityDir()
    
    def settings(self):
        print(self.showSettings())
    
    def run(self):
        if self.__welcome == True:
            print(self.__welcomeMessage())
            self.__welcome = False
        commands = self.getCommands()
        command = input("> ")
        if command == "quit":
            print(self.__goodbyeMessage())
            exit(0)
        try:
            func = commands.get(command)
            func()
        except TypeError:
            print("Command invalid. For the full list of commands, type 'help'.")
        self.run()

run = WCPMD()
print(run.run())
