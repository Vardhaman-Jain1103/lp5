// To run:
// g++ -fopenmp merge_sort.cpp -o merge_sort
// ./merge_sort 50 20

#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

auto start = chrono::high_resolution_clock::now();
auto stop = chrono::high_resolution_clock::now();
auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

void p_mergesort(int *a, int i, int j);
void s_mergesort(int *a, int i, int j);
void merge(int *a, int i1, int j1, int i2, int j2);

void p_mergesort(int *a, int i, int j) {
    if (i < j) {
        int mid;
        if ((j - i) > 1000) {
            mid = (i + j) / 2;
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

void s_mergesort(int *a, int i, int j) {
    int mid;
    if (i < j) {
        mid = (i + j) / 2;
        s_mergesort(a, i, mid);
        s_mergesort(a, mid + 1, j);
        merge(a, i, mid, mid + 1, j);
    }
}

void merge(int *a, int i1, int j1, int i2, int j2) {
    int size = j2 - i1 + 1;
    int* temp = new int[size]; // dynamically allocate memory

    int i = i1, j = i2, k = 0;

    while (i <= j1 && j <= j2) {
        if (a[i] < a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }

    while (i <= j1) temp[k++] = a[i++];
    while (j <= j2) temp[k++] = a[j++];

    for (i = i1, k = 0; i <= j2; i++, k++) {
        a[i] = temp[k];
    }

    delete[] temp; // free memory
}

string bench_traverse(function<void()> traverse_fn) {
    auto start = chrono::high_resolution_clock::now();
    traverse_fn();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    return to_string(duration.count());
}

int main(int argc, const char **argv) {
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

    int *a = new int[n];
    int *b = new int[n];

    for (int i = 0; i < n; i++) {
        a[i] = rand() % rand_max;
    }

    copy(a, a + n, b);

    cout << "Generated random array of length " << n 
         << " with elements between 0 and " << rand_max << "\n\n";

    // Sequential Merge Sort
    cout << "Sequential merge sort: " 
         << bench_traverse([&] { s_mergesort(a, 0, n-1); }) 
         << "ms\n";

    cout << "Sorted array is ready =>\n";
    // Uncomment if you want to print the sorted array
    for (int i = 0; i < n; i++) cout << a[i] << ", ";
    cout << "\n\n";

    omp_set_num_threads(16);  // Set number of threads

    // Parallel Merge Sort
    cout << "Parallel (16) merge sort: " 
         << bench_traverse([&] { parallel_mergesort(b, 0, n-1); }) 
         << "ms\n";

    // Uncomment if you want to print the parallel sorted array
    cout << "Sorted array is =>\n";
    for (int i = 0; i < n; i++) cout << b[i] << ", ";

    delete[] a;
    delete[] b;

    return 0;
}
