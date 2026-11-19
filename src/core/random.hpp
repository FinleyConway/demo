#pragma once

#include <random>
#include <utility>
#include <cstdint>
#include <type_traits>

namespace demo {
    class random {
    private:
        // stops the compiler deducing T from the arguments, so random::Float(0, 5)
        // uses the default (float) instead of deducing int and failing.
        template <typename T>
        using no_deduce = typename std::common_type<T>::type;

    public:
        random() = delete;

    public:
        static void seed(uint64_t seed) {
            engine().seed(seed);
        }

        template <typename T = int>
        static T integer(no_deduce<T> min, no_deduce<T> max) {
            static_assert(
                std::is_integral_v<T> && !std::is_same_v<T, bool>,
                "random::int requires an integer type (not bool)"
            );
    
            if (min > max) std::swap(min, max);
    
            // uniform_int_distribution doesn't support 8-bit types, so widen them.
            using Dist = std::conditional_t<(sizeof(T) < sizeof(short)), int, T>;
            
            return static_cast<T>(
                std::uniform_int_distribution<Dist>(
                    static_cast<Dist>(min),
                    static_cast<Dist>(max))(engine()
                )
            );
        }
        
        template <typename T = float>
        static T decimal() {
            return decimal<T>(T(0), T(1));
        }

        template <typename T = float>
        static T decimal(no_deduce<T> min, no_deduce<T> max) {
            static_assert(
                std::is_floating_point_v<T>,
                "random::decimal requires a floating point type"
            );
    
            if (min > max) std::swap(min, max);

            return std::uniform_real_distribution<T>(min, max)(engine());
        }

        template <typename T = float>
        static bool chance(no_deduce<T> probability) {
            return decimal<T>() < probability;
        }

    private:
        static std::mt19937_64& engine() {
            static std::mt19937_64 engine{std::random_device{}()};
            return engine;
        }
    };  
}