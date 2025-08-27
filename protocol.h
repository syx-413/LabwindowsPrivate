#ifndef		__PROTOCOL_H__
#define		__PROTOCOL_H__
extern void makeReadPrDataPacket(u16 id, u8 *buf, u16 *slen);
extern void makeWritePrDataPacket(u16 id, u8 *buf, u16 *slen);
extern void makeReadCrDataPacket(u16 id, u8 *buf, u16 *slen);
extern void makeWriteCrDataPacket(u16 id, u8 *buf, u16 *slen);
extern void makeClrfDonePacket(u16 id, u8 *buf, u16 *slen);
extern void makeClrfCanselPacket(u16 id, u8 *buf, u16 *slen);
extern void makeSRONPacket(u16 id, u8 *buf, u16 *slen);
extern void makeSROFFPacket(u16 id, u8 *buf, u16 *slen);
extern void makeSMONPacket(u16 id, u8 *buf, u16 *slen);
extern void makeSMOFFPacket(u16 id, u8 *buf, u16 *slen);
extern void makeWorkDonePacket(u16 id, u8 *buf, u16 *slen);
extern void makeWorkCanselPacket(u16 id, u8 *buf, u16 *slen);
extern void makeEMRDonePacket(u16 id, u8 *buf, u16 *slen);
extern void makeEMRCanselPacket(u16 id, u8 *buf, u16 *slen);
extern void makeClearEventPacket(u16 id, u8 *buf, u16 *slen);
extern void makeReadEventPacket(u16 id, u8 *buf, u16 *slen);
extern void makeCheckTimePacket(u16 id, u8 *buf, u16 *slen);
extern void makeSELCTSYSPacket(u16 id, u8 *buf, u16 *slen);
extern void makeSELCTLOADPacket(u16 id, u8 *buf, u16 *slen);
extern void makeAutoStartPacket(u16 id, u8 *buf, u16 *slen);
extern void makeAutoStopPacket(u16 id, u8 *buf, u16 *slen);
extern void makeReadRefPacket(u16 id, u8 *buf, u16 *slen);
extern void makeWriteRefPacket(u16 id, u8 *buf, u16 *slen);
extern void makeHBPacket(u16 id, u8 *buf, u16 *slen);
extern void makeQF3ONPacket(u16 id, u8 *buf, u16 *slen);
extern void makeQF3OFFPacket(u16 id, u8 *buf, u16 *slen);
extern void makeSTSPacket(u16 id, u16 cw, u8 *buf, u16 *slen);
extern void rtDataMaker(u8 *sbuf, u16 *slen); 
extern void rtDataGet(u8 *sbuf, u16 *slen); 
#endif
