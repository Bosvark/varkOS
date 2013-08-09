#ifndef _MAILBOX_H_
#define _MAILBOX_H_

void MailboxWrite(uint32_t message, uint32_t mailbox);
uint32_t MailboxRead(uint32_t mailbox);

#endif // _MAILBOX_H_