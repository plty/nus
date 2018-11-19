import java.util.*;
public class MissingPositive {

    public int findMissingPositive(int[] A){
        int n = A.length;

        // filtering all of the unneeded number
        // and making all of them positive at the same time
        for (int i = 0; i < n; i++) {
            if (A[i] <= 0 || A[i] > n) {
                A[i] = n + 1;
            }
        }

        for (int i = 0; i < n; i++) {
            // the actual value of this cell
            int x = Math.abs(A[i]);

            // aint dealing with nonsense
            if (x == n + 1) {
                continue;
            }

            // set the flag to "True" in this case (negative)
            A[x - 1] = -Math.abs(A[x - 1]);
        }

        // looking through the list which number never occur
        for (int i = 0; i < n; i++) {
            if (A[i] > 0) {
                return i + 1;
            }
        }

        // it would be n + 1 if every number in range of  1..n
        // occured in the list
        return n + 1;
    }
}

