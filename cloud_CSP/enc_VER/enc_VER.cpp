#include "enc_VER_t.h"

#include "sgx_trts.h"

#include <string.h>
void foo_th(char *buf, size_t len)
{
	const char *secret = "hello enclave3";
	if (len > strlen(secret))
	{
		memcpy(buf, secret, strlen(secret) + 1);
	}
}
