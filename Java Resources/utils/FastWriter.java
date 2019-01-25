package utils;

import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public class FastWriter {
    private final static int BUFFER_SIZE = 1 << 12;
    private final static int MAX_NUM_LENGTH = 1 << 7;
    private DataOutputStream dout;
    private byte[] buffer, numBuffer, tempBuffer;
    private int bufferPointer, numBufferPointer, tempBufferPointer, width, precision;
    private long precisionBase;
    private char fill;

    public FastWriter(OutputStream outputStream) {
        dout = new DataOutputStream(outputStream);
        buffer = new byte[BUFFER_SIZE];
        numBuffer = new byte[MAX_NUM_LENGTH];
        tempBuffer = new byte[MAX_NUM_LENGTH];
        bufferPointer = numBufferPointer = tempBufferPointer = 0;
        setWidth(0);
        setPrecision(6);
        setFill(' ');
    }

    public FastWriter(String fileName) throws IOException {
        dout = new DataOutputStream(new FileOutputStream(fileName));
        buffer = new byte[BUFFER_SIZE];
        numBuffer = new byte[MAX_NUM_LENGTH];
        tempBuffer = new byte[MAX_NUM_LENGTH];
        bufferPointer = numBufferPointer = tempBufferPointer = 0;
        setWidth(0);
        setPrecision(6);
        setFill(' ');
    }

    public void setWidth(int x) {
        width = x;
    }

    public void setPrecision(int x) {
        precision = x;
        precisionBase = 1;
        for (int i = 0; i < precision; i++) precisionBase *= 10;
    }

    public void setFill(char x) {
        fill = x;
    }

    public void print(boolean x) throws IOException {
        if (x) println("true");
        else println("false");
    }

    public void println(boolean x) throws IOException {
        print(x);
        write((byte) '\n');
    }


    public void print(char x) throws IOException {
        writeTempBuffer((byte) x);
        flushTempBuffer();
    }

    public void println(char x) throws IOException {
        print(x);
        write((byte) '\n');
    }

    public void print(int x) throws IOException {
        if (x < 0) {
            writeTempBuffer((byte) '-');
            x = -x;
        }
        int cnt = 0;
        for (; x > 0; x /= 10, cnt++) numBuffer[numBufferPointer++] = (byte) ('0' + x % 10);
        for (; cnt < 1; cnt++) numBuffer[numBufferPointer++] = (byte) '0';
        flushNumBuffer();
        flushTempBuffer();
    }

    public void println(int x) throws IOException {
        print(x);
        write((byte) '\n');
    }

    public void print(long x) throws IOException {
        if (x < 0) {
            writeTempBuffer((byte) '-');
            x = -x;
        }
        int cnt = 0;
        for (; x > 0; x /= 10, cnt++) numBuffer[numBufferPointer++] = (byte) ('0' + x % 10);
        for (; cnt < 1; cnt++) numBuffer[numBufferPointer++] = (byte) '0';
        flushNumBuffer();
        flushTempBuffer();
    }

    public void println(long x) throws IOException {
        print(x);
        write((byte) '\n');
    }

    public void print(double x) throws IOException {
        if (Double.isInfinite(x)) {
            println("Inf");
        } else if (Double.isNaN(x)) {
            println("NaN");
        } else {
            if (x < 0) {
                writeTempBuffer((byte) '-');
                x = -x;
            }
            long I = (long) x;
            long F = (long) ((x - I) * precisionBase + 0.5);
            if (F >= precisionBase) {
                I++;
                F = 0;
            }
            int cnt = 0;
            for (; I > 0; I /= 10, cnt++) numBuffer[numBufferPointer++] = (byte) ('0' + I % 10);
            for (; cnt < 1; cnt++) numBuffer[numBufferPointer++] = (byte) '0';
            flushNumBuffer();
            writeTempBuffer((byte) '.');
            cnt = 0;
            for (; F > 0; F /= 10, cnt++) numBuffer[numBufferPointer++] = (byte) ('0' + F % 10);
            for (; cnt < precision; cnt++) numBuffer[numBufferPointer++] = (byte) '0';
            flushNumBuffer();
            flushTempBuffer();
        }
    }

    public void println() throws IOException {
        write((byte) '\n');
    }

    public void println(double x) throws IOException {
        print(x);
        write((byte) '\n');
    }

    public void print(String x) throws IOException {
        fillBuffer(width - x.length());
        for (int i = 0; i < x.length(); i++) write((byte) x.charAt(i));
        fillBuffer(-width - x.length());
    }


    public void println(String x) throws IOException {
        print(x);
        write((byte) '\n');
    }

    public void flush() throws IOException {
        flushBuffer();
        dout.flush();
    }

    private void flushBuffer() throws IOException {
        dout.write(buffer, 0, bufferPointer);
        bufferPointer = 0;
    }

    private void flushNumBuffer() throws IOException {
        for (; numBufferPointer > 0; writeTempBuffer(numBuffer[--numBufferPointer]));
    }

    private void flushTempBuffer() throws IOException {
        int tempLen = tempBufferPointer;
        fillBuffer(width - tempLen);
        writeOutput();
        fillBuffer(-width - tempLen);
    }

    private void fillBuffer(int x) throws IOException {
        for (int i = 0; i < x; i++) write((byte) fill);
    }

    private void write(byte x) throws IOException {
        if (bufferPointer == buffer.length) flushBuffer();
        buffer[bufferPointer++] = x;
    }

    private void writeTempBuffer(byte x) throws IOException {
        tempBuffer[tempBufferPointer++] = x;
    }

    private void writeOutput() throws IOException {
        for (int i = 0; i < tempBufferPointer; i++) write(tempBuffer[i]);
        tempBufferPointer = 0;
    }

    private void close() throws IOException {
        flush();
        dout.close();
    }
}
