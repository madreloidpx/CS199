import subprocess
from pathlib import Path

def getFullPath(_dir):
    _dir = Path(_dir)
    if Path.exists(_dir) == False:
        print("Path", _dir, "does not exist.")
        exit(0)
    return Path.absolute(_dir)

NMIPATH = getFullPath("./Overlapping-NMI")

def iterThreshold(graph, **kwargs):
    try:
        step = kwargs.get("step") or 0.1
        start = kwargs.get("start") or 0
        end = kwargs.get("end") or 1
    except:
        print("Invalid parameters.", kwargs)
        exit(0)
    step = float(step)
    start = float(start)
    end = float(end)
    multiplier = 1
    while int(step * multiplier) != step*multiplier:
        multiplier = multiplier * 10
    outData = ""
    filename = kwargs.get("filename") or "nmi.txt"
    for i in range(int(start * multiplier), int(end * multiplier)+1, int(step * multiplier)):
        threshold = i/multiplier
        graph.setThreshold(threshold)
        graph.showGraphSpecificThreshold()
        outData = outData + "Threshold: " + str(threshold) + "\n"
        kwargs.update({"communityFile": filename})
        outData = outData + getNMI(**kwargs)
    saveNMIData(filename, outData)

def removeFilename(**kwargs):
    newKwargs = dict(kwargs)
    del newKwargs["filename"]
    return newKwargs

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
        return (output).decode('ascii')

def saveNMIData(filename, data):
    f = open(filename, "w")
    f.write(data)
    f.close()