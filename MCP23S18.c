/**
 **********************************************************************************
 * @file   MCP23S18.c
 * @author Mohammad Sayadi (https://github.com/msthrax)
 *         Ali Moallem (https://github.com/AliMoal)
 * @brief  For working with MCP23S18
 **********************************************************************************
 *
 *! Copyright (c) 2021 Mahda Embedded System (MIT License)
 *!
 *! Permission is hereby granted, free of charge, to any person obtaining a copy
 *! of this software and associated documentation files (the "Software"), to deal
 *! in the Software without restriction, including without limitation the rights
 *! to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *! copies of the Software, and to permit persons to whom the Software is
 *! furnished to do so, subject to the following conditions:
 *!
 *! The above copyright notice and this permission notice shall be included in all
 *! copies or substantial portions of the Software.
 *!
 *! THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *! IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *! FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *! AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *! LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *! OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *! SOFTWARE.
 *!
 **********************************************************************************
 **/

//* Private Includes -------------------------------------------------------------- //
#include "MCP23S18.h"

//* Private Defines and Macros ---------------------------------------------------- //
#define MCP23S18_USE_MACRO_SELECT_BANK // To Avoid From Conflict For Addressing | After commenting this, Toggle comment all "// FOR_SELECT_BANK_MACRO" Lines

#define MCP23S18_WRITE_ADDR 0x40 // Default opcode address of the MCP23S18
#define MCP23S18_READ_ADDR  0x41 // Default opcode address of the MCP23S18

#define IODIRn_IO0          (1<<0)
#define IODIRn_IO1          (1<<1)
#define IODIRn_IO2          (1<<2)
#define IODIRn_IO3          (1<<3)
#define IODIRn_IO4          (1<<4)
#define IODIRn_IO5          (1<<5)
#define IODIRn_IO6          (1<<6)
#define IODIRn_IO7          (1<<7)
#define IPOLn_IP0           (1<<0)
#define IPOLn_IP1           (1<<1)
#define IPOLn_IP2           (1<<2)
#define IPOLn_IP3           (1<<3)
#define IPOLn_IP4           (1<<4)
#define IPOLn_IP5           (1<<5)
#define IPOLn_IP6           (1<<6)
#define IPOLn_IP7           (1<<7)
#define GPINTENn_GPINT0     (1<<0)
#define GPINTENn_GPINT1     (1<<1)
#define GPINTENn_GPINT2     (1<<2)
#define GPINTENn_GPINT3     (1<<3)
#define GPINTENn_GPINT4     (1<<4)
#define GPINTENn_GPINT5     (1<<5)
#define GPINTENn_GPINT6     (1<<6)
#define GPINTENn_GPINT7     (1<<7)
#define DEFVALn_DEF0        (1<<0)
#define DEFVALn_DEF1        (1<<1)
#define DEFVALn_DEF2        (1<<2)
#define DEFVALn_DEF3        (1<<3)
#define DEFVALn_DEF4        (1<<4)
#define DEFVALn_DEF5        (1<<5)
#define DEFVALn_DEF6        (1<<6)
#define DEFVALn_DEF7        (1<<7)
#define INTCONn_IOC0        (1<<0)
#define INTCONn_IOC1        (1<<1)
#define INTCONn_IOC2        (1<<2)
#define INTCONn_IOC3        (1<<3)
#define INTCONn_IOC4        (1<<4)
#define INTCONn_IOC5        (1<<5)
#define INTCONn_IOC6        (1<<6)
#define INTCONn_IOC7        (1<<7)
#define IOCON_INTCC         (1<<0)
#define IOCON_INTPOL        (1<<1)
#define IOCON_ODR           (1<<2)
#define IOCON_RSVD0         (1<<3)
#define IOCON_RSVD1         (1<<4)
#define IOCON_SEQOP         (1<<5)
#define IOCON_MIRROR        (1<<6)
#define IOCON_BANK          (1<<7)
#define IOX_ALLOUTPUT       0x00000000
#define IOX_ALLINPUT        0xFFFFFFFF 
#define IOX_ALLINVERTED     0x00000000
#define IOX_NONEINVERTED    0xFFFFFFFF

//* Others ------------------------------------------------------------------------ //
#ifdef MCP23S18_USE_MACRO_SELECT_BANK 
#define SelectBank 0 // 0: BANK0 | 1:BANK1
#else
#define SelectBank Handler->SelectBank
#endif

