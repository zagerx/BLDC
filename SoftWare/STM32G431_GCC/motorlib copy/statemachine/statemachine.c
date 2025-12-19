/**
 * @file statemachine.c
 * @brief Finite State Machine implementation core
 *
 * Provides core state machine functionality including:
 * - Initialization
 * - State transitions
 * - Signal handling
 *
 * Copyright (c) 2023 DeepSeek Company
 * SPDX-License-Identifier: Apache-2.0
 */

#include "statemachine.h"
#include <stdint.h>

/**
 * @brief Initialize a state machine instance
 * @param fsm Pointer to state machine control block
 * @param name Name identifier for the state machine
 * @param initial_state Starting state for the machine
 * @param context User-defined context pointer
 * @param arr Transition table array
 * @param arr_size Size of transition table array
 * @return fsm_rt_cpl on success, fsm_rt_err on invalid parameters
 *
 * Initializes all fields of the state machine control block including:
 * - State tracking
 * - Transition table
 * - Context pointer
 */
fsm_rt_t statemachine_init(fsm_cb_t *obj, const char *name, fsm_t initial_state, void *context,
			   struct state_transition_map *map, int8_t map_size)
{
	if (!obj || !initial_state) {
		return fsm_rt_err;
	}

	obj->chState = ENTER;
	obj->name = name;
	obj->p1 = context;
	obj->current_state = initial_state;
	obj->transition_table = map;
	obj->transition_table_size = map_size;
	obj->sig = NULL_USE_SING;
	return fsm_rt_cpl;
}

/**
 * @brief Process state transition based on received signal
 * @param fsm Pointer to state machine control block
 * @param sig Signal triggering the transition
 *
 * Searches transition table for matching signal and updates state if found.
 * No action taken for NULL_USE_SING signals.
 */
void statemachine_updatestatus(fsm_cb_t *obj, enum fsm_signal sig)
{
	if (sig == NULL_USE_SING) {
		return;
	}
	for (uint8_t i = 0; i < obj->transition_table_size; i++) {
		if (sig == obj->transition_table[i].signal) {
			obj->chState = obj->transition_table[i].target_state;
			break;
		}
	}
}

/**
 * @brief Set signal for state machine processing
 * @param fsm Pointer to state machine control block
 * @param sig Signal to be processed
 *
 * Stores signal which will be handled in next state machine cycle.
 */
void statemachine_setsig(fsm_cb_t *obj, enum fsm_signal sig)
{
	obj->sig = sig;
}