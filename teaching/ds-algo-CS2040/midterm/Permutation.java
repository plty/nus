import java.util.*;

public class Permutation {
    private int[] v;
    private boolean[] used;
    private int size;

    Permutation(int k) {
        v = new int[k];
        used = new boolean[k]; 
        size = k;
    }

    void print() {
        print(0, size); 
    }

    void print(int n, int k) {
        if (n == k) {
            System.out.println(Arrays.toString(v));
            return;
        }
        
        for (v[n] = 0; v[n] < k; v[n]++) {
            if (used[v[n]]) continue; 
            used[v[n]] = true;
            print(n + 1, k);
            used[v[n]] = false;
        }
    }
}
