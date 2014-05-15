#include <stdlib.h>
#include <epicsExport.h>
#include <dbAccess.h>
#include <devSup.h>
#include <recGbl.h>
#include <boRecord.h>
#include "drvBPM.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <osiUnistd.h>
#include <osiSock.h>
#include <cantProceed.h>
#include <errlog.h>
#include <iocsh.h>
#include <epicsAssert.h>
#include <epicsExit.h>
#include <epicsStdio.h>
#include <epicsString.h>
#include <epicsThread.h>
#include <epicsTime.h>
#include <osiUnistd.h>
#include "asynInt32.h"
#include <epicsExport.h>
#include "asynDriver.h"
#include "asynOctet.h"
#include "asynInterposeCom.h"
#include "asynInterposeEos.h"
#include <asynEpicsUtils.h>
#include <asynDrvUser.h>


typedef struct {
	asynUser *pasynUser;
	void *asynDrvUserPvt;
	asynDrvUser *pasynDrvUser;
	asynInt32 *pasynInt32;
	void *asynInt32Pvt;
	size_t nread;
	int *data;
} bpmPvt;

long findDrvInfoBo(boRecord *bo, asynUser *pasynUser, char *drvInfoString, int command);
