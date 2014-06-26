#include <stdlib.h>
#include <epicsExport.h>
#include <dbAccess.h>
#include <devSup.h>
#include <recGbl.h>
#include <boRecord.h>
#include <mbboRecord.h>
#include <waveformRecord.h>
#include <aoRecord.h>
#include <aiRecord.h>
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
#include "asynInt32Array.h"
#include "asynInt16Array.h"
#include "asynInt32.h"
#include <epicsExport.h>
#include "asynDriver.h"
#include "asynOctet.h"
#include "asynInterposeCom.h"
#include "asynInterposeEos.h"
#include <asynEpicsUtils.h>
#include <asynDrvUser.h>
#include <string.h>

typedef struct {
	asynUser *pasynUser;
	void *asynDrvUserPvt;
	asynDrvUser *pasynDrvUser;
	asynInt32 *pasynInt32;
	void *asynInt32Pvt;
	asynInt32Array *pasynInt32Array;
	void *asynInt32ArrayPvt;
	asynInt16Array *pasynInt16Array;
	void *asynInt16ArrayPvt;
	size_t nread;
	int *data;
} bpmPvt;

long findDrvInfoWf(waveformRecord *ao, asynUser *pasynUser, char *drvInfoString, int command);
long findDrvInfoAo(aoRecord *ao, asynUser *pasynUser, char *drvInfoString, int command);
long findDrvInfoMbbo(mbboRecord *mbbo, asynUser *pasynUser, char *drvInfoString, int command);
long findDrvInfoBo(boRecord *bo, asynUser *pasynUser, char *drvInfoString, int command);
long findDrvInfoAi(aiRecord *ao, asynUser *pasynUser, char *drvInfoString, int command);
