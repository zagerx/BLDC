/**
 * @file statemachine.h
 * @brief Finite State Machine (FSM) framework
 *
 * Provides:
 * - State machine return codes
 * - Basic state machine control block
 * - State transition macros
 *
 * Copyright (c) 2023 Your Company
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _STATEMACHINE_H
#define _STATEMACHINE_H
#include "stdint.h"
#include <stdint.h>
/**
 * @enum fsm_rt_t
 * @brief State machine return codes
 */
typedef enum {
	fsm_rt_err = -1,	 ///< FSM error (check error code from other interface)
	fsm_rt_cpl = 0,		 ///< FSM completed successfully
	fsm_rt_on_going = 1,	 ///< FSM operation in progress
	fsm_rt_wait_for_obj = 2, ///< FSM waiting for resource/object
	fsm_rt_asyn = 3,	 ///< FSM in asynchronous mode (check status later)
} fsm_rt_t;

/**
 * @enum Standard states
 * @brief Common state machine states
 */
enum {
	ENTER = 0,  ///< Entry state handler
	EXIT,	    ///< Exit state handler
	USER_STATUS ///< First available user-defined state
};
/**
 * enum fsm_signal - Finite State Machine signal identifiers
 * @NULL_USE_SING: Null signal (unused/reserved)
 * @USER_SIG: Base value for user-defined signals
 *
 * Note: User signals should be defined consecutively after USER_SIG
 */
enum fsm_signal {
	NULL_USE_SING = 0, /* Reserved null signal identifier */
	USER_SIG	   /* First available user-defined signal */
};
/**
 * @struct state_transition_map
 * @brief Signal-to-state transition mapping table entry
 */
struct state_transition_map {
	enum fsm_signal signal; ///< Input signal that triggers transition
	int16_t target_state;	///< Resulting state after signal processing
};
/* Forward declarations */
typedef struct fsm_cb fsm_cb_t;
typedef fsm_rt_t (*fsm_t)(fsm_cb_t *);

/**
 * @struct fsm_cb
 * @brief State machine control block
 */
struct fsm_cb {
	unsigned char chState; ///< Current state
	const char *name;      ///< State machine name (for debugging)
	enum fsm_signal sig;
	struct state_transition_map *transition_table; ///< Signal-state mapping table
	int8_t transition_table_size;		       ///< Number of entries in table

	void *p1;	     ///< User data pointer
	fsm_t current_state; ///< Current state handler function
	fsm_t previous_state;
	fsm_cb_t *sub_state_machine;
};

/**
 * @brief Execute current state handler
 * @param me_ State machine control block pointer
 */
#define DISPATCH_FSM(me_) ((fsm_t)(me_)->current_state)((me_))

/**
 * @brief Transition between states
 * @param me State machine control block
 * @param targe Target state handler function
 *
 * Handles proper state transitions by:
 * 1. Executing exit handler for current state
 * 2. Changing to new state handler
 * 3. Executing entry handler for new state
 */
#define TRAN_STATE(me, targe)                                                                      \
	do {                                                                                       \
		(me)->chState = EXIT;                                                              \
		(me)->current_state(me);                                                           \
		(me)->previous_state = (me)->current_state;                                        \
		(me)->current_state = (fsm_t)(targe);                                              \
		(me)->chState = ENTER;                                                             \
		(me)->current_state(me);                                                           \
	} while (0)

/* Function prototypes */
fsm_rt_t statemachine_init(fsm_cb_t *obj, const char *name, fsm_t initial_state, void *context,
			   struct state_transition_map *arr, int8_t arr_size);
void statemachine_updatestatus(fsm_cb_t *obj, enum fsm_signal sig);
void statemachine_setsig(fsm_cb_t *obj, enum fsm_signal sig);

#endif /* _STATEMACHINE_H */