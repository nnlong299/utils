#include <algorithm>
#include <atomic>
#include <array>
#include <condition_variable>
#include <cstdint>
#include <exception>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <mutex>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <shared_mutex>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "definitions.h"

namespace Utils
{
    // enum to the underlying value
    template <typename E>
    constexpr auto to_underlying(E e) noexcept
    {
        return static_cast<std::underlying_type_t<E>>(e);
    }

    class StringCreator
    {
    public:
        template<typename ... Args>
        static std::string to_string(Args... args)
        {
            StringCreator s;
            s.doConCat(args...);
            return s.m_stream.str();
        }

    private:
        StringCreator() = default;
    
        template <typename T, typename ... Args>
        void doConCat(const T& obj, Args... args)
        {
            stream(obj);
            doConCat(args...);
        }
    
        template <typename T>
        void doConCat(const T& obj)
        {
            stream(obj);
        }
    
        template <typename T>
        void stream(const T& obj)
        {
            m_stream << obj;
        }
    
        void stream(const signed char& obj)
        {
            stream(+obj);
        }
    
        void stream (const unsigned char& obj)
        {
            stream(+obj);
        }
    
        template <typename T>
        void stream(const std::vector<T>& vector)
        {
            streamCollection(vector);
        }
    
        template <typename T>
        void stream(const std::set<T>& set)
        {
            streamCollection(set);
        }
    
        template <typename T, size_t N>
        void stream(const std::array<T, N>& array)
        {
            streamCollection(array);
        }
    
        template <typename Key, typename Value>
        void stream(const std::map<Key, Value>& map)
        {
            streamCollection(map);
        }
    
        template <typename Key, typename Value>
        void stream(const std::pair<Key, Value>& pair)
        {
            m_stream << '{';
            stream(pair.first);
            m_stream << ',';
            stream(pair.second);
            m_stream << '}';
        }
    
        template<typename Collection>
        void streamCollection(const Collection& collection)
        {
            m_stream << '{';
            std::string separator;
            for (const auto & value : collection)
            {
                m_stream << separator;
                stream(value);
                separator = ',';
            }
            m_stream << '}';
        }
    
        std::ostringstream m_stream;
    };

    // print text with color
    // default end = '\n'
    template<typename ... Args>
    void print(Definition::Color color, Args... args)
    {
        std::cout   << "\033[" << to_underlying(color) << "m"   // start color
                    << StringCreator::to_string(args...)        // text concat-ed
                    << Definition::ColorReset << '\n';          // end color with '\n'
    }

    // NOTE: do not mark static for this function, otherwise we hit data race
    template <typename Func, typename... Args>
    void time_elapsed(Func&& f, std::string_view description, Args&&... args)
    {
        auto begin = std::chrono::high_resolution_clock::now();

        // Invoke the function with forwarded arguments
        std::invoke(std::forward<Func>(f), std::forward<Args>(args)...);

        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Time difference ["<< description <<"]= " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    }
}

#define BLACK(...)      Utils::print(Definition::Color::Black,      __VA_ARGS__ )
#define RED(...)        Utils::print(Definition::Color::Red,        __VA_ARGS__ )
#define GREEN(...)      Utils::print(Definition::Color::Green,      __VA_ARGS__ )
#define YELLOW(...)     Utils::print(Definition::Color::Yellow,     __VA_ARGS__ )
#define BLUE(...)       Utils::print(Definition::Color::Blue,       __VA_ARGS__ )
#define MAGENTA(...)    Utils::print(Definition::Color::Magenta,    __VA_ARGS__ )
#define CYAN(...)       Utils::print(Definition::Color::Cyan,       __VA_ARGS__ )
#define WHITE(...)      Utils::print(Definition::Color::White,      __VA_ARGS__ )
