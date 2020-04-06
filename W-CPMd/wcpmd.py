import sys
import graph
import time
import random
import networkx as nx
import os.path as Path
import matplotlib.pyplot as plt

class WCPMD:
    def __init__(self, **kwargs):
        self.recursionLimit = kwargs.get("recursionLimit") or 2000
        self.graphDataFile = kwargs.get("graphDir") or None
        self.threshold = kwargs.get("threshold") or None
        self.__auto = kwargs.get("auto") or False
        self.__data = None
        self.__edgeData = None
        self.__graphData = None
        self.__graph = None
        self.__welcome = True
        self.__border = "+-----------------------------------------------------------------------------------------+"
        self.__outFilename = kwargs.get("filename") or "communities.txt"
        if self.__auto == False:
            sys.setrecursionlimit(self.recursionLimit)
    
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
        > setRL: changes the recursion limit. Default is {1}.                               
        > setGD: input file directory for graph data.
                 Algorithm currently only accepts a tab delimiter. Default is {2}.                       
        > settings: show current settings. 
        > showGraph: shows the current graph.
        > showCommunity: shows the community by providing a specific threshold.
        > quit: exit the program                     
    {0}
        """.format(self.__border, self.recursionLimit, self.graphDataFile)
    
    def showSettings(self):
        return """
    {0}
        Current Settings:
        > Recursion Limit: {1}
        > Graph Data Directory: {2}
        > Graph initialized: {3}
    {0}
        """.format(self.__border, self.recursionLimit, self.graphDataFile, self.__graph != None)

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
            "setRL": self.setRecursionLimit,
            "setGD": self.setGraphDir,
            "settings": self.settings,
            "showGraph": self.showUncoloredGraph,
            "showCommunity": self.showGraphSpecificThreshold,
        }
    
    def help(self):
        print(self.menu())
    
    def initialize(self):
        print("Initializing...")
        self.setRecursionLimit()
        self.setGraphDir()
        print("Done initializing.")
    
    def setRecursionLimit(self):
        if self.__auto == False:
            recursionLimit = input("Recursion Limit [%d] : " % (self.recursionLimit))
            if recursionLimit == "":
                recursionLimit = self.recursionLimit
        try:
            if self.__auto == False:
                self.recursionLimit = int(recursionLimit)
            else:
                self.recursionLimit = int(self.recursionLimit)
            sys.setrecursionlimit(self.recursionLimit)
            print("Recursion limit is set to", self.recursionLimit)
        except ValueError:
            print("Invalid recursion limit.")
            self.setRecursionLimit()
    
    def setGraphDir(self):
        validPath = False
        graphDataFile = None
        if self.__auto == False:
            graphDataFile = input("Graph Data Directory [%s] : " % (self.graphDataFile))
            if graphDataFile == "":
                graphDataFile = self.graphDataFile
        elif self.__auto == True:
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
            self.graphDataFile = None
            self.setGraphDir()
    
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
    
    def setThreshold(self, threshold):
        self.threshold = threshold

    def showGraphSpecificThreshold(self):
        if self.__graph == None:
            print("No graph data found.")
            return
        if self.__auto == False:
            threshold = input("Threshold: ")
        else:
            threshold = self.threshold
        try:
            if float(threshold) > 1 or float(threshold) < 0:
                raise Exception
        except:
            print("Invalid threshold. Please select from [0, 1].")
            self.showGraphSpecificThreshold()
        print("Generating communities...")
        start = time.time()
        communities = self.__graph.getCommunities(float(threshold))
        end = time.time()
        print("Community count:", communities.get("communities"))
        print("Communities computed in", str(end-start), "sec")
        if self.__auto == False:
            #self.__showCommunity(communities)
            save = input("Save community data? [y/n]: ")
        else:
            save = 'y'
        if save.lower() == 'y' or save.lower() == 'yes':
            if self.__auto == False:
                filename = input("Input filename [communities.txt]: ")
                if filename != "":
                    self.__outFilename = filename
            print("Saving...")
            try:
                self.__saveToTxt(communities)
            except Exception as e:
                print(e)
                return
            print("Saved.")
    
    def __saveToTxt(self, communities):
        content = ""
        nodeList = list(communities.keys())
        for i in range(len(nodeList)):
            if nodeList[i] != "communities":
                line = nodeList[i] + "\t"
                for community in communities.get(nodeList[i]):
                    line = line + str(community) + " "
                content = content + line + "\n"
        content = content[:-1]
        f = open(self.__outFilename, "w")
        f.write(content)
        f.close()
    
    def __randomColor(self):
        color = lambda: random.randint(0,255)
        return '#%02X%02X%02X' % (color(),color(),color())
    
    def __showCommunity(self, communities): #I need to find a way to better represent overlapping nodes
        print("Generating graph...")
        overlappingNodes = set()
        edgeCommunities = []
        pos = nx.spring_layout(self.__graph.graph)
        for i in range(len(communities)):
            if len(communities[i]) <= 2:
                edgeCommunities.append((communities[i][0], communities[i][1]))
                continue
            for j in range(i+1, len(communities)):
                if len(communities[j]) <= 2:
                    if i == len(communities)-1:
                        edgeCommunities.append((communities[j][0], communities[j][1]))
                    continue
                intersection = (set(communities[i])).intersection(set(communities[j]))
                overlappingNodes = overlappingNodes.union(intersection)
            nodesToColor = [node for node in communities[i] if node not in overlappingNodes]
            nx.draw_networkx_nodes(self.__graph.graph, pos, cmap=plt.get_cmap('jet'), nodelist=nodesToColor, node_color = self.__randomColor())
        overlappingNodes = list(overlappingNodes)
        if len(edgeCommunities) != 0:
            nx.draw_networkx_edges(self.__graph.graph, pos, edgelist=edgeCommunities, edge_color="black", arrows=True)
        if len(overlappingNodes) != 0:
            nx.draw_networkx_nodes(self.__graph.graph, pos, cmap=plt.get_cmap('jet'), nodelist=overlappingNodes, node_color = self.__randomColor())
        plainEdges = [edge for edge in self.__graph.graph.edges() if edge not in edgeCommunities]
        nx.draw_networkx_nodes(self.__graph.graph, pos, cmap=plt.get_cmap('jet'), nodelist=overlappingNodes, node_color = self.__randomColor())
        nx.draw_networkx_edges(self.__graph.graph, pos, edgelist=plainEdges, edge_color=self.__randomColor(), arrows=True)
        nx.draw_networkx_labels(self.__graph.graph, pos)
        print("Graph generated.")
        print("Graph will pop up in a window. Exit the window to continue...")
        plt.show()
        print("Graph is closed.")
