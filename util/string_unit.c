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

#include "util/string.h"
#include "util/test.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

static int has_suffix_succeeded(const char *str, const char *suffix)
{
	if (has_suffix(str, suffix))
		return 0;
	else
		return 1;
}

static int has_suffix_failed(const char *str, const char *suffix)
{
	if (has_suffix(str, suffix))
		return 1;
	else
		return 0;
}

static char all_zero[16];

static int test_snappend(void)
{
	char buf[16];
	char canary[16];
	memset(buf, 0, sizeof(buf));
	memset(canary, 0, sizeof(canary));
	memset(all_zero, 0, sizeof(all_zero));
	snappend(buf, sizeof(buf), "abracadabrafoomanchucalifrag");
	EXPECT_ZERO(strcmp(buf, "abracadabrafoom"));
	EXPECT_ZERO(memcmp(canary, all_zero, sizeof(canary)));
	snappend(buf, sizeof(buf), "other stuff");
	EXPECT_ZERO(strcmp(buf, "abracadabrafoom"));
	EXPECT_ZERO(memcmp(canary, all_zero, sizeof(canary)));
	memset(buf, 0, sizeof(buf));
	snappend(buf, sizeof(buf), "%d", 123);
	EXPECT_ZERO(strcmp(buf, "123"));
	snappend(buf, sizeof(buf), "456");
	EXPECT_ZERO(strcmp(buf, "123456"));
	snappend(buf, sizeof(buf), "789");
	EXPECT_ZERO(strcmp(buf, "123456789"));
	return 0;
}

static int test_linearray_to_str(const char **lines, const char *expectstr)
{
	char *str = linearray_to_str(lines);
	if (!str)
		return -ENOMEM;
	if (strcmp(str, expectstr)) {
		free(str);
		return -EDOM;
	}
	free(str);
	return 0;
}

static const char *LTS_LINES1[] = {
	"abc",
	"123",
	"ggg",
	NULL
};

static const char *LTS_STR1 = "\
abc\n\
123\n\
ggg\n\
";

static const char *LTS_LINES2[] = {
	NULL
};

static const char *LTS_STR2 = "";

static const char *LTS_LINES3[] = {
	"a",
	NULL
};

static const char *LTS_STR3 = "a\n";

static int test_zsnprintf(size_t len, const char *str, int expect_succ)
{
	int ret;
	char buf[512];
	if (len > sizeof(buf)) {
		return -ENAMETOOLONG;
	}
	ret = zsnprintf(buf, len, "%s", str);
	if ((ret == 0) != (expect_succ != 0)) {
		fprintf(stderr, "test_zsnprintf(len=%Zd, str=%s, "
			"expect_succ=%d) failed\n",
			len, str, expect_succ);
		return 1;
	}
	return 0;
}

static int test_hex_dump(void)
{
	char str[512];
	size_t i, str_len = sizeof(str);
	const char buf[] = { 'a', 'b', 'c' };
	const char buf2[] = { 'X' };
	char bigone[24];
	const char * const bigone_out = "00 01 02 03 00 01 02 03\n"
		"00 01 02 03 00 01 02 03\n"
		"00 01 02 03 00 01 02 03\n";


	hex_dump(buf, sizeof(buf), str, str_len);
	if (strcmp("61 62 63 ", str)) {
		fprintf(stderr, "expected '61 62 63'; got '%s'\n",
			str);
		return 1;
	}
	hex_dump(buf2, sizeof(buf2), str, str_len);
	if (strcmp("58 ", str)) {
		fprintf(stderr, "expected '58 '; got '%s'\n", str);
		return 1;
	}
	hex_dump(buf, 0, str, str_len);
	if (strcmp("", str)) {
		fprintf(stderr, "expected ''; got '%s'\n", str);
		return 1;
	}
	for (i = 0; i < sizeof(bigone); ++i) {
		bigone[i] = i % 4;
	}
	hex_dump(bigone, sizeof(bigone), str, str_len);
	if (strcmp(bigone_out, str)) {
		fprintf(stderr, "expected '%s'; got '%s'\n", bigone_out, str);
		return 1;
	}
	hex_dump(bigone, sizeof(bigone), str, 6);
	if (strcmp("00 ...", str)) {
		fprintf(stderr, "expected '00 ...'; got '%s'\n", str);
		return 1;
	}
	return 0;
}

static int test_fwdprintf(void)
{
	char buf[512];
	char small[5];
	size_t off;

	off = 0;
	memset(buf, 0, sizeof(buf));
	fwdprintf(buf, &off, sizeof(buf), "foo");
	fwdprintf(buf, &off, sizeof(buf), "bar");
	fwdprintf(buf, &off, sizeof(buf), "%d", 5);
	EXPECT_ZERO(strcmp(buf, "foobar5"));

	off = 0;
	memset(small, 0, sizeof(small));
	fwdprintf(small, &off, sizeof(small), "abc");
	fwdprintf(small, &off, sizeof(small), "123");
	EXPECT_ZERO(strcmp(small, "abc1"));

	return 0;
}

static int test_strdupcat(const char *a, const char *b, const char *expected)
{
	char *str;
	
	str = strdupcat(a, b);
	EXPECT_NOT_EQ(str, NULL);
	EXPECT_ZERO(strcmp(str, expected));
	free(str);
	return 0;
}

int main(void)
{
	EXPECT_ZERO(has_suffix_succeeded("abcd", "bcd"));
	EXPECT_ZERO(has_suffix_failed("", "bcd"));
	EXPECT_ZERO(has_suffix_failed("abcd", "abc"));
	EXPECT_ZERO(has_suffix_failed("ad", "ac"));
	EXPECT_ZERO(has_suffix_succeeded("zz", "zz"));
	EXPECT_ZERO(has_suffix_succeeded("long long long str", "str"));

	EXPECT_ZERO(test_linearray_to_str(LTS_LINES1, LTS_STR1));
	EXPECT_ZERO(test_linearray_to_str(LTS_LINES2, LTS_STR2));
	EXPECT_ZERO(test_linearray_to_str(LTS_LINES3, LTS_STR3));

	EXPECT_ZERO(test_snappend());

	EXPECT_ZERO(test_zsnprintf(1, "", 1));
	EXPECT_ZERO(test_zsnprintf(1, "1", 0));
	EXPECT_ZERO(test_zsnprintf(128, "foobar", 1));
	EXPECT_ZERO(test_zsnprintf(3, "ab", 1));
	EXPECT_ZERO(test_zsnprintf(3, "abc", 0));

	EXPECT_ZERO(test_strdupcat("ab", "c", "abc"));
	EXPECT_ZERO(test_strdupcat("", "c", "c"));
	EXPECT_ZERO(test_strdupcat("", "", ""));

	EXPECT_ZERO(test_hex_dump());

	EXPECT_ZERO(test_fwdprintf());

	return EXIT_SUCCESS;
}
