/* x86 port */
#ifndef __OUSIA_CFG_H__
#define __OUSIA_CFG_H__

#define OUSIA_DS_ADVANCED

#define OUSIA_PORT_STACK_INC	1
#define OUSIA_PORT_STACK_DEC	0
#define OUSIA_PORT_STACK_TYPE	OUSIA_PORT_STACK_INC

/*
 * scheduler strategy
 * 0 - EDFS
 * 1 - EDFS_OPT
 * 2 - HPFS
 * 3 - CFS
 */
/*#define OUSIA_SCHED_STRATEGY_EDFS*/
/*#define OUSIA_SCHED_STRATEGY_EDFS_OPT*/
/*#define OUSIA_SCHED_STRATEGY_HPFS*/
/*#define OUSIA_SCHED_STRATEGY_CFS*/
#define OUSIA_SCHED_STRATEGY_RGHS

#endif /* __OUSIA_CFG_H__ */
