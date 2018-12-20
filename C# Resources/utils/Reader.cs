using System;
using System.Globalization;
using System.IO;

namespace Utils {
    public class Reader {
        private TextReader reader;
        private string[] tokens;
        private int currentToken;

        public Reader(Stream s) {
            reader = new StreamReader(s);
            tokens = new string[0];
            currentToken = 0;
        }

        public Reader(string file) {
            reader = new StreamReader(file);
            tokens = new string[0];
            currentToken = 0;
        }

        public string ReadLine() { return reader.ReadLine();  }

        public string ReadToken() {
            while (currentToken == tokens.Length) {
                tokens = ReadLine().Split(new[] { ' ', '\t', }, StringSplitOptions.RemoveEmptyEntries);
                currentToken = 0;
            }
            return tokens[currentToken++];
        }

        public char ReadChar() { return ReadToken()[0]; }
        public int ReadInt() { return int.Parse(ReadToken()); }
        public long ReadLong() { return long.Parse(ReadToken()); }
        public double ReadDouble() { return double.Parse(ReadToken(), CultureInfo.InvariantCulture); }
        public decimal ReadDecimal() { return decimal.Parse(ReadToken(), CultureInfo.InvariantCulture); }
        public void Close() { reader.Close(); }
    }
}
