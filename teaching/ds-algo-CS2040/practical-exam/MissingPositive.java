import java.util.*;
public class MissingPositive {

    public int findMissingPositive(int[] A){
        int n = A.length;
        for (int i = 0; i < n; i++) {
            if (A[i] <= 0 || A[i] > n) {
                A[i] = n + 1;
            }
        }

        for (int i = 0; i < n; i++) {
            int x = Math.abs(A[i]);
            if (x == n + 1) continue;
            A[x - 1] = -Math.abs(A[x - 1]);
        }

        for (int i = 0; i < n; i++) {
            if (A[i] > 0) {
                return i + 1;
            }
        }
        return n + 1;
    }
}

