#pragma once

/*=====================================================================//

		Dread Testing Framework
		For when you hate doing it but you just have to

//=====================================================================*/

constexpr int DREAD_MAX_TESTS = 512;
constexpr char const * DREAD_SUCCESS = "S";
constexpr char const * DREAD_FAILURE = "F";

using dread_test_func_t = char const *(*)(void);

struct dread_test_runner_t
{
	int _length;
	dread_test_func_t _tests[DREAD_MAX_TESTS];
	char const * _test_names[DREAD_MAX_TESTS];

	int _failed_tests_length;
	int _failed_tests_indices[DREAD_MAX_TESTS];
};

extern dread_test_runner_t _dread_tests_storage;

/* Returns -1 for success, TestID Number for first test failure
*/
void dread_push_test(char const * test_name, dread_test_func_t fn);
void DTEST(char const * test_name, dread_test_func_t lambda_fn);
int dread_run_tests(bool verbose);