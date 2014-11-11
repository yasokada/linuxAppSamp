#ifndef APP_05COMMON_H
#define APP_05COMMON_H

#define SIZE_ONE_PACKET 2048 // size of data sent in a packet
#define EOF_POS_A (SIZE_ONE_PACKET + 1)
#define EOF_POS_B (EOF_POS_A + 1)

/*
 * Data will be sent after divided into (N * SIZE_ONE_PACKET).
 *
 * In each transmission, the record becomes like
 *  | data (2048 bytes) | A | B |
 * where A and B are used to know 
 * - whether or not EOF is included in the data
 * - position of EOF ( = A * 256 + B ) where position is starting from 1 (1..*)
 *
 * A, B : unsigned int
 */

#endif // APP_05COMMON_H
