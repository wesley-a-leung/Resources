package stdlib;

import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

public class DataInputStreamReader {
	private final int BUFFER_SIZE = 1 << 16;
	private int MAX_LENGTH = 64;
	private DataInputStream din;
	private byte[] buffer;
	private int bufferPointer, bytesRead;

	public DataInputStreamReader(InputStream inputStream) {
		din = new DataInputStream(inputStream);
		buffer = new byte[BUFFER_SIZE];
		bufferPointer = bytesRead = 0;
	}
	
	public DataInputStreamReader(InputStream inputStream, int length) {
		din = new DataInputStream(inputStream);
		buffer = new byte[BUFFER_SIZE];
		bufferPointer = bytesRead = 0;
		MAX_LENGTH = length;
	}

	public DataInputStreamReader(String file_name) throws IOException {
		din = new DataInputStream(new FileInputStream(file_name));
		buffer = new byte[BUFFER_SIZE];
		bufferPointer = bytesRead = 0;
	}
	
	public DataInputStreamReader(String file_name, int length) throws IOException {
		din = new DataInputStream(new FileInputStream(file_name));
		buffer = new byte[BUFFER_SIZE];
		bufferPointer = bytesRead = 0;
		MAX_LENGTH = length;
	}

	public String nextLine() throws IOException {
		byte[] buf = new byte[MAX_LENGTH];
		int cnt = 0, c;
		while ((c = read()) != -1) {
			if (c == '\n' || c == '\r') break;
			buf[cnt++] = (byte) c;
		}
		return new String(buf, 0, cnt);
	}

	public int nextInt() throws IOException {
		int ret = 0;
		byte c = read();
		while (c <= ' ')
			c = read();
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
		while (c <= ' ')
			c = read();
		boolean neg = (c == '-');
		if (neg) c = read();
		do {
			ret = ret * 10 + c - '0';
		} while ((c = read()) >= '0' && c <= '9');
		if (neg) return -ret;
		return ret;
	}

	public double nextDouble() throws IOException {
		double ret = 0, div = 1;
		byte c = read();
		while (c <= ' ')
			c = read();
		boolean neg = (c == '-');
		if (neg) c = read();

		do {
			ret = ret * 10 + c - '0';
		} while ((c = read()) >= '0' && c <= '9');

		if (c == '.') {
			while ((c = read()) >= '0' && c <= '9') {
				ret += (c - '0') / (div *= 10);
			}
		}

		if (neg) return -ret;
		return ret;
	}
	
	public char nextChar() throws IOException {
		int c = read();
		while (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == -1) {
			c = read();
		}
		return (char) c;
	}
	
	public String next() throws IOException {
		byte[] buf = new byte[MAX_LENGTH];
		int cnt = 0, c;
		c = read();
		while(c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == -1) {
			c = read();
		}
		buf[cnt++] = (byte) c;
		while ((c = read()) != -1) {
			if (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == -1) break;
			buf[cnt++] = (byte) c;
		}
		return new String(buf, 0, cnt);
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
