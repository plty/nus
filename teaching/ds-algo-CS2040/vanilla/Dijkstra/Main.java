import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int v = in.nextInt();
        int e = in.nextInt();
        int s = in.nextInt();
        Dijkstra dijkstra = new Dijkstra(v);
        for (int i = 0; i < e; i++) {
            int p = in.nextInt();
            int q = in.nextInt();
            int w = in.nextInt();
            dijkstra.addEdge(p, q, w);
        }

        ArrayList<Integer> distance = dijkstra.shortestPath(s);
        for (Integer d: distance)
            System.out.println(d == -1 ? "INF" : d);
    }
}
