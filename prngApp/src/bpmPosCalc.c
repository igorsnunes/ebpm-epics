#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbAccess.h"
#include "epicsTypes.h"
#include "registryFunction.h" /* for epicsExport           */
#include "epicsExport.h"      /* for epicsRegisterFunction */
#include "aSubRecord.h"       /* for struct aSubRecord  */
#include "alarm.h"


long initPosCalc(aSubRecord *pgsub){
	printf("SubRoutines init\n");
	//pgsub->vala = (long*) malloc(sizeof(long)*1000);
	memset((long*)(pgsub->vala), 0, (pgsub->nova)*sizeof(long));
	pgsub->neva = 0;
	return 0;
}
long calcX(aSubRecord *pgsub)
{
	int i;

	int32_t * buff = (int32_t*)malloc(sizeof(int32_t)*pgsub->nova);
	memset((int32_t*)(buff), 0, (pgsub->nova)*sizeof(int32_t));

        int32_t  *a = (int32_t *)(pgsub->a);

        int32_t  *b = (int32_t *)(pgsub->b);


        int32_t  *c = (int32_t *)(pgsub->c);


        int32_t  *d = (int32_t *)(pgsub->d);


        int32_t  *e = (int32_t *)(pgsub->e);

       for (i=0; i < pgsub->nova; i++)
	{
		if(a[i] == 0 && b[i] == 0 && c[i] == 0 && d[i] == 0)
			buff[i] = 0;
		else{
			buff[i] = e[0]*( b[i] + c[i] - a[i] - d[i] );
			buff[i] = buff[i]/(a[i]+d[i]+b[i]+c[i]);
			}
	}
       memcpy((int32_t*)pgsub->vala,(int32_t*)buff,sizeof(int32_t)*pgsub->nova);

       free(buff);
       pgsub->neva = (int32_t) 1000;

	return 0;
}

long calcY(aSubRecord *pgsub)
{
	int i;

	int32_t * buff = (int32_t*)malloc(sizeof(int32_t)*pgsub->nova);
	memset((int32_t*)(buff), 0, (pgsub->nova)*sizeof(int32_t));

        int32_t  *a = (int32_t *)(pgsub->a);

        int32_t  *b = (int32_t *)(pgsub->b);


        int32_t  *c = (int32_t *)(pgsub->c);


        int32_t  *d = (int32_t *)(pgsub->d);


        int32_t  *e = (int32_t *)(pgsub->e);

       for (i=0; i < pgsub->nova; i++)
	{
		if(a[i] == 0 && b[i] == 0 && c[i] == 0 && d[i] == 0)
			buff[i] = 0;
		else{
			buff[i] = e[0]*( a[i] + b[i] - c[i] - d[i] );
			buff[i] = buff[i]/(a[i]+d[i]+b[i]+c[i]);
			}
	}
       memcpy((int32_t*)pgsub->vala,(int32_t*)buff,sizeof(int32_t)*pgsub->nova);

       free(buff);
       pgsub->neva = (int32_t) 1000;

	return 0;

}

long calcQ(aSubRecord *pgsub)
{
	int i;

	int32_t * buff = (int32_t*)malloc(sizeof(int32_t)*pgsub->nova);
	memset((int32_t*)(buff), 0, (pgsub->nova)*sizeof(int32_t));

        int32_t  *a = (int32_t *)(pgsub->a);

        int32_t  *b = (int32_t *)(pgsub->b);


        int32_t  *c = (int32_t *)(pgsub->c);


        int32_t  *d = (int32_t *)(pgsub->d);


        int32_t  *e = (int32_t *)(pgsub->e);

       for (i=0; i < pgsub->nova; i++)
	{
		if(a[i] == 0 && b[i] == 0 && c[i] == 0 && d[i] == 0)
			buff[i] = 0;
		else{
			buff[i] = e[0]*( a[i] + c[i] - b[i] - d[i] );
			buff[i] = buff[i]/(a[i]+d[i]+b[i]+c[i]);
			}
	}
       memcpy((int32_t*)pgsub->vala,(int32_t*)buff,sizeof(int32_t)*pgsub->nova);

       free(buff);
       pgsub->neva = (int32_t) 1000;

	return 0;

}


long calcSUM(aSubRecord *pgsub)
{
	int i;

	int32_t * buff = (int32_t*)malloc(sizeof(int32_t)*pgsub->nova);
	memset((int32_t*)(buff), 0, (pgsub->nova)*sizeof(int32_t));

        int32_t  *a = (int32_t *)(pgsub->a);

        int32_t  *b = (int32_t *)(pgsub->b);


        int32_t  *c = (int32_t *)(pgsub->c);


        int32_t  *d = (int32_t *)(pgsub->d);


        int32_t  *e = (int32_t *)(pgsub->e);

       for (i=0; i < pgsub->nova; i++)
	{
		if(a[i] == 0 && b[i] == 0 && c[i] == 0 && d[i] == 0)
			buff[i] = 0;
		else{
			buff[i] = (a[i]+d[i]+b[i]+c[i]);
			}
	}
       memcpy((int32_t*)pgsub->vala,(int32_t*)buff,sizeof(int32_t)*pgsub->nova);

       free(buff);
       pgsub->neva = (int32_t) 1000;

	return 0;
}

epicsRegisterFunction(initPosCalc);
epicsRegisterFunction(calcX);
epicsRegisterFunction(calcY);
epicsRegisterFunction(calcQ);
epicsRegisterFunction(calcSUM);
