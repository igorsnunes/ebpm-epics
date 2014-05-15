#include "commonDev.h"

static long findDrvInfo(bpmPvt *pPvt, asynUser *pasynUser, char *drvInfoString, int command){
    /* Look up the pasynUser->reason */

    if (pPvt->pasynDrvUser->create(pPvt->asynDrvUserPvt, pasynUser, drvInfoString, NULL, NULL) != asynSuccess) {

        asynPrint(pasynUser, ASYN_TRACE_ERROR,"devMcaAsyn::findDrvInfo, drvUserCreate failed for \n");
        return(-1);

    }

    //pPvt->driverReasons[command] = pasynUser->reason;

    return(0);
}

long findDrvInfoBo(boRecord *bo, asynUser *pasynUser, char *drvInfoString, int command)

{
	bpmPvt *pPvt = (bpmPvt *)bo->dpvt;
 	return findDrvInfo(pPvt,pasynUser,drvInfoString,command);

}
