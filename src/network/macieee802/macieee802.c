#include "macieee802.h"

#include <strings/strings.h>
#include <string.h>

#include <stdio.h>


const struct macieee802 macieee802_broadcast    = MACIEEE802_IBROADCAST;

const struct macieee802 macieee802_v6allnodes   = MACIEEE802_IV6ALLNODES;
const struct macieee802 macieee802_v6allrouters = MACIEEE802_IV6ALLROUTERS;

const struct macieee802 macieee802_v4allnodes   = MACIEEE802_IV4ALLNODES;
const struct macieee802 macieee802_v4allrouters = MACIEEE802_IV4ALLROUTERS;


int
macieee802_strtou8(const char *string, struct macieee802 *network) {
  uint8_t bytes[MACIEEE802_SIZE] = {0};
  uint8_t x11, x22 = 0, x33;

  char delimiter;

  if (!string || !*string || string_length(string) != MACIEEE802_STRLENGTH-1)
    return -1;

  delimiter = string[2];

  if (delimiter != ':' && delimiter != '-')
    return -1;

  for (;;) {
#define hexadecimal(char) ((char-'0' < 10) ? (char-'0') : ((char|32)-'a' < 6) ? ((char|32)-'a'+10) : (-1))
    for (x33 = 0, x11 = 0; x33 < 2 && *string && hexadecimal(*string) >= 0; string++, x33++)
      x11 = (uint8_t)(16*x11+hexadecimal(*string));

    if ((*string && *string != delimiter) || (*string && !*(string+1) && *string == delimiter))
      return -1;
    if (x33 == 0 || x22 > 5)
      return -1;

    bytes[x22++] = x11;

    if (!*string) {
      if (x22 != MACIEEE802_SIZE /* 00:00:00:00:00:00 */)
        return -1;

      break;
    }

    string++;
  }

  if (network)
    memcpy(network, bytes, MACIEEE802_SIZE);

  return 1;
}

char *
macieee802_u8tostr(const struct macieee802 *network, char *string, size_t length) {
  char source[MACIEEE802_STRLENGTH] = {0};

  string_zero(source, sizeof(source));

  snprintf(source, sizeof(source), "%02X:%02X:%02X:%02X:%02X:%02X", network->u8[0], network->u8[1], network->u8[2], network->u8[3], network->u8[4], network->u8[5]);

  if (string)
    return (char *)memcpy(string, source, length);

  return string_new(source);
}

char *
macieee802_u8instr(const struct macieee802 *network) {
  return macieee802_u8tostr(network, NULL, 0);
}

int
macieee802_compare(const struct macieee802 *network1, const struct macieee802 *network2) {
  uint8_t x11;

  for (x11 = 0; x11 < MACIEEE802_SIZE; x11++)
    if (network1->u8[x11] < network2->u8[x11])
      return -1;
    else if (network1->u8[x11] > network2->u8[x11])
      return 1;

  return 0;
}
