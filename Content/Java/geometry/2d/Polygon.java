import java.io.*;
import java.math.*;
import java.util.*;
import java.awt.geom.*;

// Functions with polygons using the Area object
// Functions:
//   makeArea(P): creates an Area from an array of points P 
//   union(areas): returns an Area that is the union of the Area
//     objects in areas
//   intersection(areas): returns an Area that is the intersection of the Area
//     objects in areas
//   getArea(area): returns the area of the Area object area
// Tested:
//   https://ecna17.kattis.com/problems/abstractart
//   https://codeforces.com/problemset/gymProblem/100952/J
public class Polygon {
  public static Area makeArea(double[][] P) {
    Path2D.Double path = new Path2D.Double();
    path.moveTo(P[0][0], P[0][1]);
    for (int i = 1; i < P.length; i++) path.lineTo(P[i][0], P[i][1]);
    path.closePath();
    return new Area(path);
  }
  public static Area union(Area[] areas) {
    Area ret = new Area(); for (Area area : areas) ret.add(area);
    return ret;
  }
  public static Area intersection(Area[] areas) {
    if (areas.length == 0) return new Area();
    Area ret = areas[0];
    for (int i = 1; i < areas.length; i++) ret.intersect(areas[i]);
    return ret;
  }
  public static double getArea(Area area) {
    PathIterator iter = area.getPathIterator(null); double ret = 0;
    double[] buf = new double[6]; ArrayList<double[]> P = new ArrayList<>();
    for (; !iter.isDone(); iter.next()) {
      switch (iter.currentSegment(buf)) {
        case PathIterator.SEG_MOVETO:
        case PathIterator.SEG_LINETO:
          P.add(new double[]{buf[0], buf[1]}); break;
        case PathIterator.SEG_CLOSE:
          for (int i = 0; i < P.size(); i++) {
            double[] a = P.get(i), b = P.get((i + 1) % P.size());
            ret -= a[0] * b[1] - a[1] * b[0];
          }
          P = new ArrayList<>(); break;
      }
    }
    return ret / 2;
  }
}
