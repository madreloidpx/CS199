import networkx as nx
from copy import deepcopy

class Graph:
    def __init__(self, graphData, mode):
        self.graph = None
        self.mode = mode
        self.nodes = None
        # self.edges = None
        self.weakCliques = []

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
        for node in self.nodes:
            edges = []
            for neighbor in nx.neighbors(self.graph, node):
                edges.append((node, neighbor))
            weakCliques = [wq for wq in [self.weakClique(edge) for edge in edges]]
            weakCliques = sorted(weakCliques, key=len, reverse=True)
            self.weakCliques.extend(weakCliques)
        # weakCliques = sorted(weakCliques, key=len, reverse=True)
        # for wq in range(len(weakCliques)):
        #     for i in weakCliques[wq+1:]:
        #         if all(node in weakCliques[wq] for node in i):
        #             weakCliques.remove(i)
        # self.weakCliques = weakCliques
        # self.edges = edges

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
    
    def getCommunities(self, threshold, communities = None):
        if communities == None:
            communities = self.reduceCommunities(threshold)
        currCommunities = self.reduceCommunities(threshold, [], deepcopy(communities))
        print("Communities:", len(communities), communities)
        print("CurrCommunities:", len(currCommunities), currCommunities)
        print(len(currCommunities) < len(communities))
        if len(currCommunities) < len(communities) and len(communities) != 0:
            print("Rerun")
            return self.getCommunities(threshold, currCommunities)
        else:
            return self.communityAssociation(currCommunities)


    def reduceCommunities(self, threshold, communities = [], wq=None):
        if wq == None:
            wq = deepcopy(self.weakCliques)
            communities = []
        if len(wq) == 0:
            return communities
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
        return self.reduceCommunities(threshold, communities, wq)
    
    def shouldWeakCliqueMerge(self, WQu, WQv, threshold):
        print("Checking at t =", threshold, WQu, WQv)
        commonNodes = self.getCommonNodes(WQu, WQv)
        if len(commonNodes) == 0:
            return False
        if len(commonNodes) == len(WQu) or len(commonNodes) == len(WQv):
            return True
        outWQu = self.getOutsideCommunityLinks(WQu)
        outWQv = self.getOutsideCommunityLinks(WQv)
        outWQuInWQv = self.getCommonNodes(outWQu, WQv)
        outWQvInWQu = self.getCommonNodes(outWQv, WQu)
        print("common nodes:", commonNodes)
        print("out WQu", WQu, ">>>>", outWQu)
        print("out WQv", WQv, ">>>>", outWQv)
        print("out WQu in WQv >>>>", outWQuInWQv)
        print("out WQv in WQu >>>>", outWQvInWQu)
        mergeComputation = (len(commonNodes) + (len(outWQuInWQv) + len(outWQvInWQu)/2))/max(len(WQu), len(WQv))
        print("threshold:", threshold, "computation:", mergeComputation, "threshold < mergeComputation:", threshold < mergeComputation)
        if(threshold < mergeComputation):
            return True
        return False
    
    # def nodeRelation(self, x, y):
    #     if x == 0 or y == 0:
    #         return 0
    #     return 1 - (x*x*x + x*x * y + x * y*y + y*y*y)/(x*x*x * y + 2 * x*x * y*y + x * y*y*y)

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
