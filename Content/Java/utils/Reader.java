import java.io.*;
import java.math.*;
import java.util.*;

public class Reader {
  private BufferedReader in; private StringTokenizer st;
  public Reader(InputStream inputStream) {
    in = new BufferedReader(new InputStreamReader(inputStream));
  }
  public Reader(String fileName) throws FileNotFoundException {
    in = new BufferedReader(new FileReader(fileName));
  }
  public String next() throws IOException {
    while (st == null || !st.hasMoreTokens())
      st = new StringTokenizer(in.readLine().trim());
    return st.nextToken();
  }
  public String nextLine() throws IOException {
    st = null; return in.readLine();
  }
  public BigInteger nextBigInteger() throws IOException {
    return new BigInteger(next());
  }
  public byte nextByte() throws IOException {
    return Byte.parseByte(next());
  }
  public char nextChar() throws IOException { return next().charAt(0); }
  public double nextDouble() throws IOException {
    return Double.parseDouble(next());
  }
  public float nextFloat() throws IOException {
    return Float.parseFloat(next());
  }
  public int nextInt() throws IOException {
    return Integer.parseInt(next());
  }
  public long nextLong() throws IOException {
    return Long.parseLong(next());
  }
  public short nextShort() throws IOException {
    return Short.parseShort(next());
  }
  public void close() throws IOException { in.close(); }
}
