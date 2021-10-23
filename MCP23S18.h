/**
 **********************************************************************************
 * @file   MCP23S18.h
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
#ifndef _MCP23S18_H_
#define _MCP23S18_H_

#ifdef __cplusplus
extern "C" {
#endif

//* Includes ---------------------------------------------------------------------- //
#include <stdint.h>
#include <stdbool.h>

//? User Configurations and Notes ------------------------------------------------- //
// Important Notes:
// 1. SPI Configuration: 8Bits, CPOL=LOW(0), CPHA=1EDGE(0), Max speed: 10MHz (referred to Datasheet)
// 2. For interrupt, User must handle IRQ itself
// 3. Comment MCP23S18_SAVE_DATA_VALUE define whenever you want Getting/Reading or Setting/Writing functions work FASTER.
//    !BE CAREFUL if you comment this, All the data in register(for Setting/Writing) or variable(for Getting/Reading) with every Get/Read or Set/Write function lost and get new ones
//    !It means that the deselected pins will set as ZERO!
//    !This is just for Pin configs and the deselected Ports will NOT be changed
//    *Although You can use WriteFast functions without commenting MCP23S18_SAVE_DATA_VALUE define
//! 4. In this Library Delay IS NOT USED and You DO NOT NEED to initialize Delay functions !//
#define MCP23S18_SAVE_DATA_VALUE                     // READ NOTE ABOVE NO. 3
#define MCP23S18_USE_MACRO_DELAY         1           // 0: Use handler delay ,So you have to set ADC_Delay_US in Handler | 1: use Macro delay, So you have to set MACRO_DELAY_US Macro
// #define MCP23S18_MACRO_DELAY_US(x)                   // If you want to use Macro delay, place your delay function in microseconds here
#define MCP23S18_Debug_Enable                        // Uncomment if you want to use (depends on printf in stdio.h)
// #pragma anon_unions                                  // UNIMPLEMENTED !!! Uncomment if you are using Keil software
//? ------------------------------------------------------------------------------- //

//! DO NOT USE OR EDIT THIS BLOCK ------------------------------------------------- //
#if MCP23S18_USE_MACRO_DELAY == 0
#define MCP23S18_Delay_US(x)   Handler->MCP23S18_Delay_US(x)
#else
#define MCP23S18_Delay_US(x)   MACRO_DELAY_US(x)
#ifndef MCP23S18_MACRO_DELAY_US
#error "MCP23S18_MACRO_DELAY_US is not defined. Please Use handler delay or config MCP23S18_MACRO_DELAY_US macro, You can choose it on MCP23S18_USE_MACRO_DELAY define"
#endif
#endif
//! ------------------------------------------------------------------------------- //

/**
 ** ==================================================================================
 **                                ##### Enums #####                               
 ** ==================================================================================
 **/

//! For All Below Two-dimensional typedefs > First Dimension: GPIOA | Second Dimension: GPIOB !//
// ---------------
typedef uint8_t                // MSB              1:Enable | 0:Disable                 LSB
MCP23S18_Config_t[2];          // BANK | MIRROR | SEQOP | ---- | ---- | ODR  | INTPOL | INTCC
// INTCC:  Interrupt Clearing Control                      | 0: GPIO register clears the interrupt | 1: INTCAP register clears the interrupt
// INTPOL: Sets the polarity of the INT output pin         | 0: Active-low | 1: Active-high
// ODR:    Configures the INT pin as an open-drain output  | 0: Active driver output (INTPOL bit sets the polarity) | 1: Open-drain output (overrides the INTPOL bit)
// UNIMPLEMENTED
// UNIMPLEMENTED
// SEQOP:  Sequential Operation mode bit                   | 0: Sequential operation enabled, address pointer increments | 1: Sequential operation disabled, address pointer does not increment
// MIRROR: INT pins mirror bit                             | 0: The INT pins are not connected. INTA is associated with Port A and INTB is associated with Port B | 1: The INT pins are internally connected in a wired OR configuration
// BANK:   Controls how the registers are addressed        | 0: The registers are in the same bank (addresses are sequential) | 1: The registers associated with each port are separated into different banks
// ---------------
typedef uint8_t                // GPIO Direction
MCP23S18_Direction_t[2];       // Bit[0:7] To Pin[0:7] > 1: Input         | 0: Output
typedef uint8_t                // GPIO Type
MCP23S18_Type_t[2];            // Bit[0:7] To Pin[0:7] > 1: ActiveHigh    | 0: ActiveLow
typedef uint8_t                // GPIO Pull Up
MCP23S18_PullUp_t[2];          // Bit[0:7] To Pin[0:7] > 1: PullUpEnable  | 0: PullUpDisable
// ---------------
typedef uint8_t                // Interrupt Enable
MCP23S18_IntEnable_t[2];       // Bit[0:7] To Pin[0:7] > 1: IntEnable     | 0: IntDisable
typedef uint8_t                // Interrupt Default Value
MCP23S18_IntDefaultVal_t[2];   // Bit[0:7] To Pin[0:7] > 1: 1             | 0: 0
typedef uint8_t                // Interrupt Mode To Compare
MCP23S18_IntCompare_t[2];      // Bit[0:7] To Pin[0:7] > 1: DefaultVal    | 0: PreviousVal
typedef uint8_t                // Interrupt Flag
MCP23S18_IntOccurred_t[2];     // Bit[0:7] To Pin[0:7] > 1: IntOccurred   | 0: IntNOTOccurred
typedef uint8_t                // Interrupt Capture State
MCP23S18_IntCaptureState_t[2]; // Bit[0:7] To Pin[0:7] > 1: IntLogicHigh  | 0: IntLogicLow
// ---------------
typedef uint8_t                // GPIO Value
MCP23S18_GPIOState_t[2];       // Bit[0:7] To Pin[0:7] > 1: GPIOLogicHigh | 0: GPIOLogicLow

