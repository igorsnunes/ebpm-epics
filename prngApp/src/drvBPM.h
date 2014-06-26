#define BPMBLinkLedsString "BLINK_LEDS"
#define BPMAcqOriginString "ACQ_ORIGIN"

#define BPMAcqSamplesStringADC "ACQ_SAMPLES_ADC"
#define BPMAcqSamplesStringFOFBAMP "ACQ_SAMPLES_FOFB_AMP"
#define BPMAcqSamplesStringTBTAMP "ACQ_SAMPLES_TBT_AMP"
#define BPMAcqSamplesStringFOFBPOS "ACQ_SAMPLES_FOFB_POS"
#define BPMAcqSamplesStringTBTPOS "ACQ_SAMPLES_TBT_POS"

#define BPMAcqTrigStringADC "ACQ_TRIG_ADC"
#define BPMAcqTrigStringFOFBAMP "ACQ_TRIG_FOFB_AMP"
#define BPMAcqTrigStringTBTAMP "ACQ_TRIG_TBT_AMP"
#define BPMAcqTrigStringFOFBPOS "ACQ_TRIG_FOFB_POS"
#define BPMAcqTrigStringTBTPOS "ACQ_TRIG_TBT_POS"

#define BPMAcqAdcChAWFString "ACQ_ADC_CHA_WF"
#define BPMAcqAdcChBWFString "ACQ_ADC_CHB_WF"
#define BPMAcqAdcChCWFString "ACQ_ADC_CHC_WF"
#define BPMAcqAdcChDWFString "ACQ_ADC_CHD_WF"

#define BPMAcqTbtAmpChXWFString "ACQ_TBT_AMP_CHX_WF"
#define BPMAcqTbtAmpChYWFString "ACQ_TBT_AMP_CHY_WF"
#define BPMAcqTbtAmpChQWFString "ACQ_TBT_AMP_CHQ_WF"
#define BPMAcqTbtAmpChSUMWFString "ACQ_TBT_AMP_CHSUM_WF"

#define BPMAcqTbtPosChXWFString "ACQ_TBT_POS_CHX_WF"
#define BPMAcqTbtPosChYWFString "ACQ_TBT_POS_CHY_WF"
#define BPMAcqTbtPosChQWFString "ACQ_TBT_POS_CHQ_WF"
#define BPMAcqTbtPosChSUMWFString "ACQ_TBT_POS_CHSUM_WF"


#define BPMAcqFofbAmpChXWFString "ACQ_FOFB_AMP_CHX_WF"
#define BPMAcqFofbAmpChYWFString "ACQ_FOFB_AMP_CHY_WF"
#define BPMAcqFofbAmpChQWFString "ACQ_FOFB_AMP_CHQ_WF"
#define BPMAcqFofbAmpChSUMWFString "ACQ_FOFB_AMP_CHSUM_WF"

#define BPMAcqFofbPosChXWFString "ACQ_FOFB_POS_CHX_WF"
#define BPMAcqFofbPosChYWFString "ACQ_FOFB_POS_CHY_WF"
#define BPMAcqFofbPosChQWFString "ACQ_FOFB_POS_CHQ_WF"
#define BPMAcqFofbPosChSUMWFString "ACQ_FOFB_POS_CHSUM_WF"


typedef enum {
	BPMBlinkLeds,/*int 32 write*/
	BPMAcqOrigin,/*int 32 write*/
	BPMAcqSamplesADC,/*int 32 write*/
	BPMAcqSamplesTBTAMP,/*int 32 write*/
	BPMAcqSamplesTBTPOS,/*int 32 write*/
	BPMAcqSamplesFOFBAMP,/*int 32 write*/
	BPMAcqSamplesFOFBPOS,/*int 32 write*/
	BPMAcqTrigADC,/*int 32 write*/
	BPMAcqTrigTBTAMP,/*int 32 write*/
	BPMAcqTrigTBTPOS,/*int 32 write*/
	BPMAcqTrigFOFBAMP,/*int 32 write*/
	BPMAcqTrigFOFBPOS,/*int 32 write*/
	BPMAcqAdcChAWF,/*int 16 array read*/
	BPMAcqAdcChBWF,/*int 16 array read*/
	BPMAcqAdcChCWF,/*int 16 array read*/
	BPMAcqAdcChDWF,/*int 16 array read*/
	BPMAcqTbtAmpChXWF,/*uint 32 array read*/
	BPMAcqTbtAmpChYWF,/*uint 32 array read*/
	BPMAcqTbtAmpChQWF,/*uint 32 array read*/
	BPMAcqTbtAmpChSUMWF,/*uint 32 array read*/
	BPMAcqTbtPosChXWF,/*uint 32 array read*/
	BPMAcqTbtPosChYWF,/*uint 32 array read*/
	BPMAcqTbtPosChQWF,/*uint 32 array read*/
	BPMAcqTbtPosChSUMWF,/*uint 32 array read*/
	BPMAcqFofbAmpChXWF,/*uint 32 array read*/
	BPMAcqFofbAmpChYWF,/*uint 32 array read*/
	BPMAcqFofbAmpChQWF,/*uint 32 array read*/
	BPMAcqFofbAmpChSUMWF,/*uint 32 array read*/
	BPMAcqFofbPosChXWF,/*uint 32 array read*/
	BPMAcqFofbPosChYWF,/*uint 32 array read*/
	BPMAcqFofbPosChQWF,/*uint 32 array read*/
	BPMAcqFofbPosChSUMWF,/*uint 32 array read*/
	BPMLastCommand
}BPMCommand;
	
