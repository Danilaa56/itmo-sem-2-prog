//
// Created by danil on 20.05.2021.
//

//#include "lab4algo.h"

namespace lab4 {
    template <typename Iterator, typename Predicate>
    bool all_of(Iterator begin, Iterator end, Predicate cond) {
        while(begin != end) {
            if(!cond(*begin)) {
                return false;
            }
            begin++;
        }
        return true;
    }

    template <typename Iterator, typename Predicate>
    bool any_of(Iterator begin, Iterator end, Predicate cond) {
        while(begin != end) {
            if(cond(*begin)) {
                return true;
            }
            begin++;
        }
        return false;
    }

    template <typename Iterator, typename Predicate>
    bool none_of(Iterator begin, Iterator end, Predicate cond) {
        while(begin != end) {
            if(cond(*begin)) {
                return false;
            }
            begin++;
        }
        return true;
    }

    template <typename Iterator, typename Predicate>
    bool one_of(Iterator begin, Iterator end, Predicate cond) {
        bool found = false;
        while(begin != end) {
            if(cond(*begin)) {
                if(found)
                    return false;
                else
                    found = true;
            }
            begin++;
        }
        return found;
    }

    template <typename Iterator, typename Compare>
    bool is_sorted(Iterator begin, Iterator end, Compare cmp) {
        if(begin == end)
            return true;
        Iterator last = begin;
        begin++;
        while(begin != end) {
            if(cmp(*begin, *last)) {
                return false;
            }
            last = begin;
            begin++;
        }
        return true;
    }

    template <typename Iterator, typename Predicate>
    bool is_partitioned(Iterator begin, Iterator end, Predicate cond) {
        if(begin == end)
            return true;
        bool lastValue = cond(*begin);
        bool hasChanged = false;
        begin++;
        while(begin != end) {
            if(lastValue != cond(*begin)) {
                if(hasChanged)
                    return false;
                hasChanged = true;
                lastValue = !lastValue;
            }
            begin++;
        }
        return true;
    }

    template <typename Iterator, typename DataType>
    Iterator find_not(Iterator begin, Iterator end, DataType value) {
        while(begin != end) {
            if(*begin != value) {
                break;
            }
            begin++;
        }
        return begin;
    }

    template <typename Iterator, typename DataType>
    Iterator find_backward(Iterator begin, Iterator end, DataType value) {
        if(begin == end)
            return begin;
        for(auto iter = end-1; iter != begin; iter--) {
            if(*iter == value)
                return iter;
        }
        return begin;
    }

    template <typename ForwardIterator, typename BackwardIterator>
    bool is_palindrome(ForwardIterator begin, BackwardIterator end) {
        if(begin == end)
            return true;
        BackwardIterator postEnd = end;
        end--;
        while(begin != end && begin != postEnd) {
            if(*begin != *end)
                return false;
            begin++;
            end--;
            postEnd--;
        }
        return true;
    }
}