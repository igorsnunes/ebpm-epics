#!../../bin/linux-x86_64/prng

## You may have to change prng to something else
## everywhere it appears in this file

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/prng.dbd"
prng_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords("db/dsp.db","P=0")
dbLoadRecords("db/leds_test.db","P=0")
dbLoadRecords("db/acquisition.db","P=0")
bpmConfig("0")

cd ${TOP}/iocBoot/${IOC}
iocInit

## Start any sequence programs
#seq sncxxx,"user=igorHost"
