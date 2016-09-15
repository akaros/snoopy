/*
 * This file is part of the UCB release of Plan 9. It is subject to the license
 * terms in the LICENSE file found in the top-level directory of this
 * distribution and at http://akaros.cs.berkeley.edu/files/Plan9License. No
 * part of the UCB release of Plan 9, including this file, may be copied,
 * modified, propagated, or distributed except according to the terms contained
 * in the LICENSE file.
 */


#include "ip.h"
#include "dat.h"
#include <stdarg.h>
#include <sys/time.h>

/* convenience function: do a print, return the pointer to the end. */
char *seprint(char *buf, char *end, const char *fmt, ...)
{
	va_list ap;
	int rc;
	int n = end - buf;

	if (n <= 0)
		return buf;

	va_start(ap, fmt);
	rc = vsnprintf(buf, n, fmt, ap);
	va_end(ap);

	if (rc >= 0)
		return buf + rc;
	else
		return buf;
}

void sysfatal(const char *fmt, ...)
{
	va_list ap;
	int rc;
	static char buf[512];

	va_start(ap, fmt);
	rc = vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	if (rc >= 0)
		fprintf(stderr, buf);
	exit(1);
}

#if 0
uint64_t nsec(void)
{
	uint64_t ret;
	struct timeval t;
	gettimeofday(&t, NULL);

	ret = t.tv_sec;
	ret *= 1000000ULL;
	ret += t.tv_usec;
	ret *= 1000ULL;
	return ret;
}
#endif
