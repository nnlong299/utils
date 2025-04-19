#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <queue>

// Partition function to rearrange elements around the pivot.
template <typename T>
typename T::iterator partition(typename T::iterator begin, typename T::iterator end, typename T::iterator pivot) {
    auto left = begin, right = end - 1;

    while (true) {
        // Move left iterator to the right past elements less than pivot
        while (*left < *pivot) {
            ++left;
        }
        // Move right iterator to the left past elements greater than pivot
        while (*right > *pivot) {
            --right;
        }

        // If both iterators haven't crossed, swap the elements
        if (left < right) {
            std::swap(*left, *right);
            ++left;
            --right;
        } else {
            return left; // Return the partition index
        }
    }
}

// Quickselect-like partitioning to find nth element
template <typename T>
void nth_element_impl(typename T::iterator begin, typename T::iterator end, typename T::iterator nth) {
    if (begin == end) return;
    
    auto pivot = begin + (end - begin) / 2; // Choose a pivot (middle element for simplicity)
    pivot = partition<T>(begin, end, pivot);
    
    if (pivot == nth) {
        return; // nth element is found
    } else if (pivot < nth) {
        nth_element_impl<T>(pivot, end, nth); // Look in the right half
    } else {
        nth_element_impl<T>(begin, pivot, nth); // Look in the left half
    }
}

// nth_element function
template <typename T>
void nth_element(T& arr, size_t n) {
    if (n < 0 || n >= arr.size()) {
        throw std::out_of_range("Index out of range");
    }

    nth_element_impl<T>(arr.begin(), arr.end(), arr.begin() + n);
}

void mostEffecient(std::vector<int> arr, int n)
{
    nth_element(arr, n - 1); // Find the 3rd largest element (index n-1)
    
    std::cout << "The " << n << "th element is: " << arr[n - 1] << std::endl;
} 

void secondEffect(std::vector<int> arr, int n)
{
    // Min-heap to store the n largest elements
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    for (int num : arr) {
        minHeap.push(num);
        if (minHeap.size() > n) {
            minHeap.pop();  // Remove the smallest element to keep only n largest
        }
    }

    // The n-th largest element is at the top of the min-heap
    std::cout << "The " << n << "-th largest element is: " << minHeap.top() << std::endl;
}

int main() {
    std::vector<int> arr = {12, 5, 787, 1, 23};
    size_t n = 3;
    mostEffecient(arr, 3);
    secondEffect(arr, 3);

    return 0;
}
