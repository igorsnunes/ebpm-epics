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

//#include <asynInt16.h>
#include <asynInt16Array.h>
#include <epicsExport.h>

/*Libraries from ZeroMQ and majordome*/
#include "/usr/local/lib/bpm_client.h"
#include "/usr/local/lib/bpm_client_codes.h"



#include <inttypes.h>

#define MAX_BPM_COMMANDS 23

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

    {BPMBlinkLeds,          BPMBLinkLedsString},           /* int32, write */

    {BPMAcqOrigin,          BPMAcqOriginString},           /* int32, write */

    {BPMAcqSamples,          BPMAcqSamplesString},           /* int32, write */

    {BPMAcqAdcChAWF,        BPMAcqAdcChAWFString},           /* int16, read */
    {BPMAcqAdcChBWF,        BPMAcqAdcChBWFString},           /* int16, read */
    {BPMAcqAdcChCWF,        BPMAcqAdcChCWFString},           /* int16, read */
    {BPMAcqAdcChDWF,        BPMAcqAdcChDWFString},           /* int16, read */

    {BPMAcqTbtAmpChXWF,     BPMAcqTbtAmpChXWFString},           /* uint32, read */
    {BPMAcqTbtAmpChYWF,     BPMAcqTbtAmpChYWFString},           /* uint32, read */
    {BPMAcqTbtAmpChQWF,     BPMAcqTbtAmpChQWFString},           /* uint32, read */
    {BPMAcqTbtAmpChSUMWF,   BPMAcqTbtAmpChSUMWFString},           /* uint32, read */

    {BPMAcqTbtPosChXWF,     BPMAcqTbtPosChXWFString},           /* uint32, read */
    {BPMAcqTbtPosChYWF,     BPMAcqTbtPosChYWFString},           /* uint32, read */
    {BPMAcqTbtPosChQWF,     BPMAcqTbtPosChQWFString},           /* uint32, read */
    {BPMAcqTbtPosChSUMWF,   BPMAcqTbtPosChSUMWFString},           /* uint32, read */

    {BPMAcqFofbAmpChXWF,    BPMAcqFofbAmpChXWFString},           /* uint32, read */
    {BPMAcqFofbAmpChYWF,    BPMAcqFofbAmpChYWFString},           /* uint32, read */
    {BPMAcqFofbAmpChQWF,    BPMAcqFofbAmpChQWFString},           /* uint32, read */
    {BPMAcqFofbAmpChSUMWF,  BPMAcqFofbAmpChSUMWFString},           /* uint32, read */

    {BPMAcqFofbPosChXWF,    BPMAcqFofbPosChXWFString},           /* uint32, read */
    {BPMAcqFofbPosChYWF,    BPMAcqFofbPosChYWFString},           /* uint32, read */
    {BPMAcqFofbPosChQWF,    BPMAcqFofbPosChQWFString},           /* uint32, read */
    {BPMAcqFofbPosChSUMWF,  BPMAcqFofbPosChSUMWFString}           /* uint32, read */

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
	asynInterface int16Array;/**asyn int16array interface**/
	asynInterface drvUser;/**asyn drvUser interface**/
	char *portNumber;/**portNumber, or bpm Number for acess**/
	char *bpm_param;/**ipc directory**/
	int BYTES_READ;/**bytes last read**/
	epicsInt32 N_SAMPLES;/**number of samples to acquire**/
	uint32_t *data_buffer;/**data buffer last acquired**/
	BPMAcqOriginType ORIGIN;/**data origin**/
} bpmDrvPvt;

/* These functions are in public interfaces */
static asynStatus int32Write        (void *drvPvt, asynUser *pasynUser,
                                     epicsInt32 value);
static asynStatus int32Read         (void *drvPvt, asynUser *pasynUser,
                                    epicsInt32 *value);
static asynStatus int32ArrayRead    (void *drvPvt, asynUser *pasynUser,
                                    epicsInt32 *value, size_t nelements, size_t *nIn);
static asynStatus int16ArrayRead    (void *drvPvt, asynUser *pasynUser,
                                    epicsInt16 *value, size_t nelements, size_t *nIn);
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

