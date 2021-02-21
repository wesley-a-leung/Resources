#pragma once
#include <bits/stdc++.h>

// Unsyncs the C++ and C std output streams and unties cin and cout to
//   speed up standard input and output
void unSyncUntie() {
  std::ios::sync_with_stdio(0); std::cin.tie(0); std::cout.tie(0);
}

// Functions for fast IO
namespace IO {
#define INTERACTIVE_INPUT 0
  template <class T> struct is_iterator {
    template <class U, typename std::enable_if<
        !std::is_convertible<U, const char *>::value, int>::type = 0>
    static constexpr auto has_indirection(int)
        ->decltype(*std::declval<U>(), bool()) {
      return true;
    }
    template <class>
    static constexpr bool has_indirection(long) { return false; }
    static constexpr bool value = has_indirection<T>(0);
  };
  constexpr const int _bufSize = 1 << 16, _maxNumLength = 128;
  char _inputBuf[_bufSize + 1], *_inputPtr = _inputBuf, _sign, _c, _last = -1;
  char *_tempInputBuf = nullptr, _tempOutputBuf[_maxNumLength];
  char _outputBuf[_bufSize], _numBuf[_maxNumLength], _fill = ' ';
  int _cur, _tempOutputPtr = 0, _outputPtr = 0, _cnt, _numPtr = 0;
  int _width = 0, _precision = 9;
  const char *_delimiter = " "; unsigned long long _precisionBase = 1000000000;
  FILE *_input = stdin, *_output = stdout, *_error = stderr;
#if INTERACTIVE_INPUT
  char _getchar() { return _last = getchar(); }
#else
  char _getchar() {
    if (!*_inputPtr)
      _inputBuf[fread(_inputPtr = _inputBuf, 1, _bufSize, _input)] = '\0';
    return _last = *_inputPtr++;
  }
#endif
  char _getcharskipr() {
    while (_getchar() == '\r');
    return _last;
  }
  template <class I> void _readSigned(I &x) {
    while ((x = _getchar()) <= ' ');
    if ((_sign = x == '-')) x = _getchar();
    for (x -= '0'; (_c = _getchar()) >= '0'; x = x * 10 + _c - '0');
    if (_sign) x = -x;
  }
  template <class UI> void _readUnsigned(UI &x) {
    while ((x = _getchar()) <= ' ');
    for (x -= '0'; (_c = _getchar()) >= '0'; x = x * 10 + _c - '0');
  }
  template <class F> void _readFloatingPoint(F &x) {
    for (x = 0; (_c = _getchar()) <= ' ';);
    if ((_sign = _c == '-')) _c = _getchar();
    if (_c >= '0')
      for (x = _c - '0'; (_c = _getchar()) >= '0'; x = x * 10 + _c - '0');
    if (_c == '.') {
      F _div = 1.0;
      for (; (_c = _getchar()) >= '0'; x += (_c - '0') / (_div *= 10));
    }
    if (_sign) x = -x;
  }
  void setLength(int x) {
    if (_tempInputBuf) delete[](_tempInputBuf);
    _tempInputBuf = new char[x + 1];
  }
  template <class I> typename std::enable_if<std::is_integral<I>::value
      && std::is_signed<I>::value>::type read(I &x) { _readSigned(x); }
  template <class UI> typename std::enable_if<std::is_integral<UI>::value
      && std::is_unsigned<UI>::value>::type read(UI &x) {
    _readUnsignedSigned(x);
  }
#if __SIZEOF_INT128__
  void read(__int128_t &x) { _readSigned(x); }
  void read(__uint128_t &x) { _readUnsigned(x); }
#endif
  template <class F>
  typename std::enable_if<std::is_floating_point<F>::value>::type read(F &x) {
    _readFloatingPoint(x);
  }
  void read(char &x) { while ((x = _getchar()) <= ' '); }
  void read(char *x) {
    _cur = 0; do { _c = _getchar(); } while (_c <= ' ');
    do { x[_cur++] = _c; } while ((_c = _getchar()) > ' ');
    x[_cur] = '\0';
  }
  void readln(char *x) {
    if (_last == '\r') _getcharskipr();
    for (_cur = 0; (_c = _getcharskipr()) != '\n' && _c; x[_cur++] = _c);
    x[_cur] = '\0';
  }
  void read(std::string &x) {
    if (!_tempInputBuf) assert(0);
    read(_tempInputBuf); x = std::string(_tempInputBuf, _cur);
  }
  void readln(std::string &x) {
    if (!_tempInputBuf) assert(0);
    readln(_tempInputBuf); x = std::string(_tempInputBuf, _cur);
  }
  template <class T> typename std::enable_if<
      is_iterator<typename T::iterator>::value>::type read(T &x);
  template <class T1, class T2>
  void read(std::pair<T1, T2> &x) { read(x.first); read(x.second); }
  template <class T> void read(std::complex<T> &x) {
    T _re, _im; read(_re); read(_im); x.real(_re); x.imag(_im);
  }
  template <class T, class ...Ts> void read(T &x, Ts &&...xs);
  template <class It>
  typename std::enable_if<is_iterator<It>::value>::type read(It st, It en) {
    for (It _i = st; _i != en; _i++) read(*_i);
  }
  template <class It, class ...Ts>
  typename std::enable_if<is_iterator<It>::value>::type read(
      It st, It en, Ts &&...xs) {
    read(st, en); read(std::forward<Ts>(xs)...);
  }
  template <class T> typename std::enable_if<
      is_iterator<typename T::iterator>::value>::type read(T &x) {
    for (auto &&_i : x) read(_i);
  }
  template <class T, class...Ts> void read(T &x, Ts &&...xs) {
    read(x); read(std::forward<Ts>(xs)...);
  }
  void setInput(FILE *file) { *(_inputPtr = _inputBuf) = 0; _input = file; }
  void setInput(const std::string &s) {
    *(_inputPtr = _inputBuf) = 0; _input = fopen(s.c_str(), "r");
  }
  int _flushBuf() {
    fwrite(_outputBuf, 1, _outputPtr, _output); return _outputPtr = 0;
  }
  void flush() { _flushBuf(); fflush(_output); }
  int _putchar(char x) {
    _outputBuf[_outputPtr == _bufSize ? _flushBuf() : _outputPtr] = x;
    return _outputPtr++;
  }
  void _writeTempBuf(char x) { _tempOutputBuf[_tempOutputPtr++] = x; }
  void _writeOutput() {
    for (int _i = 0; _i < _tempOutputPtr; _putchar(_tempOutputBuf[_i++]));
    _tempOutputPtr = 0;
  }
  void _fillBuf(int x) { for (int _i = 0; _i < x; _i++) _putchar(_fill); }
  void _flushNumBuf() { for (; _numPtr; _writeTempBuf(_numBuf[--_numPtr])); }
  void _flushTempBuf() {
    int _tempLen = _tempOutputPtr; _fillBuf(_width - _tempLen);
    _writeOutput(); _fillBuf(-_width - _tempLen);
  }
  void setPrecision(int x) {
    _precision = x; _precisionBase = 1;
    for (int _i = 0; _i < x; _i++, _precisionBase *= 10);
  }
  void setWidth(int x) { _width = x; }
  void setFill(char x) { _fill = x; }
  void setDelimiter(const char *x) { _delimiter = x; }
  void setDelimiter(const std::string &x) { _delimiter = x.c_str(); }
  void writeDelimiter() {
    for (const char *_p = _delimiter; *_p; _putchar(*_p++));
  }
  template <class T> void _writeNum(const T &x, int digits) {
    _cnt = 0;
    for (T _y = x; _y; _y /= 10, _cnt++) _numBuf[_numPtr++] = '0' + _y % 10;
    for (; _cnt < digits; _cnt++) _numBuf[_numPtr++] = '0';
    _flushNumBuf();
  }
  template <class F> void _writeFloatingPoint(const F &x) {
    unsigned long long _I = x, _F = (x - _I) * _precisionBase + F(0.5);
    if (_F >= _precisionBase) { _I++; _F = 0; }
    _writeNum(_I, 1); _writeTempBuf('.'); _writeNum(_F, _precision);
  }
  void write(const bool &x) {
    if (x) _writeTempBuf('1');
    else _writeTempBuf('0');
    _flushTempBuf();
  }
  void write(const char &x) { _writeTempBuf(x); _flushTempBuf(); }
  void write(const char *x) {
    int _slen = strlen(x); _fillBuf(_width - _slen);
    for (const char *_p = x; *_p; _putchar(*_p++));
    _fillBuf(-_width - _slen);
  }
  void write(const std::string &x) {
    _fillBuf(_width - int(x.length()));
    for (int _i = 0; _i < int(x.length()); _putchar(x[_i++]));
    _fillBuf(-_width - int(x.length()));
  }
  template <class I>
  typename std::enable_if<std::is_integral<I>::value
      && std::is_signed<I>::value>::type write(const I &x) {
    using UI = typename std::make_unsigned<I>::type;
    if (x < 0) { _writeTempBuf('-'); _writeNum(UI(-x), 1); }
    else { _writeNum(UI(x), 1); }
    _flushTempBuf();
  }
  template <class UI>
  typename std::enable_if<std::is_integral<UI>::value
      && std::is_unsigned<UI>::value >::type write(const UI &x) {
    _writeNum(x, 1); _flushTempBuf();
  }
  template <class F>
  typename std::enable_if<std::is_floating_point<F>::value>::type write(
      const F &x) {
    if (std::isnan(x)) write("NaN");
    else if (std::isinf(x)) write("Inf");
    else if (x < 0) { _writeTempBuf('-'); _writeFloatingPoint(-x); }
    else _writeFloatingPoint(x);
    _flushTempBuf();
  }
#if __SIZEOF_INT128__
  void write(const __int128_t &x) {
    if (x < 0) { _writeTempBuf('-'); _writeNum(__uint128_t(-x), 1); }
    else _writeNum(__uint128_t(x), 1);
    _flushTempBuf();
  }
  void write(const __uint128_t &x) { _writeNum(x, 1); _flushTempBuf(); }
#endif
  template <class T> typename std::enable_if<
      is_iterator<typename T::iterator>::value>::type write(const T &x);
  template <class T1, class T2> void write(const std::pair<T1, T2> &x) {
    write(x.first); writeDelimiter(); write(x.second);
  }
  template <class T> void write(const std::complex<T> &x) {
    write(x.real()); writeDelimiter(); write(x.imag());
  }
  template <class T, class ...Ts> void write(const T &x, Ts &&...xs);
  template <class It>
  typename std::enable_if<is_iterator<It>::value>::type write(It st, It en) {
    bool _first = 1; for (It _i = st; _i != en; _i++) {
      if (_first) _first = 0;
      else writeDelimiter();
      write(*_i);
    }
  }
  template <class It, class ...Ts>
  typename std::enable_if<is_iterator<It>::value>::type write(
      It st, It en, Ts &&...xs) {
    write(st, en); writeDelimiter(); write(std::forward<Ts>(xs)...);
  }
  template <class T> typename std::enable_if<
      is_iterator<typename T::iterator>::value>::type write(const T &x) {
    bool _first = 1; for (auto &&_i : x) {
      if (_first) _first = 0;
      else writeDelimiter();
      write(_i);
    }
  }
  template <class T, class ...Ts> void write(const T &x, Ts &&...xs) {
    write(x); writeDelimiter(); write(std::forward<Ts>(xs)...);
  }
  void writeln() { _putchar('\n'); }
  template <class ...Ts>
  void writeln(Ts &&...xs) { write(std::forward<Ts>(xs)...); _putchar('\n'); }
  struct IOManager {
    ~IOManager() { flush(); if (_tempInputBuf) delete[](_tempInputBuf); }
  };
  std::unique_ptr<IOManager> iomanager = std::unique_ptr<IOManager>(
      new IOManager());
  void setOutput(FILE *file) { flush(); _output = file; }
  void setOutput(const std::string &s) {
    flush(); _output = fopen(s.c_str(), "w");
  }
  template <class ...Ts> void debug(Ts &&...xs) {
    FILE *_temp = _output; setOutput(_error);
    write(std::forward<Ts>(xs)...); setOutput(_temp);
  }
  template <class...Ts> void debugln(Ts &&...xs) {
    FILE *_temp = _output; setOutput(_error);
    writeln(std::forward<Ts>(xs)...); setOutput(_temp);
  }
  void setError(FILE *file) { flush(); _error = file; }
  void setError(const std::string &s) {
    flush(); _error = fopen(s.c_str(), "w");
  }
#undef INTERACTIVE_INPUT
}