/**
 ** ==================================================================================
 **                               ##### Structs #####                               
 ** ==================================================================================
 **/
/**
 * @brief  Handling of library
 * @note   Must be initialize at first before calling MCP23S18_Init function
 */
typedef struct
MCP23S18_Handler_s {
  // Functions:
  void (*MCP23S18_CS_Low)(void);                   // Must be Initialize
  void (*MCP23S18_CS_High)(void);                  // Must be Initialize
  void (*MCP23S18_Spi_WriteByte)(uint8_t);         // Must be Initialize
  uint8_t (*MCP23S18_Spi_ReadByte)(void);          // Must be Initialize
#if MCP23S18_USE_MACRO_DELAY == 0
  void (*MCP23S18_Delay_US)(void);                 //! Must be initialized If You do not use Macro Delay Otherwise Pass it as NULL (Place here your delay in Microsecond)
#endif
  // bool (*MCP23S18_INT0_Read)(void);                // UNIMPLEMENTED
  // bool (*MCP23S18_INT1_Read)(void);                // UNIMPLEMENTED
  // Variables:
  // bool SelectBank;                                 // UNIMPLEMENTED // !!! DO NOT USE OR EDIT THIS !!! // FOR_SELECT_BANK_MACRO
} MCP23S18_Handler_t;

/**
 ** ==================================================================================
 **                          ##### Public Functions #####                               
 ** ==================================================================================
 **/
// Initialization Function: ------------------------------------------------------- //
/**
 * @brief  Initializes the library
 * @note   Must be called at FIRST and ONCE!
 * @param  Handler: Pointer of library handler
 * @retval None
 */
void MCP23S18_Init(MCP23S18_Handler_t *Handler);
// Setting Functions: ------------------------------------------------------------- //
/**
 * @brief  Sets Configurations on IOCON registers
 * @param  Handler: Pointer of library handler
 * @param  MaskA:   Mask for port A to choose which pins have to be set
 * @param  MaskB:   Mask for port B to choose which pins have to be set
 * @param  Config:  See MCP23S18_Config_t
 * @retval None
 */
void MCP23S18_SetConfig(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_Config_t Config);
/**
 * @brief  Gets Configurations from IOCON registers
 * @param  Handler: Pointer of library handler
 * @param  MaskA:   Mask for port A to choose which pins have to be get
 * @param  MaskB:   Mask for port B to choose which pins have to be get
 * @param  Config:  See MCP23S18_Config_t
 * @retval None
 */
void MCP23S18_GetConfig(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_Config_t Config);
/**
 * @brief  Sets Pin Directions on IODIR registers
 * @param  Handler:    Pointer of library handler
 * @param  MaskA:      Mask for port A to choose which pins have to be set
 * @param  MaskB:      Mask for port B to choose which pins have to be set
 * @param  Direction:  See MCP23S18_Direction_t
 * @retval None
 */
void MCP23S18_SetDirection(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_Direction_t Direction);
/**
 * @brief  Gets Pin Directions from IODIR registers
 * @param  Handler:    Pointer of library handler
 * @param  MaskA:      Mask for port A to choose which pins have to be get
 * @param  MaskB:      Mask for port B to choose which pins have to be get
 * @param  Direction:  See MCP23S18_Direction_t
 * @retval None
 */
