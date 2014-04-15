#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>


#include <errlog.h>
#include <iocsh.h>
#include <cantProceed.h>
#include <epicsString.h>
#include <epicsTime.h>
#include <asynDriver.h>
#include <asynInt32.h>
#include <asynFloat64.h>
#include <asynInt32Array.h>
#include <asynDrvUser.h>

#include <epicsExport.h>

/*Libraries from ZeroMQ and majordome*/

#include <clientzeromq.h>/*example*/
#include <mdp.h>
#include <inttypes.h>

typedef struct {
	bpm_client_t *bpm_client; 
	asynInterface common;
	asynInterface int32;
	asynInterface float64;
	asynInterface int32Array;
	asynInterface drvUser;
	char *portNumber;
	char *bpm_param;
} bpmDrvPvt;

/* These functions are in public interfaces */
static asynStatus int32Write         (void *drvPvt, asynUser *pasynUser,
                                     epicsInt32 value);
static asynStatus int32Read         (void *drvPvt, asynUser *pasynUser,
                                     epicsInt32 *value);
static void bpmReport               (void *drvPvt, FILE *fp, int details);
static asynStatus bpmConnect        (void *drvPvt, asynUser *pasynUser);
static asynStatus bpmDisconnect     (void *drvPvt, asynUser *pasynUser);

/*
 * asynCommon methods
 */
static struct asynCommon bpmDrvCommon = {
    bpmReport,
    bpmConnect,
    bpmDisconnect
};

/* asynInt32 methods */
static asynInt32 bpmDrvInt32 = {
    int32Write,
    int32Read,
    NULL,
    NULL,
    NULL
};

int bpmConfig(const char *portNumber)
{

	bpmDrvPvt *pPvt;
	int status;
	pPvt = callocMustSucceed(1, sizeof(bpmDrvPvt), "bpmDrvPvt");
	pPvt->portNumber = epicsStrDup(portNumber);
	/*
	*  Link with higher level routines
	*/
	pPvt->common.interfaceType = asynCommonType;
	pPvt->common.pinterface  = (void *)&bpmDrvCommon;
	pPvt->common.drvPvt = pPvt;
	pPvt->int32.interfaceType = asynInt32Type;
	pPvt->int32.pinterface  = (void *)&bpmDrvInt32;
	pPvt->int32.drvPvt = pPvt;
	status = pasynManager->registerPort(pPvt->portNumber,
                                        ASYN_MULTIDEVICE | ASYN_CANBLOCK,
                                        1,  /* autoconnect */
                                        0,  /* medium priority */
                                        0); /* default stacksize */
	if (status != asynSuccess) {
		errlogPrintf("bpmConfig ERROR: Can't register myself.\n");
		return -1;
	}
	status = pasynManager->registerInterface(pPvt->portNumber, &pPvt->common);
	if (status != asynSuccess) {
		errlogPrintf("bpmConfig: Can't register common.\n");
		return -1;
	}
	status = pasynInt32Base->initialize(pPvt->portNumber, &pPvt->int32);
	if (status != asynSuccess) {
		errlogPrintf("bpmConfig: Can't register int32.\n");
		return -1;
	}

	/*Instantiate bpm client class*/
	char bpm_param[50];
	int verbose = 0;
	sprintf(bpm_param,"ipc:///tmp/bpm/%s",pPvt->portNumber);
	pPvt->bpm_param = epicsStrDup(bpm_param); 
	bpm_client_t *bpm_client = bpm_client_new(bpm_param,verbose);
	if(bpm_client != NULL)
		pPvt->bpm_client = bpm_client;
	else
		return -1;

	return(0);
}


/* Report  parameters */
static void bpmReport(void *drvPvt, FILE *fp, int details)
{
    bpmDrvPvt *pPvt = (bpmDrvPvt *)drvPvt;

    //assert(pPvt);
    fprintf(fp, "bpm %s: connected on drv device \n",
            pPvt->portNumber);
    if (details >= 1) {
        fprintf(fp, "              maxChans: \n");
    }
}

/* Connect */
static asynStatus bpmConnect(void *drvPvt, asynUser *pasynUser)
{
	int verbose = 0;
	bpmDrvPvt *pPvt = (bpmDrvPvt*)drvPvt;
	if(pPvt->bpm_client == NULL)
		pPvt->client = bpm_client_new(pPvt->bpm_param,verbose);
	
	/* Does nothing for now.  
	* May be used if connection management is implemented */
	pasynManager->exceptionConnect(pasynUser);
	return(asynSuccess);
}

/* Connect */
static asynStatus bpmDisconnect(void *drvPvt, asynUser *pasynUser)
{
	bpmDrvPvt *pPvt = (bpmDrvPvt*)drvPvt;
	bpm_client_destroy(pPvt->bpm_client);
	/* Does nothing for now.  
	* May be used if connection management is implemented */
	pasynManager->exceptionDisconnect(pasynUser);
	return(asynSuccess);
}

static asynStatus int32Read(void *drvPvt, asynUser *pasynUser,
                            epicsInt32 *value)
{
    bpmDrvPvt *priv = (bpmDrvPvt*)drvPvt;
    *value=0;
    return asynSuccess;
}

static asynStatus int32Write(void *drvPvt, asynUser *pasynUser,
                            epicsInt32 value)
{
    return asynSuccess;
}

/* iocsh functions */

static const iocshArg bpmConfigArg0 = {"portNumber",iocshArgString};
static const iocshArg * const bpmConfigArgs[1] = {&bpmConfigArg0}
static const iocshFuncDef bpmConfigDef = {"bpmConfig",1,bpmConfigArgs};
static void bpmConfigFunc(const iocshArgBuf *args)
{
    bpmConfig(args[0].sval);
}

static void bpmRegister(void)
{
    iocshRegister(&bpmConfigDef,bpmConfigFunc);
}

epicsExportRegistrar(bpmRegister);
 