#ifdef MCP23S18_Debug_Enable
#include <stdio.h> // for debug
#define PROGRAMLOG(arg...) printf(arg)
#else
#define PROGRAMLOG(arg...)
#endif
/**
 ** ==================================================================================
 **                            ##### Private Enums #####                               
 ** ==================================================================================
 **/
enum RegisterNames{
  IODIRA = 0, IODIRB, IPOLA, IPOLB, GPINTENA, GPINTENB, DEFVALA, DEFVALB, INTCONA, INTCONB,
  IOCONA, IOCONB, GPPUA, GPPUB, INTFA, INTFB, INTCAPA, INTCAPB, GPIOA, GPIOB, OLATA, OLATB
};
/**
 ** ==================================================================================
 **                          ##### Private Constants #####                               
 ** ==================================================================================
 **/
static const uint8_t
BankAdd[2][22] = {// First Dir: BANK0 | Second Dir: BANK1
  {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15},
  {0x00, 0x10, 0x01, 0x11, 0x02, 0x12, 0x03, 0x13, 0x04, 0x14, 0x05, 0x15, 0x06, 0x16, 0x07, 0x17, 0x08, 0x18, 0x09, 0x19, 0x0A, 0x1A}
};
/**
 *! ==================================================================================
 *!                          ##### Private Functions #####                               
 *! ==================================================================================
 **/
static uint8_t MCP23S18_ReadReg(MCP23S18_Handler_t * MCP23S18_Handler, uint8_t reg) {
  MCP23S18_Handler->MCP23S18_CS_Low();
  MCP23S18_Handler->MCP23S18_Spi_WriteByte(MCP23S18_READ_ADDR);
  MCP23S18_Handler->MCP23S18_Spi_WriteByte(reg);
  uint8_t ret = MCP23S18_Handler->MCP23S18_Spi_ReadByte();    
  MCP23S18_Handler->MCP23S18_CS_High();
  return ret;
}

static void MCP23S18_WriteReg(MCP23S18_Handler_t * MCP23S18_Handler, uint8_t reg, uint8_t value) {
  MCP23S18_Handler->MCP23S18_CS_Low();
  MCP23S18_Handler->MCP23S18_Spi_WriteByte(MCP23S18_WRITE_ADDR);
  MCP23S18_Handler->MCP23S18_Spi_WriteByte(reg);
  MCP23S18_Handler->MCP23S18_Spi_WriteByte(value);
  MCP23S18_Handler->MCP23S18_CS_High();
}

/**
 ** ==================================================================================
 **                           ##### Public Functions #####                               
 ** ==================================================================================
 **/
// Initialization Function: ------------------------------------------------------- //
void MCP23S18_Init(MCP23S18_Handler_t *Handler) { 
  // SelectBank = 0; // POR/RST: BANK0 // FOR_SELECT_BANK_MACRO
  //? USER INIT CODE begin ?//
  
  
  //? USER INIT CODE End   ?//
  PROGRAMLOG("IODIRA:   0x%02X | IODIRB:   0x%02X\r\n"
             "IPOLA:    0x%02X | IPOLB:    0x%02X\r\n"
             "GPINTENA: 0x%02X | GPINTENB: 0x%02X\r\n"
             "DEFVALA:  0x%02X | DEFVALB:  0x%02X\r\n"
             "INTCONA:  0x%02X | INTCONB:  0x%02X\r\n"
             "IOCONA:   0x%02X | IOCONB:   0x%02X\r\n"
             "GPPUA:    0x%02X | GPPUB:    0x%02X\r\n"
             "INTFA:    0x%02X | INTFB:    0x%02X\r\n"
             "INTCAPA:  0x%02X | INTCAPB:  0x%02X\r\n"
             "GPIOA:    0x%02X | GPIOB:    0x%02X\r\n"
             "OLATA:    0x%02X | OLATB:    0x%02X\r\n",
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IODIRA]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IODIRB]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IPOLA]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IPOLB]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPINTENA]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPINTENB]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][DEFVALA]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][DEFVALB]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCONA]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCONB]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IOCONA]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IOCONB]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPPUA]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPPUB]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTFA]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTFB]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCAPA]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCAPB]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPIOA]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPIOB]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][OLATA]),
  MCP23S18_ReadReg(Handler, BankAdd[SelectBank][OLATB]));
}
// Setting Functions: ------------------------------------------------------------- //
void MCP23S18_SetConfig(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_Config_t Config) { // IOCON
  Config[0] &= 0x7F; // For Now Must Use BANK0 // FOR_SELECT_BANK_MACRO
  Config[1] &= 0x7F; // For Now Must Use BANK0 // FOR_SELECT_BANK_MACRO
#ifdef MCP23S18_SAVE_DATA_VALUE  
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][IOCONA], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IOCONA]) & (~MaskA)) | (MaskA & Config[0]));
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][IOCONB], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IOCONB]) & (~MaskB)) | (MaskB & Config[1]));
#else
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][IOCONA], MaskA & Config[0]);
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][IOCONB], MaskB & Config[1]);
#endif
}
void MCP23S18_GetConfig(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_Config_t Config) { // IOCON
#ifdef MCP23S18_SAVE_DATA_VALUE   
  if (MaskA) // GPIOA 
    Config[0] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IOCONA])) & MaskA) | (Config[0] & (~MaskA));
  if (MaskB) // GPIOB
    Config[1] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IOCONB])) & MaskB) | (Config[1] & (~MaskB));
