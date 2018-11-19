import java.io.*;
import java.math.*;
import java.util.*;

class Lectures {
    private static int calculateMinimumHalls(int N, int[] start, int[] end) {
    	return 0;
    }

    private static int calculateMinimumCancels(int N, int L, int[] start, int[] end) {
       return 0;
    }

    public static void main(String[] args) throws IOException {
        Reader sc = new Reader();
        int testCases = sc.nextInt();
        for (int tc = 0; tc < testCases; tc++) {
            int N = sc.nextInt();
            int L = sc.nextInt();
            int[] start = new int[N];
            for (int i = 0; i < N; i++) {
                start[i] = sc.nextInt();
            }
            int[] end = new int[N];
            for (int i = 0; i < N; i++) {
                end[i] = sc.nextInt();
            }
            int minHalls = calculateMinimumHalls(N, Arrays.copyOf(start, start.length), Arrays.copyOf(end, end.length));
            System.out.println(minHalls);
            int minCancels = calculateMinimumCancels(N, L, start, end);
            System.out.println(minCancels);
        }
    }

    private static class Reader {
        final private int BUFFER_SIZE = 1 << 16;
        private DataInputStream din;
        private byte[] buffer;
        private int bufferPointer, bytesRead;

        public Reader() {
            din = new DataInputStream(System.in);
            buffer = new byte[BUFFER_SIZE];
            bufferPointer = bytesRead = 0;
        }

        public int nextInt() throws IOException {
            int ret = 0;
            byte c = read();
            while (c <= ' ') c = read();
            do {
                ret = ret * 10 + c - '0';
            } while ((c = read()) >= '0' && c <= '9');
            return ret;
        }

        private void fillBuffer() throws IOException {
            bytesRead = din.read(buffer, bufferPointer = 0, BUFFER_SIZE);
            if (bytesRead == -1) buffer[0] = -1;
        }

        private byte read() throws IOException {
            if (bufferPointer == bytesRead) fillBuffer();
            return buffer[bufferPointer++];
        }
    }
}