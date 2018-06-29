
#ifndef __LOG_H__
#define __LOG_H__

void logOpen(void);
void logPrint(char *p);
void logStream(char *p, unsigned short len);
void logClose(void);

#endif
