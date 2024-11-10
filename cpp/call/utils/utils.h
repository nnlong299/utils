#include <array>
#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace Utils
{
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
}
