// Mouse interface constants (in case you need them)

#define MSSTATP         0x64    // mouse controller status port(I)
#define MSDATAP         0x60    // mouse data port(I)

// MOUSE COMMANDS
#define MSEN            0xa8    // Code to enable the secondary PS/2 device (mouse)
#define PS2MS           0xd4    // Code to tell the PS/2 Controller to address the mouse
#define MSACK           0xfa    // Mouse ACK

// Mouse Packet Position
#define MSLC    0x01    // Mouse left click
#define MSRC    0x02    // Mouse right click
#define MSMC    0x04    // Mourse middle click