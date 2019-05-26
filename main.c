#include <stdio.h>
#include "erpc.h"
#include <string.h>

/*
 * socat EXEC:/path/to/erpc_client_pipe TCP4:localhost:10002
 */

bool erpc_send_request(erpc_packet_t *request) {
    fwrite(request->data, 1, request->length, stdout);
    fflush(stdout);
    return true;
}

int main() {
    erpc_packet_t request;
    erpc_packet_zero(&request);
    erpc_packet_t response;
    erpc_packet_zero(&response);

    char hello[128];

    for (int i = 0; i < 1000000; i++) {
        erpc_init_packet(&request, i, 1);
        snprintf(hello, sizeof(hello), "Hello, %d", i);
        erpc_set_param_array(&request, 0, (uint8_t *) hello, strlen(hello) + 1);
        erpc_update_packet_length(&request, 1);
        erpc_finish_packet(&request);
        erpc_send_request(&request);
        bool result;
        int c;
        erpc_packet_zero(&response);
        while ((c = fgetc(stdin)) != EOF) {
            result = erpc_feed_packet(&response, c);
            if (result == true) {
                uint8_t *param0;
                uint8_t length;
                if (erpc_get_param_array(&response, 0, &param0, &length) == true) {
                    fprintf(stderr, "result: %s\n", param0);
                }
                break;
            }
        }
    }

    return 0;
}