import networkx as nx
from copy import deepcopy

class Graph:
    def __init__(self, graphData, mode):
        self.graph = None
        self.mode = mode
        self.nodes = None
        self.edges = None
        self.weakCliques = None

        chooseMode = { #add more modes in the future
            "directed": self.createMultiDiGraph,
        }
        func = chooseMode.get(self.mode, lambda: "Invalid network type")
        func(graphData)
        self.addAndSortNodes()
        self.addEdgesandWeakCliques()

    def createMultiDiGraph(self, graphData):
        print("Directed Network")
        G = nx.MultiDiGraph()
        G.add_edges_from(graphData["edge_data"])
        self.graph = G
    
    def addAndSortNodes(self):
        nodes = list(nx.nodes(self.graph))
        self.nodes = sorted(nodes, key=lambda node: self.getNodeStrength(node), reverse=True)
    
    def addEdgesandWeakCliques(self):
        edges = []
        for node in self.nodes:
            for neighbor in nx.neighbors(self.graph, node):
                edges.append((node, neighbor))
        weakCliques = [wq for wq in [self.weakClique(edge) for edge in edges]]
        weakCliques = sorted(weakCliques, key=len, reverse=True)
        for wq in range(len(weakCliques)):
            for i in weakCliques[wq+1:]:
                if all(node in weakCliques[wq] for node in i):
                    weakCliques.remove(i)
        self.weakCliques = weakCliques
        self.edges = edges

    def getNodeStrength(self, node):
        neighbors = nx.neighbors(self.graph, node)
        all_neighbors = nx.all_neighbors(self.graph, node)
        return len(list(all_neighbors)) - len(list(neighbors))
    
    def weakClique(self, edge):
        source, drain = edge
        neighborSource = list(nx.neighbors(self.graph, source))
        neighborDrain = list(nx.neighbors(self.graph, drain))
        allNeighborSource = list(nx.all_neighbors(self.graph, source))
        incomingLinkSource = [node for node in allNeighborSource if node not in neighborSource]
        commonNeighbors = [node for node in neighborDrain if node in incomingLinkSource]
        return [source] + commonNeighbors + [drain]
    
    def communityAssociation(self, communities):
        comSet = {"communities": len(communities)}
        for i in range(len(communities)):
            for node in communities[i]:
                if comSet.get(node) == None:
                    comSet[node] = [i+1]
                else:
                    comSet[node].append(i+1)
        return comSet

    def getCommunities(self, threshold, communities = [], wq=None):
        if wq == None:
            wq = deepcopy(self.weakCliques)
            communities = []
        if len(wq) == 0:
            return self.communityAssociation(communities)
        currentWQ = wq.pop(0)
        isCommunity = True
        try:
            for i in range(len(wq)):
                if self.shouldWeakCliqueMerge(currentWQ, wq[i], threshold) == True:
                    currentWQ = self.weakCliqueMerge(currentWQ, wq[i])
                    popped = wq.pop(i)
                    i = i-1
                    isCommunity = False
        except IndexError:
            pass
        except Exception as error:
            print(error)
        if isCommunity == False:
            wq.insert(0, currentWQ)
        elif len(currentWQ) < 3:
            pass
        else:
            communities.append(currentWQ)
        return self.getCommunities(threshold, communities, wq)
    
    def shouldWeakCliqueMerge(self, WQu, WQv, threshold):
        commonNodes = self.getCommonNodes(WQu, WQv)
        if len(commonNodes) == 0:
            return False
        outWQu = self.getOutsideCommunityLinks(WQu)
        outWQv = self.getOutsideCommunityLinks(WQv)
        outWQuInWQv = self.getCommonNodes(outWQu, WQv)
        outWQvInWQu = self.getCommonNodes(outWQv, WQu)
        mergeComputation = (len(commonNodes) + (len(outWQuInWQv) + len(outWQvInWQu))/2)/max(len(WQu), len(WQv))
        if(threshold <= mergeComputation):
            return True
        return False
    
    def getOutsideCommunityLinks(self, weakClique):
        outsideLinks = []
        for node in weakClique:
            neighbors = list(nx.neighbors(self.graph, node))
            for n in neighbors:
                if n not in weakClique and n not in outsideLinks:
                    outsideLinks.append(n)
        return outsideLinks
    
    def getCommonNodes(self, u, v):
        commonNodes = []
        for node in u:
            if node in v:
                commonNodes.append(node)
        return commonNodes
    
    def weakCliqueMerge(self, WQu, WQv):
        diff = set(WQv) - set(WQu)
        return list(set(WQu).union(diff))
