/**
 * @file hash.c
 * @version 0.4.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"
#include <arpa/inet.h>

static inline uint32_t rotateleft(uint32_t value, int positions)
{
    return ((value << positions) | (value >> (32 - positions)));
}

static void block_update(struct hash_sha1_t *self_p,
                         uint8_t *block_p)
{
    uint32_t a ,b ,c ,d ,e, f, i ,t ,w[80];
    uint32_t *b_p = (uint32_t *)block_p;

    for (i = 0; i < 16; i++) {
        w[i] = ntohl(b_p[i]);
    }

    for (i = 16; i < 80; i++) {
        w[i] = rotateleft(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    }

    a = self_p->h[0];
    b = self_p->h[1];
    c = self_p->h[2];
    d = self_p->h[3];
    e = self_p->h[4];

    for (i = 0; i < 20; i++) {
        f = ((b & c) ^ (~b & d));
        t = rotateleft(a, 5) + f + e + 0x5a827999 + w[i];
        e = d;
        d = c;
        c = rotateleft(b, 30);
        b = a;
        a = t;
    }

    for (; i < 40; i++) {
        f = (b ^ c ^ d);
        t = rotateleft(a, 5) + f + e + 0x6ed9eba1 + w[i];
        e = d;
        d = c;
        c = rotateleft(b, 30);
        b = a;
        a = t;
    }

    for (; i < 60; i++) {
        f = ((b & c) ^ (b & d) ^ (c & d));
        t = rotateleft(a, 5) + f + e + 0x8f1bbcdc + w[i];
        e = d;
        d = c;
        c = rotateleft(b, 30);
        b = a;
        a = t;
    }

    for (; i < 80; i++) {
        f = (b ^ c ^ d);
        t = rotateleft(a,5) + f + e + 0xca62c1d6 + w[i];
        e = d;
        d = c;
        c = rotateleft(b, 30);
        b = a;
        a = t;
    }

    self_p->h[0] += a;
    self_p->h[1] += b;
    self_p->h[2] += c;
    self_p->h[3] += d;
    self_p->h[4] += e;
}

int hash_sha1_init(struct hash_sha1_t *self_p)
{
    self_p->block.size = 0;
    self_p->h[0] = 0x67452301;
    self_p->h[1] = 0xefcdab89;
    self_p->h[2] = 0x98badcfe;
    self_p->h[3] = 0x10325476;
    self_p->h[4] = 0xc3d2e1f0;
    self_p->size = 0;

    return (0);
}

int hash_sha1_update(struct hash_sha1_t *self_p,
                     void *buf_p,
                     size_t size)
{
    uint32_t temp;
    uint8_t *b_p = buf_p;

    self_p->size += size;

    /* Prologue: Fill the buffer. */
    if (self_p->block.size > 0) {
        if ((self_p->block.size + size) >= 64) {
            temp = (64 - self_p->block.size);
            memcpy(&self_p->block.buf[self_p->block.size], b_p, temp);
            size -= temp;
            b_p += temp;
            block_update(self_p, self_p->block.buf);
            self_p->block.size = 0;
        }
    }

    /* Main loop. */
    while (size >= 64) {
        memcpy(&self_p->block.buf[0], b_p, 64);
        block_update(self_p, self_p->block.buf);
        size -= 64;
        b_p += 64;
    }

    /* Epilogue: Save left over block in buffer. */
    if (size > 0) {
        memcpy(&self_p->block.buf[self_p->block.size], b_p, size);
        self_p->block.size += size;
    }

    return (0);
}

int hash_sha1_digest(struct hash_sha1_t *self_p,
                     uint8_t *hash_p)
{
    int i;

    i = self_p->block.size;

    /* Add the last byte 0x80 and zero-padding. */
    if (self_p->block.size < 56) {
        self_p->block.buf[i++] = 0x80;

        if (i < 56) {
            memset(&self_p->block.buf[i], 0, 56 - i);
        }
    } else {
        self_p->block.buf[i++] = 0x80;

        if (i < 64) {
            memset(&self_p->block.buf[i], 0, 64 - i);
        }

        block_update(self_p, self_p->block.buf);
        memset(self_p->block.buf, 0, 56);
    }

    /* Append the message length and do the last block update. */
    for (i = 0; i < 8; i++) {
        self_p->block.buf[56 + i] = ((8 * self_p->size) >> (56 - 8 * i));
    }

    block_update(self_p, self_p->block.buf);

    /* Copy the hash to the output buffer. */
    for (i = 0; i < membersof(self_p->h); i++) {
        self_p->h[i] = htonl(self_p->h[i]);
    }

    memcpy(hash_p, self_p->h, 20);

    return (0);
}
