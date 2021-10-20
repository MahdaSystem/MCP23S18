#ifndef _MCP23S18_H_
#define _MCP23S18_H_

#include <stdint.h>

// Default opcode address of the MCP23S18
#define MCP23S18_WRITE_ADDR 0x40
#define MCP23S18_READ_ADDR  0x41

// MCP23S18 
// BANK 0
//      RegisterName            Address(hex)   bit7   | bit6   | bit5   | bit4   | bit3   | bit2   | bit1   | bit0   | POR/RST
#define MCP23S18_BANK0_IODIRA   0x00        // IO7    | IO6    | IO5    | IO4    | IO3    | IO2    | IO1    | IO0    | 1111 1111
#define MCP23S18_BANK0_IODIRB   0x01        // IO7    | IO6    | IO5    | IO4    | IO3    | IO2    | IO1    | IO0    | 1111 1111
#define MCP23S18_BANK0_IPOLA    0x02        // IP7    | IP6    | IP5    | IP4    | IP3    | IP2    | IP1    | IP0    | 0000 0000
#define MCP23S18_BANK0_IPOLB    0x03        // IP7    | IP6    | IP5    | IP4    | IP3    | IP2    | IP1    | IP0    | 0000 0000
#define MCP23S18_BANK0_GPINTENA 0x04        // GPINT7 | GPINT6 | GPINT5 | GPINT4 | GPINT3 | GPINT2 | GPINT1 | GPINT0 | 0000 0000
#define MCP23S18_BANK0_GPINTENB 0x05        // GPINT7 | GPINT6 | GPINT5 | GPINT4 | GPINT3 | GPINT2 | GPINT1 | GPINT0 | 0000 0000
#define MCP23S18_BANK0_DEFVALA  0x06        // DEF7   | DEF6   | DEF5   | DEF4   | DEF3   | DEF2   | DEF1   | DEF0   | 0000 0000
#define MCP23S18_BANK0_DEFVALB  0x07        // DEF7   | DEF6   | DEF5   | DEF4   | DEF3   | DEF2   | DEF1   | DEF0   | 0000 0000
#define MCP23S18_BANK0_INTCONA  0x08        // IOC7   | IOC6   | IOC5   | IOC4   | IOC3   | IOC2   | IOC1   | IOC0   | 0000 0000
#define MCP23S18_BANK0_INTCONB  0x09        // IOC7   | IOC6   | IOC5   | IOC4   | IOC3   | IOC2   | IOC1   | IOC0   | 0000 0000
#define MCP23S18_BANK0_IOCONA   0x0A        // BANK   | MIRROR | SEQOP  | —      | —      | ODR    | INTPOL | INTCC  | 0000 0000
#define MCP23S18_BANK0_IOCONB   0x0B        // BANK   | MIRROR | SEQOP  | —      | —      | ODR    | INTPOL | INTCC  | 0000 0000
#define MCP23S18_BANK0_GPPUA    0x0C        // PU7    | PU6    | PU5    | PU4    | PU3    | PU2    | PU1    | PU0    | 0000 0000
#define MCP23S18_BANK0_GPPUB    0x0D        // PU7    | PU6    | PU5    | PU4    | PU3    | PU2    | PU1    | PU0    | 0000 0000
#define MCP23S18_BANK0_INTFA    0x0E        // INT7   | INT6   | INT5   | INT4   | INT3   | INT2   | INT1   | INTO   | 0000 0000
#define MCP23S18_BANK0_INTFB    0x0F        // INT7   | INT6   | INT5   | INT4   | INT3   | INT2   | INT1   | INTO   | 0000 0000
#define MCP23S18_BANK0_INTCAPA  0x10        // ICP7   | ICP6   | ICP5   | ICP4   | ICP3   | ICP2   | ICP1   | ICP0   | 0000 0000
#define MCP23S18_BANK0_INTCAPB  0x11        // ICP7   | ICP6   | ICP5   | ICP4   | ICP3   | ICP2   | ICP1   | ICP0   | 0000 0000
#define MCP23S18_BANK0_GPIOA    0x12        // GP7    | GP6    | GP5    | GP4    | GP3    | GP2    | GP1    | GP0    | 0000 0000
#define MCP23S18_BANK0_GPIOB    0x13        // GP7    | GP6    | GP5    | GP4    | GP3    | GP2    | GP1    | GP0    | 0000 0000
#define MCP23S18_BANK0_OLATA    0x14        // OL7    | OL6    | OL5    | OL4    | OL3    | OL2    | OL1    | OL0    | 0000 0000
#define MCP23S18_BANK0_OLATB    0x15        // OL7    | OL6    | OL5    | OL4    | OL3    | OL2    | OL1    | OL0    | 0000 0000

