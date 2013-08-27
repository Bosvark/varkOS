#include <stdint.h>
#include <mmio.h>
#include <mailbox.h>

enum {
    MAILBOX_BASE            = 0x2000B880,
    MAILBOX_POLL            = 0x2000B890,    // Receive without retrieving.	 R
    MAILBOX_SENDER          = 0x2000B894,    // Sender information.	 R
    MAILBOX_STATUS          = 0x2000B898,    // Information.	 R
    MAILBOX_CONFIGURATION   = 0x2000B89C,    // Settings.	 RW
    MAILBOX_WRITE           = 0x2000B8A0     // Sending mail.	 W
};

void MailboxWrite(uint32_t message, uint32_t mailbox)
{
    uint32_t status;
    
    if(mailbox > 15)            // Not sure why thisis neccesary - find out!
        return;
    
    // Wait for the mailbox to become available
    do{
        status = mmio_read(MAILBOX_STATUS);
    }while(status & 0x80000000);             // Check that the top bit is set
    
    mmio_write(MAILBOX_WRITE, (message << 4) | mailbox);   // Combine message and mailbox channel and write to the mailbox
}

uint32_t MailboxRead(uint32_t mailbox)
{
    uint32_t status;
    
    if(mailbox > 15)            // Not sure why thisis neccesary - find out!
        return 0;

    while(1){
		// Wait for the mailbox to become available
		do{
			status = mmio_read(MAILBOX_STATUS);
		}while(status & 0x40000000);             // Check that the 30th bit is set

		status = mmio_read(MAILBOX_BASE);

		if(mailbox == (status & 0x0000000f))
			return status & 0x0000000f;
    }
}
