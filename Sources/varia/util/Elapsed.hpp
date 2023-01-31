#pragma once

#include "varia/logging.hpp"
#include "varia/validation.hpp"
#include "kinc/system.h"


struct varia_elapsed_t
{
	double m_start = 0.0;
	double m_end = 0.0;
	bool expired = false;

	varia_elapsed_t(void)
	{
		this->m_start = kinc_time();
	}
	~varia_elapsed_t()
	{
		if (this->expired) return; 

		this->m_end = kinc_time();
		VARIA_LOG_FLOAT(m_end - m_start);
	}
};


void varia_elapsed_begin(varia_elapsed_t * self) 
{ 
	DEBUG_ENSURE_FALSE(self->expired, "This Elapsed has already been used. Create another one.");

	self->m_start = kinc_time(); 
}
double varia_elapsed_end(varia_elapsed_t * self) 
{ 
	DEBUG_ENSURE_FALSE(self->expired, "This Elapsed has already been used. Create another one.");

	self->expired = true; self->m_end = kinc_time(); return self->m_end - self->m_start; 
}
double varia_elapsed_delta(varia_elapsed_t * self) { return self->m_end - self->m_start; }
void varia_elapsed_reset(varia_elapsed_t * self) { self->expired = false; }
void varia_elapsed_log_time(varia_elapsed_t * self) { VARIA_LOG_FLOAT(varia_elapsed_delta(self)); }
void varia_elapsed_log_time_message(varia_elapsed_t * self, char const * str) { VARIA_QLOG("%s :: %f", str, varia_elapsed_delta(self)); }
void varia_elapsed_begin_and_log(varia_elapsed_t * self, char const * msg) { VARIA_QLOG(msg); varia_elapsed_begin(self); }
void varia_elapsed_end_and_log(varia_elapsed_t * self) { varia_elapsed_end(self); varia_elapsed_log_time(self); }
