package utils;

import java.io.IOException;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.math.BigInteger;
import java.util.StringTokenizer;

public class Reader {
    private BufferedReader in;
    private StringTokenizer st;

    public Reader(InputStream inputStream) { in = new BufferedReader(new InputStreamReader(inputStream)); }

    public Reader(String fileName) throws FileNotFoundException { in = new BufferedReader(new FileReader(fileName)); }

    public String next() throws IOException {
        while (st == null || !st.hasMoreTokens()) {
            st = new StringTokenizer(in.readLine().trim());
        }
        return st.nextToken();
    }

    public String next(String delim) throws IOException {
        while (st == null || !st.hasMoreTokens()) {
            st = new StringTokenizer(in.readLine().trim());
        }
        return st.nextToken(delim);
    }
    
    public String nextLine() throws IOException {
        st = null;
        return in.readLine();
    }

    // public BigInteger nextBigInteger() throws IOException { return new BigInteger(next()); }

    public byte nextByte() throws IOException { return Byte.parseByte(next()); }
    public byte nextByte(String delim) throws IOException { return Byte.parseByte(next(delim)); }
    public char nextChar() throws IOException { return next().charAt(0); }
    public char nextChar(String delim) throws IOException { return next(delim).charAt(0); }
    public double nextDouble() throws IOException { return Double.parseDouble(next()); }
    public double nextDouble(String delim) throws IOException { return Double.parseDouble(next(delim)); }
    public float nextFloat() throws IOException { return Float.parseFloat(next()); }
    public float nextFloat(String delim) throws IOException { return Float.parseFloat(next(delim)); }
    public int nextInt() throws IOException { return Integer.parseInt(next()); }
    public int nextInt(String delim) throws IOException { return Integer.parseInt(next(delim)); }
    public long nextLong() throws IOException { return Long.parseLong(next()); }
    public long nextLong(String delim) throws IOException { return Long.parseLong(next(delim)); }
    public short nextShort() throws IOException { return Short.parseShort(next()); }
    public short nextShort(String delim) throws IOException { return Short.parseShort(next(delim)); }
    public void close() throws IOException { in.close(); }
} // Reader class
