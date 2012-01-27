/*
 * Copyright 2011-2012 the Redfish authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef REDFISH_STEST_STEST_DOT_H
#define REDFISH_STEST_STEST_DOT_H

#include "util/compiler.h"

struct redfish_client;
struct redfish_mds_locator;

struct stest_custom_opt
{
	/** Pointer to a key which identifies the option */
	const char *key;

	/** Pointer to a statically allocated string */
	char *val;

	/** Help string to print out in usage function */
	const char *help;
};

/** Get a custom option value from a list of custom options
 *
 * @param key		the key to get
 * @param copt		array of custom options
 * @param ncopt		length of copt
 *
 * @returnv		NULL if key isn't found; the matchinvalue otherwise
 */
const char *copt_get(const char *key, struct stest_custom_opt *copt,
		     int ncopt);

/** Parse argv.
 *
 * Custom options are given on the commandline as key=val.
 *
 * Will abort the program if a parse error is encountered.
 *
 * @param argc		argc as passed to main
 * @param argv		argv as passed to main
 * @param copt		(in-out param) NULL-terminated list of custom stest
 *			options. All custom options are treated as
 *			non-mandatory. Custom options not found on the
 *			commandline will not have their entries in copt
 *			altered.
 * @param ncopt		number of custom options
 * @param cpath		(out-param) The path to the configuration file to use.
 *			Statically allocated.
 * @param user		(out-param) The username to connect as. Statically
 *			allocated.
 */
extern void stest_init(int argc, char **argv, struct stest_custom_opt *copt,
		       int ncopt, const char **cpath, const char **user);

/** Free mlocs array returned from stest_init
 *
 * @param mlocs		the mlocs array returned from stest_init
 */
void stest_mlocs_free(struct redfish_mds_locator **mlocs);

/** Set the stest done status
 *
 * @param pdone		Percent done, from 0 to 100
 */
extern void stest_set_status(int pdone);

/** Log an error that happened during our test.
 *
 * @param err		The format string
 * @param ...		Printf-style arguments
 */
extern void stest_add_error(const char *fmt, ...) PRINTF_FORMAT(1, 2);

/** Should be called at the end of a system test main().
 *
 * @return		The status that should be returned from the main
 *			function of a system test.
 */
extern int stest_finish(void);

/** result from stest_stat */
enum stest_stat_res {
	STEST_STAT_RES_ERR = 1,
	STEST_STAT_RES_ENOENT = 2,
	STEST_STAT_RES_FILE = 100,
	STEST_STAT_RES_DIR = 101,
};

/** Simple wrapper function for redfish_stat
 *
 * @param cli		Redfish client
 * @param path		path to stat
 *
 * @return		the result
 */
extern enum stest_stat_res stest_stat(struct redfish_client *cli,
				      const char *path);

#define ST_EXPECT_ZERO(expr) \
	do { \
		int __e__ = expr; \
		if (__e__) { \
			stest_add_error("failed on file %s, line %d: %s\n" \
					"expected 0, got %d\n", \
					__FILE__, __LINE__, #expr, __e__); \
			return 1; \
		} \
	} while(0);

#define ST_EXPECT_NONZERO(expr) \
	do { \
		if (!expr) { \
			stest_add_error("failed on file %s, line %d: %s\n", \
					"expected nonzero, got 0\n", \
					__FILE__, __LINE__, #expr); \
			return 1; \
		} \
	} while(0);

#define ST_EXPECT_EQ(expr1,  expr2) \
	do { \
		if ((expr1) != (expr2)) { \
			stest_add_error("failed on file %s, line %d\n", \
					__FILE__, __LINE__); \
			return 1; \
		} \
	} while(0);

#define ST_EXPECT_NOT_EQ(expr1, expr2) \
	do { \
		if ((expr1) == (expr2)) { \
			stest_add_error("failed on file %s, line %d\n", \
					__FILE__, __LINE__); \
			return 1; \
		} \
	} while(0);

#endif
