Directory for external libraries

* CIF Device Driver

Installation (Linux)
------------------------------------------------------------------------------
1. Download archive from https://www.hilscher.com/products/product-groups/software/cif-device-driver/drv-lnx/ ,
   click Downloads, CIF Device Driver for Linux Kernel 2.6.31/3.x,
   Development CD (DRV-Linux) (i.e. DRV-Linux_CD_2014-08-1_V3_0_0_0.zip).
   Unpack with unzip DRV-Linux_CD_2014-08-1_V3_0_0_0.zip, then tar -xvf cif3000.txz
2. Download and unzip the windows driver (see below) and move file
   DrvPrg/HEADER/DNM_USER.H to cif3.000/usr-inc/dnm_user.h
3. Go to cif3.000/api, modify Makefile and add -fPIC flag to CC
4. Run make (to generate cif_api.o object)
5. Convert cif_user.h and dnm_user.h to UNIX format and apply patches:
   patch cif3.000/usr-inc/dnm_user.h dnm_user.h.diff 
   patch cif3.000/usr-inc/rcs_user.h rcs_user.h.diff 

Installation (Windows)
------------------------------------------------------------------------------
1. Download archive from https://www.hilscher.com/products/product-groups/software/cif-device-driver/drv-win/ ,
   click Downloads, CIF Device Driver for Windows 10/8/7/Vista/XP/2000,
   Development CD (DRV-WIN) (i.e. DRV-WIN-CD_2014-01-1_V3_2_10_0.zip).
2. Unzip with unzip DRV-WIN-CD_2014-01-1_V3_2_10_0.zip
3. Go to Win_2K_XP_VISTA_7_8 and start installation


* NI-DNET driver

Installation (Windows)
------------------------------------------------------------------------------
1. Download installation file from http://www.ni.com/en-rs/support/downloads/drivers/download.ni-dnet.html
   (i.e. NI-DNET167.exe)
2. Start the installation

