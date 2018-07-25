/* ************************************************************************** */
// NAME:    modbus_common.h
// AUTHOR:  Markus Jellitsch
// DATE:    19.7.2016
// DESCRIPTION: ModbusRTU common header file
/* ************************************************************************** */

#ifndef MODBUSRTU_H    
#define MODBUSRTU_H


/*++++++++++++++++++++  MODBUS RTU ++++++++++++++++++++++++++++++++ */
/*  CODE    |   MODBUS FUNCTION         |   REGISTER| IMPLEMENTED   */
/*------------------------------------------------------------------*/
/*  0x01    |   READ COIL STATUS        |   0xxxx   |               */ 
/*  0x02    |   READ INPUT STATUS       |   1xxxx   |               */
/*  0x03    |   READ HOLDING REGISTER   |   4xxxx   |   X           */
/*  0x06    |   WRITE SINGLE REGISTER   |   4xxxx   |   X           */
/*  0x08    |   DIAGNOSTIC              |           |               */
/*  0x0F    |   FORCE MULTIPLE COILS    |   0xxxx   |               */
/*------------------------------------------------------------------*/

/* SOFTWARE DEFINED SLAVE ADDRESS*/
#define MODBUS_MY_DEVICEADDRESS 0x11

#define MODBUS_OK 0x00

#define MODBUS_NUM_REGISTER 25

/* -------------- FUNCTIONS FOR PIC32MX -----------------*/
#define MODBUS_FUNC_READHOLDINGREGISTER 0x03
#define MODBUS_FUNC_WRITESINGLEREGISTER 0x06

/* macro for function code validation */
#define MODBUS_VALID_FUNC(func) (!(func & 0xF8))

#define MODBUS_DATALEN_DEFAULT              4

/*------------------- EXCEPTION CODES -------------------*/
#define MODBUS_ERRORCODE                    0x80 // on error last bit in function code is set

#define MODBUS_EXCP_ILLEGAL_FUNCTION        0x01
#define MODBUS_EXCP_ILLEGAL_DATAADDRESS     0x02
#define MODBUS_EXCP_ILLEGAL_DATAVALUE       0x03

#define MODBUS_HOLDING_4001                     0
#define MODBUS_HOLDING_4002                     1
#define MODBUS_HOLDING_4003                     2
#define MODBUS_HOLDING_4004                     3
#define MODBUS_HOLDING_4005                     4
#define MODBUS_HOLDING_4006                     5
#define MODBUS_HOLDING_4007                     6
#define MODBUS_HOLDING_4008                     7
#define MODBUS_HOLDING_4009                     8
#define MODBUS_HOLDING_4010                    9
#define MODBUS_HOLDING_4011                    10
#define MODBUS_HOLDING_4012                    11
#define MODBUS_HOLDING_4013                    12
#define MODBUS_HOLDING_4014                    13
#define MODBUS_HOLDING_4015                    14
#define MODBUS_HOLDING_4016                    15
#define MODBUS_HOLDING_4017                    16
#define MODBUS_HOLDING_4018                    17
#define MODBUS_HOLDING_4019                    18
#define MODBUS_HOLDING_4020                    19
#define MODBUS_HOLDING_4021                    20
#define MODBUS_HOLDING_4022                    21
#define MODBUS_HOLDING_4023                    22
#define MODBUS_HOLDING_4024                    23
#define MODBUS_HOLDING_4025                    24


#endif
