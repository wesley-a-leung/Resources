package utils;

import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

public class FastReader {
    private final int BUFFER_SIZE = 1 << 16;
    private int LENGTH = -1;
    private DataInputStream din;
    private byte[] buffer;
    private int bufferPointer, bytesRead;

    public FastReader(InputStream inputStream) {
        din = new DataInputStream(inputStream);
        buffer = new byte[BUFFER_SIZE];
        bufferPointer = bytesRead = 0;
    }

    public FastReader(String file_name) throws IOException {
        din = new DataInputStream(new FileInputStream(file_name));
        buffer = new byte[BUFFER_SIZE];
        bufferPointer = bytesRead = 0;
    }

    public String nextLine() throws IOException {
        byte[] buf = new byte[LENGTH];
        int cnt = 0, c;
        while ((c = read()) != -1) {
            if (c == '\n' || c == '\r') break;
            buf[cnt++] = (byte) c;
        }
        return new String(buf, 0, cnt);
    }
    
    public byte nextByte() throws IOException {
        byte ret = 0;
        byte c = read();
        while (c <= ' ') c = read();
        boolean neg = (c == '-');
        if (neg) c = read();
        do {
            ret *= 10;
            ret += c - '0';
        } while ((c = read()) >= '0' && c <= '9');
        if (neg) return (byte) (-ret);
        return ret;
    }
    
    public short nextShort() throws IOException {
        short ret = 0;
        byte c = read();
        while (c <= ' ') c = read();
        boolean neg = (c == '-');
        if (neg) c = read();
        do {
            ret *= 10;
            ret += c - '0';
        } while ((c = read()) >= '0' && c <= '9');
        if (neg) return (short) (-ret);
        return ret;
    }

    public int nextInt() throws IOException {
        int ret = 0;
        byte c = read();
        while (c <= ' ') c = read();
        boolean neg = (c == '-');
        if (neg) c = read();
        do {
            ret = ret * 10 + c - '0';
        } while ((c = read()) >= '0' && c <= '9');
        if (neg) return -ret;
        return ret;
    }

    public long nextLong() throws IOException {
        long ret = 0;
        byte c = read();
        while (c <= ' ') c = read();
        boolean neg = (c == '-');
        if (neg) c = read();
        do {
            ret = ret * 10 + c - '0';
        } while ((c = read()) >= '0' && c <= '9');
        if (neg) return -ret;
        return ret;
    }
    
    public float nextFloat() throws IOException {
        float ret = 0, div = 1;
        byte c = read();
        while (c <= ' ') c = read();
        boolean neg = (c == '-');
        if (neg) c = read();
        do {
            ret = ret * 10 + c - '0';
        } while ((c = read()) >= '0' && c <= '9');
        if (c == '.') while ((c = read()) >= '0' && c <= '9') ret += (c - '0') / (div *= 10);
        if (neg) return -ret;
        return ret;
    }

    public double nextDouble() throws IOException {
        double ret = 0, div = 1;
        byte c = read();
        while (c <= ' ') c = read();
        boolean neg = (c == '-');
        if (neg) c = read();
        do {
            ret = ret * 10 + c - '0';
        } while ((c = read()) >= '0' && c <= '9');
        if (c == '.') while ((c = read()) >= '0' && c <= '9') ret += (c - '0') / (div *= 10);
        if (neg) return -ret;
        return ret;
    }

    public char nextChar() throws IOException {
        byte c;
        do {
            c = read();
        } while (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == -1);
        return (char) c;
    }

    public String next() throws IOException {
        byte[] buf = new byte[LENGTH];
        int cnt = 0, c;
        c = read();
        while (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == -1) c = read();
        buf[cnt++] = (byte) c;
        while ((c = read()) != -1) {
            if (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == -1) break;
            buf[cnt++] = (byte) c;
        }
        return new String(buf, 0, cnt);
    }
    
    public void setLength(int length) {
        LENGTH = length;
    }

    private void fillBuffer() throws IOException {
        bytesRead = din.read(buffer, bufferPointer = 0, BUFFER_SIZE);
        if (bytesRead == -1) buffer[0] = -1;
    }

    private byte read() throws IOException {
        if (bufferPointer == bytesRead) fillBuffer();
        return buffer[bufferPointer++];
    }

    public void close() throws IOException {
        if (din == null) return;
        din.close();
    }
}
