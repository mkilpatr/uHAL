#!/bin/bash
#initial setup conditions to run the code
#before each session if you want to use the AMC13 integration you need to include the source
source /home/fectest/FEDtester/MrPixel/Desktop/amc13/env.sh

export LD_LIBRARY_PATH=/opt/cactus/lib:$LD_LIBRARY_PATH
export PATH=/opt/cactus/bin:$PATH

export LD_LIBRARY_PATH=/home/fectest/FEDtester/MrPixel/build/ttctest/lib:$LD_LIBRARY_PATH
export PATH=/home/fectest/FEDtester/MrPixel/build/ttctest/bin:$PATH

#export LS_LIBRARY_PATH=/usr/lib64/root:$LD_LIBRARY_PATH
#export PATH=/usr/bin/root:$PATH

