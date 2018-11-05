import java.util.*;

public class Main {
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		int n, m, q, s;
		n = in.nextInt();
		m = in.nextInt();
		q = in.nextInt();
		s = in.nextInt();

		while (n + m + q + s > 0) {
			Dijkstra dijkstra = new Dijkstra(n);
			for (int i = 0; i < m; i++) {
				int u = in.nextInt();
				int v = in.nextInt();
				int w = in.nextInt();
				dijkstra.addEdge(u, v, w);
			}

			ArrayList<Integer> distance = dijkstra.shortestPath(s);
			for (int i = 0; i < q; i++) {
				int t = in.nextInt();
				System.out.println(distance.get(t) == -1 ? "Impossible" : distance.get(t));
			}
			System.out.println();

			n = in.nextInt();
			m = in.nextInt();
			q = in.nextInt();
			s = in.nextInt();
		}
	}
}
