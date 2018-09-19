import java.util.Arrays;
import java.util.TreeSet;

public class Market {
    // the fishmonger index is from 1 to n
    // this array stores which fishmonger sells which type of fish
    private int stall[];
    
    // this transition `Set`(Basically an AVL) stores every
    // fishmongers' index selling different kind of fish with the next guy.
    // as in, x is stored in the `Set` iff stall[x] != stall [x + 1]
    private TreeSet<Integer> transition;

    /** err quite trivial eh ? */
    public Market(int n) {
        transition = new TreeSet<>();
        transition.add(1 << 30);

        // i don't want to deal with OutOfBounds Exception okay.
        stall = new int[n + 2];
    }

    /** Change fishmonger k, is now selling v. */
    public void change(int k, int v) {
        // just remove and don't care.
        transition.remove(k - 1);
        transition.remove(k);
        
        stall[k] = v;
        // add it in again if you have to do so.
        if (stall[k] != stall[k - 1]) 
            transition.add(k - 1);
        if (stall[k + 1] != stall[k]) 
            transition.add(k);
    }

    /** basically the magical part of this problem. */
    public boolean same(int l, int r) {
        // set.ceiling(x) returns the smallest element >= x in the set
        return transition.ceiling(l) >= r;
    }
    
    @Override 
    public String toString() {
        return "Stalls:     " + Arrays.toString(stall) + "\n" +
               "Transition: " + transition + "\n";
    }
}
