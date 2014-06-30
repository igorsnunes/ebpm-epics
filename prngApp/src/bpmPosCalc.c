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

	long * buff = (long *)pgsub->vala;
	for(i = 0; i < pgsub->nova; i++)
	{
		buff[i] = 0;
	}

        long  **a = (long **)&(pgsub->a);


        long  **b = (long **)&(pgsub->b);


        long  **c = (long **)&(pgsub->c);


        long  **d = (long **)&(pgsub->d);


        long  **e = (long **)&(pgsub->e);

       
        for (i=0; i < pgsub->nova; i++)
	{
                buff[i] = ((long) *(e[0])) * ((((long) *(a[i])) + ((long) *(d[i])) - ((long) *(b[i])) - ((long) *(c[i]))) / (((long) *(a[i])) + ((long) *(d[i])) + ((long) *(b[i])) + ((long) *(c[i]))));
        }
	return 0;
}

long calcY(aSubRecord *pgsub)
{
	int i;

	long * buff = (long *)pgsub->vala;
	for(i = 0; i < pgsub->nova; i++)
	{
		buff[i] = 0;
	}

        long  **a = (long **)&(pgsub->a);


        long  **b = (long **)&(pgsub->b);


        long  **c = (long **)&(pgsub->c);


        long  **d = (long **)&(pgsub->d);


        long  **e = (long **)&(pgsub->e);

       
        for (i=0; i < pgsub->nova; i++)
	{
                buff[i] = ((long) *(e[0])) * ((((long) *(a[i])) + ((long) *(b[i])) - ((long) *(c[i])) - ((long) *(d[i]))) / (((long) *(a[i])) + ((long) *(d[i])) + ((long) *(b[i])) + ((long) *(c[i]))));
        }
	return 0;
}

long calcQ(aSubRecord *pgsub)
{
	int i;

	long * buff = (long *)pgsub->vala;
	for(i = 0; i < pgsub->nova; i++)
	{
		buff[i] = 0;
	}

        long  **a = (long **)&(pgsub->a);


        long  **b = (long **)&(pgsub->b);


        long  **c = (long **)&(pgsub->c);


        long  **d = (long **)&(pgsub->d);


        long  **e = (long **)&(pgsub->e);

       
        for (i=0; i < pgsub->nova; i++)
	{
                buff[i] = ((long) *(e[0])) * ((((long) *(a[i])) + ((long) *(c[i])) - ((long) *(b[i])) - ((long) *(d[i]))) / (((long) *(a[i])) + ((long) *(d[i])) + ((long) *(b[i])) + ((long) *(c[i]))));
        }
	return 0;
}


long calcSUM(aSubRecord *pgsub)
{
	int i;

	long * buff = (long *)pgsub->vala;
	for(i = 0; i < pgsub->nova; i++)
	{
		buff[i] = 0;
	}

        long  **a = (long **)&(pgsub->a);


        long  **b = (long **)&(pgsub->b);


        long  **c = (long **)&(pgsub->c);


        long  **d = (long **)&(pgsub->d);


        long  **e = (long **)&(pgsub->e);

       
        for (i=0; i < pgsub->nova; i++)
	{
                buff[i] = ((long) *(e[0])) * (((long) *(a[i])) + ((long) *(d[i])) + ((long) *(b[i])) + ((long) *(c[i])));
        }
	return 0;
}


epicsRegisterFunction(calcX);
epicsRegisterFunction(calcY);
epicsRegisterFunction(calcQ);
epicsRegisterFunction(calcSUM);
