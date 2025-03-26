#include <string.h>
#include <stdint.h>
#define RTW_SUCCESS 0
static int wifi_get_mac_address(char mac[6]){   };
int myHalWifiGetMacAddr(char* mac, uint32_t macBufSize)
{
    char buf[32] = {0};

    if (!mac || macBufSize < 32) {
        return -1;
    }
    if (RTW_SUCCESS != wifi_get_mac_address(buf)) {
        return -2;
    }
    for (uint8_t i = 0; i < strlen(buf); i++) {
        if (buf[i] >= 'a' && buf[i] <= 'f') {
            buf[i] -= 32;
        }
    }
    memcpy(&mac[0], &buf[0], 2);
    memcpy(&mac[2], &buf[3], 2);
    memcpy(&mac[4], &buf[6], 2);
    memcpy(&mac[6], &buf[9], 2);
    memcpy(&mac[8], &buf[12], 2);
    memcpy(&mac[10], &buf[15], 2);
    return 0;
}