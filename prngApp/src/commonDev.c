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

long findDrvInfoMbbo(mbboRecord *mbbo, asynUser *pasynUser, char *drvInfoString, int command)

{
	bpmPvt *pPvt = (bpmPvt *)mbbo->dpvt;
 	return findDrvInfo(pPvt,pasynUser,drvInfoString,command);

}

long findDrvInfoAo(aoRecord *ao, asynUser *pasynUser, char *drvInfoString, int command)

{
	bpmPvt *pPvt = (bpmPvt *)ao->dpvt;
 	return findDrvInfo(pPvt,pasynUser,drvInfoString,command);

}

long findDrvInfoAi(aiRecord *ao, asynUser *pasynUser, char *drvInfoString, int command)

{
	bpmPvt *pPvt = (bpmPvt *)ao->dpvt;
 	return findDrvInfo(pPvt,pasynUser,drvInfoString,command);

}

long findDrvInfoWf(waveformRecord *ao, asynUser *pasynUser, char *drvInfoString, int command)

{
	bpmPvt *pPvt = (bpmPvt *)ao->dpvt;
 	return findDrvInfo(pPvt,pasynUser,drvInfoString,command);

}

