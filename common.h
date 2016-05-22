#pragma once

#include "Encoder.h"
#include "Serializer.h"
#include "EncodedPackage.h"
#include "PaddingPackage.h"
#include "UDPRawClient.h"
#include "UDPRawServer.h"
#include "UDPEncodedClient.h"
#include "UDPEncodedServer.h"
#include <cstdio>

inline void
err_quit(char* msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}