void MCP23S18_GetDirection(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_Direction_t Direction);
/**
 * @brief  Sets Pin Directions on IPOL registers
 * @param  Handler: Pointer of library handler
 * @param  MaskA:   Mask for port A to choose which pins have to be set
 * @param  MaskB:   Mask for port B to choose which pins have to be set
 * @param  Type:    See MCP23S18_Type_t
 * @retval None
 */
void MCP23S18_SetTypes(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_Type_t Type);
/**
 * @brief  Gets Pin Directions from IPOL registers
 * @param  Handler: Pointer of library handler
 * @param  MaskA:   Mask for port A to choose which pins have to be get
 * @param  MaskB:   Mask for port B to choose which pins have to be get
 * @param  Type:    See MCP23S18_Type_t
 * @retval None
 */
void MCP23S18_GetTypes(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_Type_t Type);
/**
 * @brief  Sets Pin Pull-up on GPPU registers
 * @param  Handler: Pointer of library handler
 * @param  MaskA:   Mask for port A to choose which pins have to be set
 * @param  MaskB:   Mask for port B to choose which pins have to be set
 * @param  PullUp:  See MCP23S18_PullUp_t
 * @retval None
 */
void MCP23S18_SetPullUp(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_PullUp_t PullUp);
/**
 * @brief  Gets Pin Pull-up from GPPU registers
 * @param  Handler: Pointer of library handler
 * @param  MaskA:   Mask for port A to choose which pins have to be get
 * @param  MaskB:   Mask for port B to choose which pins have to be get
 * @param  PullUp:  See MCP23S18_PullUp_t
 * @retval None
 */
void MCP23S18_GetPullUp(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_PullUp_t PullUp);
/**
 * @brief  Sets Pin Interrupt enabling on GPINTEN registers
 * @param  Handler:    Pointer of library handler
 * @param  MaskA:      Mask for port A to choose which pins have to be set
 * @param  MaskB:      Mask for port B to choose which pins have to be set
 * @param  IntEnable:  See MCP23S18_IntEnable_t
 * @retval None
 */
void MCP23S18_SetIntEnable(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntEnable_t IntEnable);
/**
 * @brief  Gets Pin Interrupt enabling from GPINTEN registers
 * @param  Handler:    Pointer of library handler
 * @param  MaskA:      Mask for port A to choose which pins have to be get
 * @param  MaskB:      Mask for port B to choose which pins have to be get
 * @param  IntEnable:  See MCP23S18_IntEnable_t
 * @retval None
 */
void MCP23S18_GetIntEnable(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntEnable_t IntEnable);
/**
 * @brief  Sets Pin Interrupt default value on DEFVAL registers
 * @param  Handler:        Pointer of library handler
 * @param  MaskA:          Mask for port A to choose which pins have to be set
 * @param  MaskB:          Mask for port B to choose which pins have to be set
 * @param  IntDefaultVal:  See MCP23S18_IntDefaultVal_t
 * @retval None
 */
void MCP23S18_SetIntDefaultVal(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntDefaultVal_t IntDefaultVal);
/**
 * @brief  Gets Pin Interrupt default value from DEFVAL registers
 * @param  Handler:        Pointer of library handler
 * @param  MaskA:          Mask for port A to choose which pins have to be get
 * @param  MaskB:          Mask for port B to choose which pins have to be get
 * @param  IntDefaultVal:  See MCP23S18_IntDefaultVal_t
 * @retval None
 */
void MCP23S18_GetIntDefaultVal(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntDefaultVal_t IntDefaultVal);
/**
 * @brief  Sets Pin Interrupt comparison mode on INTCON registers
 * @param  Handler:     Pointer of library handler
 * @param  MaskA:       Mask for port A to choose which pins have to be set
 * @param  MaskB:       Mask for port B to choose which pins have to be set
 * @param  IntCompare:  See MCP23S18_IntCompare_t
 * @retval None
 */
void MCP23S18_SetIntCompare(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntCompare_t IntCompare);
/**
 * @brief  Gets Pin Interrupt comparison mode from INTCON registers
 * @param  Handler:     Pointer of library handler
 * @param  MaskA:       Mask for port A to choose which pins have to be get
 * @param  MaskB:       Mask for port B to choose which pins have to be get
 * @param  IntCompare:  See MCP23S18_IntCompare_t
 * @retval None
 */
void MCP23S18_GetIntCompare(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntCompare_t IntCompare);
/**
 * @brief  Sets Pin Interrupt flags on INTF registers
 * @param  Handler:      Pointer of library handler
 * @param  MaskA:        Mask for port A to choose which pins have to be set
 * @param  MaskB:        Mask for port B to choose which pins have to be set
 * @param  IntOccurred:  See MCP23S18_IntOccurred_t
 * @retval None
 */
