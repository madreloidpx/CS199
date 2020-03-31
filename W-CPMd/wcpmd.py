import sys
import graph
import time
import networkx as nx
import os.path as Path
import matplotlib.pyplot as plt

class WCPMD:
    def __init__(self, graphDir = None, trueCommunityDir = None, recursionLimit = 2000, nmiDir = "./Overlapping-NMI"):
        self.recursionLimit = recursionLimit
        self.nmiDir = None
        self.graphDataFile = graphDir
        self.trueCommunityFile = trueCommunityDir
        self.__data = None
        self.__edgeData = None
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
    {0}
        Command List:                                                                         
        > help: opens the commands menu
        > initialize: runs settings                                                          
        > setNMIDir: sets the directory for NMI evaluator. Default is {1}.     
        > setRL: changes the recursion limit. Default is {2}.                               
        > setGD: input file directory for graph data.
                 Algorithm currently only accepts a tab delimiter. Default is {3}.                       
        > setTCD: input true community file directory. Default is {4}.
        > settings: show current settings. 
        > showGraph: shows the current graph. Graph data directory required to be set beforehand.
        > quit: exit the program                     
    {0}
        """.format(self.__border, self.nmiDir, self.recursionLimit, self.graphDataFile, self.trueCommunityFile)
    
    def showSettings(self):
        return """
    {0}
        Current Settings:
        > NMI Directory: {1}
        > Recursion Limit: {2}
        > Graph Data Directory: {3}
        > True Community Directory: {4}
        > Graph initialized: {5}
    {0}
        """.format(self.__border, self.nmiDir, self.recursionLimit, self.graphDataFile, self.trueCommunityFile, self.__graph != None)

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
            "setGD": self.setGraphDir,
            "setTCD": self.setTrueCommunityDir,
            "settings": self.settings,
            "showGraph": self.showUncoloredGraph,
        }
    
    def help(self):
        print(self.menu())
    
    def initialize(self):
        print("Initializing...")
        self.setRecursionLimit()
        self.setNMI()
        self.setGraphDir()
        self.setTrueCommunityDir()
        print("Done initializing.")
    
    def setNMI(self):
        nmiDir = input("NMI Directory [%s] : " % (self.nmiDir))
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
        recursionLimit = input("Recursion Limit [%d] : " % (self.recursionLimit))
        if recursionLimit == "":
            recursionLimit = self.recursionLimit
        try:
            self.recursionLimit = int(recursionLimit)
            sys.setrecursionlimit(self.recursionLimit)
            print("Recursion limit is set to", self.recursionLimit)
        except ValueError:
            print("Invalid input.")
            self.setRecursionLimit()
    
    def setGraphDir(self):
        graphDataFile = input("Graph Data Directory [%s] : " % (self.graphDataFile))
        if graphDataFile == "":
            graphDataFile = self.graphDataFile
        validPath = Path.exists(graphDataFile)
        if validPath == True:
            self.graphDataFile = graphDataFile
            print("Graph Data directory is set.")
            self.__readFile()
            self.__createEdgeData()
            self.__generateGraph()
        else:
            print("File directory doesn't exist.")
            self.setGraphDir()
    
    def setTrueCommunityDir(self):
        tcDir = input("True Community File Directory [%s] : " % (self.trueCommunityFile))
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

    def showUncoloredGraph(self):
        if self.__graph == None:
            print("Graph data not found. Please enter graph directory using 'setGD'.")
            return
        print("Generating graph...")
        pos = nx.spring_layout(self.__graph.graph)
        nx.draw_networkx_nodes(self.__graph.graph, pos, cmap=plt.get_cmap('jet'))
        nx.draw_networkx_labels(self.__graph.graph, pos)
        nx.draw_networkx_edges(self.__graph.graph, pos, edgelist=self.__edgeData, edge_color='black', arrows=True)
        print("Graph will pop up in a window. Exit the window to continue...")
        plt.show()
        print("Graph is closed.")
    
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
    
    def __readFile(self):
        print("Parsing graph data...")
        f = open(self.graphDataFile, "r")
        data = []
        for line in f:
            contents = line.split("\t")
            for i in range(len(contents)):
                    contents[i] = contents[i].strip()
            data.append(contents)
        f.close()
        self.__data = data
        print("Data parsed.")
    
    def __createEdgeData(self):
        print("Creating edge data...")
        edges = []
        for line in self.__data:
            if len(line) == 2: edges.append((line[0], line[1]))
            if len(line) == 3: edges.append((line[0], line[1], line[2]))
        self.__edgeData = edges
        self.__graphData = {"edge_data": self.__edgeData}
        print("Edge data generated.")
    
    def __generateGraph(self):
        print("Generating graph data...")
        start = time.time()
        self.__graph = graph.Graph(self.__graphData, "directed")
        end = time.time()
        print("Graph data generated in", str(end-start), "sec")

run = WCPMD()
print(run.run())
