/*
 * Copyright (c) 2012 - 2013 Tony Finch <dot@dotat.at>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/param.h>
#include <sys/stat.h>
#ifndef __APPLE__
#include <sys/tree.h>
#else
#include <copyfile.h>
#include "tree.h"
#endif

#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* portability stubs */

#define fbinmode(fp) (fp)

#ifdef __APPLE__
static inline int
replace(const char *old, const char *new)
{
	copyfile_state_t s;
	int ret;

	s = copyfile_state_alloc();
	if (s == NULL)
		return (-1);
	ret = copyfile(old, new, s, COPYFILE_ALL);
	copyfile_state_free(s);
	if (ret != 0)
		return (-1);
	ret = unlink(old);
	if (ret != 0)
		return (-1);
	return (0);
}
#else /* !__APPLE__ */
#define replace(old,new) rename(old,new)
#endif /* __APPLE__ */

static FILE *
mktempmode(char *tmp, int mode)
{
	int fd = mkstemp(tmp);
	if (fd < 0) return (NULL);
	fchmod(fd, mode & (S_IRWXU|S_IRWXG|S_IRWXO));
	return (fdopen(fd, "wb"));
}
