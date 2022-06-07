# MCP4725 Library for STM32

This is a library for the [MCP4725 DAC (datasheet)](http://ww1.microchip.com/downloads/en/devicedoc/22039d.pdf) designed for STM32 MCUs. It uses the fast transmission mode whenever possible.	

To use a DAC, first a new variable of the type `MCP4725_t` has to be created. Then the initialize function can be called. The setup looks like this:

```C
// define struct for external DAC
MCP4725_t mcp4725;

MCP4725_Initialize(&mcp4725, &hi2c1, 0x62);
```

The reference of said variable is then passed to the initialize function, together with a reference to the I2C object. It is followed by the address (`0x62` is the default address of most breakout boards).

A full example would look like this:

```C
// define struct for external DAC
MCP4725_t mcp4725;

MCP4725_Initialize(&mcp4725, &hi2c1, 0x62);
MCP4725_SetVoltage(&mcp4725, 1458, MCP4725_DAC_ONLY);

// read the value back from the DAC
MCP4725_GetDataFromChip(&mcp4725);
```

Note that the I2C transmissions are blocking for now. Also the read function updates the voltage value in the struct after the data is received. The setter functions update the struct as well and then transmit the data.

## Documentation

The documentation can be found in the [header file `MCP4725.h`](MCP4725.h).

## License

[MIT](LICENSE)