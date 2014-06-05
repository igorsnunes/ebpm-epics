#include <commonDev.h>

static long init_record_wf(waveformRecord *pao);
static long read_wf(waveformRecord *pao);

struct {

	long num;
	DEVSUPFUN   report;
	DEVSUPFUN   init;
	DEVSUPFUN   init_record;
	DEVSUPFUN   get_ioint_info;
	DEVSUPFUN   read_wf;

} devBPMwf = {
    5,
    NULL,
    NULL,
    init_record_wf,
    NULL,
    read_wf
};
epicsExportAddress(dset,devBPMwf);

static long init_record_wf(waveformRecord *pao)
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

    status = pasynEpicsUtils->parseLink(pasynUser, &pao->inp,
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

	if((strcmp(BPMAcqAdcChAWFString,userParam) == 0) || (strcmp(BPMAcqAdcChBWFString,userParam) == 0)
	|| (strcmp(BPMAcqAdcChCWFString,userParam) == 0) || (strcmp(BPMAcqAdcChDWFString,userParam) == 0)){
		/* Get the asyniInt16Array interface */
		pasynInterface = pasynManager->findInterface(pasynUser, asynInt16ArrayType, 1);
		if (!pasynInterface) {
			asynPrint(pasynUser, ASYN_TRACE_ERROR,"devMcaAsyn::init_record, find int32 interface failed\n");
			return 1;
		}
		pPvt->pasynInt16Array = (asynInt16Array *)pasynInterface->pinterface;
		pPvt->asynInt16ArrayPvt = pasynInterface->drvPvt;
	}
	else{
		/* Get the asyniInt32Array interface */
		pasynInterface = pasynManager->findInterface(pasynUser, asynInt32ArrayType, 1);
		if (!pasynInterface) {
			asynPrint(pasynUser, ASYN_TRACE_ERROR,"devMcaAsyn::init_record, find int32 interface failed\n");
			return 1;
		}
		pPvt->pasynInt32Array = (asynInt32Array *)pasynInterface->pinterface;
		pPvt->asynInt32ArrayPvt = pasynInterface->drvPvt;
	}


    /* Get the asynDrvUser interface */

    pasynInterface = pasynManager->findInterface(pasynUser, 

                                                 asynDrvUserType, 1);

	if (!pasynInterface) {
		asynPrint(pasynUser, ASYN_TRACE_ERROR,"devMcaAsyn::init_record, find drvUser interface failed\n");

	}

	pPvt->pasynDrvUser = (asynDrvUser *)pasynInterface->pinterface;
	pPvt->asynDrvUserPvt = pasynInterface->drvPvt;


	//if (findDrvInfoWf(pao, pasynUser, BPMAcqAdcChAWFString, 0))
	//if (findDrvInfoWf(pao, pasynUser, BPMAcqAdcChBWFString, 0))
	//if (findDrvInfoWf(pao, pasynUser, BPMAcqAdcChCWFString, 0))
	//if (findDrvInfoWf(pao, pasynUser, BPMAcqAdcChDWFString, 0))

	//if (findDrvInfoWf(pao, pasynUser, BPMAcqTbtAmpChXWFString, 0))
	//if (findDrvInfoWf(pao, pasynUser, BPMAcqTbtAmpChYWFString, 0))
	//if (findDrvInfoWf(pao, pasynUser, BPMAcqTbtAmpChQWFString, 0))
	//if (findDrvInfoWf(pao, pasynUser, BPMAcqTbtAmpChSUMWFString, 0))

	//if (findDrvInfoWf(pao, pasynUser, BPMAcqTbtPosChXWFString, 0))
	//if (findDrvInfoWf(pao, pasynUser, BPMAcqTbtPosChYWFString, 0))
	//if (findDrvInfoWf(pao, pasynUser, BPMAcqTbtPosChQWFString, 0))
	//if (findDrvInfoWf(pao, pasynUser, BPMAcqTbtPosChSUMWFString, 0))

	//if (findDrvInfoWf(pao, pasynUser, BPMAcqFofbAmpChXWFString, 0))
	//if (findDrvInfoWf(pao, pasynUser, BPMAcqFofbAmpChYWFString, 0))
	//if (findDrvInfoWf(pao, pasynUser, BPMAcqFofbAmpChQWFString, 0))
	//if (findDrvInfoWf(pao, pasynUser, BPMAcqFofbAmpChSUMWFString, 0))

	//if (findDrvInfoWf(pao, pasynUser, BPMAcqFofbPosChXWFString, 0))
	//if (findDrvInfoWf(pao, pasynUser, BPMAcqFofbPosChYWFString, 0))
	//if (findDrvInfoWf(pao, pasynUser, BPMAcqFofbPosChQWFString, 0))
	//if (findDrvInfoWf(pao, pasynUser, BPMAcqFofbPosChSUMWFString, 0))

	findDrvInfoWf(pao, pasynUser, userParam, 0);

	return 0;
}

static long read_wf(waveformRecord *pao)
{

	bpmPvt *pPvt = (bpmPvt *)pao->dpvt;
	epicsInt16 *val16;
	epicsInt32 *val32;
	size_t nread = 0;
	int i=0;
	if((pPvt->pasynUser->reason >= BPMAcqAdcChAWF) && (pPvt->pasynUser->reason <= BPMAcqAdcChDWF)){
		val16 = (epicsInt16*)pao->bptr;
		if(val16 == NULL)
			return asynError;
		
		pPvt->pasynInt16Array->read(pPvt->asynInt16ArrayPvt, pPvt->pasynUser,val16,(size_t)pao->nelm,&nread);
		for(i=0;i<10;i++)
			printf("%hd\n",val16[i]);
		pao->bptr = (void*)val16;
	}
	else{
		val32 = (epicsInt32*)pao->bptr;
		if(val32 == NULL)
			return asynError;
		pPvt->pasynInt32Array->read(pPvt->asynInt32ArrayPvt, pPvt->pasynUser,val32,(size_t)pao->nelm,&nread);

		pao->bptr = (void*)val32;
	}

	pao->nord = nread;
	printf("%d",nread);
	return 0;
}
