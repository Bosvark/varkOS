#ifndef _KLOG_H_
#define _KLOG_H_

void klogInt(const char *description, unsigned int intval);
void klogBin(const char *description, unsigned char *bindata, unsigned int binlen);

#endif // _KLOG_H_
