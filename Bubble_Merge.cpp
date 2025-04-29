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

// Sequential Merge Sort
void merge(int *a, int i1, int j1, int i2, int j2) {
    int size = j2 - i1 + 1;
    int *temp = new int[size];
    int i = i1, j = i2, k = 0;

    while (i <= j1 && j <= j2)
        temp[k++] = (a[i] < a[j]) ? a[i++] : a[j++];
    while (i <= j1)
        temp[k++] = a[i++];
    while (j <= j2)
        temp[k++] = a[j++];

    for (i = i1, k = 0; i <= j2; i++, k++)
        a[i] = temp[k];

    delete[] temp;
}

void s_mergesort(int *a, int i, int j) {
    if (i < j) {
        int mid = (i + j) / 2;
        s_mergesort(a, i, mid);
        s_mergesort(a, mid + 1, j);
        merge(a, i, mid, mid + 1, j);
    }
}

// Parallel Merge Sort
void p_mergesort(int *a, int i, int j) {
    if (i < j) {
        int mid = (i + j) / 2;
        if ((j - i) > 1000) {
            #pragma omp task firstprivate(a, i, mid)
            p_mergesort(a, i, mid);
            #pragma omp task firstprivate(a, mid, j)
            p_mergesort(a, mid + 1, j);
            #pragma omp taskwait
            merge(a, i, mid, mid + 1, j);
        } else {
            s_mergesort(a, i, j);
        }
    }
}

void parallel_mergesort(int *a, int i, int j) {
    #pragma omp parallel num_threads(16)
    {
        #pragma omp single
        p_mergesort(a, i, j);
    }
}

// Measure execution time of a function
string bench_traverse(function<void()> traverse_fn) {
    auto start = chrono::high_resolution_clock::now();
    traverse_fn();
    auto stop = chrono::high_resolution_clock::now();
    return to_string(chrono::duration_cast<chrono::milliseconds>(stop - start).count());
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

    if (argc < 3) {
        cout << "Specify array length and maximum random value\n";
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

    // Sequential Bubble Sort
    string seq_bubble_time = bench_traverse([&] { s_bubble(a.data(), n); });
    cout << "Array after sequential bubble sort: ";
    print_array(a);  // Print sorted array
    cout << "Sequential Bubble Sort execution time: " << seq_bubble_time << " ms\n";

    // Parallel Bubble Sort
    string par_bubble_time = bench_traverse([&] { p_bubble(b.data(), n); });
    cout << "Array after parallel bubble sort: ";
    print_array(b);  // Print sorted array
    cout << "Parallel (16 threads) Bubble Sort execution time: " << par_bubble_time << " ms\n";

    // Sequential Merge Sort
    vector<int> a_for_merge = a;
    string seq_merge_time = bench_traverse([&] { s_mergesort(a_for_merge.data(), 0, n - 1); });
    cout << "Array after sequential merge sort: ";
    print_array(a_for_merge);  // Print sorted array
    cout << "Sequential Merge Sort execution time: " << seq_merge_time << " ms\n";

    // Parallel Merge Sort
    vector<int> b_for_merge = b;
    string par_merge_time = bench_traverse([&] { parallel_mergesort(b_for_merge.data(), 0, n - 1); });
    cout << "Array after parallel merge sort: ";
    print_array(b_for_merge);  // Print sorted array
    cout << "Parallel (16 threads) Merge Sort execution time: " << par_merge_time << " ms\n";

    return 0;
}
