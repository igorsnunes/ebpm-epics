/****************************************************************************
 * Copyright (C) 2014 by Igor Nunes, Bruno Martins, Lucas Russo,            *
 * Daniel Tavares                                                           *
 *                                                                          *
 *                                                                          *
 * This file is part of BPM epics software                                  *
 *                                                                          *
 *   BPM epics software  is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU Lesser General Public License as published  *
 *   by the Free Software Foundation, either version 3 of the License, or   *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   Bpm epics software is distributed in the hope that it will be useful,                 *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with BPM epics software.  If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

/**
 * @file prngdrv.c
 * @author Igor Nunes
 * @date 14 Mar 2014
 * @brief Driver support for BPM. Controls hardware parameters using ZeroMQ
 *
 * This Driver Support provides access to hardware using an API for accessing
 * and modifing its parameters and acquiring data. The API is build over ZeroMq.
 *
 * 
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

//#ifndef _BPM_PROTOTYPES_DOXYGEN_H
//#  define _BPM_PROTOTYPES_DOXYGEN_H

#include "drvBPM.h"

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
#include "/usr/local/lib/bpm_client.h"
#include "/usr/local/lib/bpm_client_codes.h"



#include <inttypes.h>

#define MAX_BPM_COMMANDS 1

typedef struct {

    BPMCommand command;

    char *commandString;

} BPMCommandStruct;

/**
 *@brief BPM command vector
 *
 *Vector for selecting the right command for hardware execution
 *
 */

static BPMCommandStruct BPMCommands[MAX_BPM_COMMANDS] = {

    {BPMBlinkLeds,          BPMBLinkLedsString}           /* int32, write */

};


/**
 * @brief BPM Driver private struct.
 *
 * Detailed explanation.
 */
typedef struct {
	bpm_client_t *bpm_client; /**< Instance of bpm client to use the API. */
	asynInterface common;/**< asyn common interface**/
	asynInterface int32;/**< asyn int32 interface**/
	asynInterface float64;/**asyn float64 interface**/
	asynInterface int32Array;/**asyn int32array interface**/
	asynInterface drvUser;/**asyn drvUser interface**/
	char *portNumber;/**portNumber, or bpm Number for acess**/
	char *bpm_param;/**ipc directory**/
} bpmDrvPvt;

/* These functions are in public interfaces */
static asynStatus int32Write        (void *drvPvt, asynUser *pasynUser,
                                     epicsInt32 value);
static asynStatus int32Read         (void *drvPvt, asynUser *pasynUser,
                                    epicsInt32 *value);
static void bpmReport               (void *drvPvt, FILE *fp, int details);
static asynStatus bpmConnect        (void *drvPvt, asynUser *pasynUser);
static asynStatus bpmDisconnect     (void *drvPvt, asynUser *pasynUser);
static asynStatus drvUserCreate     (void *drvPvt, asynUser *pasynUser,                                          const char *drvInfo,                                                        const char **pptypeName,                                                    size_t *psize);
static asynStatus drvUserGetType    (void *drvPvt, asynUser *pasynUser,                                          const char **pptypeName,                                                    size_t *psize);
static asynStatus drvUserDestroy    (void *drvPvt, asynUser *pasynUser);


/*
 *asynDrvUser methods
 */
static struct asynDrvUser bpmDrvUser = {
   drvUserCreate,
   drvUserGetType,
   drvUserDestroy
};

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

/**
 *@brief Function for initial configuration of Driver Support called by st.cmd
 *
 *This function exports standard interfaces for use in device support, register a port
 * for communicating with hardware and setting some basic hardware configuration.
 *For each interface, some functions are defined for: connecting/disconecting driver support,
 *reading/writting values from lower layers.
 *
 *@param [in] *portNumber BPM id/port number for hardware access 
 */ 
int bpmConfig(const char *portNumber)
{

	bpmDrvPvt *pPvt;
	int status;
	pPvt = callocMustSucceed(1, sizeof(bpmDrvPvt), "bpmDrvPvt");
	pPvt->portNumber = epicsStrDup(portNumber);
	/*
	*  Link with higher level routines
	*/
	pPvt->drvUser.interfaceType = asynDrvUserType;
	pPvt->drvUser.pinterface  = (void *)&bpmDrvUser;
	pPvt->drvUser.drvPvt = pPvt;
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

	status = pasynManager->registerInterface(pPvt->portNumber,&pPvt->drvUser);

	if (status != asynSuccess) {
	        errlogPrintf("AIMConfig ERROR: Can't register drvUser\n");
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

/* asynDrvUser routines */

static asynStatus drvUserCreate(void *drvPvt, asynUser *pasynUser,
                                const char *drvInfo,
                                const char **pptypeName, size_t *psize)

{
    int i;
    const char *pstring;

    for (i=0; i<MAX_BPM_COMMANDS; i++) {
        pstring = BPMCommands[i].commandString;
        if (epicsStrCaseCmp(drvInfo, pstring) == 0) {
            pasynUser->reason = BPMCommands[i].command;
            if (pptypeName) *pptypeName = epicsStrDup(pstring);
            if (psize) *psize = sizeof(BPMCommands[i].command);
            asynPrint(pasynUser, ASYN_TRACE_FLOW,
              "drvBPM::drvUserCreate, command=%s\n", pstring);
            return(asynSuccess);
        }
    }
    asynPrint(pasynUser, ASYN_TRACE_ERROR,
              "drvBPM::drvUserCreate, unknown command=%s\n", drvInfo);
    return(asynError);
}

static asynStatus drvUserGetType(void *drvPvt, asynUser *pasynUser,

                                 const char **pptypeName, size_t *psize)

{
    BPMCommand command = pasynUser->reason;
    *pptypeName = NULL;
    *psize = 0;
    if (pptypeName)
        *pptypeName = epicsStrDup(BPMCommands[command].commandString);

    if (psize) *psize = sizeof(command);
    return(asynSuccess);

}

static asynStatus drvUserDestroy(void *drvPvt, asynUser *pasynUser)

{
    return(asynSuccess);
}

/* Report  parameters */
static void bpmReport(void *drvPvt, FILE *fp, int details)
{
    bpmDrvPvt *pPvt = (bpmDrvPvt *)drvPvt;

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
		pPvt->bpm_client = bpm_client_new(pPvt->bpm_param,verbose);
	
	pasynManager->exceptionConnect(pasynUser);
	return(asynSuccess);
}

/* Connect */
static asynStatus bpmDisconnect(void *drvPvt, asynUser *pasynUser)
{
	bpmDrvPvt *pPvt = (bpmDrvPvt*)drvPvt;
	bpm_client_destroy(&pPvt->bpm_client);
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
static const iocshArg * const bpmConfigArgs[1] = {&bpmConfigArg0};
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
 

