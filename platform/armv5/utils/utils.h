#ifndef __ARMV5_UTILS_H__
#define __ARMV5_UTILS_H__

#define ITCM_BASE		0x00000000
#define ITCM_SIZE		0x00010000
#define ITCM_END		(ITCM_BASE + ITCM_SIZE)
#define ITCM_ALIAS_SIZE		0x00020000

#define DTCM_BASE		0x04000000
#define DTCM_SIZE		0x00004000
#define DTCM_END		(DTCM_BASE + DTCM_SIZE)
#define DTCM_ALIAS_SIZE		0x00020000

#define APBC_BASE		0x10000000
#define APBC_SIZE		0x00010000
#define APBC_END		(APBC_BASE + APBC_SIZE)
#define APBC_ALIAS_SIZE		0x00010000

#define __raw_read(reg)         (*(volatile uint32 *)reg)
#define __raw_write(reg, value)	(*(volatile uint32 *)reg = (uint32)(value))

inline void __io_putc(void *dev, char ch);
inline char __io_getc(void *dev);
void bsp_init(void);

#endif /* __ARMV5_UTILS_H__ */
