#ifndef __CUSTOM_MEMORYDEVICE__
#define __CUSTOM_MEMORYDEVICE__

/*
 ****************************************************************************
 [README , VERY IMPORTANT NOTICE]
 --------------------------------
 After user configured this C header file, not only C compiler compile it but
 also auto-gen tool parse user's configure setting.
 Here are recommend configure convention to make both work fine.

 1. All configurations in this file form as #define MACRO_NAME MACRO_VALUE format.
    Note the #define must be the first non-space character of a line

 2. To disable the optional configurable item. Please use // before #define,
    for example: //#define MEMORY_DEVICE_TYPE

 3. Please don't use #if , #elif , #else , #endif conditional macro key word here.
    Such usage might cause compile result conflict with auto-gen tool parsing result.
    Auto-Gen tool will show error and stop.
    3.1.  any conditional keyword such as #if , #ifdef , #ifndef , #elif , #else detected.
          execpt this #ifndef __CUSTOM_MEMORYDEVICE__
    3.2.  any duplicated MACRO_NAME parsed. For example auto-gen tool got 
          2nd MEMORY_DEVICE_TYPE macro value.
 ****************************************************************************
*/

/*
 ****************************************************************************
 Step 1: Specify memory device type and its complete part number
         Possible memory device type: LPSDRAM (SDR, DDR)
 ****************************************************************************
*/

#define BOARD_ID                MT6582_EVB
//p201 4+1
#define CS_PART_NUMBER[0]	KE4CN2L2HA8A2A
#define CS_PART_NUMBER[1]	KMK5U000VM_B309
#define CS_PART_NUMBER[2]	H9TP32A8JDACPR_KGM
#define CS_PART_NUMBER[3]	SD7DP28C_4G
//p201 8+1
#define CS_PART_NUMBER[4] SD7DP28C_8G
#define CS_PART_NUMBER[5] KMK7X000VM_B314
#define CS_PART_NUMBER[6] TYD0GH231624RC
#define CS_PART_NUMBER[7] H9TP65A8JDACPR_KGM

#endif /* __CUSTOM_MEMORYDEVICE__ */
