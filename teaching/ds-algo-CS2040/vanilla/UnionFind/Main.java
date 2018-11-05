import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt();
        int q = in.nextInt();

        UnionFind uf = new UnionFind(n);

        for (int i = 0; i < q; i++) {
            int cmd = in.nextInt();
            int a = in.nextInt();
            int b = in.nextInt();
            if (cmd == 0)
                uf.union(a, b);
            if (cmd == 1)
                System.out.println(uf.same(a, b) ? 1 : 0);
        }
    }
}
