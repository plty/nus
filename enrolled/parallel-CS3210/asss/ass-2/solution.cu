#include "hash.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdint.h>
#include <cstring>
using namespace std;

__managed__ bool found;
__managed__ unsigned long long ans;
__managed__ unsigned long long target;

// in byte 
__managed__ size_t prefix_length;

long long nativesGetTimeInSeconds() {
    return time(NULL);
}

void check_cuda_errors() {
    cudaError_t rc;
    rc = cudaGetLastError();
    if (rc != cudaSuccess) {
        printf("Last CUDA error %s\n", cudaGetErrorString(rc));
    }

}

__global__ void solve(uint8_t *prefix_byte_string, unsigned long long init) {
    unsigned long long idx = init + threadIdx.x;
    uint8_t* byte_string;

    // Append idx to byte_string
    size_t sz = (prefix_length + sizeof(long long)) * sizeof(uint8_t);
    byte_string = (uint8_t*) malloc(sz);
    memcpy(byte_string, prefix_byte_string ,prefix_length);

    uint8_t* suffix = byte_string + prefix_length;
    for(int i = 0 ; i < 8 ; i ++) {
        suffix[7 - i] = idx >> (i * 8);
    }

    // Compute hash
    uint8_t result[32];
    sha256(result, byte_string, prefix_length + sizeof(long long));

    // Check for result
    unsigned long long prefix_value = 0;
    for(int i = 0 ; i < 8 ; i ++) {
        prefix_value <<= 8;
        prefix_value |= result[i];
    }

    if(prefix_value < target) {
        found = true;
        atomic_exch(&ans, idx);
    }

    free(byte_string);
}

int main() {
    unsigned long long cur_time = nativesGetTimeInSeconds();
    vector<uint8_t> byte_string;

    // Add time;
    vector<uint8_t> tmp;
    for(int i = 0 ; i < 64 ; i += 8) {
        tmp.push_back(cur_time);
        cur_time >>= 8;
    }
    reverse(tmp.begin(), tmp.end());
    byte_string.insert(byte_string.end(), tmp.begin(), tmp.end());

    string digest; cin >> digest;

    // Add previous digest
    for (int i = 0; i < digest.size() ; i += 2) {
        uint8_t p;
        sscanf(digest.c_str() + i, "%2x", &p);
        byte_string.push_back(p);
    }

    const string nusnet_id = "gganteng";

    // Add previous digest
    for (int i = 0; i < nusnet_id.size(); i += 2) {
        uint8_t p;
        sscanf(nusnet_id.c_str() + i, "%2x", &p);
        byte_string.push_back(p);
    }


    // Get target
    cin >> target;


    check_cuda_errors();

    // Copy precomputed prefix to device
    uint8_t* prefix_byte_string;
    prefix_length = byte_string.size();
    cudaMalloc((void**)&prefix_byte_string, prefix_length);

    check_cuda_errors();


    uint8_t x[byte_string.size()];
    for (int i = 0; i < byte_string.size(); i ++) {
        x[i] = byte_string[i];
    }
    cudaMemcpy(prefix_byte_string, x, prefix_length, cudaMemcpyHostToDevice );

    long long blockSize = 64;

    // coba coba
    found = false;

    for(long long  i = 0; i <= ULLONG_MAX ; i += blockSize) {
        // cout << "Trying with anchor: " << i << endl;
        solve<<<1, blockSize>>>(prefix_byte_string, i);
        cudaDeviceSynchronize();
        check_cuda_errors();
        if(found) {
            cout << "KETEMU COI " << ans << endl;
            break;
        }
    }

    return 0;
}
