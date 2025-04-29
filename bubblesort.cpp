#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

// Sequential Odd-Even Bubble Sort
void s_bubble(int* a, int n) {
    for (int i = 0; i < n; i++) {
        int first = i % 2;
        for (int j = first; j < n - 1; j += 2) {
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
        }
    }
}

// Parallel Odd-Even Bubble Sort
void p_bubble(int* a, int n) {
    for (int i = 0; i < n; i++) {
        int first = i % 2;
        #pragma omp parallel for shared(a, first)
        for (int j = first; j < n - 1; j += 2) {
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
        }
    }
}

// Measure execution time of a function
string bench_traverse(function<void()> traverse_fn) {
    auto start = chrono::high_resolution_clock::now();
    traverse_fn();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    return to_string(duration.count());
}

// Function to print the array
void print_array(const vector<int>& arr) {
    for (int val : arr) {
        cout << val << " ";
    }
    cout << endl;
}

int main(int argc, const char** argv) {
    int n, rand_max;

    // Input array size and max value
    if (argc < 3) {
        cout << "Enter array length: ";
        cin >> n;
        cout << "Enter maximum random value: ";
        cin >> rand_max;
    } else {
        n = stoi(argv[1]);
        rand_max = stoi(argv[2]);
    }

    // Create and fill array
    vector<int> a(n), b(n);
    for (int& x : a) x = rand() % rand_max;
    b = a; // copy array

    cout << "\nGenerated random array of length " << n 
         << " with elements between 0 and " << rand_max << "\n";

    cout << "Array before sorting: ";
    print_array(a);  // Print original array

    // Sequential Sort
    string seq_time = bench_traverse([&] { s_bubble(a.data(), n); });
    cout << "Array after sequential sort: ";
    print_array(a);  // Print sorted array
    cout << "Sequential Bubble Sort execution time: " << seq_time << " ms\n";

    // Parallel Sort
    omp_set_num_threads(16);
    string par_time = bench_traverse([&] { p_bubble(b.data(), n); });
    cout << "Array after parallel sort: ";
    print_array(b);  // Print sorted array
    cout << "Parallel (16 threads) Bubble Sort execution time: " << par_time << " ms\n";

    return 0;
}
