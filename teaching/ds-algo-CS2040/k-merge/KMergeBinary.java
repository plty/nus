import java.util.Scanner;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.PriorityQueue;

public class KMergeBinary {
    /**
     * merge an array full of sorted LinkedList into a new sorted LinkedList
     * @param lists the ArrayList of LinkedList
     */
    public static LinkedList<Integer> 
            join(ArrayList<LinkedList<Integer>> lists) {
        return join(lists, 0, lists.size());
    }

    /**
     * merge an array of sorted LinkedList from index l to r 
     * into a new sorted LinkedList.
     * @param lists the ArrayList of LinkedList
     * @param l left-most index (inclusive)
     * @param r right-most index (exclusive)
     */
    private static LinkedList<Integer> join(
            ArrayList<LinkedList<Integer>> lists, int l, int r) {
        if (r == l) return new LinkedList<>();
        if (r - l == 1) 
            return lists.get(l);

        int piv = (l + r) / 2;
        return merge(join(lists, l, piv), join(lists, piv, r));
    }

    /** Merge two sorted LinkedList into a new sorted LinkedList. */
    public static LinkedList<Integer> merge(
            LinkedList<Integer> left, LinkedList<Integer> right){
        // this is the same as the merging part of mergesort
        int size = left.size() + right.size();
        LinkedList<Integer> result = new LinkedList<>();
        left.add(1 << 30); right.add(1 << 30);
        for (int i = 0; i < size; i++) {
            if (left.getFirst() < right.getFirst()) {
                result.add(left.remove());
            } else {
                result.add(right.remove());
            }
        }
        return result;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int k = in.nextInt();
        ArrayList<LinkedList<Integer>> lists = new ArrayList<>();
        for (int i = 0; i < k; i++) {
            lists.add(new LinkedList<>());
            int len = in.nextInt();
            for (int j = 0; j < len; j++) {
                lists.get(i).add(in.nextInt());
            }
        }
        in.close();
        System.out.println(join(lists));
    }
}
