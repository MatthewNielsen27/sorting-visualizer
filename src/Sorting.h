#pragma once

#include <vector>

namespace Sorting {
namespace detail {

// note: this is loosely based on the c++ example from geeksforgeeks.
std::size_t partition(
    std::vector<std::uint8_t>& items,
    std::size_t lo,
    std::size_t hi
) {
    // For now, let's use 'hi' as the pivot.
    std::size_t pivot_i = hi;
    const auto  pivot_v = items[pivot_i];

    // Index of smaller element and Indicate
    // the right position of pivot found so far
    std::ptrdiff_t i = static_cast<std::ptrdiff_t>(lo);

    for (auto j = lo; j < hi; j++) {
        //If current element is smaller than the pivot
        if (items[j] < pivot_v) {
            // Increment index of smaller element
            std::swap(items[i], items[j]);
            i++;
        }
    }

    // Move the pivot
    std::swap(items[i], items[pivot_i]);

    return i;
}

template <typename Fn>
void quicksort(
    std::vector<std::uint8_t>& items,
    std::ptrdiff_t lo,
    std::ptrdiff_t hi,
    std::size_t depth,
    Fn on_epoch
) {
    // when low is less than high
    if (lo < hi) {
        // pi is the partition return index of pivot
        const auto pi = partition(items, lo, hi);

        on_epoch(items, lo, hi, depth);

        // Recursion Call
        // smaller element than pivot goes left and
        // higher element goes right
        quicksort(items, lo, pi - 1, depth + 1, on_epoch);
        quicksort(items, pi + 1, hi, depth + 1, on_epoch);
    }
}

} // namespace Sorting::detail

/// An implementation of the 'Bubble Sort' algorithm, which triggers a callback on its internal state after each epoch.
template <typename Fn>
void bubblesort(
    std::vector<std::uint8_t>& items,
    Fn on_epoch
) {
    for (auto i = 0; i < items.size(); ++i) {
        on_epoch(items);
        bool did_something = false;
        for (auto j = i+1; j < items.size(); ++j) {
            if (items[i] > items[j]) {
                std::swap(items[i], items[j]);
                did_something = true;
            }
        }

        if (!did_something) {
            break;
        }
    }

    assert(std::is_sorted(items.begin(), items.end()));
}

/// An implementation of the 'Quick Sort' algorithm, which triggers a callback on its internal state after each epoch.
template <typename Fn>
void quicksort(
    std::vector<std::uint8_t>& items,
    Fn on_epoch
) {
    detail::quicksort(
        items,
        0,
        items.size() - 1,
        0,
        on_epoch
    );

    assert(std::is_sorted(items.begin(), items.end()));
}

} // namespace Sorting
