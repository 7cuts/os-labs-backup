#ifndef _ERRNO_H /* use the same define as errno.h does */
#define _ERRNO_H

/* provide E* constants */
#include <bits/errno.h>

int *
my_errno_location();

#define errno (*my_errno_location())

#endif
