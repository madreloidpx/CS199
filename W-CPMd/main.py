import networkx as nx
import matplotlib.pyplot as plt
import random
import sys
import graph
import subprocess

def readFile(filename, delimiter=","):
    f = open(filename, "r")
    data = []
    for line in f:
        contents = line.split(delimiter)
        for i in range(len(contents)):
                contents[i] = contents[i].strip()
        data.append(contents)
    f.close()
    return data

def createEdgeData(data):
    edges = []
    for line in data:
        if len(line) == 2: edges.append((line[0], line[1]))
        if len(line) == 3: edges.append((line[0], line[1], line[2]))
    return edges

def createGraph(graph, communities):
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
        nx.draw_networkx_edges(graph, pos, edgelist=edgeCommunities, edge_color="black", arrows=True)
    if len(overlappingNodes) != 0:
        nx.draw_networkx_nodes(graph, pos, cmap=plt.get_cmap('jet'), nodelist=overlappingNodes, node_color = randomColor())
    plainEdges = [edge for edge in graph.edges() if edge not in edgeCommunities]
    nx.draw_networkx_nodes(graph, pos, cmap=plt.get_cmap('jet'), nodelist=overlappingNodes, node_color = randomColor())
    nx.draw_networkx_edges(graph, pos, edgelist=plainEdges, edge_color=randomColor(), arrows=True)
    nx.draw_networkx_labels(graph, pos)
    plt.show()

def saveToTxt(communities):
    filename = "communities.txt"
    content = ""
    for i in range(len(communities)):
        for node in communities[i]:
            content = content + node + "\t" + str(i+1) + "\n"
    content = content[:-1]
    f = open(filename, "w")
    f.write(content)
    f.close()

def randomColor():
    color = lambda: random.randint(0,255)
    return '#%02X%02X%02X' % (color(),color(),color())

if __name__ == '__main__':
    sys.setrecursionlimit(2000)
    data = readFile("network.nsa", '\t')
    edgeData = createEdgeData(data)
    graphData = {"edge_data": edgeData}
    G = graph.Graph(graphData, "directed")
    threshold = 0.1775
    communities = G.getCommunities(threshold)
    print("Threshold:", threshold, "Number of communities:", len(communities))
    print(communities)
    saveToTxt(communities)
    process = subprocess.Popen(["./onmi", "../communities.txt", "../community.dat"],  cwd="./Overlapping-NMI", stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, err = process.communicate()
    print((output).decode('ascii'))
    createGraph(G.graph, communities)