void MCP23S18_SetIntOccurred(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntOccurred_t IntOccurred);
/**
 * @brief  Gets Pin Interrupt flags from INTF registers
 * @param  Handler:      Pointer of library handler
 * @param  MaskA:        Mask for port A to choose which pins have to be get
 * @param  MaskB:        Mask for port B to choose which pins have to be get
 * @param  IntOccurred:  See MCP23S18_IntOccurred_t
 * @retval None
 */
void MCP23S18_GetIntOccurred(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntOccurred_t IntOccurred);
/**
 * @brief  Sets Pin Interrupt capturing state on INTCAP registers
 * @param  Handler:          Pointer of library handler
 * @param  MaskA:            Mask for port A to choose which pins have to be set
 * @param  MaskB:            Mask for port B to choose which pins have to be set
 * @param  IntCaptureState:  See MCP23S18_IntCaptureState_t
 * @retval None
 */
void MCP23S18_SetIntCaptureState(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntCaptureState_t IntCaptureState);
/**
 * @brief  Gets Pin Interrupt capturing state from INTCAP registers
 * @param  Handler:          Pointer of library handler
 * @param  MaskA:            Mask for port A to choose which pins have to be get
 * @param  MaskB:            Mask for port B to choose which pins have to be get
 * @param  IntCaptureState:  See MCP23S18_IntCaptureState_t
 * @retval None
 */
void MCP23S18_GetIntCaptureState(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_IntCaptureState_t IntCaptureState);
// Read/Write Functions: ---------------------------------------------------------- //
/**
 * @brief  Reads Input pins from GPIO registers
 * @note   To read Output pins use MCP23S18_ReadOutput function
 * @param  Handler:    Pointer of library handler
 * @param  MaskA:      Mask for port A to choose which pins have to be read
 * @param  MaskB:      Mask for port B to choose which pins have to be read
 * @param  GPIOState:  See MCP23S18_GPIOState_t
 * @retval None
 */
void MCP23S18_ReadInput(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_GPIOState_t GPIOState);
/**
 * @brief  Reads Output pins state from OLAT registers
 * @note   To read Input pins use MCP23S18_ReadInput function
 * @param  Handler:    Pointer of library handler
 * @param  MaskA:      Mask for port A to choose which pins have to be read
 * @param  MaskB:      Mask for port B to choose which pins have to be read
 * @param  GPIOState:  See MCP23S18_GPIOState_t
 * @retval None
 */
void MCP23S18_ReadOutput(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_GPIOState_t GPIOState);
/**
 * @brief  Writes Output pins state on OLAT registers
 * @param  Handler:    Pointer of library handler
 * @param  MaskA:      Mask for port A to choose which pins have to be written
 * @param  MaskB:      Mask for port B to choose which pins have to be written
 * @param  GPIOState:  See MCP23S18_GPIOState_t
 * @retval None
 */
void MCP23S18_WriteOutput(MCP23S18_Handler_t *Handler, uint8_t MaskA, uint8_t MaskB, MCP23S18_GPIOState_t GPIOState);
/**
 * @brief  Writes Output pins state on OLAT registers fast!
 * @note   This Sets All New Data In GPIOState Variables
 * @param  Handler:    Pointer of library handler
 * @param  MaskA:      Mask for port A to choose which pins have to be written
 * @param  MaskB:      Mask for port B to choose which pins have to be written
 * @param  GPIOState:  See MCP23S18_GPIOState_t
 * @retval None
 */
void MCP23S18_WriteOutputFAST(MCP23S18_Handler_t *Handler, MCP23S18_GPIOState_t GPIOState);
/**
 * @brief  Writes Output All port A pins HIGH on OLAT registers fast!
 * @param  Handler: Pointer of library handler
 * @retval None
 */
void MCP23S18_WriteOutputPortAHighFAST(MCP23S18_Handler_t *Handler);
/**
 * @brief  Writes Output All port A pins LOW on OLAT registers fast!
 * @param  Handler: Pointer of library handler
 * @retval None
 */
void MCP23S18_WriteOutputPortALowFAST(MCP23S18_Handler_t *Handler);
/**
 * @brief  Writes Output All port B pins HIGH on OLAT registers fast!
 * @param  Handler: Pointer of library handler
 * @retval None
 */
void MCP23S18_WriteOutputPortBHighFAST(MCP23S18_Handler_t *Handler);
/**
 * @brief  Writes Output All port B pins LOW on OLAT registers fast!
 * @param  Handler: Pointer of library handler
 * @retval None
 */
void MCP23S18_WriteOutputPortBLowFAST(MCP23S18_Handler_t *Handler);
#ifdef _cplusplus
}
#endif
#endif
