#include "MCP23S18.h"

uint32_t faultCode = 0;
uint32_t MCP23S18_ReadRegister(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver, uint8_t reg) {
  mcp23S18_SpiDriver->Mcp23s18_CS_Low();
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(MCP23S18_READ_ADDR);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(reg);
  uint32_t ret = mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(reg);    
  mcp23S18_SpiDriver->Mcp23s18_CS_High();
  return ret;
}
uint32_t MCP23S18_ReadPort(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver) {    
  mcp23S18_SpiDriver->Mcp23s18_CS_Low();
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(MCP23S18_READ_ADDR);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(GPIOA_BANK0);   
  uint32_t lower_byte = mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(GPIOB_BANK0);
  uint32_t upper_byte = mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(GPIOA_BANK0);       
  mcp23S18_SpiDriver->Mcp23s18_CS_High();    
  uint32_t val = lower_byte | (upper_byte << 8);
  return val;
}
uint32_t MCP23S18_ReadLatch(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver) {
  mcp23S18_SpiDriver->Mcp23s18_CS_Low();
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(MCP23S18_READ_ADDR);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(OLATA_BANK0);   
  int lower_byte = mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(OLATB_BANK0);
  int upper_byte = mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(OLATA_BANK0);       
  mcp23S18_SpiDriver->Mcp23s18_CS_High();    
  int val = lower_byte | (upper_byte << 8);
  return val;
}
uint32_t MCP23S18_Initialize(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver, uint32_t setType) {
  uint32_t ret;  
  mcp23S18_SpiDriver->Mcp23s18_CS_Low();
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(MCP23S18_WRITE_ADDR);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(IOCONA_BANK0);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(IOCON_MIRROR | IOCON_SEQOP);
  mcp23S18_SpiDriver->Mcp23s18_CS_High();
  mcp23S18_SpiDriver->Mcp23s18_CS_Low();
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(MCP23S18_READ_ADDR);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(IOCONA_BANK0);
  ret = mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(IOCONA_BANK0);
  mcp23S18_SpiDriver->Mcp23s18_CS_High();
  ret = MCP23S18_ReadRegister(mcp23S18_SpiDriver, IOCONA_BANK0);
  //printf("GO away Init MCP23S18 0x%x\r\n", ret);
  faultCode = 0;
  return ret;
}
uint32_t MCP23S18_ReadConfig(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver) {
  uint32_t ret = MCP23S18_ReadRegister(mcp23S18_SpiDriver, IOCONA_BANK0);      
  return ret;
}
void MCP23S18_SetDirection(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver, uint32_t dirpins) {
  mcp23S18_SpiDriver->Mcp23s18_CS_Low();
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(MCP23S18_WRITE_ADDR);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(IODIRA_BANK0);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(0x000000FF & dirpins);
  mcp23S18_SpiDriver->Mcp23s18_CS_High();
  mcp23S18_SpiDriver->Mcp23s18_CS_Low();    
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(MCP23S18_WRITE_ADDR);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(IODIRB_BANK0);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(0x000000FF & (dirpins>>8));    
  mcp23S18_SpiDriver->Mcp23s18_CS_High();
}
uint32_t MCP23S18_ReadDirection(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver) {
  mcp23S18_SpiDriver->Mcp23s18_CS_Low();
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(MCP23S18_READ_ADDR);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(IODIRA_BANK0);   
  uint32_t lower_byte = mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(IODIRB_BANK0);
  uint32_t upper_byte = mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(IODIRA_BANK0);       
  mcp23S18_SpiDriver->Mcp23s18_CS_High();    
  uint32_t val = lower_byte|(upper_byte << 8);
  return val;
}
void MCP23S18_SetAllOutput(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver) {
  MCP23S18_SetDirection(mcp23S18_SpiDriver, IOX_ALLOUTPUT);
}
void MCP23S18_SetAllInput(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver) {
  MCP23S18_SetDirection(mcp23S18_SpiDriver, IOX_ALLINPUT);
}
void MCP23S18_SetInverted(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver, uint32_t invpins) {
  mcp23S18_SpiDriver->Mcp23s18_CS_Low();
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(MCP23S18_WRITE_ADDR);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(IPOLA_BANK0);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(0x000000FF & invpins);
  mcp23S18_SpiDriver->Mcp23s18_CS_High();
  mcp23S18_SpiDriver->Mcp23s18_CS_Low();    
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(MCP23S18_WRITE_ADDR);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(IPOLB_BANK0);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(0x000000FF & (invpins>>8));    
  mcp23S18_SpiDriver->Mcp23s18_CS_High();
}
void MCP23S18_SetAllInverted(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver) {
  MCP23S18_SetInverted(mcp23S18_SpiDriver, IOX_ALLINVERTED);
}
void MCP23S18_SetNoneInverted(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver) {
  MCP23S18_SetInverted(mcp23S18_SpiDriver, IOX_NONEINVERTED);
}
void MCP23S18_SetPullups(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver, uint32_t pupins) {
  mcp23S18_SpiDriver->Mcp23s18_CS_Low();
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(MCP23S18_WRITE_ADDR);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(GPPUA_BANK0);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(0x000000FF & pupins);    
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(0x000000FF & (pupins>>8));
  mcp23S18_SpiDriver->Mcp23s18_CS_High();    
}
uint32_t MCP23S18_ReadPullups(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver) {
  mcp23S18_SpiDriver->Mcp23s18_CS_Low();
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(MCP23S18_READ_ADDR);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(GPPUA_BANK0);   
  uint32_t lower_byte = mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(GPPUB_BANK0);
  uint32_t upper_byte = mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(GPPUA_BANK0);       
  mcp23S18_SpiDriver->Mcp23s18_CS_High();    
  uint32_t val = lower_byte|(upper_byte << 8);
  return val;
}
void MCP23S18_SetAllPullups(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver) {
  MCP23S18_SetPullups(mcp23S18_SpiDriver, 0xFFFF);
}
void MCP23S18_SetNonePullups(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver) {
  MCP23S18_SetPullups(mcp23S18_SpiDriver, 0x0000);
}
void MCP23S18_WritePort(MCP23S18_SpiDriver_Typedef * mcp23S18_SpiDriver, uint32_t value) {
  //printf("Select MUX A\r\n");
  mcp23S18_SpiDriver->Mcp23s18_CS_Low();
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(MCP23S18_WRITE_ADDR);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(OLATA_BANK0);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(0x000000FF & value);
  mcp23S18_SpiDriver->Mcp23s18_CS_High();
  //printf("Deselect MUX A\r\n");
  //printf("Select MUX B\r\n");
  mcp23S18_SpiDriver->Mcp23s18_CS_Low();
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(MCP23S18_WRITE_ADDR);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(OLATB_BANK0);
  mcp23S18_SpiDriver->Mcp23s18_Spi_WriteReadByte(0x000000FF & (value>>8));
  mcp23S18_SpiDriver->Mcp23s18_CS_High();
  //printf("Deselect MUX B\r\n");
}
//----------------------------------------------------------------------------------------------------------------------------------------------
