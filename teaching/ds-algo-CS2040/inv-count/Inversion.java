import java.util.Scanner;

public class Inversion {
    private int[] arr;

    public Inversion(int n) {
        arr = new int[n];
    }

    public void set(int k, int v) {
        arr[k] = v;
    }

    public int get(int k) {
        return arr[k];
    }

    public long countInversion() {
        return countInversion(0, arr.length);
    }

    /**
     * pre-cond  : 0 <= l < r <= arr.length
     * post-cond : this.arr is sorted
     * @param l index of the left (inclusive)
     * @param r index of the right (exclusive)
     * @return 2-inversion of the array
     */
    public long countInversion(int l, int r) {
        int size = (r - l) / 2;
        if (size == 0) return 0;

        long count = countInversion(l, l + size) + countInversion(l + size, r);
        int lsize = (l + size) - l;
        int rsize = r - (l + size);
        int[] left = new int[lsize + 1];
        int[] right = new int[rsize + 1];
        left[lsize] = 1 << 30;
        right[rsize] = 1 << 30;
        for (int i = 0; i < lsize; i++)
            left[i] = this.arr[l + i];
        for (int i = 0; i < rsize; i++)
            right[i] = this.arr[l + size + i];
        int ll = 0, rr = 0;
        for (int i = 0; i < r - l; i++) {
            if (left[ll] > right[rr])
                count += lsize - ll;
            if (left[ll] <= right[rr])
                this.arr[l + i] = left[ll++];
            else
                this.arr[l + i] = right[rr++];
        }
        return count;
    }

    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
        int n = s.nextInt();
        Inversion inv = new Inversion(n);
        for (int i = 0; i < n; i++)
            inv.set(i, s.nextInt());
        System.out.println(inv.countInversion());
    }
}
