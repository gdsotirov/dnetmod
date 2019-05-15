#!/bin/sh
# Script to download and prepare dependencies for DNetMod (see README)

CWD=$(pwd)

# Check necessary programs
WGET=$(which wget)

if [ "${WGET}" == "" ]; then
  echo "Error: Program 'wget' not found!"
  exit 1
fi

UNZIP=$(which unzip)

if [ "${UNZIP}" == "" ]; then
  echo "Error: Program 'unzip' not found!"
  exit 2
fi

# Download driver archives
CIF_DRV_LNX_DWNLD_URL="https://www.hilscher.com/fileadmin/big_data/en-US/Resources/zip/DRV-Linux_CD_2014-08-1_V3_0_0_0.zip"
CIF_DRV_WIN_DWNLD_URL="https://www.hilscher.com/fileadmin/big_data/en-US/Resources/zip/DRV-WIN-CD_2014-01-1_V3_2_10_0.zip"
CIF_DRV_LNX_ARCH_NAME=$(basename ${CIF_DRV_LNX_DWNLD_URL})
CIF_DRV_WIN_ARCH_NAME=$(basename ${CIF_DRV_WIN_DWNLD_URL})
CIF_DRV_LNX_TMP_DIR=DRV-LNX
CIF_DRV_WIN_TMP_DIR=DRV-WIN

${WGET} "${CIF_DRV_LNX_DWNLD_URL}" -O "${CIF_DRV_LNX_ARCH_NAME}"
${WGET} "${CIF_DRV_WIN_DWNLD_URL}" -O "${CIF_DRV_WIN_ARCH_NAME}"

# Unpack driver archives
mkdir -p "${CIF_DRV_LNX_TMP_DIR}" "${CIF_DRV_WIN_TMP_DIR}"

${UNZIP} "${CIF_DRV_LNX_ARCH_NAME}" -d "${CIF_DRV_LNX_TMP_DIR}"
${UNZIP} "${CIF_DRV_WIN_ARCH_NAME}" -d "${CIF_DRV_WIN_TMP_DIR}"

tar -xvf "${CIF_DRV_LNX_TMP_DIR}/cif3000.txz"

# Prepare driver
CIF_DRV_LNX_DIR="cif3.000"

# - move DNM_USER.H
mv "${CIF_DRV_WIN_TMP_DIR}/DrvPrg/HEADER/DNM_USER.H" "${CIF_DRV_LNX_DIR}/usr-inc/dnm_user.h"
# - convert DOS to UNIX
tr -d '\r' < "${CIF_DRV_LNX_DIR}/usr-inc/cif_user.h" > "${CIF_DRV_LNX_DIR}/usr-inc/cif_user.h.unix"
mv "${CIF_DRV_LNX_DIR}/usr-inc/cif_user.h.unix" "${CIF_DRV_LNX_DIR}/usr-inc/cif_user.h"
tr -d '\r' < "${CIF_DRV_LNX_DIR}/usr-inc/dnm_user.h" > "${CIF_DRV_LNX_DIR}/usr-inc/dnm_user.h.unix"
mv "${CIF_DRV_LNX_DIR}/usr-inc/dnm_user.h.unix" "${CIF_DRV_LNX_DIR}/usr-inc/dnm_user.h"
tr -d '\r' < "${CIF_DRV_LNX_DIR}/usr-inc/rcs_user.h" > "${CIF_DRV_LNX_DIR}/usr-inc/rcs_user.h.unix"
mv "${CIF_DRV_LNX_DIR}/usr-inc/rcs_user.h.unix" "${CIF_DRV_LNX_DIR}/usr-inc/rcs_user.h"
# - patch
patch "${CIF_DRV_LNX_DIR}/usr-inc/dnm_user.h" dnm_user.h.diff
patch "${CIF_DRV_LNX_DIR}/usr-inc/rcs_user.h" rcs_user.h.diff

# Build driver
sed -i -r 's/^CC\s+\=.+$/CC = gcc -Wall -fPIC/' "${CIF_DRV_LNX_DIR}/api/Makefile"
cd ${CIF_DRV_LNX_DIR}/api; make; cd "${CWD}"

# Cleanup
rm -rf "${CIF_DRV_LNX_TMP_DIR}" "${CIF_DRV_WIN_TMP_DIR}"
rm -f "${CIF_DRV_LNX_ARCH_NAME}" "${CIF_DRV_WIN_ARCH_NAME}"

