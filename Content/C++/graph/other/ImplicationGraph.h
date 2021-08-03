#pragma once
#include <bits/stdc++.h>
using namespace std;

// Functions for generating an implication graph
// Each function runs a callback on the created edges
// N variables will require a graph with N * 2 vertices, with vertex
//   a * 2 + 1 representing the affirmative for variable a, and a * 2
//   representing the negative for variable a

// Adds an implication (a -> b) with affA indicating if a is the affirmative
//   (if true) or the negative (if false), and similarly with b
// Template Arguments:
//   F: the type of f
// Functions Arguments:
//   affA: whether a is affirmative
//   a: the first variable
//   affB: whether b is affirmative
//   b: the second variable
//   f(i, j): the function to run a callback on for the corresponding edge
//     created by the implication
template <class F> void addImpl(bool affA, int a, bool affB, int b, F f) {
  f(a * 2 + affA, b * 2 + affB);
}

// Adds a disjunction (a | b) with affA indicating if a is the affirmative
//   (if true) or the negative (if false), and similarly with b
// Template Arguments:
//   F: the type of f
// Functions Arguments:
//   affA: whether a is affirmative
//   a: the first variable
//   affB: whether b is affirmative
//   b: the second variable
//   f(i, j): the function to run a callback on for the corresponding edge
//     created by the disjunction
template <class F> void addOr(bool affA, int a, bool affB, int b, F f) {
  addImpl(!affA, a, affB, b, f); addImpl(!affB, b, affA, a, f);
}

// Sets the variable a to true
// Template Arguments:
//   F: the type of f
// Functions Arguments:
//   a: the variable
//   f(i, j): the function to run a callback on for the corresponding edge
//     created by setting this variable to true
template <class F> void setTrue(int a, F f) {
  addImpl(false, a, true, a, f);
}
