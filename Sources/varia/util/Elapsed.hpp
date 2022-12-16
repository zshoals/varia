#pragma once

#include "varia/logging.hpp"
#include "varia/validation.hpp"
#include "kinc/system.h"

namespace Varia
{

struct Elapsed
{
	double m_start = 0.0;
	double m_end = 0.0;
	bool expired = false;

	void begin(void) 
	{ 
		DEBUG_ENSURE_FALSE(this->expired, "This Elapsed has already been used. Create another one.");

		this->m_start = kinc_time(); 
	}
	double end(void) 
	{ 
		DEBUG_ENSURE_FALSE(this->expired, "This Elapsed has already been used. Create another one.");

		this->expired = true; this->m_end = kinc_time(); return this->m_end - this->m_start; 
	}
	double dt(void) { return this->m_end - this->m_start; }
	void reset(void) { this->expired = false; }
	void log_time(void) { VARIA_LOG_FLOAT(this->dt()); }
	void log_time_message(char const * str) { VARIA_QLOG("%s :: %f", str, this->dt()); }
};

}