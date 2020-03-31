import subprocess

def saveToTxt(communities, filename="communities.txt"):
    content = ""
    for i in range(len(communities)):
        for node in communities[i]:
            content = content + node + "\t" + str(i+1) + "\n"
    content = content[:-1]
    f = open(filename, "w")
    f.write(content)
    f.close()

def iterThreshold(graph, step=0.1, start=0, end=1):
    multiplier = 1
    while int(step * multiplier) != step*multiplier:
        multiplier = multiplier * 10
    for i in range(int(start * multiplier), int(end * multiplier)+1, int(step * multiplier)):
        threshold = i/multiplier
        communities = graph.getCommunities(threshold)
        print("Threshold:", threshold, "Number of communities:", len(communities))
        communityFile = "communities.txt"
        trueCommunityFile = "community.dat"
        saveToTxt(communities, communityFile)
        getNMI(communityFile, trueCommunityFile)

def getNMI(communityFile, trueCommunityFile):
    process = subprocess.Popen(["./onmi", "../" + communityFile, "../" + trueCommunityFile],  cwd="./Overlapping-NMI", stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, err = process.communicate()
    print((output).decode('ascii'))

if __name__ == '__main__':
    iterThreshold(G)
    # createGraph(G.graph, communities)