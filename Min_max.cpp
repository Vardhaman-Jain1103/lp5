#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

void s_avg(int arr[], int n) {
    long sum = accumulate(arr, arr + n, 0L);
    cout << "Average: " << sum / long(n) << "\n";
}

void p_avg(int arr[], int n) {
    long sum = 0L;
    #pragma omp parallel for reduction(+ : sum) num_threads(16)
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    cout << "Average: " << sum / long(n) << "\n";
}

void s_sum(int arr[], int n) {
    long sum = accumulate(arr, arr + n, 0L);
    cout << "Sum: " << sum << "\n";
}

void p_sum(int arr[], int n) {
    long sum = 0L;
    #pragma omp parallel for reduction(+ : sum) num_threads(16)
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    cout << "Sum: " << sum << "\n";
}

void s_max(int arr[], int n) {
    int max_val = *max_element(arr, arr + n);
    cout << "Max: " << max_val << "\n";
}

void p_max(int arr[], int n) {
    int max_val = INT_MIN;
    #pragma omp parallel for reduction(max : max_val) num_threads(16)
    for (int i = 0; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    cout << "Max: " << max_val << "\n";
}

void s_min(int arr[], int n) {
    int min_val = *min_element(arr, arr + n);
    cout << "Min: " << min_val << "\n";
}

void p_min(int arr[], int n) {
    int min_val = INT_MAX;
    #pragma omp parallel for reduction(min : min_val) num_threads(16)
    for (int i = 0; i < n; i++) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }
    cout << "Min: " << min_val << "\n";
}

int main() {
    int n, rand_max;

    // Prompt user for input
    cout << "Enter array length: ";
    cin >> n;
    cout << "Enter maximum random value: ";
    cin >> rand_max;

    // Allocate and initialize array with random values
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        a[i] = rand() % rand_max;
    }

    vector<int> b = a;  // Copy array a to b

    cout << "Generated random array of length " << n << " with elements between 0 to " << rand_max << "\n\n";

    // Sequential and parallel function calls with formatted output
    auto start = chrono::high_resolution_clock::now();
    cout << "Sequential Min: ";
    s_min(a.data(), n);
    auto stop = chrono::high_resolution_clock::now();
    cout << "Execution time: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " ms\n";
    
    start = chrono::high_resolution_clock::now();
    cout << "Parallel (16) Min: ";
    p_min(a.data(), n);
    stop = chrono::high_resolution_clock::now();
    cout << "Execution time: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " ms\n";
    cout << "=============================\n";

    start = chrono::high_resolution_clock::now();
    cout << "Sequential Max: ";
    s_max(a.data(), n);
    stop = chrono::high_resolution_clock::now();
    cout << "Execution time: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " ms\n";
    
    start = chrono::high_resolution_clock::now();
    cout << "Parallel (16) Max: ";
    p_max(a.data(), n);
    stop = chrono::high_resolution_clock::now();
    cout << "Execution time: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " ms\n";
    cout << "=============================\n";

    start = chrono::high_resolution_clock::now();
    cout << "Sequential Sum: ";
    s_sum(a.data(), n);
    stop = chrono::high_resolution_clock::now();
    cout << "Execution time: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " ms\n";
    
    start = chrono::high_resolution_clock::now();
    cout << "Parallel (16) Sum: ";
    p_sum(a.data(), n);
    stop = chrono::high_resolution_clock::now();
    cout << "Execution time: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " ms\n";
    cout << "=============================\n";

    start = chrono::high_resolution_clock::now();
    cout << "Sequential Average: ";
    s_avg(a.data(), n);
    stop = chrono::high_resolution_clock::now();
    cout << "Execution time: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " ms\n";
    
    start = chrono::high_resolution_clock::now();
    cout << "Parallel (16) Average: ";
    p_avg(a.data(), n);
    stop = chrono::high_resolution_clock::now();
    cout << "Execution time: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " ms\n";
    cout << "=============================\n";

    return 0;
}