// BANK 1
//      RegisterName            Address(hex)   bit7   | bit6   | bit5   | bit4   | bit3   | bit2   | bit1   | bit0   | POR/RST
#define MCP23S18_BANK1_IODIRA   0x00        // IO7    | IO6    | IO5    | IO4    | IO3    | IO2    | IO1    | IO0    | 1111 1111
#define MCP23S18_BANK1_IPOLA    0x01        // IP7    | IP6    | IP5    | IP4    | IP3    | IP2    | IP1    | IP0    | 0000 0000
#define MCP23S18_BANK1_GPINTENA 0x02        // GPINT7 | GPINT6 | GPINT5 | GPINT4 | GPINT3 | GPINT2 | GPINT1 | GPINT0 | 0000 0000
#define MCP23S18_BANK1_DEFVALA  0x03        // DEF7   | DEF6   | DEF5   | DEF4   | DEF3   | DEF2   | DEF1   | DEF0   | 0000 0000
#define MCP23S18_BANK1_INTCONA  0x04        // IOC7   | IOC6   | IOC5   | IOC4   | IOC3   | IOC2   | IOC1   | IOC0   | 0000 0000
#define MCP23S18_BANK1_IOCONA   0x05        // BANK   | MIRROR | SEQOP  | —      | —      | ODR    | INTPOL | INTCC  | 0000 0000
#define MCP23S18_BANK1_GPPUA    0x06        // PU7    | PU6    | PU5    | PU4    | PU3    | PU2    | PU1    | PU0    | 0000 0000
#define MCP23S18_BANK1_INTFA    0x07        // INT7   | INT6   | INT5   | INT4   | INT3   | INT2   | INT1   | INTO   | 0000 0000
#define MCP23S18_BANK1_INTCAPA  0x08        // ICP7   | ICP6   | ICP5   | ICP4   | ICP3   | ICP2   | ICP1   | ICP0   | 0000 0000
#define MCP23S18_BANK1_GPIOA    0x09        // GP7    | GP6    | GP5    | GP4    | GP3    | GP2    | GP1    | GP0    | 0000 0000
#define MCP23S18_BANK1_OLATA    0x0A        // OL7    | OL6    | OL5    | OL4    | OL3    | OL2    | OL1    | OL0    | 0000 0000
#define MCP23S18_BANK1_IODIRB   0x10        // IO7    | IO6    | IO5    | IO4    | IO3    | IO2    | IO1    | IO0    | 1111 1111
#define MCP23S18_BANK1_IPOLB    0x11        // IP7    | IP6    | IP5    | IP4    | IP3    | IP2    | IP1    | IP0    | 0000 0000
#define MCP23S18_BANK1_GPINTENB 0x12        // GPINT7 | GPINT6 | GPINT5 | GPINT4 | GPINT3 | GPINT2 | GPINT1 | GPINT0 | 0000 0000
#define MCP23S18_BANK1_DEFVALB  0x13        // DEF7   | DEF6   | DEF5   | DEF4   | DEF3   | DEF2   | DEF1   | DEF0   | 0000 0000
#define MCP23S18_BANK1_INTCONB  0x14        // IOC7   | IOC6   | IOC5   | IOC4   | IOC3   | IOC2   | IOC1   | IOC0   | 0000 0000
#define MCP23S18_BANK1_IOCONB   0x15        // BANK   | MIRROR | SEQOP  | —      | —      | ODR    | INTPOL | INTCC  | 0000 0000
#define MCP23S18_BANK1_GPPUB    0x16        // PU7    | PU6    | PU5    | PU4    | PU3    | PU2    | PU1    | PU0    | 0000 0000
#define MCP23S18_BANK1_INTFB    0x17        // INT7   | INT6   | INT5   | INT4   | INT3   | INT2   | INT1   | INTO   | 0000 0000
#define MCP23S18_BANK1_INTCAPB  0x18        // ICP7   | ICP6   | ICP5   | ICP4   | ICP3   | ICP2   | ICP1   | ICP0   | 0000 0000
#define MCP23S18_BANK1_GPIOB    0x19        // GP7    | GP6    | GP5    | GP4    | GP3    | GP2    | GP1    | GP0    | 0000 0000
#define MCP23S18_BANK1_OLATB    0x1A        // OL7    | OL6    | OL5    | OL4    | OL3    | OL2    | OL1    | OL0    | 0000 0000

