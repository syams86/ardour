/*
 * Copyright (C) 2015 Tim Mayberry <mojofunk@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef ARDOUR_DSP_LOAD_CALCULATOR_H
#define ARDOUR_DSP_LOAD_CALCULATOR_H

#include <stdint.h>
#include <cassert>
#include <algorithm>

#include "ardour/libardour_visibility.h"

namespace ARDOUR {

class LIBARDOUR_API DSPLoadCalculator {
public:
	DSPLoadCalculator()
	    : m_max_time_us(0)
	    , m_start_timestamp_us(0)
	    , m_stop_timestamp_us(0)
	    , m_dsp_load(0)
	{

	}

	void set_max_time_us(uint64_t max_time_us) {
		assert(max_time_us != 0);
		m_max_time_us = max_time_us;
	}

	int64_t get_max_time_us() const { return m_max_time_us; }

	void set_start_timestamp_us(int64_t start_timestamp_us)
	{
		m_start_timestamp_us = start_timestamp_us;
	}

	void set_stop_timestamp_us(int64_t stop_timestamp_us);

	int64_t elapsed_time_us()
	{
		return m_stop_timestamp_us - m_start_timestamp_us;
	}

	/**
	 * @return a decimal value between 0.0 and 1.0 representing the percentage
	 * of time spent between start and stop in proportion to the max expected time
	 * in microseconds(us).
	 */
	float get_dsp_load() const
	{
		if (m_dsp_load > m_max_time_us) {
			return 1.0f;
		}
		if (m_dsp_load < 0.0f) {
			return 0.0f;
		}
		return m_dsp_load;
	}

private: // methods
	int64_t max_timer_error () { return 4 * m_max_time_us; }

private: // data
	int64_t m_max_time_us;
	int64_t m_start_timestamp_us;
	int64_t m_stop_timestamp_us;
	float m_dsp_load;
};

} // namespace ARDOUR

#endif // ARDOUR_DSP_LOAD_CALCULATOR_H
