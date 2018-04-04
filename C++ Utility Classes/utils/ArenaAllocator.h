#ifndef UTILS_ARENAALLOCATOR_H_
#define UTILS_ARENAALLOCATOR_H_

#include <bits/stdc++.h>
using namespace std;

const size_t MB = 200; // size in megabytes

// BUFFER_SIZE is the size of the buffer in bytes
// ID is the unique id of each Arena with the same size
template <size_t BUFFER_SIZE = MB << 20, uint64_t ID = ~0> class Arena {
private:
    static char buffer[];
    static size_t index;

public:
    template <typename T> struct Allocator {
    public:
        typedef T                 type;
        typedef type              value_type;
        typedef value_type*       pointer;
        typedef value_type const* const_pointer;
        typedef value_type&       reference;
        typedef value_type const& const_reference;
        typedef std::size_t       size_type;
        typedef std::ptrdiff_t    difference_type;

        Allocator() {}

        template <typename U> Allocator(const U&) {}

        // n is the number of elements to be allocated
        pointer allocate(size_type n) {
            index -= n * sizeof(value_type);
            index &= ~alignof(value_type);
            return (pointer) (buffer + index);
        }

        void deallocate(pointer, size_type) {}

        bool operator ==(const Allocator<value_type> &other) const {
            return false;
        }

        bool operator !=(const Allocator<value_type> &other) const {
            return true;
        }

        template <typename OtherAllocator>
        bool operator ==(const OtherAllocator &other) const {
            return false;
        }

        template <typename OtherAllocator>
        bool operator !=(const OtherAllocator &other) const {
            return true;
        }
    };

    static void reset() {
        index = BUFFER_SIZE;
    }
};

template <size_t BUFFER_SIZE, uint64_t ID> char Arena<BUFFER_SIZE, ID>::buffer[BUFFER_SIZE];
template <size_t BUFFER_SIZE, uint64_t ID> size_t Arena<BUFFER_SIZE, ID>::index = BUFFER_SIZE;

#endif /* UTILS_ARENAALLOCATOR_H_ */
