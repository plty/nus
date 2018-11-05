import java.util.*;

public class Kruskal {
    private UnionFind uf;
    private ArrayList<Triplet> edges;
    private int size;

    public Kruskal(int size) {
        this.size = size;
        edges = new ArrayList<>();
        uf = new UnionFind(size);
    }

    public void addEdge(int u, int v, int w) {
        edges.add(new Triplet(u, v, w));
    }

    public ArrayList<Triplet> getMinimumSpanningTree() {
        ArrayList<Triplet> mst = new ArrayList<>();
        edges.sort((a, b) -> a.w - b.w);
        for (Triplet edge: edges) {
            if (uf.same(edge.u, edge.v))
                continue;
            mst.add(edge);
            uf.union(edge.u, edge.v);
        }
        return mst;
    }
}

class Triplet {
    public int u;
    public int v;
    public int w;

    public Triplet(int u, int v, int w) {
        this.u = u;
        this.v = v;
        this.w = w;
    }

    public String toString() {
        return "("
                + Integer.toString(u) + ", "
                + Integer.toString(v) + ", "
                + Integer.toString(w) + ")";
    }
}