#else
  if (MaskA) // GPIOA 
    Config[0] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IOCONA]) & MaskA;
  if (MaskB) // GPIOB
    Config[1] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IOCONB]) & MaskB;
#endif
}
void MCP23S18_SetDirection(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_Direction_t Direction) { // IODIR
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][IODIRA], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IODIRA]) & (~MaskA)) | (MaskA & Direction[0]));
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][IODIRB], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IODIRB]) & (~MaskB)) | (MaskB & Direction[1]));
#else
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][IODIRA], MaskA & Direction[0]);
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][IODIRB], MaskB & Direction[1]);
#endif
}
void MCP23S18_GetDirection(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_Direction_t Direction) { // IODIR
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    Direction[0] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IODIRA])) & MaskA) | (Direction[0] & (~MaskA));
  if (MaskB) // GPIOB
    Direction[1] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IODIRB])) & MaskB) | (Direction[1] & (~MaskB));
#else
  if (MaskA) // GPIOA 
    Direction[0] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IODIRA]) & MaskA;
  if (MaskB) // GPIOB
    Direction[1] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IODIRB]) & MaskB;
#endif
}
void MCP23S18_SetTypes(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_Type_t Type) { // IPOL
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][IPOLA], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IPOLA]) & (~MaskA)) | (MaskA & Type[0]));
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][IPOLB], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IPOLB]) & (~MaskB)) | (MaskB & Type[1]));
#else
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][IPOLA], MaskA & Type[0]);
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][IPOLB], MaskB & Type[1]);
#endif
}
void MCP23S18_GetTypes(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_Type_t Type) { // IPOL
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    Type[0] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IPOLA])) & MaskA) | (Type[0] & (~MaskA));
  if (MaskB) // GPIOB
    Type[1] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IPOLB])) & MaskB) | (Type[1] & (~MaskB));
#else
  if (MaskA) // GPIOA 
    Type[0] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IPOLA]) & MaskA;
  if (MaskB) // GPIOB
    Type[1] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][IPOLB]) & MaskB;
#endif
}
void MCP23S18_SetPullUp(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_PullUp_t PullUp) { // GPPU
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][GPPUA], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPPUA]) & (~MaskA)) | (MaskA & PullUp[0]));
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][GPPUB], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPPUB]) & (~MaskB)) | (MaskB & PullUp[1]));
#else
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][GPPUA], MaskA & PullUp[0]);
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][GPPUB], MaskB & PullUp[1]);
#endif
}
void MCP23S18_GetPullUp(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_PullUp_t PullUp) { // GPPU
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    PullUp[0] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPPUA])) & MaskA) | (PullUp[0] & (~MaskA));
  if (MaskB) // GPIOB
    PullUp[1] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPPUB])) & MaskB) | (PullUp[1] & (~MaskB));
#else
  if (MaskA) // GPIOA 
    PullUp[0] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPPUA]) & MaskA;
  if (MaskB) // GPIOB
    PullUp[1] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPPUB]) & MaskB;
#endif
}
void MCP23S18_SetIntEnable(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntEnable_t IntEnable) { // GPINTEN
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][GPINTENA], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPINTENA]) & (~MaskA)) | (MaskA & IntEnable[0]));
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][GPINTENB], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPINTENB]) & (~MaskB)) | (MaskB & IntEnable[1]));
#else
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][GPINTENA], MaskA & IntEnable[0]);
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][GPINTENB], MaskB & IntEnable[1]);
#endif
}
void MCP23S18_GetIntEnable(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntEnable_t IntEnable) { // GPINTEN
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    IntEnable[0] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPINTENA])) & MaskA) | (IntEnable[0] & (~MaskA));
  if (MaskB) // GPIOB
    IntEnable[1] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPINTENB])) & MaskB) | (IntEnable[1] & (~MaskB));
