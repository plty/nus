import java.util.Scanner;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.PriorityQueue;

public class KMergeHeap {
    public static LinkedList<Integer> join(ArrayList<LinkedList<Integer>> lists) {
        // Creating PriorityQueue of LinkedList
        // The concensus is LinkedList a < LinkedList b is done by 
        //      comparing the first element of LinkedList
        // In Java comparator function is done by returning an integer, 
        //      given a, b return < 0 if a < b, return > 0 if a > b, return 0 iff a == b, 
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
        System.out.println(join(lists));
    }
}