/* asynInt32Array methods */
static asynInt32Array bpmDrvInt32Array = {
    NULL,
    int32ArrayRead,
    NULL,
    NULL,
};

/* asynInt16Array methods */
static asynInt16Array bpmDrvInt16Array = {
    NULL,
    int16ArrayRead,
    NULL,
    NULL,
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

	/*Instantiate bpm client class*/
	char bpm_param[50];
	int verbose = 0;
	sprintf(bpm_param,"tcp://10.0.18.35:8888");
	//sprintf(bpm_param,"ipc:///tmp/bpm/%s",pPvt->portNumber);
	pPvt->BYTES_READ = 0;
	pPvt->data_buffer = NULL;
	pPvt->ORIGIN = ADC;
	pPvt->N_SAMPLES = 1000;
	pPvt->bpm_param = epicsStrDup(bpm_param); 
	bpm_client_t *bpm_client;
	pPvt->bpm_client = bpm_client = NULL;
	bpm_client = bpm_client_new(bpm_param,verbose);
	if(bpm_client != NULL){
		pPvt->bpm_client = bpm_client;
		printf("\ndebug client created\n");
	}
	else{
		return -1;
	}

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
	pPvt->int32Array.interfaceType = asynInt32ArrayType;
	pPvt->int32Array.pinterface  = (void *)&bpmDrvInt32Array;
	pPvt->int32.drvPvt = pPvt;
	pPvt->int16Array.interfaceType = asynInt16ArrayType;
	pPvt->int16Array.pinterface  = (void *)&bpmDrvInt16Array;
	pPvt->int16Array.drvPvt = pPvt;
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

	status = pasynInt32ArrayBase->initialize(pPvt->portNumber, &pPvt->int32Array);
	if (status != asynSuccess) {
		errlogPrintf("bpmConfig: Can't register int32Array.\n");
		return -1;
	}

	status = pasynInt16ArrayBase->initialize(pPvt->portNumber, &pPvt->int16Array);
	if (status != asynSuccess) {
		errlogPrintf("bpmConfig: Can't register int16Array.\n");
		return -1;
	}

	status = pasynManager->registerInterface(pPvt->portNumber,&pPvt->drvUser);

	if (status != asynSuccess) {
	        errlogPrintf("AIMConfig ERROR: Can't register drvUser\n");
	        return -1;
	}

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

static void blink_leds(bpm_client_t *client, char *variable_zmq)
{
	int i,j;
	for (i = 0; i < 50; ++i) {
		uint32_t leds = (1 << 1);
		unsigned int j;
		for (j = 0; j < 3; ++j) {
			if (!zctx_interrupted) {
				bpm_blink_leds (client, variable_zmq, leds);
				usleep (80000);
				leds <<= 1;
			}
		}
	}
	return;
}
typedef struct _acq_chan_t {
    uint32_t chan;
    uint32_t sample_size;
} acq_chan_t;
/* Global structure merging all of the channel's sample sizes */
acq_chan_t __acq_chan[END_CHAN_ID] = { [0] = {.chan = ADC_CHAN_ID, .sample_size = ADC_SAMPLE_SIZE},
                                     [1] = {.chan = TBTAMP_CHAN_ID, .sample_size = TBTAMP_SAMPLE_SIZE},
                                     [2] = {.chan = TBTPOS_CHAN_ID, .sample_size = TBTPOS_SAMPLE_SIZE},
                                     [3] = {.chan = FOFBAMP_CHAN_ID, .sample_size = FOFBAMP_SAMPLE_SIZE},
                                     [4] = {.chan = FOFBPOS_CHAN_ID, .sample_size = FOFBPOS_SAMPLE_SIZE} };


static asynStatus int32Write(void *drvPvt, asynUser *pasynUser,epicsInt32 value)
	{
	bpmDrvPvt *priv = (bpmDrvPvt*)drvPvt;
	char variable_zmq[100];
	if (pasynUser->reason == BPMBlinkLeds){
		if(value){
			sprintf(variable_zmq,"BPM%s:DEVIO:FMC130M_4CH",priv->portNumber);
			blink_leds(priv->bpm_client, variable_zmq);
			printf("\nblinked\n");
		}
	}
	else if(pasynUser->reason == BPMAcqSamples){
		if (value <= 1000) 
			priv->N_SAMPLES = value;
		else
			priv->N_SAMPLES = 1000;
	}
	else if(pasynUser->reason == BPMAcqOrigin){
		if(priv->data_buffer != NULL)
			free(priv->data_buffer);
		priv->ORIGIN = value;
		uint32_t data_size = (priv->N_SAMPLES)*__acq_chan[(priv->ORIGIN)].sample_size;
		priv->data_buffer = (uint32_t *) zmalloc (data_size*sizeof (uint8_t));
		//memset(data,0,1000);
		acq_trans_t acq_trans = {.req =   {
			.num_samples = priv->N_SAMPLES,
			.chan = priv->ORIGIN,
			},
			.block = {
			.data = priv->data_buffer,
			.data_size = data_size,
                        }
                };
		bpm_client_err_e err = bpm_get_curve (priv->bpm_client, "BPM0:DEVIO:ACQ", &acq_trans);
		priv->BYTES_READ = acq_trans.block.bytes_read;

	}

	return asynSuccess;
}

static void copy_data (uint32_t chan, uint32_t *data_acq, uint32_t size, uint32_t nch,epicsInt32 *val32, epicsInt16 *val16)
{
	uint32_t i = 0;
	if ((nch >= 4) || (size == 0))
		return;
	/* FIXME: Make it more generic */
	if (chan == 0) {
		if(val16 == NULL){
			printf("null error\n");
			return;
		}
		int16_t *raw_data16 = (int16_t *) data_acq;
		for (i = 0; i < (size/sizeof(uint16_t)) / 4; i++) {
			if (!zctx_interrupted) {
				val16[i] = (epicsInt16) raw_data16[(i*4)+nch];
			
			}
		}
	}
	else {
		if(val32 == NULL){
			printf("null error\n");
			return;
		}
		int32_t *raw_data32 = (int32_t *) data_acq;
		for (i = 0; i < (size/sizeof(uint32_t)) / 4; i++) {
			if (!zctx_interrupted) {
				val32[i] = (epicsInt32) raw_data32[(i*4)+nch];
			}
		}
	}
	return;
}


static asynStatus int32ArrayRead(void *drvPvt, asynUser *pasynUser,epicsInt32 *value, size_t nelements, size_t *nIn){
	bpmDrvPvt *priv = (bpmDrvPvt*)drvPvt;
	if(pasynUser->reason == BPMAcqTbtAmpChXWF){
		copy_data(TBTAMP_CHAN_ID,priv->data_buffer,priv->BYTES_READ,0,value,NULL);
		*nIn = (priv->BYTES_READ/TBTAMP_SAMPLE_SIZE);
	}
	else if(pasynUser->reason == BPMAcqTbtAmpChYWF){
		copy_data(TBTAMP_CHAN_ID,priv->data_buffer,priv->BYTES_READ,1,value,NULL);
		*nIn = (priv->BYTES_READ/TBTAMP_SAMPLE_SIZE);
	}
	else if(pasynUser->reason == BPMAcqTbtAmpChQWF){
		copy_data(TBTAMP_CHAN_ID,priv->data_buffer,priv->BYTES_READ,2,value,NULL);
		*nIn = (priv->BYTES_READ/TBTAMP_SAMPLE_SIZE);
	}
	else if(pasynUser->reason == BPMAcqTbtAmpChSUMWF){
		copy_data(TBTAMP_CHAN_ID,priv->data_buffer,priv->BYTES_READ,3,value,NULL);
		*nIn = (priv->BYTES_READ/TBTAMP_SAMPLE_SIZE);
	}
	else if(pasynUser->reason == BPMAcqTbtPosChXWF){
		copy_data(TBTPOS_CHAN_ID,priv->data_buffer,priv->BYTES_READ,0,value,NULL);
		*nIn = (priv->BYTES_READ/TBTPOS_SAMPLE_SIZE);
	}
	else if(pasynUser->reason == BPMAcqTbtPosChYWF){
		copy_data(TBTPOS_CHAN_ID,priv->data_buffer,priv->BYTES_READ,1,value,NULL);
		*nIn = (priv->BYTES_READ/TBTPOS_SAMPLE_SIZE);
	}
	else if(pasynUser->reason == BPMAcqTbtPosChQWF){
		copy_data(TBTPOS_CHAN_ID,priv->data_buffer,priv->BYTES_READ,2,value,NULL);
		*nIn = (priv->BYTES_READ/TBTPOS_SAMPLE_SIZE);
	}
	else if(pasynUser->reason == BPMAcqTbtPosChSUMWF){
		copy_data(TBTPOS_CHAN_ID,priv->data_buffer,priv->BYTES_READ,3,value,NULL);
		*nIn = (priv->BYTES_READ/TBTPOS_SAMPLE_SIZE);
	}
	else if(pasynUser->reason == BPMAcqFofbAmpChXWF){
		copy_data(FOFBAMP_CHAN_ID,priv->data_buffer,priv->BYTES_READ,0,value,NULL);
		*nIn = (priv->BYTES_READ/FOFBAMP_SAMPLE_SIZE);
	}
	else if(pasynUser->reason == BPMAcqFofbAmpChYWF){
		copy_data(FOFBAMP_CHAN_ID,priv->data_buffer,priv->BYTES_READ,1,value,NULL);
		*nIn = (priv->BYTES_READ/FOFBAMP_SAMPLE_SIZE);
	}
	else if(pasynUser->reason == BPMAcqFofbAmpChQWF){
		copy_data(FOFBAMP_CHAN_ID,priv->data_buffer,priv->BYTES_READ,2,value,NULL);
		*nIn = (priv->BYTES_READ/FOFBAMP_SAMPLE_SIZE);
	}
	else if(pasynUser->reason == BPMAcqFofbAmpChSUMWF){
		copy_data(FOFBAMP_CHAN_ID,priv->data_buffer,priv->BYTES_READ,3,value,NULL);
		*nIn = (priv->BYTES_READ/FOFBAMP_SAMPLE_SIZE);
	}
	else if(pasynUser->reason == BPMAcqFofbPosChXWF){
		copy_data(FOFBPOS_CHAN_ID,priv->data_buffer,priv->BYTES_READ,0,value,NULL);
		*nIn = (priv->BYTES_READ/FOFBPOS_SAMPLE_SIZE);
	}
	else if(pasynUser->reason == BPMAcqFofbPosChYWF){
		copy_data(FOFBPOS_CHAN_ID,priv->data_buffer,priv->BYTES_READ,1,value,NULL);
		*nIn = (priv->BYTES_READ/FOFBPOS_SAMPLE_SIZE);
	}
	else if(pasynUser->reason == BPMAcqFofbPosChQWF){
		copy_data(FOFBPOS_CHAN_ID,priv->data_buffer,priv->BYTES_READ,2,value,NULL);
		*nIn = (priv->BYTES_READ/FOFBPOS_SAMPLE_SIZE);
	}
	else if(pasynUser->reason == BPMAcqFofbPosChSUMWF){
		copy_data(FOFBPOS_CHAN_ID,priv->data_buffer,priv->BYTES_READ,3,value,NULL);
		*nIn = (priv->BYTES_READ/FOFBPOS_SAMPLE_SIZE);
	}


	return asynSuccess;
}

static asynStatus int16ArrayRead(void *drvPvt, asynUser *pasynUser,epicsInt16 *value, size_t nelements, size_t *nIn){
	bpmDrvPvt *priv = (bpmDrvPvt*)drvPvt;

	if(pasynUser->reason == BPMAcqAdcChAWF)
		copy_data(ADC_CHAN_ID,priv->data_buffer,priv->BYTES_READ,0,NULL,value);
	else if(pasynUser->reason == BPMAcqAdcChBWF)
		copy_data(ADC_CHAN_ID,priv->data_buffer,priv->BYTES_READ,1,NULL,value);
	else if(pasynUser->reason == BPMAcqAdcChCWF)
		copy_data(ADC_CHAN_ID,priv->data_buffer,priv->BYTES_READ,2,NULL,value);
	else if(pasynUser->reason == BPMAcqAdcChDWF)
		copy_data(ADC_CHAN_ID,priv->data_buffer,priv->BYTES_READ,3,NULL,value);
	*nIn = (priv->BYTES_READ/ADC_SAMPLE_SIZE);

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
 

