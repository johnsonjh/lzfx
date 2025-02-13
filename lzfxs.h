/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * lzfxs.h  --  LZFXS compression / decompression header
 *
 * Copyright (c) 2000-2008 Marc Alexander Lehmann <schmorp@schmorp.de>
 * Copyright (c) 2009 Andrew Collette <andrew.collette@gmail.com>
 * Copyright (c) 2023 Jeffrey H. Johnson <trnsz@pobox.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1.  Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *   2.  Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LZFXS_H
# define LZFXS_H

# ifdef __cplusplus
  extern "C"
  {
# endif /* ifdef __cplusplus */

/*
 * Documented behavior, including function signatures and error codes,
 * is guaranteed to remain unchanged for releases with the same major
 * version number.  Releases of the same major version are also able
 * to read each other's output, although the output itself is not
 * guaranteed to be byte-for-byte identical.
 */

# define LZFXS_VERSION_MAJOR    0
# define LZFXS_VERSION_MINOR    1
# define LZFXS_VERSION_STRING   "0.1"

/* Hash table size (2**LZFXS_HLOG entries) */
# ifndef LZFXS_HLOG
#  define LZFXS_HLOG  16
# endif /* ifndef LZFXS_HLOG */

/* Predefined errors. */
# define LZFXS_ESIZE      -1  /* Output buffer too small        */
# define LZFXS_ECORRUPT   -2  /* Invalid data for decompression */
# define LZFXS_EARGS      -3  /* Arguments invalid (NULL)       */

/*
 * Buffer-to buffer compression.
 *
 * Supply pre-allocated input and output buffers via ibuf and obuf, and
 * their size in bytes via ilen and olen.  Buffers may not overlap.
 *
 * On success, the function returns a non-negative value and the argument
 * olen contains the compressed size in bytes.  On failure, a negative
 * value is returned and olen is not modified.
 */

int lzfxs_compress(const void *ibuf, unsigned int ilen, void *obuf,
                   unsigned int *olen);

/*
 * Buffer-to-buffer decompression.
 *
 * Supply pre-allocated input and output buffers via ibuf and obuf, and
 * their size in bytes via ilen and olen.  Buffers may not overlap.
 *
 * On success, the function returns a non-negative value and the argument
 * olen contains the uncompressed size in bytes.  On failure, a negative
 * value is returned.
 *
 * If the failure code is LZFXS_ESIZE, olen contains the minimum buffer size
 * required to hold the decompressed data.  Otherwise, olen is not modified.
 *
 * Supplying a zero *olen is a valid and supported strategy to determine the
 * required buffer size.  This does not require decompression of the entire
 * stream and is consequently very fast.  Argument obuf may be NULL in
 * this case only.
 */

int lzfxs_decompress(const void *ibuf, unsigned int ilen, void *obuf,
                     unsigned int *olen);

# ifdef __cplusplus
  } /* extern "C" */
# endif /* ifdef __cplusplus */

#endif /* ifndef LZFXS_H */
