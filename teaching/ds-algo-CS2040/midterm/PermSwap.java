public class PermSwap {
    int[] arr;
    private int size;

    PermSwap(int k) {
        size = k;
        arr = new int[k];
        for (int i = 0; i < k; i++) {
            arr[i] = i;
        }
    }

    void print() {
        print(0); 
    }

    void print(int n) {
        if (n == size) {
            System.out.println(Arrays.toString(arr));
            return;
        }
        
        for (int i = n; i < size; i++) {
            swap(arr, n, i);
            print(n + 1);
            swap(arr, n, i);
        }
    }
    
    void swap(int[] a, int p, int q) {
        int temp = a[p];
        a[p] = a[q];
        a[q] = temp;
    }
}
