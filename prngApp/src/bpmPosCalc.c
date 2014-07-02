#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbAccess.h"
#include "epicsTypes.h"
#include "registryFunction.h" /* for epicsExport           */
#include "epicsExport.h"      /* for epicsRegisterFunction */
#include "aSubRecord.h"       /* for struct aSubRecord  */
#include "alarm.h"


long calcX(aSubRecord *pgsub)
{
	int i;

	int32_t * buff = (int32_t *)pgsub->vala;
	for(i = 0; i < pgsub->nova; i++)
	{
		buff[i] = 0;
	}

        int32_t  *a = (int32_t *)(pgsub->a);

        int32_t  *b = (int32_t *)(pgsub->b);


        int32_t  *c = (int32_t *)(pgsub->c);


        int32_t  *d = (int32_t *)(pgsub->d);


        long  *e = (long *)(pgsub->e);

       for (i=0; i < pgsub->nova; i++)
	{
		if(a[i] == 0 && b[i] == 0 && c[i] == 0 && d[i] == 0)
			buff[i] = 0;
                else{
			buff[i] = ((a[i]+d[i]-b[i]-c[i])/(a[i]+b[i]+c[i]+d[i]));
			//printf("%d %d %d %d %d\n",a[i],b[i],c[i],d[i],buff[i]); 
        	}
	}
	return 0;
}

long calcY(aSubRecord *pgsub)
{
	int i;

	int32_t * buff = (int32_t *)pgsub->vala;
	for(i = 0; i < pgsub->nova; i++)
	{
		buff[i] = 0;
	}

        int32_t  *a = (int32_t *)(pgsub->a);


        int32_t  *b = (int32_t *)(pgsub->b);


        int32_t  *c = (int32_t *)(pgsub->c);


        int32_t  *d = (int32_t *)(pgsub->d);


        int32_t  *e = (int32_t *)(pgsub->e);

       
        for (i=0; i < pgsub->nova; i++)
	{
		if(a[i] == 0 && b[i] == 0 && c[i] == 0 && d[i] == 0)
			buff[i] = 0;

		else
			//printf("%lu %lu %lu %lu %lu\n",((long)*(e[0])),((long)*(a[i])),((long)*(b[i])),((long)*(c[i])),((long)*(d[i])));
                	buff[i] = (a[i] + b[i] - c[i] - d[i]) / (a[i] +d[i] + b[i] + c[i]);
        }
	return 0;
}

long calcQ(aSubRecord *pgsub)
{
	int i;

	int32_t * buff = (int32_t *)pgsub->vala;
	for(i = 0; i < pgsub->nova; i++)
	{
		buff[i] = 0;
	}

        int32_t  *a = (int32_t *)(pgsub->a);


        int32_t  *b = (int32_t *)(pgsub->b);


        int32_t  *c = (int32_t *)(pgsub->c);


        int32_t  *d = (int32_t *)(pgsub->d);


        int32_t  *e = (int32_t *)(pgsub->e);

        for (i=0; i < pgsub->nova; i++)
	{
		if(a[i] == 0 && b[i] == 0 && c[i] == 0 && d[i] == 0)
			buff[i] = 0;
		else
                	buff[i] = (a[i]+c[i]-b[i]-d[i]) / (a[i]+d[i]+b[i]+c[i]);
        }
	return 0;
}


long calcSUM(aSubRecord *pgsub)
{
	int i;

	int32_t * buff = (int32_t *)pgsub->vala;
	for(i = 0; i < pgsub->nova; i++)
	{
		buff[i] = 0;
	}

        int32_t  *a = (int32_t *)(pgsub->a);


        int32_t  *b = (int32_t *)(pgsub->b);


        int32_t  *c = (int32_t *)(pgsub->c);


        int32_t  *d = (int32_t *)(pgsub->d);


        int32_t  *e = (int32_t *)(pgsub->e);

        for (i=0; i < pgsub->nova; i++)
	{
                buff[i] = a[i] + d[i] + b[i] + c[i];
        }
	for(i=0; i < 10; i++){
		//int32_t aux = (int32_t)pgsub->vala[i];
		printf("%d\n",buff[i]);
	}
	return 0;
}


epicsRegisterFunction(calcX);
epicsRegisterFunction(calcY);
epicsRegisterFunction(calcQ);
epicsRegisterFunction(calcSUM);
