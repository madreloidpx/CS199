import networkx as nx
import matplotlib.pyplot as plt

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
    print(graphData)
    G = nx.Graph()
    G.add_edges_from(graphData["edge_data"])
    return G

def createMultiDiGraph(graphData):
    print("Directed Network")
    print(graphData)
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

def showGraph(graph):
    pos = nx.spring_layout(graph)
    nx.draw_networkx_nodes(graph, pos, cmap=plt.get_cmap('jet'), node_size = 500)
    nx.draw_networkx_labels(graph, pos)
    nx.draw_networkx_edges(graph, pos, edgelist=graph.edges(), arrows=True)
    plt.show()

def getNodeStrength(graph, node):
    neighbors = nx.neighbors(graph, node)
    all_neighbors = nx.all_neighbors(graph, node)
    return len(list(all_neighbors)) - len(list(neighbors))

def weakClique(graph, source, drain):
    neighborSource = list(nx.neighbors(graph, source))
    neighborDrain = list(nx.neighbors(graph, drain))
    allNeighborSource = list(nx.all_neighbors(graph, source))
    incomingLinkSource = [node for node in allNeighborSource if node not in neighborSource]
    commonNeighbors = [node for node in neighborDrain if node in incomingLinkSource]
    return [source] + commonNeighbors + [drain]

def weakCliqueSize(graph, nodePair):
    source, drain = nodePair
    return len(weakClique(graph, source, drain))

if __name__ == '__main__':
    data = readFile("test.txt", '\t')
    edgeData = createEdgeData(data)
    graphData = {"edge_data": edgeData}
    graph = createGraph(graphData, "directed")
    nodes = list(nx.nodes(graph))
    nodes = sorted(nodes, key=lambda node: getNodeStrength(graph, node), reverse=True)
    nodePairs = []
    for node in nodes:
        for neighbor in nx.neighbors(graph, node):
            nodePairs.append((node, neighbor))
    nodePairs = sorted(nodePairs, key=lambda nodePair: weakCliqueSize(graph, nodePair), reverse=True)
    print(nodePairs)
    # for i in range(1, len(nodes)):
    #     print(numCommonNeighbors(graph, nodes[0], nodes[i]))
    # for node in nodes:
    #     print("neighbors", node)
    #     for i in nx.neighbors(graph, node): #only those pointing to
    #         print(i)
    #     print("all neighbors", node)
    #     for i in nx.all_neighbors(graph, node): #opposite from directed included
    #         print(i)
    #     print("non neighbors", node)
    #     for i in nx.non_neighbors(graph, node): #only non neighbors from pointing
    #         print(i)
    #     print("node strength", node, getNodeStrength(graph, node)) #get node strength
    showGraph(graph)