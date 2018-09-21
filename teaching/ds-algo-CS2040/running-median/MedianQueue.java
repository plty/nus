import java.util.Scanner;
import java.util.PriorityQueue;

public class MedianQueue {
    private int size = 0;
    private PriorityQueue<Integer> left = 
        new PriorityQueue<>((a, b) -> b - a);
    private PriorityQueue<Integer> right = 
        new PriorityQueue<>((a, b) -> a - b);

    public MedianQueue() {
        // yep, you guessed it, some inf sentinel. 
        // IMO, uni should teach more about picking sentinel.
        // I don't want to peek and catch or add an if, ew.
        int inf = 1 << 30;
        left.add(-inf);
        right.add(inf);
    }

    public void add(int x) {
        size++;

        // just add it to the right and think later
        right.add(x);

        // size balancing
        if (right.size() > left.size())
            left.add(right.remove());

        // assure middle value is the "median"
        if (left.peek() > right.peek()) {
            left.add(right.remove());
            right.add(left.remove());
        }
    }

    public double get() {
        double median = left.peek();
        median += left.size() > right.size() ? left.peek() : right.peek();
        return median / 2.0;
    }

    public static void main(String[] args) {
        MedianQueue mq = new MedianQueue();

        Scanner in = new Scanner(System.in);
        int n = in.nextInt();

        // one of the most weird way to iterate n times
        while (n-- > 0) {  
            int x = in.nextInt();
            mq.add(x);
            System.out.println(mq.get());
        }
        in.close();
    }
}
