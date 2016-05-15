#include "common.h"

int
main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "./UDPRawClient <sender ip> <receiver ip>\n");
        return -1;
    } else {
        Encoder e;
        UDPRawClient client(e, argv[1],argv[2]);
        client.run();
    }
    return 0;
}
