#!/bin/bash
# perform valring run to gather statistics, then conver them do some dot format and then to png
# finally display it
valgrind --tool=callgrind ./sim 
./gprof2dot.py --format=callgrind --output=out.dot `ls --sort=time callgrind.* | head -1`
dot -Tpng out.dot -o graph.png
geeqie graph.png
