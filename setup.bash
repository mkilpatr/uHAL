#!/bin/bash
#initial setup conditions to run the code
#before each session if you want to use the AMC13 integration you need to include the source
if [ ! -d "lib" ]
then
    echo 'creating lib!'
    mkdir lib
fi

if [ ! -d "bin" ]
then
    echo 'creating bin!'
    mkdir bin
fi

#CACTUS
export CACTUSBIN=/opt/cactus/bin
export CACTUSLIB=/opt/cactus/lib
export CACTUSINCLUDE=/opt/cactus/include

# BOOST
export BOOST_LIB=/opt/cactus/lib
export BOOST_INCLUDE=/opt/cactus/include

#FED Burnin
export BASE_DIR=$(pwd)

export PATH=$BASE_DIR/bin:$PATH
export LD_LIBRARY_PATH=$CACTUSLIB:$BASE_DIR/lib:${LD_LIBRARY_PATH}:/opt/root_v5.34.14.source/lib
#export PATH=/usr/bin:/usr/local/bin:~/bin:${CACTUSBIN}:${AMC13BIN}:$PATH
export PATH=${CACTUSBIN}:${AMC13BIN}:$PATH
export ROOTSYS=/opt/root_v5.34.14.source

cd ../../Desktop/amc13
source env.sh
cd -

export AMC13_STANDALONE_ROOT CACTUS_ROOT PATH LD_LIBRARY_PATH PYTHONPATH

export LD_LIBRARY_PATH=/opt/cactus/lib:$LD_LIBRARY_PATH
export PATH=/opt/cactus/bin:$PATH

export LD_LIBRARY_PATH=/home/fectest/FEDtester/MrPixel/build/ttctest/lib:$LD_LIBRARY_PATH
export PATH=/home/fectest/FEDtester/MrPixel/build/ttctest/bin:$PATH

export LD_LIBRARY_PATH=${XDAQ_ROOT}/lib/:${ROOTSYS}/lib:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=/usr/lib64:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=/usr/lib64/root:${LD_LIBRARY_PATH}

cd $ROOTSYS
source bin/thisroot.sh
cd -


#export LS_LIBRARY_PATH=/usr/lib64/root:$LD_LIBRARY_PATH
#export PATH=/usr/bin/root:$PATH