#else
  if (MaskA) // GPIOA 
    IntEnable[0] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPINTENA] & MaskA);
  if (MaskB) // GPIOB
    IntEnable[1] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPINTENB] & MaskB);
#endif
}
void MCP23S18_SetIntDefaultVal(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntDefaultVal_t IntDefaultVal) { // DEFVAL
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][DEFVALA], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][DEFVALA]) & (~MaskA)) | (MaskA & IntDefaultVal[0]));
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][DEFVALB], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][DEFVALB]) & (~MaskB)) | (MaskB & IntDefaultVal[1]));
#else
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][DEFVALA], MaskA & IntDefaultVal[0]);
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][DEFVALB], MaskB & IntDefaultVal[1]);
#endif
}
void MCP23S18_GetIntDefaultVal(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntDefaultVal_t IntDefaultVal) { // DEFVAL
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    IntDefaultVal[0] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][DEFVALA])) & MaskA) | (IntDefaultVal[0] & (~MaskA));
  if (MaskB) // GPIOB
    IntDefaultVal[1] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][DEFVALB])) & MaskB) | (IntDefaultVal[1] & (~MaskB));
#else
  if (MaskA) // GPIOA 
    IntDefaultVal[0] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][DEFVALA]) & MaskA;
  if (MaskB) // GPIOB
    IntDefaultVal[1] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][DEFVALB]) & MaskB;
#endif
}
void MCP23S18_SetIntCompare(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntCompare_t IntCompare) { // INTCON
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][INTCONA], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCONA]) & (~MaskA)) | (MaskA & IntCompare[0]));
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][INTCONB], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCONB]) & (~MaskB)) | (MaskB & IntCompare[1]));
#else
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][INTCONA], MaskA & IntCompare[0]);
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][INTCONB], MaskB & IntCompare[1]);
#endif
}
void MCP23S18_GetIntCompare(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntCompare_t IntCompare) { // INTCON
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    IntCompare[0] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCONA])) & MaskA) | (IntCompare[0] & (~MaskA));
  if (MaskB) // GPIOB
    IntCompare[1] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCONB])) & MaskB) | (IntCompare[1] & (~MaskB));
#else
  if (MaskA) // GPIOA 
    IntCompare[0] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCONA]) & MaskA;
  if (MaskB) // GPIOB
    IntCompare[1] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCONB]) & MaskB;
#endif
}
void MCP23S18_SetIntOccurred(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntOccurred_t IntOccurred) { // INTF
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][INTFA], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTFA]) & (~MaskA)) | (MaskA & IntOccurred[0]));
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][INTFB], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTFB]) & (~MaskB)) | (MaskB & IntOccurred[1]));
#else
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][INTFA], MaskA & IntOccurred[0]);
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][INTFB], MaskB & IntOccurred[1]);
#endif
}
void MCP23S18_GetIntOccurred(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntOccurred_t IntOccurred) { // INTF
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    IntOccurred[0] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTFA])) & MaskA) | (IntOccurred[0] & (~MaskA));
  if (MaskB) // GPIOB
    IntOccurred[1] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTFB])) & MaskB) | (IntOccurred[1] & (~MaskB));
#else
  if (MaskA) // GPIOA 
    IntOccurred[0] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTFA]) & MaskA;
  if (MaskB) // GPIOB
    IntOccurred[1] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTFB]) & MaskB;
#endif
}
void MCP23S18_SetIntCaptureState(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntCaptureState_t IntCaptureState) { // INTCAP
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][INTCAPA], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCAPA]) & (~MaskA)) | (MaskA & IntCaptureState[0]));
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][INTCAPB], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCAPB]) & (~MaskB)) | (MaskB & IntCaptureState[1]));
#else
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][INTCAPA], MaskA & IntCaptureState[0]);
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][INTCAPB], MaskB & IntCaptureState[1]);
#endif
}
void MCP23S18_GetIntCaptureState(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntCaptureState_t IntCaptureState) { // INTCAP
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    IntCaptureState[0] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCAPA])) & MaskA) | (IntCaptureState[0] & (~MaskA));
  if (MaskB) // GPIOB
    IntCaptureState[1] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCAPB])) & MaskB) | (IntCaptureState[1] & (~MaskB));
