TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application


PROD_IOC = prng
DBD += prng.dbd
prng_INCLUDES += -I/usr/local/lib

# prng.dbd will be created and installed

# prng.dbd will be made up from these files:
prng_DBD += base.dbd
prng_DBD += bpmPosCalc.dbd
prng_DBD += prngdev.dbd
# Include dbd files from all support applications:
#prng_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#prng_LIBS += xxx
# prng_registerRecordDeviceDriver.cpp derives from prng.dbd
prng_SRCS += prng_registerRecordDeviceDriver.cpp
prng_SRCS += devprng.c
prng_SRCS += drvBPM.c
prng_SRCS += devBPMbo.c
prng_SRCS += devBPMmbbo.c
prng_SRCS += devBPMwf.c
prng_SRCS += devBPMao.c
prng_SRCS += devBPMai.c
prng_SRCS += commonDev.c
prng_SRCS += bpmPosCalc.c
# Build the main IOC entry point on workstation OSs.
prng_SRCS_DEFAULT += prngMain.cpp
prng_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#prng_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
prng_LIBS += $(EPICS_BASE_IOC_LIBS)
prng_LIBS += asyn

prng_SYS_LIBS += bpmclient
prng_SYS_LIBS += zmq
prng_SYS_LIBS += czmq
prng_SYS_LIBS += mdp

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

