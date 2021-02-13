import java.io.*
import java.math.*
import java.util.*

class Reader {
  private val In: BufferedReader
  private var st: StringTokenizer? = null
  constructor(inputStream: InputStream) {
    In = BufferedReader(InputStreamReader(inputStream))
  }
  constructor(fileName: String) {
    In = BufferedReader(FileReader(fileName))
  }
  fun next(): String {
    while (st == null || !st!!.hasMoreTokens())
      st = StringTokenizer(In.readLine().trim())
    return st!!.nextToken()
  }
  fun nextLine(): String {
    st = null
    return In.readLine()
  }
  fun nextChar(): Char = next()[0]
  fun nextDouble(): Double = next().toDouble()
  fun nextInt(): Int = next().toInt()
  fun nextLong(): Long = next().toLong()
  fun close(): Unit = In.close()
}
