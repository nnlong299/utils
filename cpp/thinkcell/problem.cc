#include <map>
#include <iostream>
#include <string>
#include <type_traits>
#include <mutex>

template<typename K, typename V>
class interval_map {
    friend void IntervalMapTest();
    V m_valBegin;
    std::map<K,V> m_map;

public:
    // constructor associates whole range of K with val
    template<typename V_forward>
    interval_map(V_forward&& val)
    : m_valBegin(std::forward<V_forward>(val)) {}

    // Assign value val to interval [keyBegin, keyEnd).
    // Overwrite previous values in this interval.
    template<typename V_forward>
    void assign(K const& keyBegin, K const& keyEnd, V_forward&& val)
        requires (std::is_same<std::remove_cvref_t<V_forward>, V>::value)
    {
        std::once_flag flag;
        std::call_once(flag, [&]()
        {
            m_map.insert(m_map.end(), std::make_pair(std::numeric_limits<K>::lowest(), m_valBegin));
        });

        if (!(keyBegin < keyEnd)) return; // Empty range, do nothing

        // Step 1: Adjust the interval map to start at keyBegin
        auto itLow = m_map.lower_bound(keyBegin);
        if (itLow != m_map.begin() && std::prev(itLow)->second == val) {
            --itLow;
        } else {
            itLow = m_map.emplace_hint(itLow, keyBegin, std::prev(itLow)->second);
        }

        // Step 2: Adjust the interval map to end at keyEnd
        auto itHigh = m_map.lower_bound(keyEnd);
        if (itHigh == m_map.end() || itHigh->first != keyEnd) {
            m_map[keyEnd] = std::prev(itHigh)->second;
        }

        // Step 3: Erase all intervals between keyBegin and keyEnd
        m_map.erase(std::next(itLow), m_map.lower_bound(keyEnd));

        // Step 4: Insert the new interval value
        itLow->second = val;
    }

    // look-up of the value associated with key
    V const& operator[](K const& key) const {
        auto it = m_map.upper_bound(key);
        if (it == m_map.begin()) {
            return m_valBegin;
        } else {
            return (--it)->second;
        }
    }

    void print() const {
        for (auto&& [key, val] : m_map) {
            std::cout << "[" << key << ':' << val << "]";
        }
        std::cout << '\n';
    }
};

void testIntervalMap() {
    interval_map<int, char> map{ 'a' };
    map.print(); // [-2147483648:a]

    map.assign(3, 5, 'b');
    map.print(); // [-2147483648:a][3:b][5:a]

    map.assign(2, 3, 'c');
    map.print(); // [-2147483648:a][2:c][3:b][5:a]

    map.assign(2, 3, 'd');
    map.print(); // [-2147483648:a][2:d][3:b][5:a]

    map.assign(2, 4, 'e');
    map.print(); // [-2147483648:a][2:e][4:b][5:a]
}

int main() {
    testIntervalMap();
    return 0;
}
