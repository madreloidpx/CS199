import subprocess
from pathlib import Path

def getFullPath(_dir):
    _dir = Path(_dir)
    if Path.exists(_dir) == False:
        print("Path", _dir, "does not exist.")
        exit(0)
    return Path.absolute(_dir)

NMIPATH = getFullPath("./Overlapping-NMI")

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

def getNMI(**kwargs):
    communityFile = kwargs.get("communityFile")
    trueCommunityFile = kwargs.get("trueCommunityFile")
    if (communityFile and trueCommunityFile) == None:
        print("Missing args.")
        exit(0)
    process = subprocess.Popen(["./onmi", getFullPath(communityFile), getFullPath(trueCommunityFile)],  cwd=NMIPATH, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, err = process.communicate()
    if kwargs.get("filename") == None:
        print((output).decode('ascii'))
    else:
        saveNMIData(kwargs.get("filename"), (output).decode('ascii'))

def saveNMIData(filename, data):
    f = open(filename, "w")
    f.write(data)
    f.close()