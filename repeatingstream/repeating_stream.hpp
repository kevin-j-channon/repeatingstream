#pragma once

#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/stream.hpp>

#include <utility>
#include <algorithm>


template<typename Buffer_T>
class RepeatingInputSource : public boost::iostreams::source
{
public:
	RepeatingInputSource() = delete;

	explicit RepeatingInputSource(Buffer_T&& buf, size_t repeat_count)
		: m_buf{ std::move(buf) }
		, m_current{ m_buf.cbegin() }
		, m_remaining_repeats{ repeat_count }
	{
	}

	RepeatingInputSource(const RepeatingInputSource& other)
		: m_buf{ other.m_buf }
		, m_current{ std::next(m_buf.cbegin(), std::distance(other.m_buf.cbegin(), other.m_current)) }
		, m_remaining_repeats{ other.m_remaining_repeats }
	{
	}

	RepeatingInputSource& operator=(const RepeatingInputSource& other)
	{
		m_buf = other.m_buf;
		m_current = std::next(m_buf.cbegin(), std::distance(other.m_buf.cbegin(), other.m_current));
		m_remaining_repeats = other.m_remaining_repeats;
	}

	RepeatingInputSource(RepeatingInputSource&&) noexcept = default;
	RepeatingInputSource& operator=(RepeatingInputSource&&) noexcept = default;

	std::streamsize read(char_type* s, std::streamsize n)
	{
		const auto remaining = std::distance(m_current, m_buf.cend());
		auto read_count = std::min<std::streamsize>(n, remaining);
		const auto over_run = m_remaining_repeats > 1 ? n - read_count : 0;

		if (read_count > 0) {
			auto new_s = std::copy_n(m_current, read_count, s);
			std::advance(m_current, read_count);

			_handle_any_overrun_by_wrapping(over_run);

			return read_count;
		}
		else {
			return -1; // EOF
		}
	}

private:
	void _handle_any_overrun_by_wrapping(size_t over_run)
	{
		if (over_run <= 0) {
			return;
		}

		--m_remaining_repeats;
		m_current = m_buf.cbegin();

		read_count += this->read(new_s, over_run);
	}

	Buffer_T m_buf;
	typename Buffer_T::const_iterator m_current;
	size_t m_remaining_repeats;
};

template<typename Buffer_T>
using RepeatingInputStream = boost::iostreams::stream<RepeatingInputSource<Buffer_T>>;
