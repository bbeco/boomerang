#pragma region License
/*
 * This file is part of the Boomerang Decompiler.
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 */
#pragma endregion License
#pragma once


#include "boomerang/util/Interval.h"

#include <map>
#include <cassert>

/**
 * A map that maps intervals of Key types to Value types.
 * Intervals may overlap each other.
 */
template<typename Key, typename Value>
class IntervalMap
{
public:
    typedef typename std::map<Interval<Key>, Value> Data;

    typedef typename Data::iterator iterator;
    typedef typename Data::const_iterator const_iterator;
    typedef typename Data::reverse_iterator reverse_iterator;
    typedef typename Data::const_reverse_iterator const_reverse_iterator;

public:
    IntervalMap() {}
    ~IntervalMap() = default;

    /// \returns true if the map does not contain any elements.
    bool isEmpty() const { return m_data.empty(); }

    /// Remove all elements from this map.
    void clear() { m_data.clear(); }

    /// Inserts an interval with a mapped value into this map.
    iterator insert(const Interval<Key>& key, const Value& value)
    {
        std::pair<typename Data::iterator, bool> p = m_data.insert({ key, value });
        return p.second ? p.first : m_data.end();
    }

    iterator insert(const Key& lower, const Key& upper, const Value& value)
    {
        return insert(Interval<Key>(lower, upper), value);
    }

    /**
     * Finds the mapped value at \p key.
     * If there are muliple candidate intervals,
     * the interval with the lowest lower bound is retrieved.
     */
    const_iterator find(const Key& key) const
    {
        // todo: speed up
        for (const_iterator it = begin(); it != end(); it++) {
            const Key& lower = it->first.lower();
            const Key& upper = it->first.upper();

            if (upper <= key) {
                continue;
            }
            else if (lower > key) {
                break;
            }
            else {
                assert(it->first.isContained(key));
                return it;
            }
        }
        return end();
    }

    iterator find(const Key& key)
    {
        // todo: speed up
        for (iterator it = begin(); it != end(); it++) {
            const Key& lower = it->first.lower();
            const Key& upper = it->first.upper();

            if (upper <= key) {
                continue;
            }
            else if (lower > key) {
                break;
            }
            else {
                assert(it->first.isContained(key));
                return it;
            }
        }
        return end();
    }

    /**
     * \returns an iterator range containing all intervals between \p lower and \p upper.
     * If there are no intervals between lower and upper, the function returns (end(), end).
     */
    std::pair<const_iterator, const_iterator> equalRange(const Key& lower, const Key& upper) const
    {
        return equalRange(Interval<Key>(lower, upper));
    }

    std::pair<const_iterator, const_iterator> equalRange(const Interval<Key>& interval) const
    {
        const_iterator itLower = end();
        const_iterator itUpper = end();

        // todo: speed up
        for (const_iterator it = begin(); it != end(); it++) {
            if (it->first.upper() > interval.lower() && itLower == end()) {
                itLower = it;
            }

            if (itLower != end() && it->first.lower() <= interval.upper()) {
                itUpper = it;
            }
        }

        return std::make_pair(itLower, itUpper);
    }


    std::pair<iterator, iterator> equalRange(const Key& lower, const Key& upper)
    {
        return equalRange(Interval<Key>(lower, upper));
    }

    std::pair<iterator, iterator> equalRange(const Interval<Key>& interval)
    {
        iterator itLower = end();
        iterator itUpper = end();

        // todo: speed up
        for (iterator it = begin(); it != end(); it++) {
            if (it->first.upper() > interval.lower() && itLower == end()) {
                itLower = it;
            }

            if (itLower != end() && it->first.lower() <= interval.upper()) {
                itUpper = it;
            }
        }

        return std::make_pair(itLower, itUpper);
    }


    /// Erase the item referenced by \p it
    /// \returns an iterator to the element immediately after the deleted element
    iterator erase(iterator it) { return m_data.erase(it); }

    /// Remove all intervals containing \p key
    void eraseAll(const Key& key)
    {
        iterator it = find(key);
        if (it == end()) {
            return;
        }

        do {
            it = erase(it);
        } while (it != end() && it->first.isContained(key));
    }

    /// Remove all intervals overlapping with \p interval
    void eraseAll(const Interval<Key>& interval)
    {
        iterator it1, it2;
        std::tie(it1, it2) = equalRange(interval);

        while (it1 != it2) { // case it1 == it2 == end() accounted for
            it1 = erase(it1);
        }
    }

    iterator begin() { return m_data.begin(); }
    iterator end()   { return m_data.end(); }
    const_iterator begin() const { return m_data.begin(); }
    const_iterator end()   const { return m_data.end(); }
    reverse_iterator rbegin() { return m_data.rbegin(); }
    reverse_iterator rend()   { return m_data.rend();   }
    const_reverse_iterator rbegin() const { return m_data.rbegin(); }
    const_reverse_iterator rend() const { return m_data.rend(); }

private:
    std::map<Interval<Key>, Value, std::less<Interval<Key>>> m_data;
};
