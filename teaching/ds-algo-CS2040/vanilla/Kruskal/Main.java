import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int v = in.nextInt();
        int e = in.nextInt();
        Kruskal kruskal = new Kruskal(v);
        for (int i = 0; i < e; i++) {
            int a = in.nextInt();
            int b = in.nextInt();
            int w = in.nextInt();

            kruskal.addEdge(a, b, w);
        }

        ArrayList<Triplet> mst = kruskal.getMinimumSpanningTree();
        int sum = 0;
        for (Triplet edge: mst)
            sum += edge.w;
        System.out.println(sum);
    }
}
