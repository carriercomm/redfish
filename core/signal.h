/*
 * The RedFish distributed filesystem
 *
 * Copyright (C) 2011 Colin Patrick McCabe <cmccabe@alumni.cmu.edu>
 *
 * This is licensed under the Apache License, Version 2.0.  See file COPYING.
 */

#ifndef REDFISH_CORE_SIGNAL_DOT_H
#define REDFISH_CORE_SIGNAL_DOT_H

#include <unistd.h> /* for size_t */

struct log_config;

typedef void (*signal_cb_t)(int);

/** The global fast log manager */
extern struct fast_log_mgr *g_fast_log_mgr;

/** Install the signal handlers for a RedFish daemon.
 *
 * @param err			a buffer to write any errors to
 * @param err_len		length of the error buffer
 * @param lc			The log config. If crash_log is configured, we
 *				will open a file that will be written to when a
 *				fatal signal happens.
 * @param fatal_signal_cb	Callback that is executed after a fatal signal,
 *				or NULL for none.
 *
 * We write out an error message to error if signal_init fails.
 */
extern void signal_init(const char *argv0, char *err, size_t err_len,
		const struct log_config *lc, signal_cb_t fatal_signal_cb);

/** Clear all signal handlers, free the alternate signal stack, and disable the
 * crash log.
 */
extern void signal_shutdown(void);

#endif
