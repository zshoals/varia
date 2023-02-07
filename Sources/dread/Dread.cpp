#include "Dread.hpp"

#include "tests/Test_StringViews.hpp"

#include "kinc/log.h"

dread_test_runner_t _dread_tests_storage = {};


//Note(zshoals 02-06-2023):> Place all tests in this function
//Make sure to also ==INCLUDE YOUR TESTS== at the top of this file.
void dread_unitlocal_run_test_batch(void)
{
	test_strview_all_tests();
}

void dread_push_test(char const * test_name, dread_test_func_t fn)
{
	if (_dread_tests_storage._length < DREAD_MAX_TESTS)
	{
		_dread_tests_storage._tests[_dread_tests_storage._length] = fn;
		_dread_tests_storage._test_names[_dread_tests_storage._length] = test_name;

		++_dread_tests_storage._length;
	}
	else
	{
		//Note(zshoals 02-06-2023):> How did you run out of test spaces exactly?
		__debugbreak();
	}
}

void DTEST(char const * test_name, dread_test_func_t lambda_fn)
{
	dread_push_test(test_name, lambda_fn);
}

int dread_run_tests(void)
{
	dread_unitlocal_run_test_batch();

	int first_test_failed_id = -1;

	for (int i = 0; i < _dread_tests_storage._length; ++i)
	{
		char const * result = _dread_tests_storage._tests[i]();
		if (*result == 'S' || *result == 's')
		{
			//Success
		}
		else
		{
			int failed_len = _dread_tests_storage._failed_tests_length;
			_dread_tests_storage._failed_tests_indices[failed_len] = i;
			++_dread_tests_storage._failed_tests_length;

			if (first_test_failed_id == -1)
			{
				first_test_failed_id = i;
			}
		}
	}

	//We actually failed some tests, ONO!!!
	if (_dread_tests_storage._failed_tests_length > 0)
	{
		kinc_log(KINC_LOG_LEVEL_ERROR, "Tests Failed: %d", _dread_tests_storage._failed_tests_length);
		for (int i = 0; i < _dread_tests_storage._failed_tests_length; ++i)
		{
			int failed_test_idx = _dread_tests_storage._failed_tests_indices[i];
			kinc_log(KINC_LOG_LEVEL_ERROR, "Failed: %s", _dread_tests_storage._test_names[failed_test_idx]);
		}
		return first_test_failed_id;
	}
	else
	{
		//All tests passed
		kinc_log(KINC_LOG_LEVEL_INFO, "Tests Completed Successfully: %d", _dread_tests_storage._length);
		return -1;
	}
}
