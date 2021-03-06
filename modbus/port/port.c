/***************************************************************
*琴岛学院 创新实验室
*机电工程系 NERV
*30-Apr-2012
*杨悦 13589226458
*QQ 357017598 
FreeModbus 通过 
eMBRegInputCB eMBRegHoldingCB 
eMBRegCoilsCB	eMBRegDiscreteCB 四个接口函数完成数据的读写操作 
**************************************************************/
//#include "stm32f10x.h"
#include "stm32f0xx.h"
#include "mb.h" 
#define u16 uint16_t
#define u8 uint8_t
void ENTER_CRITICAL_SECTION(void)//进入超临界 关总中断
{
	__set_PRIMASK(1);
}

void EXIT_CRITICAL_SECTION(void)//退出超临界 开总中断
{
	__set_PRIMASK(0);
}


u16 usRegInputBuf[10]={0x0000,0xfe02,0x1203,0x1304,0x1405,0x1506,0x1607,0x1708,0x1809};
u16 *usRegHoldingBuf=usRegInputBuf;	//一个测试用的 寄存器数组 地址0-7

u8 REG_INPUT_START=0,REG_HOLDING_START=0;
u8 REG_INPUT_NREGS=8,REG_HOLDING_NREGS=8;
u8 usRegInputStart=0,usRegHoldingStart=0;

//读数字寄存器 功能码0x04

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )&& ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );

        while( usNRegs > 0 )
         {
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );//PDU的先输出reg的高位；
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );//再输出低位；
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;

}
// 寄存器的读写函数 支持的命令为读 0x03 和写0x06

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        switch ( eMode )
        {
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
            *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] & 0xFF );
	    iRegIndex++;
             usNRegs--;
            }
            break;

        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
             usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
             usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
             iRegIndex++;
             usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

//读/写开关寄存器  0x01  x05

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNCoils;
    ( void )eMode;
    return MB_ENOREG;
}

//读开关寄存器 0x02
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNDiscrete;
    return MB_ENOREG;
}
