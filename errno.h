#ifndef _ERRNO_H
#define _ERRNO_H

/* variável global de erro */
extern int errno;

/* erros básicos suficientes pra libc + Doom */
#define EPERM        1
#define ENOENT       2
#define EINTR        4
#define EIO          5
#define EBADF        9
#define EAGAIN      11
#define ENOMEM      12
#define EACCES      13
#define EFAULT      14
#define EISDIR      21
#define EINVAL      22
#define EMFILE      24
#define ENOSPC      28
#define ERANGE      34

#endif
