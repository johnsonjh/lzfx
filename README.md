<!--
SPDX-License-Identifier: BSD-2-Clause
Copyright (c) 2023 Jeffrey H. Johnson <trnsz@pobox.com>
-->

# LZFXS

<!-- toc -->

- [Overview](#overview)
- [History](#history)
- [LZFXS buffer format](#lzfxs-buffer-format)
  - [Literal runs](#literal-runs)
  - [Back references](#back-references)
- [LZFXS file format](#lzfxs-file-format)
  - [Block header format](#block-header-format)
  - [Uncompressed blocks](#uncompressed-blocks)
  - [Compressed blocks](#compressed-blocks)
- [API](#api)
  - [Buffer-to-buffer compression](#buffer-to-buffer-compression)
  - [Buffer-to-buffer decompression](#buffer-to-buffer-decompression)

<!-- tocstop -->

## Overview

**LZFXS** is a compact and low-complexity data compression algorithm.

## History

**LZFXS** is based on **Simplified LZFX** by _Jan Ding_, which is based on
**LZFX** by _Andrew Collette_, which is based on **LZF** by _Marc Lehmann_.

## LZFXS buffer format

There are two kinds of data structures in **LZFXS**: literal runs and back
references.

### Literal runs

Literals are encoded as follows:

The length of a literal run is encoded as L - 1, as it must contain at least
one byte.

    000LLLLL <L+1 bytes>

### Back references

Back references are encoded as follows:

The smallest possible encoded length value is 1, since otherwise, the control
byte would be recognized as a literal run. Since at least three bytes must
match for a back reference to be inserted, the length is encoded as L - 2
instead of L - 1.

The offset (distance to the desired data in the output buffer) is encoded as
o - 1, as all offsets are at least 1.

The binary format is:

    LLLooooo oooooooo           for backrefs of real length  < 9  (1 <= L < 7)
    111ooooo LLLLLLLL oooooooo  for backrefs of real length >= 9  (L > 7)

## LZFXS file format

An **LZFXS** _file_ (as opposed to a compressed _buffer_) is made up of a
sequence of blocks. Each block contains a header followed by a data payload.

### Block header format

Each block begins with a 10-byte header in the following format:

| 'L' | 'Z' | 'F' | 'X' | K1  | K2  | L1  | L2  | L3  | L4  |
| :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- |

**K1** and **K2** are a two-byte code identifying the contents, or _kind_, of
the block. The values are stored in big-endian format; K1 is the high byte
and K2 is the low byte.

The currently defined _kinds_ are:

| `0` | Reserved                          |
| :-- | :-------------------------------- |
| `1` | Compressed (**LZFXS**) data block |
| `2` | Uncompressed data block           |

**L1** through **L4** contain the length of the data payload. L1 is the most
significant byte and L4 is the least. Decompression programs which cannot
understand a given type code are required to skip the block.

### Uncompressed blocks

The block contains raw data which should be copied to the output file.

### Compressed blocks

The first four bytes contain the uncompressed size of the data, in big-endian
format. The remainder of the data payload contains compressed data.

## API

### Buffer-to-buffer compression

```c
int lzfxs_compress(const void* ibuf, unsigned int ilen,
                         void* obuf, unsigned int *olen);
```

Supply pre-allocated input and output buffers via `ibuf` and `obuf`, and
their sizes, in bytes, via `ilen` and `olen`. Buffers may not overlap.

On success, the function returns a non-negative value and the argument `olen`
contains the compressed size in bytes.

On failure, a negative value is returned and `olen` is not modified.

### Buffer-to-buffer decompression

```c
int lzfxs_decompress(const void* ibuf, unsigned int ilen,
                           void* obuf, unsigned int *olen);
```

Supply pre-allocated input and output buffers via `ibuf` and `obuf`, and
their sizes, in bytes, via `ilen` and `olen`. Buffers may not overlap.

On success, the function returns a non-negative value and the argument `olen`
contains the uncompressed size in bytes.

On failure, a negative value is returned.

If the failure code is `LZFXS_ESIZE`, `olen` contains the minimum buffer size
required to hold the decompressed data. Otherwise, `olen` is not modified.

Supplying a zero `*olen` is a valid and supported strategy to determine the
required buffer size. This does not require decompression of the entire
stream and is consequently very fast. Argument `obuf` may be `NULL` in
this case only.
