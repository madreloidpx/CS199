import sys
import inspect
import wcpmd

def help():
    print("""
    To run interactive CLI, run 'python3 main.py'.
    Note: CLI currently does not have an NMI checker.
    
    -h, --help, help: run help

    generateCommunity: generates a community file data given the graph data and a threshold.
    --run: Runs the CLI for generating communities and accepts user input.
    -gd: Required. Accepts graph data directory. Currently only accepts tabs as delimiter.
    -t: Required. Sets the threshold.
    -rl: Sets recursion limit.
    -out: Filename for resulting community data. Default is 'community.txt'

    getCommunityNMI: gets the nmi of a community file based on its true community file.
    -cf: Required. Accepts the community file directory
    -tcf: Required. Accepts the true community file directory
    -out: Filename for results file. If not provided, outputs the results in the terminal.
    Note: Currently dependent in 'Overlapping-NMI' in getting the nmi results.

    """)

def getMode(_mode):
    mode = {
        "generateCommunity": generateCommunity,
        "getCommunityNMI": getCommunityNMI,
    }
    return mode.get(_mode)

def generateCommunity(commands, **kwargs):
    currCommand = commands.pop(0)
    func = getGenerateCommunityModes(currCommand)
    if func == None:
        print("Command", currCommand, "does not exist in generateCommunity.")
        exit(0)
    argsNeeded = len(inspect.getargspec(func).args)
    if argsNeeded == 0:
        func()
        return
    elif argsNeeded == 1:
        try:
            arg = commands.pop(0)
            if arg[0] == "-":
                raise Exception
            kwargs.update(func(arg))
        except:
            print("Missing arguments.")
            exit(0)
    if len(commands) != 0:
        generateCommunity(commands, **kwargs)
    else:
        kwargs.update({ "auto": True })
        run = wcpmd.WCPMD(**kwargs)
        run.initialize()
        run.showGraphSpecificThreshold()

def getGenerateCommunityModes(_mode):
    mode = {
        "--run": runGenerateCommunity,
        "-gd": setGraphData,
        "-t": setThreshold,
        "-rl": setRecursionLimit,
        "-out": setFilename,
    }
    return mode.get(_mode)

def runGenerateCommunity():
    run = wcpmd.WCPMD()
    run.initialize()
    run.showGraphSpecificThreshold()

def setGraphData(filedir):
    return { "graphDir": filedir }

def setThreshold(threshold):
    return { "threshold": threshold }

def setRecursionLimit(recursionLimit):
    return { "recursionLimit": recursionLimit }

def setFilename(filename):
    return { "filename": filename }

def getCommunityNMI(commands):
    pass

if __name__ == '__main__':
    if len(sys.argv) == 1:
        wcpmd.WCPMD().run()
    elif len(sys.argv) == 2 and (sys.argv[1] == "-h" or sys.argv[1] == "--help" or sys.argv[1] == "help"):
        help()
    else:
        func = getMode(sys.argv[1])
        if func == None:
            print("Invalid command.")
            exit(0)
        commands = sys.argv[2:]
        func(commands)