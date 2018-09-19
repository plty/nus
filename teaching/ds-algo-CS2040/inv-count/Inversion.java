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

        // counting inversion of [l ... l+size) and [l + size ... r)
        long count = countInversion(l, l + size) + countInversion(l + size, r);

        // calculate size needed for the array
        int lsize = (l + size) - l;
        int rsize = r - (l + size);

        // we set one extra space to append `inf` to the array
        int[] left = new int[lsize + 1];
        int[] right = new int[rsize + 1];

        // 1 << 30 is just a quick and dirty way to get "infinite"
        // as 1 << 30 is just above 1 Billion (1073741824 to be exact)
        left[lsize] = 1 << 30;
        right[rsize] = 1 << 30;

        // copying from this.arr into the respective array
        for (int i = 0; i < lsize; i++)
            left[i] = this.arr[l + i];
        for (int i = 0; i < rsize; i++)
            right[i] = this.arr[l + size + i];


        // ll is the left[] pointer and rr is the right[] pointer
        int ll = 0, rr = 0;
        for (int i = 0; i < r - l; i++) {
            // we don't have to do any bounding of ll and rr
            // the `inf` will handle those cases,
            // for example:
            //     left = [12, 44, 58, 200, inf]
            //     right = [inf]
            // the rr won't progress forward as any number will be
            // less than `inf`

            // count if there is jump
            if (left[ll] > right[rr])
                count += lsize - ll;

            // regular merging part of mergesort
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
