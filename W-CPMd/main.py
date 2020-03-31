import subprocess

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