#ifndef __BYTE_OP_H__
#define __BYTE_OP_H__

#include <cstdio>
#include <cstring>

#define VAR_TO_MEM_1BYTE_BIG_ENDIAN(v, p) \
  *(p++) = v & 0xff;
// Stores a 1-byte value in big-endian format into memory.
// The lower 1 byte of v (v & 0xff) is stored at the location pointed to by p (*p), and then p is incremented by 1 (p++).

#define VAR_TO_MEM_2BYTES_BIG_ENDIAN(v, p) \
  *(p++) = (v >> 8) & 0xff;                \
  *(p++) = v & 0xff;
// Stores a 2-byte value in big-endian format into memory.
// The upper byte of v ((v >> 8) & 0xff) is stored first (*p), followed by the lower byte (v & 0xff).
// The pointer p is incremented by 2 (p++ twice) after storing the value.

#define VAR_TO_MEM_4BYTES_BIG_ENDIAN(v, p) \
  *(p++) = (v >> 24) & 0xff;               \
  *(p++) = (v >> 16) & 0xff;               \
  *(p++) = (v >> 8) & 0xff;                \
  *(p++) = v & 0xff;
// Stores a 4-byte value in big-endian format into memory.
// The uppermost byte of v ((v >> 24) & 0xff) is stored first (*p), followed by the next bytes in sequence.
// ((v >> 16) & 0xff), ((v >> 8) & 0xff), and (v & 0xff).
// The pointer p is incremented by 4 (p++ four times) after storing the value.

#define MEM_TO_VAR_1BYTE_BIG_ENDIAN(p, v) \
  v = (p[0] & 0xff);                      \
  p += 1;
// Reads a 1-byte value from memory and stores it in variable v.
// The value at the location pointed to by p (p[0] & 0xff) is stored in v, and then p is incremented by 1 (p += 1).

#define MEM_TO_VAR_2BYTES_BIG_ENDIAN(p, v)  \
  v = ((p[0] & 0xff) << 8) | (p[1] & 0xff); \
  p += 2;
// Reads a 2-byte value from memory and stores it in variable v.
// The value at p[0] (p[0] & 0xff) is shifted left by 8 bits ((p[0] & 0xff) << 8) and ORed with the value at p[1] (p[1] & 0xff), then stored in v.
// The pointer p is incremented by 2 (p += 2).

#define MEM_TO_VAR_4BYTES_BIG_ENDIAN(p, v)                                                   \
  v = ((p[0] & 0xff) << 24) | ((p[1] & 0xff) << 16) | ((p[2] & 0xff) << 8) | (p[3] & 0xff); \
  p += 4;
// Reads a 4-byte value from memory and stores it in variable v.
// The value at p[0] (p[0] & 0xff) is shifted left by 24 bits ((p[0] & 0xff) << 24), the value at p[1] (p[1] & 0xff) is shifted left by 16 bits ((p[1] & 0xff) << 16),
// the value at p[2] (p[2] & 0xff) is shifted left by 8 bits ((p[2] & 0xff) << 8), and the value at p[3] (p[3] & 0xff) is added to form the final value in v.
// The pointer p is incremented by 4 (p += 4).

#define PRINT_MEM(p, len)         \
  printf("Print buffer:\n  >> "); \
  for (int i = 0; i < len; i++)   \
  {                               \
    printf("%02x ", p[i]);        \
    if (i % 16 == 15)             \
      printf("\n  >> ");          \
  }                               \
  printf("\n");
// Prints the contents of memory in a formatted manner.
// the parameter p is the starting address of the memory, and len is the number of bytes to print.
// The memory content is printed in hexadecimal format (printf("%02x ", p[i])), with a new line every 16 bytes.

#endif /* __BYTE_OP_H__ */
