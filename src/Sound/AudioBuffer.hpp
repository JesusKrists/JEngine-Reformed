#pragma once

#include <concepts>
#include <cstddef>
#include <span>

namespace JE
{

    /// Class referencing a block of audio buffer memory
    template<std::floating_point T>
    class AudioBufferView
    {
      public:
        /// From floating point span
        explicit AudioBufferView(std::span<T> samples)
            : m_Samples(samples)
        {
        }

        /// \brief Write samples to another buffer
        /// \param scale multiplied by (default 1)
        /// \param offset offset by (default 0)
        template<typename U>
        constexpr void WriteTo(std::span<U> destination, T scale = 1, T offset = 0)
        {
            auto len = std::min<std::size_t>(destination.size(), m_Samples.size());
            auto out = destination.data();
            for (std::size_t idx = 0; idx < len; idx++) {
                *out++ = static_cast<U>((m_Samples[idx] + offset) * scale);
            }
        }

      private:
        std::span<T> m_Samples;
    };

}  // namespace JE
