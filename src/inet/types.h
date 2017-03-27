/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

#ifndef __INET_TYPES_H__
#define __INET_TYPES_H__

#define HTTP_TYPE_TEXT   1
#define HTTP_TYPE_BINARY 2

/**
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-------+-+-------------+-------------------------------+
 * |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
 * |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
 * |N|V|V|V|       |S|             |   (if payload len==126/127)   |
 * | |1|2|3|       |K|             |                               |
 * +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
 * |     Extended payload length continued, if payload len == 127  |
 * + - - - - - - - - - - - - - - - +-------------------------------+
 * |                               |Masking-key, if MASK set to 1  |
 * +-------------------------------+-------------------------------+
 * | Masking-key (continued)       |          Payload Data         |
 * +-------------------------------- - - - - - - - - - - - - - - - +
 * :                     Payload Data continued ...                :
 * + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
 * |                     Payload Data continued ...                |
 * +---------------------------------------------------------------+
 *
 * FIN:  1 bit
 *
 *    Indicates that this is the final fragment in a message. The
 *    first fragment MAY also be the final fragment.
 *
 * RSV1, RSV2, RSV3:  1 bit each
 *
 *    MUST be 0 unless an extension is negotiated that defines
 *    meanings for non-zero values. If a nonzero value is received and
 *    none of the negotiated extensions defines the meaning of such a
 *    nonzero value, the receiving endpoint MUST _Fail the WebSocket
 *    Connection_.
 *
 * Opcode:  4 bits
 *
 *    Defines the interpretation of the "Payload data". If an unknown
 *    opcode is received, the receiving endpoint MUST _Fail the
 *    WebSocket Connection_. The following values are defined.
 *
 *    *  0x0   denotes a continuation frame
 *    *  0x1   denotes a text frame
 *    *  0x2   denotes a binary frame
 *    *  0x3-7 are reserved for further non-control frames
 *    *  0x8   denotes a connection close
 *    *  0x9   denotes a ping
 *    *  0xA   denotes a pong
 *    *  0xb-f are reserved for further control frames
 *
 * Mask:  1 bit
 *
 *    Defines whether the "Payload data" is masked. If set to 1, a
 *    masking key is present in masking-key, and this is used to
 *    unmask the "Payload data" as per Section 5.3. All frames sent
 *    from client to server have this bit set to 1.
 *
 * Payload length:  7 bits, 7+16 bits, or 7+64 bits
 *
 *    The length of the "Payload data", in bytes: if 0-125, that is
 *    the payload length. If 126, the following 2 bytes interpreted as
 *    a 16-bit unsigned integer are the payload length. If 127, the
 *    following 8 bytes interpreted as a 64-bit unsigned integer (the
 *    most significant bit MUST be 0) are the payload length.
 *    Multibyte length quantities are expressed in network byte order.
 *    Note that in all cases, the minimal number of bytes MUST be used
 *    to encode the length, for example, the length of a 124-byte-long
 *    string can't be encoded as the sequence 126, 0, 124. The payload
 *    length is the length of the "Extension data" + the length of the
 *    "Application data". The length of the "Extension data" may be
 *    zero, in which case the payload length is the length of the
 *    "Application data".
 *
 * Masking-key:  0 or 4 bytes
 *
 *    All frames sent from the client to the server are masked by a
 *    32-bit value that is contained within the frame. This field is
 *    present if the mask bit is set to 1 and is absent if the mask
 *    bit is set to 0. See Section 5.3 for further information on
 *    client- to-server masking.
 *
 * Payload data:  (x+y) bytes
 *
 *    The "Payload data" is defined as "Extension data" concatenated
 *    with "Application data".
 *
 * Extension data:  x bytes
 *
 *    The "Extension data" is 0 bytes unless an extension has been
 *    negotiated. Any extension MUST specify the length of the
 *    "Extension data", or how that length may be calculated, and how
 *    the extension use MUST be negotiated during the opening
 *    handshake. If present, the "Extension data" is included in the
 *    total payload length.
 *
 * Application data:  y bytes
 *
 *    Arbitrary "Application data", taking up the remainder of the
 *    frame after any "Extension data". The length of the "Application
 *    data" is equal to the payload length minus the length of the
 *    "Extension data".
 */

#define INET_HTTP_WEBSOCKET_FIN  0x80
#define INET_HTTP_WEBSOCKET_MASK 0x80

#endif
