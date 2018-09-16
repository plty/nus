import java.util.Scanner;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.PriorityQueue;

public class Main {
    public static LinkedList<Integer> heapJoin(ArrayList<LinkedList<Integer>> lists) {
        // Creating PriorityQueue of LinkedList
        // The concensus is LinkedList a < LinkedList b is done by 
        //      comparing the first element of LinkedList
        // In Java comparator function is done by returning an integer, 
        //      given a, b return <0 if a < b, return >0 if a > b, return 0 iff a == b, 
        // That comparator is this lambda, (a, b) -> a.getFirst() - b.getFirst()
        // This is a lifehack, a quick and easy way to create a function / comparator.
        PriorityQueue<LinkedList<Integer>> pq = 
                new PriorityQueue<>((a, b) -> a.getFirst() - b.getFirst());

        LinkedList<Integer> result = new LinkedList<>();
        int size = 0;

        for (LinkedList<Integer> list : lists) {
            size += list.size();
            // adding sentinel value of +`inf` to make the merging easier
            list.add(1 << 30);
            // `offer` is pushing a value to the priority queue 
            pq.offer(list);
        }

        for (int i = 0; i < size; i++) {
            // `poll` is get the top value and pop
            LinkedList<Integer> top = pq.poll();
            // `remove` is get the left-most value and remove it
            result.add(top.remove());
            // `offer` is pushing a value to the priority queue 
            pq.offer(top);
        }
        return result;
    }

    /**
     * merge an array full of sorted LinkedList into a new sorted LinkedList
     * @param lists the ArrayList of LinkedList
     */
    public static LinkedList<Integer> dncJoin(ArrayList<LinkedList<Integer>> lists) {
        return dncJoin(lists, 0, lists.size());
    }

    /**
     * merge an array of sorted LinkedList from index l to r into a new sorted LinkedList.
     * @param lists the ArrayList of LinkedList
     * @param l left-most index (inclusive)
     * @param r right-most index (exclusive)
     */
    public static LinkedList<Integer> dncJoin(ArrayList<LinkedList<Integer>> lists, int l, int r) {
        if (r == l) return new LinkedList<>();
        if (r - l == 1) 
            return lists.get(l);

        int piv = (l + r) / 2;
        return merge(dncJoin(lists, l, piv), dncJoin(lists, piv, r));
    }

    /** Merge two sorted LinkedList into a new sorted LinkedList. */
    public static LinkedList<Integer> merge(LinkedList<Integer> left, LinkedList<Integer> right){
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
        int type = in.nextInt(), k = in.nextInt();
        ArrayList<LinkedList<Integer>> lists = new ArrayList<>();
        for (int i = 0; i < k; i++) {
            lists.add(new LinkedList<>());
            int len = in.nextInt();
            for (int j = 0; j < len; j++) {
                lists.get(i).add(in.nextInt());
            }
        }
        in.close();

        if (type == 0)
            System.out.println(dncJoin(lists));
        if (type == 1)
            System.out.println(heapJoin(lists));
    }
}
