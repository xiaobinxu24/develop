
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_MD5_H_INCLUDED_
#define _NGX_MD5_H_INCLUDED_
/*
#ifndef NGX_HAVE_MD5
#define NGX_HAVE_MD5 1
#endif
*/

#ifndef NGX_HAVE_LITTLE_ENDIAN
#define NGX_HAVE_LITTLE_ENDIAN 1
#endif


#ifndef NGX_HAVE_NONALIGNED
#define NGX_HAVE_NONALIGNED 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#ifdef _MAXOS
#define u_char char
#endif

#define ngx_memcpy(dst, src, n)      (void) memcpy(dst, src, n)

#define ngx_memzero(buf, n)          (void) memset(buf, 0, n)

typedef struct {
    uint64_t  bytes;
    uint32_t  a, b, c, d;
    u_char    buffer[64];
} ngx_md5_t;


void ngx_md5_init(ngx_md5_t *ctx);
void ngx_md5_update(ngx_md5_t *ctx, const void *data, size_t size);
void ngx_md5_final(u_char result[16], ngx_md5_t *ctx);

int kfmd5_ngx(unsigned char *input, size_t ilen, unsigned char output[16]);

int kfmd5_ngx_tostring(unsigned char output[16], char *save);

#ifdef __cplusplus
}
#endif

#endif /* _NGX_MD5_H_INCLUDED_ */