#else
  if (MaskA) // GPIOA 
    IntCaptureState[0] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCAPA]) & MaskA;
  if (MaskB) // GPIOB
    IntCaptureState[1] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][INTCAPB]) & MaskB;
#endif
}
// Read/Write Functions: ---------------------------------------------------------- //
void MCP23S18_ReadInput(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_GPIOState_t GPIOState) { // GPIO
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    GPIOState[0] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPIOA])) & MaskA) | (GPIOState[0] & (~MaskA));
  if (MaskB) // GPIOB
    GPIOState[1] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPIOB])) & MaskB) | (GPIOState[1] & (~MaskB));
#else
  if (MaskA) // GPIOA 
    GPIOState[0] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPIOA]) & MaskA;
  if (MaskB) // GPIOB
    GPIOState[1] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][GPIOB]) & MaskB;
#endif
}
void MCP23S18_ReadOutput(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_GPIOState_t GPIOState) { // OLAT
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    GPIOState[0] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][OLATA])) & MaskA) | (GPIOState[0] & (~MaskA));
  if (MaskB) // GPIOB
    GPIOState[1] = ((MCP23S18_ReadReg(Handler, BankAdd[SelectBank][OLATB])) & MaskB) | (GPIOState[1] & (~MaskB));
#else
  if (MaskA) // GPIOA 
    GPIOState[0] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][OLATA]) & MaskA;
  if (MaskB) // GPIOB
    GPIOState[1] = MCP23S18_ReadReg(Handler, BankAdd[SelectBank][OLATB]) & MaskB;
#endif
}
void MCP23S18_WriteOutput(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_GPIOState_t GPIOState) { // OLAT
#ifdef MCP23S18_SAVE_DATA_VALUE 
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][OLATA], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][OLATA]) & (~MaskA)) | (MaskA & GPIOState[0]));
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][OLATB], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][OLATB]) & (~MaskB)) | (MaskB & GPIOState[1]));
#else
  if (MaskA) // GPIOA 
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][OLATA], MaskA & GPIOState[0]);
  if (MaskB) // GPIOB
    MCP23S18_WriteReg(Handler, BankAdd[SelectBank][OLATB], MaskB & GPIOState[1]);
#endif
}
void MCP23S18_WriteOutputFAST(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_GPIOState_t GPIOState) { // OLAT
  MCP23S18_WriteReg(Handler, BankAdd[SelectBank][OLATA], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][OLATA]) & (~MaskA)) | (MaskA & GPIOState[0])); // GPIOA
  MCP23S18_WriteReg(Handler, BankAdd[SelectBank][OLATB], (MCP23S18_ReadReg(Handler, BankAdd[SelectBank][OLATB]) & (~MaskB)) | (MaskB & GPIOState[1])); // GPIOB
}
void MCP23S18_WriteOutputPortAHigh(MCP23S18_Handler_t *Handler) { // OLAT
  MCP23S18_WriteReg(Handler, BankAdd[SelectBank][OLATA], 0xFF);
//  PROGRAMLOG("OLATA: 0x%X\r\n",
//  MCP23S18_ReadReg(MCP23S18_Handler, OLATA_BANK0));
}
void MCP23S18_WriteOutputPortALow(MCP23S18_Handler_t *Handler) { // OLAT
  MCP23S18_WriteReg(Handler, BankAdd[SelectBank][OLATA], 0x00);
//  PROGRAMLOG("OLATA: 0x%X\r\n",
//  MCP23S18_ReadReg(MCP23S18_Handler, OLATA_BANK0));
}
void MCP23S18_WriteOutputPortBHigh(MCP23S18_Handler_t *Handler) { // OLAT
  MCP23S18_WriteReg(Handler, BankAdd[SelectBank][OLATB], 0xFF);
//  PROGRAMLOG("OLATB: 0x%X\r\n",
//  MCP23S18_ReadReg(MCP23S18_Handler, OLATB_BANK0));
}
void MCP23S18_WriteOutputPortBLow(MCP23S18_Handler_t *Handler) { // OLAT
  MCP23S18_WriteReg(Handler, BankAdd[SelectBank][OLATB], 0x00);
//  PROGRAMLOG("OLATB: 0x%X\r\n",0
//  MCP23S18_ReadReg(MCP23S18_Handler, OLATB_BANK0));
}
//----------------------------------------------------------------------------------------------------------------------------------------------
