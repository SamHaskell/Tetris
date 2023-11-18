#pragma once

#include "core/base.h"

#include <chrono>

namespace Utils {
    inline u32 HashPCG(u32 in) {
        u32 state = in * 747796405u + 2891336453u;
        u32 word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
        return (word >> 22u) ^ word;
    }

    class Clock {
        public:
            Clock() = default;
            ~Clock() = default;

            f64 Tick() {
                auto next = std::chrono::high_resolution_clock::now();
                f64 dt = std::chrono::duration<f64, std::chrono::seconds::period>(next - m_Now).count();
                m_Now = next;
                return dt;
            }

        private:
            std::chrono::steady_clock::time_point m_Now = std::chrono::high_resolution_clock::now();
    };
}