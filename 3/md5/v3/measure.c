/* 
 * LEON3 timing measurement utilities - md5 test
 * ----------------------------------------------
 * ex3
 */

#include <stdio.h>
#include <string.h>
#include "bench.h"
#include "md5.h"

typedef struct {
  char		*ss_string;			/* test string */
  char		*ss_sig;			/* result signature */
} str_sig_t;

/* MD5 Test Suite from RFC1321 */
static	str_sig_t	tests[] = {
  { "", "d41d8cd98f00b204e9800998ecf8427e" },
  { "a", "0cc175b9c0f1b6a831c399e269772661" },
  { "abc", "900150983cd24fb0d6963f7d28e17f72" },
  { "message digest", "f96b697d7cb7938d525a2f31aaf161d0" },
  { "abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b" },
  { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
    "d174ab98d277d9f5a5611c2c9f419d9f" },
  { "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "57edf4a22be3c955ac49da2e2107b67a" },
  
  { "This string is precisely 56 characters long for a reason", "93d268e9bef6608ff1a6a96adbeee106" },
  { "This string is exactly 64 characters long for a very good reason", "655c37c2c8451a60306d09f2971e49ff" },
  { "This string is also a specific length.  It is exactly 128 characters long for a very good reason as well. We are testing bounds.", "2ac62baa5be7fa36587c55691c026b35" },
  { NULL }
};

#define HEX_STRING	"0123456789abcdef"
unsigned char	sig[MD5_SIZE];

void md5_sig_to_string(void *signature, char *str, const int str_len)
{
	unsigned char *sig_p;
	char *str_p, *max_p;
	unsigned int high, low;

	str_p = str;
	max_p = str + str_len;

	for (sig_p = (unsigned char *)signature;
	sig_p < (unsigned char *)signature + MD5_SIZE;
	sig_p++) {
		high = *sig_p / 16;
		low = *sig_p % 16;
		/* account for 2 chars */
		if (str_p + 1 >= max_p) {
			break;
		}
		*str_p++ = HEX_STRING[high];
		*str_p++ = HEX_STRING[low];
	}
	/* account for 2 chars */
	if (str_p < max_p) {
		*str_p++ = '\0';
	}
}

void call_md5(char *test_p,int len) {
	md5_buffer_58(test_p,len,sig);
}

/*
 * A test driver for the routine we want to analyze
 */
void measure_md5() {
	char str[33];
	str_sig_t *test_p = &tests[4];
	int len = strlen(test_p->ss_string);
	
	flush_icache_blocking();
	
	MEASURE_START();
	
	call_md5(test_p->ss_string,len);
	
	MEASURE_STOP();
	
	md5_sig_to_string(sig, str, sizeof(str));
	
	if (strcmp(str, test_p->ss_sig)) 
		printf("ERROR: Sig for '%s' is '%s' not '%s'\n", test_p->ss_string, test_p->ss_sig, str);
	else
		printf("Sig correct\n"),
	
	printf("md5 estimated: %ld [incl.function call]\n", ELAPSED_CYCLES());
}

void measure_md5_2() {
	str_sig_t *test_p = &tests[8];
	for (int i = 1; i <= 56; i++)
	{
		flush_icache_blocking();
	
		MEASURE_START();
	
		call_md5(test_p->ss_string,i);
	
		MEASURE_STOP();
		
		printf("%d;%ld\n", i, ELAPSED_CYCLES());
	}
}

/*
 * The main entry point 
 */
int main(int argc, char ** argv)
{
  init_benchmarks(1);
  measure_md5();
  measure_md5_2();
}
