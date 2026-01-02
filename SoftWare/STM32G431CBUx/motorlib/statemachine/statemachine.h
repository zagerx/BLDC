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
	fsm_rt_err = -1,         ///< FSM error (check error code from other interface)
	fsm_rt_cpl = 0,          ///< FSM completed successfully
	fsm_rt_on_going = 1,     ///< FSM operation in progress
	fsm_rt_wait_for_obj = 2, ///< FSM waiting for resource/object
	fsm_rt_asyn = 3,         ///< FSM in asynchronous mode (check status later)
} fsm_rt_t;

/**
 * @enum Standard states
 * @brief Common state machine states
 */
enum {
	ENTER = 0,  ///< Entry state handler
	EXIT,       ///< Exit state handler
	USER_STATUS ///< First available user-defined state
};
/* Forward declarations */
typedef struct fsm_cb fsm_cb_t;
typedef fsm_rt_t (*fsm_t)(fsm_cb_t *);

/**
 * @struct fsm_cb
 * @brief State machine control block
 */
struct fsm_cb {
	int16_t phase; ///< Current state
	const char *name;
	uint32_t count;
	void *p1;            ///< User data pointer
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
		(me)->phase = EXIT;                                                                \
		(me)->current_state(me);                                                           \
		(me)->previous_state = (me)->current_state;                                        \
		(me)->current_state = (fsm_t)(targe);                                              \
		(me)->phase = ENTER;                                                               \
		(me)->current_state(me);                                                           \
	} while (0)

/* Function prototypes */

#endif /* _STATEMACHINE_H */