import java.util.*;

public class UnionFind {
    private int parent[];
    private int height[];

    public UnionFind(int size) {
        parent = new int[size];
        height = new int[size];
        for (int i = 0; i < size; i++)
            parent[i] = i;
    }

    public int set(int p) {
        if (parent[p] == p)
            return p;
        parent[p] = set(parent[p]);
        return parent[p];
    }

    public void union(int p, int q) {
        if (set(p) == set(q))
            return;

        p = set(p);
        q = set(q);

        if (height[p] < height[q]) {
            union(q, p);
            return;
        }

        if (height[p] == height[q])
            height[p]++;

        parent[q] = p;
    }

    public boolean same(int p, int q) {
        return set(p) == set(q);
    }

    public String toString() {
        return Arrays.toString(parent);
    }
}

