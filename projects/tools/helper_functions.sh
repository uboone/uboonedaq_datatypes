#!/bin/bash
# Setup UPS

function setup_privateups_repository
{
if [ -z "${PRIVATE_UPS_DIR}" ]; then
echo Error: PRIVATE_UPS_DIR is not set. 
echo Set it with \"export PRIVATE_UPS_DIR=${HOME}/install\" and rerun this command.


else
echo Setting up a private UPS repository in ${PRIVATE_UPS_DIR}

THIS_DIR=`pwd`

#configure a private ups repository for development
#needs to be done once
rm -rf $PRIVATE_UPS_DIR; mkdir -p  $PRIVATE_UPS_DIR; cd $PRIVATE_UPS_DIR;
ls -tr `find /uboone*/dist* -name ups-*Linux*.tar.bz2` |tail -1 |xargs -n 1 -I {} tar jxf {}
source ${PRIVATE_UPS_DIR}/setup
cd $THIS_DIR
fi
}