//---------------------------------------------------------------------------------------------------------------------------------

// If you set IOCON.BANK = 0 then all
// the registers are 
// BUT each bank uses the lowest significant bit
// in the upper word for identifying the bank
// We will NOT use this method to address
// the registers since it hurts my head :)
// I am documenting these registers here simply
// as an exercise
#define IODIRA_BANK1        0x00
#define IODIRB_BANK1        0x10
#define IPOLA_BANK1         0x01
#define IPOLB_BANK1         0x11
#define GPINTENA_BANK1      0x02
#define GPINTENB_BANK1      0x12
#define DEFVALA_BANK1       0x03
#define DEFVALB_BANK1       0x13
#define INTCONA_BANK1       0x04
#define INTCONB_BANK1       0x14
#define IOCONA_BANK1        0x05
#define IOCONB_BANK1        0x15
#define GPPUA_BANK1         0x06
#define GPPUB_BANK1         0x16
#define INTFA_BANK1         0x07
#define INTFB_BANK1         0x17
#define INTCAPA_BANK1       0x08
#define INTCAPB_BANK1       0x18
#define GPIOA_BANK1         0x09
#define GPIOB_BANK1         0x19
#define OLATA_BANK1         0x0A
#define OLATB_BANK1         0x1A

// If you set IOCON.BANK = 1 then all
// the registers are set sequentially
// We will use this method to address
// the registers since it hurts my head less :)

#define IODIRA_BANK0        0x00
#define IODIRB_BANK0        0x01
#define IPOLA_BANK0         0x02
#define IPOLB_BANK0         0x03
#define GPINTENA_BANK0      0x04
#define GPINTENB_BANK0      0x05
#define DEFVALA_BANK0       0x06
#define DEFVALB_BANK0       0x07
#define INTCONA_BANK0       0x08
#define INTCONB_BANK0       0x09
#define IOCONA_BANK0        0x0A
#define IOCONB_BANK0        0x0B
#define GPPUA_BANK0         0x0C
#define GPPUB_BANK0         0x0D
#define INTFA_BANK0         0x0E
#define INTFB_BANK0         0x0F
#define INTCAPA_BANK0       0x10
#define INTCAPB_BANK0       0x11
#define GPIOA_BANK0         0x12
#define GPIOB_BANK0         0x13
#define OLATA_BANK0         0x14
#define OLATB_BANK0         0x15

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

typedef struct MCP23S18_SpiDriver_Struct {
  void (*Mcp23s18_CS_Low)(void);
  void (*Mcp23s18_CS_High)(void);
  void (*Mcp23s18_Spi_WriteByte)(uint8_t);
  uint8_t (*Mcp23s18_Spi_ReadByte)(void);
  uint8_t (*Mcp23s18_Spi_WriteReadByte)(uint8_t);
  void (*Mcp23s18_Delay_1ms)(void);
} MCP23S18_SpiDriver_Typedef;

uint32_t MCP23S18_ReadRegister(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver, uint8_t reg);
uint32_t MCP23S18_ReadPort(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver);
uint32_t MCP23S18_ReadLatch(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver);
uint32_t MCP23S18_Initialize(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver, uint32_t setType);
uint32_t MCP23S18_ReadConfig(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver);
void MCP23S18_SetDirection(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver, uint32_t dirpins);
uint32_t MCP23S18_ReadDirection(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver);
void MCP23S18_SetAllOutput(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver);
void MCP23S18_SetAllInput(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver);
void MCP23S18_SetInverted(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver, uint32_t invpins);
void MCP23S18_SetAllInverted(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver);
void MCP23S18_SetNoneInverted(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver);
void MCP23S18_SetPullups(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver, uint32_t pupins);
uint32_t MCP23S18_ReadPullups(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver);
void MCP23S18_SetAllPullups(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver);
void MCP23S18_SetNonePullups(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver);
void MCP23S18_WritePort(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver, uint32_t value);
#endif
