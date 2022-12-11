#include "logging.hpp"

#include "varia/vcommon.hpp"
#include "varia/validation.hpp"
#include "kinc/log.h"
#include "kinc/system.h"

#include <string.h>
#include <stdio.h>

static char const * log_type_names[] =
{
	"Info ",
	"Warning ",
	"Error ",
	"Verbose ",
	"Engine ",
	"Game ",
	"ECS ",
	"Physics ",
	"Graphics ",
	"Input ",
	"Performance ",
};

//Allow all logs by default
static tec_logging_flags active_permitted = ~0;
//Exclude nothing by default
static tec_logging_flags active_exclusions = 0;
//Whether or not all log messages should be required to print detailed info
static bool tec_logging_force_verbose = false;

void tec_internal_log_append(char * buffer, size_t current_offset, char const * string)
{
	snprintf(buffer + current_offset, TEC_LOG_MAX_SUBBUFFER_LEN - current_offset, string);
}

void tec_internal_log_type(char * buffer, tec_logging_flags flags)
{
	size_t offset = 0;
	//Flags size = 64
	for_range_var(i, 64)
	{
		//Hack and skip info, warning, error, and verbose flags
		if (i < 4) continue;

		if (flags & (1ULL << i))
		{
			if (log_type_names[i])
			{
				tec_internal_log_append(buffer, offset, log_type_names[i]);
				offset += strlen(log_type_names[i]);
			}
		}
	}
}

void tec_internal_log_verbose(kinc_log_level_t level, tec_logging_flags flags, char const * format, char const * file, size_t line, va_list args)
{
	char buffer[TEC_LOG_MAX_BUFFER_LEN] = {0};
	char subbuf[TEC_LOG_MAX_SUBBUFFER_LEN] = {0};
	tec_internal_log_type(subbuf, flags);
	switch (level)
	{
		case KINC_LOG_LEVEL_INFO:
		{
			snprintf(buffer, TEC_LOG_MAX_BUFFER_LEN, "[Info: %0.5f]::[Class: %s]::[%s]::[File: %s]::[Line: %zu]", kinc_time(), subbuf, format, file, line);
			break;
		}
		case KINC_LOG_LEVEL_WARNING:
		{
			snprintf(buffer, TEC_LOG_MAX_BUFFER_LEN, "[WARNING: %0.5f]::[Class: %s]::[%s]::[File: %s]::[Line: %zu]", kinc_time(), subbuf, format, file, line);
			break;
		}
		case KINC_LOG_LEVEL_ERROR:
		{
			snprintf(buffer, TEC_LOG_MAX_BUFFER_LEN, "[ERROR: %0.5f]::[Class: %s]::[%s]::[File: %s]::[Line: %zu]", kinc_time(), subbuf, format, file, line);
			break;
		}
		default:
			ENSURE_UNREACHABLE("Incorrect logging level set. Did you forget to select an appropriate Info, Warning, or Error filter for this log message?");
	}

	char const * string = &buffer[0];
	kinc_log_args(level, string, args);
}

void tec_internal_log(kinc_log_level_t level, tec_logging_flags flags, char const * format, char const * file, size_t line, va_list args)
{
	char buffer[TEC_LOG_MAX_BUFFER_LEN];
	char subbuf[TEC_LOG_MAX_SUBBUFFER_LEN] = {0};
	tec_internal_log_type(subbuf, flags);
	switch (level)
	{
		case KINC_LOG_LEVEL_INFO:
		{
			snprintf(buffer, TEC_LOG_MAX_BUFFER_LEN, "[Info: %0.5f]::[Class: %s]::[%s]", kinc_time(), subbuf, format);
			break;
		}
		case KINC_LOG_LEVEL_WARNING:
		{
			snprintf(buffer, TEC_LOG_MAX_BUFFER_LEN, "[WARNING: %0.5f]::[Class: %s]::[%s]", kinc_time(), subbuf, format);
			break;
		}
		case KINC_LOG_LEVEL_ERROR:
		{
			snprintf(buffer, TEC_LOG_MAX_BUFFER_LEN, "[ERROR: %0.5f]::[Class: %s]::[%s]", kinc_time(), subbuf, format);
			break;
		}
		default:
			ENSURE_UNREACHABLE("Incorrect logging level set. Did you forget to select an appropriate Info, Warning, or Error filter for this log message?");
	}

	char const * string = &buffer[0];
	kinc_log_args(level, string, args);

}

void varia_log_set_filtering(tec_logging_flags flags, tec_logging_flags exclusions, bool force_verbose)
{
	active_permitted = flags; 
	active_exclusions = exclusions;
	tec_logging_force_verbose = force_verbose;
}

void tec_log_filtered(tec_logging_flags flags, char const * format, ...)
{
	va_list args;
	va_start(args, format);

	tec_logging_flags filtered_flags;

	//If flags contains any flag that's currently excluded, this log message
	//will not be output
	if (flags & active_exclusions)
	{
		filtered_flags = 0;
	} 
	else
	{
		filtered_flags = flags;
	}

	if (filtered_flags & active_permitted)
	{
		if (tec_logging_force_verbose || (flags & LOG_VERBOSE))
		{
			if (flags & LOG_INFO)
			{
				log_info_verbose(format, flags, args);
			}
			if (flags & LOG_WARNING)
			{
				log_warn_verbose(format, flags, args);
			}
			if (flags & LOG_ERROR)
			{
				log_error_verbose(format, flags, args);
			}
		}
		else
		{
			if (flags & LOG_INFO)
			{
				log_info(format, flags, args);
			}
			if (flags & LOG_WARNING)
			{
				log_warn(format, flags, args);
			}
			if (flags & LOG_ERROR)
			{
				log_error(format, flags, args);
			}
		}
	}

	va_end(args);
}
