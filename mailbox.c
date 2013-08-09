#include <stdint.h>
#include <mmio.h>
#include <mailbox.h>

enum {
    MAILBOX_BASE            = 0x2000B880,
    MAILBOX_POLL            = (MAILBOX_BASE+4), // Receive without retrieving.	 R
    MAILBOX_SENDER          = (MAILBOX_BASE+4), // Sender information.	 R
    MAILBOX_STATUS          = (MAILBOX_BASE+4), // Information.	 R
    MAILBOX_CONFIGURATION   = (MAILBOX_BASE+4), // Settings.	 RW
    MAILBOX_WRITE           = (MAILBOX_BASE+4)  // Sending mail.	 W
};

void MailboxWrite(uint32_t message, uint32_t mailbox)
{
    uint32_t status;
    
    if((message & 0x0f) != 0)   // Check that the lower 4 bitys are all 0
        return;
    
    if(mailbox > 15)            // Not sure why thisis neccesary - find out!
        return;
    
    // Wait for the mailbox to become available
    do{
        status = mmio_read(MAILBOX_STATUS);
    }while((status & 0x80000000) == 0);             // Check that the top bit is set
    
    
    mmio_write(MAILBOX_WRITE, message | mailbox);   // Combine messahe and mailbox channel and write to the mailbox
}

uint32_t MailboxRead(uint32_t mailbox)
{
    uint32_t status;
    
    if(mailbox > 15)            // Not sure why thisis neccesary - find out!
        return 0;

    // Wait for the mailbox to become available
    do{
        status = mmio_read(MAILBOX_STATUS);
    }while((status & 0x40000000) == 0);             // Check that the 30th bit is set
    
    return mmio_read(MAILBOX_BASE);
}