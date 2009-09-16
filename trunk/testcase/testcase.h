#define BEGIN_TESTCASE(testname) \
	static unsigned int testcase_passes = 0; \
	static unsigned int testcase_fails = 0; \
	static int multicond_result = 0; \
	static int yet_printed_fail_header = 0; \
	static int nconds = 0; \
	static void * temp = NULL; \
	static const char *tname = #testname; \
	char temp_testname[256] = {NULL}; \
	int main(int argc, char **argv) { \
		printf("starting testcase: %s\n\n", tname);

#define TEST(func_call, verify_call) \
	printf("test:%d :: %s :: ", __LINE__, #func_call); \
	if( verify_call ((func_call))) { \
		printf("pass\n"); \
		testcase_passes++; \
	} else { \
		printf("fail\n"); \
		testcase_fails++; \
	}

#define CHECK_EQU(func_call, objective_value) \
	printf("test:%d :: %s :: ", __LINE__, #func_call); \
	if((temp = (func_call)) == (objective_value)) { \
		printf("pass\n"); \
		testcase_passes++; \
	} else { \
		printf("fail\n"); \
		testcase_fails++; \
	}

#define CHECK_NEQU(func_call, objective_value) \
	printf("test:%d :: %s :: ", __LINE__, #func_call); \
	if((temp = (func_call)) != (objective_value)) { \
		printf("pass\n"); \
		testcase_passes++; \
	} else { \
		printf("fail\n"); \
		testcase_fails++; \
	}

#define TEST_CONDS(func_call) \
	printf("test:%d :: %s :: ", __LINE__, #func_call); \
	temp = (func_call); \
	strncpy(temp_testname, #func_call, 255); \
	multicond_result = 1;

#define TEST_CONDS_VOID(func_call) \
	printf("test:%d :: %s :: ", __LINE__, #func_call); \
	strncpy(temp_testname, #func_call, 255); \
	(func_call); \
	multicond_result = 1;

#define COND(stmt) \
	nconds++; \
	multicond_result &= (stmt); \
	if(!(stmt)) { \
		if(!yet_printed_fail_header) { \
			yet_printed_fail_header = 1; \
			printf("fail\n  ***failure details***\n"); \
		} \
		printf("  condition %d failed:%d: %s\n", nconds, __LINE__, #stmt); \
	}

#define END_TEST_CONDS() \
	if(multicond_result) { \
		printf("pass\n"); \
		testcase_passes++; \
	} else { \
		testcase_fails++; \
	} \
	multicond_result = 0; \
	*temp_testname = '\0'; \
	nconds = 0; \
	yet_printed_fail_header = 0;

#define UNALLOC_TEMP() \
	if(temp) free(temp); \
	temp = NULL;

#define END_OF_TESTCASE() \
	printf("\ntestcase %s complete\npasses: %u\nfails: %u\n", tname, testcase_passes, testcase_fails); \
	return 0; \
	}


