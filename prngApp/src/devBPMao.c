#include <commonDev.h>

static long init_record_ao(aoRecord *pao);
static long write_ao(aoRecord *pao);

struct {

	long num;
	DEVSUPFUN   report;
	DEVSUPFUN   init;
	DEVSUPFUN   init_record_ao;
	DEVSUPFUN   get_ioint_info;
	DEVSUPFUN   write_ao;
	DEVSUPFUN   special_linconv;

} devBPMao = {
    6,
    NULL,
    NULL,
    init_record_ao,
    NULL,
    write_ao,
    NULL
};
epicsExportAddress(dset,devBPMao);

static long init_record_ao(aoRecord *pao)
{

    asynUser *pasynUser;
    char *port, *userParam;
    asynStatus status;
    asynInterface *pasynInterface;
    bpmPvt *pPvt;
    int addr;

    pPvt = callocMustSucceed(1, sizeof(bpmPvt), "devMcaAsyn init_record()");
    /* Create asynUser */
    pasynUser = pasynManager->createAsynUser(0, 0);
    pasynUser->userPvt = pPvt;
    pPvt->pasynUser = pasynUser;
    pao->dpvt = pPvt;

    status = pasynEpicsUtils->parseLink(pasynUser, &pao->out,
                                    &port, &addr, &userParam);

    if (status != asynSuccess) {
        errlogPrintf("devMcaAsyn::init_record  bad link %s\n",
                      pasynUser->errorMessage);
        return 1;
    }

    /* Connect to device */
    status = pasynManager->connectDevice(pasynUser, port, addr);
    if (status != asynSuccess) {
        asynPrint(pasynUser, ASYN_TRACE_ERROR,
                  "devMcaAsyn::init_record, connectDevice failed to %s\n",
                   port);
        return 1;
    }

    /* Get the asynInt32 interface */
    pasynInterface = pasynManager->findInterface(pasynUser, asynInt32Type, 1);
    if (!pasynInterface) {
        asynPrint(pasynUser, ASYN_TRACE_ERROR,"devMcaAsyn::init_record, find int32 interface failed\n");
        return 1;
    }
    pPvt->pasynInt32 = (asynInt32 *)pasynInterface->pinterface;
    pPvt->asynInt32Pvt = pasynInterface->drvPvt;

    /* Get the asynDrvUser interface */

    pasynInterface = pasynManager->findInterface(pasynUser, 

                                                 asynDrvUserType, 1);

	if (!pasynInterface) {
		asynPrint(pasynUser, ASYN_TRACE_ERROR,"devMcaAsyn::init_record, find drvUser interface failed\n");

	}

	pPvt->pasynDrvUser = (asynDrvUser *)pasynInterface->pinterface;
	pPvt->asynDrvUserPvt = pasynInterface->drvPvt;
	
	findDrvInfoAo(pao, pasynUser, userParam, 0);
	//if (findDrvInfoAo(pao, pasynUser, BPMAcqSamplesString, 0))

	return 0;
}

static long write_ao(aoRecord *pao)
{
	bpmPvt *pPvt = (bpmPvt *)pao->dpvt;
	pPvt = (bpmPvt *)pPvt->pasynUser->userPvt;
	pPvt->pasynInt32->write(pPvt->asynInt32Pvt, pPvt->pasynUser,pao->rval);
	return 0;
}
