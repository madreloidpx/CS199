import networkx as nx
import matplotlib.pyplot as plt
import random

def readFile(filename, delimiter=","):
    f = open(filename, "r")
    data = []
    for line in f:
        contents = line.split(delimiter)
        for i in range(len(contents)):
                contents[i] = contents[i].strip()
        data.append(contents)
    return data

def createEdgeData(data):
    edges = []
    for line in data:
        if len(line) == 2: edges.append((line[0], line[1]))
        if len(line) == 3: edges.append((line[0], line[1], line[2]))
    return edges

def createUndirectedGraph(graphData):
    print("Undirected Network")
    G = nx.Graph()
    G.add_edges_from(graphData["edge_data"])
    return G

def createMultiDiGraph(graphData):
    print("Directed Network")
    G = nx.MultiDiGraph()
    G.add_edges_from(graphData["edge_data"])
    return G

def createGraph(graphData, mode="undirected"):
    chooseMode = {
        "undirected": createUndirectedGraph,
        "directed": createMultiDiGraph,
    }
    func = chooseMode.get(mode, lambda: "Invalid network type")
    return func(graphData)

def getNodeStrength(graph, node):
    neighbors = nx.neighbors(graph, node)
    all_neighbors = nx.all_neighbors(graph, node)
    return len(list(all_neighbors)) - len(list(neighbors))

def weakClique(graph, nodePair):
    source, drain = nodePair
    neighborSource = list(nx.neighbors(graph, source))
    neighborDrain = list(nx.neighbors(graph, drain))
    allNeighborSource = list(nx.all_neighbors(graph, source))
    incomingLinkSource = [node for node in allNeighborSource if node not in neighborSource]
    commonNeighbors = [node for node in neighborDrain if node in incomingLinkSource]
    return [source] + commonNeighbors + [drain]

def getOutsideCommunityLinks(graph, weakClique):
    outsideLinks = []
    for node in weakClique:
        neighbors = list(nx.neighbors(graph, node))
        for n in neighbors:
            if n not in weakClique and n not in outsideLinks:
                outsideLinks.append(n)
    return outsideLinks

def getCommonNodes(u, v):
    commonNodes = []
    for node in u:
        if node in v:
            commonNodes.append(node)
    return commonNodes

def shouldWeakCliqueMerge(graph, WQu, WQv, threshold):
    commonNodes = getCommonNodes(WQu, WQv)
    if len(commonNodes) == 0:
        return False
    outWQu = getOutsideCommunityLinks(graph, WQu)
    outWQv = getOutsideCommunityLinks(graph, WQv)
    outWQuInWQv = getCommonNodes(outWQu, WQv)
    outWQvInWQu = getCommonNodes(outWQv, WQu)
    mergeComputation = (len(commonNodes) + (len(outWQuInWQv) + len(outWQvInWQu))/2)/max(len(WQu), len(WQv))
    if(threshold <= mergeComputation):
        return True
    return False

def weakCliqueMerge(WQu, WQv):
    diff = set(WQv) - set(WQu)
    return list(set(WQu).union(diff))

def runMerge(graph, weakCliques, threshold, communities = []):
    if len(weakCliques) == 0:
        return communities
    currentWQ = weakCliques.pop(0)
    isCommunity = True
    for wq in weakCliques:
        if shouldWeakCliqueMerge(graph, currentWQ, wq, threshold) == True:
            currentWQ = weakCliqueMerge(currentWQ, wq)
            weakCliques.remove(wq)
            isCommunity = False
    if isCommunity == False:
        weakCliques.insert(0, currentWQ)
    else:
        communities.append(currentWQ)
    return runMerge(graph, weakCliques, threshold, communities)

def colorGraph(graph, communities):
    overlappingNodes = set()
    edgeCommunities = []
    pos = nx.spring_layout(graph)
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
        nx.draw_networkx_nodes(graph, pos, cmap=plt.get_cmap('jet'), nodelist=nodesToColor, node_color = randomColor())
    overlappingNodes = list(overlappingNodes)
    if len(edgeCommunities) != 0:
        nx.draw_networkx_edges(graph, pos, edgelist=edgeCommunities, edge_color=randomColor(), arrows=True)
    if len(overlappingNodes) != 0:
        nx.draw_networkx_nodes(graph, pos, cmap=plt.get_cmap('jet'), nodelist=overlappingNodes, node_color = randomColor())
    plainEdges = [edge for edge in graph.edges() if edge not in edgeCommunities]
    nx.draw_networkx_nodes(graph, pos, cmap=plt.get_cmap('jet'), nodelist=overlappingNodes, node_color = randomColor())
    nx.draw_networkx_edges(graph, pos, edgelist=plainEdges, edge_color=randomColor(), arrows=True)
    nx.draw_networkx_labels(graph, pos)
    plt.show()
    return graph

def randomColor():
    color = lambda: random.randint(0,255)
    return '#%02X%02X%02X' % (color(),color(),color())

if __name__ == '__main__':
    data = readFile("test3.txt", '\t')
    edgeData = createEdgeData(data)
    graphData = {"edge_data": edgeData}
    graph = createGraph(graphData, "directed")
    nodes = list(nx.nodes(graph))
    nodes = sorted(nodes, key=lambda node: getNodeStrength(graph, node), reverse=True)
    nodePairs = []
    for node in nodes:
        for neighbor in nx.neighbors(graph, node):
            nodePairs.append((node, neighbor))
    weakCliques = [wq for wq in [weakClique(graph, np) for np in nodePairs]]
    weakCliques = sorted(weakCliques, key=len, reverse=True)
    for wq in range(len(weakCliques)):
        for i in weakCliques[wq+1:]:
            if all(node in weakCliques[wq] for node in i):
                weakCliques.remove(i)
    print("weak cliques:", weakCliques)
    communities = runMerge(graph, weakCliques, 0.7)
    print("communities:", communities)
    graph = colorGraph(graph, communities)