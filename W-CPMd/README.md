How to install (Linux): <br />
1. Install networkx <br />
'pip3 install networkx' <br />
2. Save network and community data in tests with a filename "network"+i".dat" or "community"+i+".dat" where i is a number. <br />
3. To iterate the network tests to search for a threshold to use, run 'getNMIThresholds.sh' with the range of networks to test and the recursion limit for python to use. <br />
ex: "./getNMIThresholds 1 3 10000" <br />
4. For more help running, run 'python3 main.py help" <br />
5. To use the CLI, run "python3 main.py". <br />
