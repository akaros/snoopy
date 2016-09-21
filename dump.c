/*
 * This file is part of the UCB release of Plan 9. It is subject to the license
 * terms in the LICENSE file found in the top-level directory of this
 * distribution and at http://akaros.cs.berkeley.edu/files/Plan9License. No
 * part of the UCB release of Plan 9, including this file, may be copied,
 * modified, propagated, or distributed except according to the terms contained
 * in the LICENSE file.
 */


#include "ip.h"
#include <ctype.h>
#include "dat.h"
#include "protos.h"

static void
p_compile(Filter *_)
{
}

static char tohex[16] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'a', 'b', 'c', 'd', 'e', 'f'
};

static int
p_seprint(Msg *m)
{
	int c, i, n, isstring;
	uint8_t *ps = m->ps;
	char *p = m->p;
	char *e = m->e;

	n = m->pe - ps;
	if((Nflag != 0) && (n > Nflag))
		n = Nflag;

	isstring = 1;
	for(i = 0; i < n; i++){
		c = ps[i];
		if(!isprint(c) && !isspace(c)){
			isstring = 0;
			break;
		}
	}

	if(isstring){
		for(i = 0; i < n && p+1<e; i++){
			c = ps[i];
			switch(c){
			case '\t':
				*p++ = '\\';
				*p++ = 't';
				break;
			case '\r':
				*p++ = '\\';
				*p++ = 'r';
				break;
			case '\n':
				*p++ = '\\';
				*p++ = 'n';
				break;
			default:
				*p++ = c;
			}
		}
	} else {
		for(i = 0; i < n && p+2<e; i++){
			if (i % 16)
				*p++ = ' ';
			c = ps[i];
			*p++ = tohex[c>>4];
			*p++ = tohex[c&0xf]; 
			/* Attempt to dump individual bytes in ASCII */
			if (i && (i % 16 == 15)) {
				/* 25: 2 space, 16 chars, \n, \t, 'dump', (. */
				if (p + 24 >= e)
					break;
				*p++ = ' ';
				*p++ = ' ';
				/* print out the *last* 16 bytes we just dumped. */
				for (int j = i - 16; j < i; j++)
					*p++ = isprint(ps[j]) ? ps[j] : '.';
				*p++ = '\n';
				*p++ = '\t';
				for (int j = 0; j < 4; j++)
					*p++ = ' ';
				*p++ = ' ';
			}
		}
	}

	m->pr = NULL;
	m->p = p;
	m->ps = ps;

	return 0;
}

Proto dump =
{
	"dump",
	p_compile,
	NULL,
	p_seprint,
	NULL,
	NULL,
	NULL,
	defaultframer,
};
