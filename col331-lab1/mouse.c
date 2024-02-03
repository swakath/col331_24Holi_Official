#include "types.h"
#include "defs.h"
#include "x86.h"
#include "mouse.h"
#include "traps.h"

// Wait until the mouse controller is ready for us to send a packet
void 
mousewait_send(void) 
{
    volatile uchar out_status = 0x02;
    while ((out_status & 0x02) != 0)
        out_status = inb(MSSTATP);
    return;
}

// Wait until the mouse controller has data for us to receive
void 
mousewait_recv(void) 
{
    volatile unsigned char in_status = 0x00;
    while ((in_status & 0x01) == 0)
        in_status = inb(MSSTATP);
    return;
}

// Send a one-byte command to the mouse controller, and wait for it
// to be properly acknowledged
void 
mousecmd(uchar cmd) 
{
    //cprintf("Inside mousecmd");
    // Wait until the controller is ready for the command
    mousewait_send();

    // Send the command
    outb(MSSTATP, 0xD4);  // 0xD4 indicates that the next byte is for the mouse
    mousewait_send();
    outb(MSDATAP, cmd);   // Send the actual command
    
    mousewait_recv();

    volatile uchar ack = inb(MSDATAP);
    if (ack != 0xFA){
        cprintf("[NOK] Ack Filed. Expected[0xFA] Received[%c] \n", ack);
    }
    
    // volatile uchar ack_data = 0x00;
    // mousewait_recv();
    // ack_data = inb(MSDATAP);

    // cprintf("Completed mousecmd%s ", ack_data);
    return;
}

void
mouseinit(void)
{
    // Enables Mouse
    mousewait_send();
    outb(MSSTATP, 0xA8);

    // Setting compaq status byte
    mousewait_send();
    outb(MSSTATP, 0x20);    // To retrieve Compaq status byte 
    
    mousewait_recv();
    uchar compaqStatus = inb(MSDATAP);  // Reading Compaq status byte
    compaqStatus |= 0x02;               // Setting 2nd bit to 1 to enable interrupts
    
    mousewait_send();                   
    outb(MSSTATP, 0x60);                // To send status byte
    mousewait_send();
    outb(MSDATAP, compaqStatus);        // Sending status byte

    mousecmd(0xF6);                     // Set default mouse settings
    mousecmd(0xF4);                     // Mouse to activate and send interrupts

    ioapicenable(IRQ_MOUSE, 0);         // Set mouse interrupt on CPU 0
    cprintf("Mouse has been initialized\n");
    
    return;
}

void mouseintr(void)
{
    //cprintf("Intr is called\n");
    // Implement your code here
    volatile uchar buffer_status = 0x01;
    //volatile uchar byteCount = 0;
    volatile uchar packets[3];

    while((buffer_status & 0x01) != 0){
        for(int i = 0; i<3; ++i){
            mousewait_recv();
            packets[i] = inb(MSDATAP);
        }
        
        if((packets[0] & MSLC) == MSLC){
            cprintf("LEFT\n");
        }
        
        if((packets[0] & MSRC) == MSRC){
            cprintf("RIGHT\n");
        }
        
        if((packets[0] & MSMC) == MSMC){
            cprintf("MID\n");
        }
        // else{
        //     cprintf("Unrecogonized click");
        // }

        buffer_status = inb(MSSTATP);
    }

    return;
}