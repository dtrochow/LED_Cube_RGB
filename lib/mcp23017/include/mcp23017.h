#include "hardware/i2c.h"

#define GET_ORDER_NUBER(ADDR) ((int)ADDR - 32)

#define ADDRESS0    0x20
#define ADDRESS1    0x21
#define ADDRESS2    0x22
#define ADDRESS3    0x23
#define ADDRESS4    0x24
#define ADDRESS5    0x25
#define ADDRESS6    0x26
#define ADDRESS7    0x27

#define IODIRA      0x00
#define IODIRB      0x01
#define GPIOA       0x12
#define GPIOB       0x13

#define IPOLA       0x02
#define IPOLB       0x03
#define GPINTENA    0x04
#define GPINTENB    0x05
#define DEFVALA     0x06
#define DEFVALB     0x07
#define INTCONA     0x08
#define INTCONB     0x09
#define IOCON1      0x0A
#define IOCON2      0x0B
#define GPPUA       0x0C
#define GPPUB       0x0D
#define INTFA       0x0E
#define INTFB       0x0F
#define INTCAPA     0x10
#define INTCAPB     0x11
#define OLATA       0x14
#define OLATB       0x15

#define MAX_NUMBER_OF_MCP_MODULES       (8)

typedef enum
{
    OUTPUT_MODE = 0,
    INPUT_MODE = 1
} mcpIOMode_e;

typedef enum
{
    LOW = 0,
    HIGH = 1
} mcpIOState_e;

typedef struct
{
    uint8_t porta;
    uint8_t portb;
} ports_t;

union IOStates_u
{
    ports_t portState;
    uint16_t all;
};

union IOMode_u
{
    ports_t portMode;
    uint16_t all;
};

typedef struct
{   
    union IOStates_u state;
    union IOMode_u mode;
} mcpData_t;

/**
 * @brief 
 * 
 * @param i2c 
 * @param port 
 * @param config 
 */
void mcp_set_mode(i2c_inst_t *i2c, uint8_t address, uint8_t port, uint8_t config);

/**
 * @brief 
 * 
 * @param i2c 
 * @param port 
 * @param value 
 */
void mcp_write(i2c_inst_t *i2c, uint8_t address, uint8_t port, uint8_t value);

/**
 * @brief 
 * 
 * @param address 
 * @param port 
 * @return uint8_t 
 */
uint8_t mcp_get_port_value(uint8_t address, uint8_t port);