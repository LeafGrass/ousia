/* x86 port */
#ifndef __OUSIA_CFG_H__
#define __OUSIA_CFG_H__

/*
 * scheduler strategy
 * 0 - EDFS
 * 1 - EDFS_OPT
 * 2 - HPFS
 * 3 - CFS
 */
#define OUSIA_SCHED_STRATEGY		OUSIA_SCHED_STRATEGY_EDFS_OPT
#define OUSIA_SCHED_STRATEGY_EDFS	0
#define OUSIA_SCHED_STRATEGY_EDFS_OPT	1
#define OUSIA_SCHED_STRATEGY_HPFS	2
#define OUSIA_SCHED_STRATEGY_CFS	3

#endif /* __OUSIA_CFG_H__ */

