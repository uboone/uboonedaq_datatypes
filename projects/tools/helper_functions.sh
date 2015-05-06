#!/bin/bash
# Setup UPS
source /uboone/setup


function setup_privateups_repository
{
if [ -z "${PRIVATE_UPS_DIR}" ]; then
echo Error: PRIVATE_UPS_DIR is not set. 
echo Set it with \"export PRIVATE_UPS_DIR=${HOME}/install/privateups\" and rerun this command.

else
echo Setting up a private UPS repository in ${PRIVATE_UPS_DIR}

THIS_DIR=`pwd`
source /uboone/setup

setup ups
#configure a private ups repository for development
#needs to be done once
rm -rf $PRIVATE_UPS_DIR; mkdir -p $PRIVATE_UPS_DIR/ups; mkdir -p $PRIVATE_UPS_DIR/upd ; setup upd
TMP_STR=`which ups` && cp -ra ${TMP_STR%/Linux*}*  $PRIVATE_UPS_DIR/ups/
TMP_STR=`which ups` && cp -ra ${TMP_STR%/Linux*}/../current.chain  $PRIVATE_UPS_DIR/ups/
TMP_STR=`which ups` && cp -ra ${TMP_STR%/Linux*}/../../setup  $PRIVATE_UPS_DIR/
TMP_STR=`which ups` && cp -ra ${TMP_STR%/Linux*}/../../.upsfiles  $PRIVATE_UPS_DIR/
TMP_STR=`which upd` && cp -ra ${TMP_STR%/NULL*}*  $PRIVATE_UPS_DIR/upd/
#TMP_STR=`which upd` && cp -ra ${TMP_STR%/NULL*}/../current.chain  $PRIVATE_UPS_DIR/upd/
TMP_STR=`find $PRIVATE_UPS_DIR/ups/ -name buildUps.sh` && cd ${TMP_STR%/buildUps.sh*} && $TMP_STR
source ${PRIVATE_UPS_DIR}/setup

cd $THIS_DIR
fi
}

