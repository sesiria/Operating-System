/****************************************************************
        miniOS HAL Global data structure halglobal_t.h
*****************************************************************
****************************************************************/
#ifndef _HALGLOBAL_T_H
#define _HALGLOBAL_T_H
#define HAL_DEFGLOB_VARIABLE(vartype,varname) \
EXTERN  __attribute__((section(".data"))) vartype varname

#endif // HALGLOBAL_T_H
