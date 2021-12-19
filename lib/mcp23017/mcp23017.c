#include "mcp23017.h"

static mcpData_t mcpData[MAX_NUMBER_OF_MCP_MODULES];

void mcp_set_mode(i2c_inst_t *i2c, uint8_t address, uint8_t port, uint8_t config)
{
    uint8_t buf[2];
    buf[0] = port;
    buf[1] = config;
    switch(port)
    {
        case GPIOA:
            mcpData[GET_ORDER_NUBER(address)].mode.portMode.porta = config;
            break;
        case GPIOB:
            mcpData[GET_ORDER_NUBER(address)].mode.portMode.portb = config;
            break;
        default:
            break;
    }
    i2c_write_blocking(i2c, address, buf, 2, false);
}

void mcp_write(i2c_inst_t *i2c, uint8_t address, uint8_t port, uint8_t value)
{
    uint8_t buf[2];
    buf[0] = port;
    buf[1] = value;
    switch(port)
    {
        case GPIOA:
            mcpData[GET_ORDER_NUBER(address)].state.portState.porta = value;
            break;
        case GPIOB:
            mcpData[GET_ORDER_NUBER(address)].state.portState.portb = value;
            break;
        default:
            break;
    }
    i2c_write_blocking(i2c, address, buf, 2, false);
}

uint8_t mcp_get_port_value(uint8_t address, uint8_t port)
{
        switch(port)
    {
        case GPIOA:
            return mcpData[GET_ORDER_NUBER(address)].state.portState.porta;
            break;
        case GPIOB:
            return mcpData[GET_ORDER_NUBER(address)].state.portState.portb;
            break;
        default:
            return -1;
    }
}