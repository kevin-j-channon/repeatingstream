#pragma once

#include <boost/iostreams/concepts.hpp>
#include <utility>
#include <algorithm>

namespace repeatstream
{
    template<typename Buffer_T>
    class SrcFromBuffer : public boost::iostreams::source {
    public:
        explicit source(Buffer_T&& buf, size_t repeat_count)
            : m_buf{ std::move(buf) }
            , m_current{ m_buf.cbegin() }
            , m_remaining_repeats{ repeat_count }
        {
        }

        std::streamsize read(char_type* s, std::streamsize n)
        {
            const auto remainig = std::distance(m_current, m_buf.cend());
            const auto read_count = std::streamsize{ std::min(n, remaining) };
            const auto over_run = m_remaining_repeats > 0 ? n - read_count : 0;

            if (read_count > 0) {
                it_current = std::copy_n(it_current, read_count, s);
                if (over_run > 0) {
                    --m_remaining_repeats;
                    m_current = m)buf.cbegin();

                    read_count += this->read(s + read_count, over_run);
                }

                return read_count;
            }
            else {
                return -1; // EOF
            }
        }

    private:

        Buffer_T m_buf;
        Buffer_T::const_iterator m_current;
        size_t m_remaining_repeats;
    };
}
