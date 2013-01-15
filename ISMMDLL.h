
#ifndef __ISMMDLL_H__
#define __ISMMDLL_H__

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HWM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ISMM_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef __cplusplus
extern "C" { /* Assume C declarations for C++ */
#endif  /* __cplusplus */ 

#ifdef ISMM_EXPORTS
#define ISMM_API __declspec(dllexport)
#else
#define ISMM_API __declspec(dllimport)
#endif

typedef struct _HWM_DATA {
  ULONG FAN0;
  ULONG FAN1;
  ULONG FAN2;
  ULONG FAN3;
  ULONG FAN4;
  FLOAT VIN0;
  FLOAT VIN1;
  FLOAT VIN2;
  FLOAT VIN3;
  FLOAT VIN4;
  FLOAT VIN5;
  FLOAT VIN6;
  FLOAT VIN7;
  FLOAT VIN8;
  FLOAT VIN9;
  FLOAT TMPIN0;
  FLOAT TMPIN1;
  FLOAT TMPIN2;
} HWM_DATA, *PHWM_DATA;

typedef struct _HWM_FAN_CNTRL {
  UCHAR FanMode;		// 0 for ON/OFF mode, 1 for SW mode, 2 for auto mode
  UCHAR ModeData;		// 0/1 for FanMode 0, 0..127 for FanMode 1
  UCHAR FanOffTmpr;		// fan off temperature
  UCHAR FanStartTmpr;	// fan start temperature
  UCHAR FanFullTmpr;	// fan full speed temperature
  UCHAR FanStartPwm;	// fan start PWM
  UCHAR FanSlopePwm;	// PWM slope
} HWM_FAN_CNTRL, *PHWM_FAN_CNTRL;

typedef enum {
    TYPE_FAN = 0,
    TYPE_VOLTAGE = 1,
    TYPE_TEMPERATURE = 2,
} ITEM_TYPE_ENUM;

typedef enum {
    FAN_OP_MODE_ONOFF = 0,	// On-Off mode
    FAN_OP_MODE_SW = 1,		// controllable PWM
    FAN_OP_MODE_AUTO = 2,	// smart mode - automatic
} FAN_OP_MODE_ENUM;

typedef enum {
    WDT_TIMEOPT_SECOND = 1,
    WDT_TIMEOPT_MINUTE = 2,
} WDT_TIMEOPT_ENUM;

ISMM_API HANDLE ISMM_Open(void);
ISMM_API BOOL ISMM_Close(HANDLE hIsmm);
ISMM_API BOOL HWM_GetItemNodes(HANDLE hIsmm, WORD wItemType, LPWORD pwNodeCount);
ISMM_API BOOL HWM_GetItemDesc(HANDLE hIsmm, WORD wItemType, WORD wItemIndex, 
											LPWORD pcbDataSize, LPBYTE pData);
ISMM_API BOOL HWM_GetData(HANDLE hIsmm, PHWM_DATA pHwmData);
ISMM_API BOOL HWM_FanControl(HANDLE hIsmm, WORD wFanIndex,
						 PHWM_FAN_CNTRL pHwmFanCntrl);

ISMM_API BOOL HWM_DbgGetReg(HANDLE hIsmm, UCHAR ucRegIndex, LPBYTE pRegData);
ISMM_API BOOL HWM_DbgSetReg(HANDLE hIsmm, UCHAR ucRegIndex, UCHAR ucRegData); 

ISMM_API BOOL DIO_GetConfig(HANDLE hIsmm, PWORD pwDioConfig);
ISMM_API BOOL DIO_GetInput(HANDLE hIsmm, PWORD pwDioData);
ISMM_API BOOL DIO_SetOutput(HANDLE hIsmm, WORD wDioData);
ISMM_API BOOL DIO_GetOutput(HANDLE hIsmm, PWORD pwDioData);

ISMM_API BOOL DIO_GetCDInput(HANDLE hIsmm, PUCHAR pcDioCDData);
ISMM_API BOOL DIO_GetCDConfig(HANDLE hIsmm, PUCHAR pcDioCDData);
ISMM_API BOOL DIO_SetCDOutput(HANDLE hIsmm, UCHAR ucDioCDNum);
ISMM_API BOOL DIO_SetCDOutputEx(HANDLE hIsmm, UCHAR ucDioCDNum, bool open);
ISMM_API BOOL DIO_SetSpecificSetting(HANDLE hIsmm, UCHAR ucMask);

ISMM_API BOOL WDT_SetTimeout(HANDLE hIsmm, WORD wTimeout, WORD wTimeOption);
ISMM_API BOOL WDT_GetTimeoutOption(HANDLE hIsmm, PWORD pwTimeout, PWORD pwTimeOption);
ISMM_API BOOL WDT_Start(HANDLE hIsmm, BOOL bControlState);
ISMM_API BOOL WDT_Restart(HANDLE hIsmm);


#ifdef __cplusplus
}       /* End of extern "C" { */
#endif  /* __cplusplus */


  
#endif	// __ISMMDLL_H__