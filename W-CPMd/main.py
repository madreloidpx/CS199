import sys
import wcpmd

def help():
    print("""
    To run interactive CLI, run 'python3 main.py'.
    Note: CLI currently does not have an NMI checker.
    
    -h, --help, help: run help

    generateCommunity: generates a community file data given the graph data and a threshold.
    -gd: Required. Accepts graph data directory. Currently only accepts tabs as delimiter.
    -t: Required. Sets the threshold.
    -rl: Sets recursion limit.
    -out: Filename for resulting community data. Default is 'community.txt'

    """)

def getMode(_mode):
    mode = {
        "generateCommunity": generateCommunity
    }
    return mode.get(_mode)

def generateCommunity(commands):
    print("generate community")

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