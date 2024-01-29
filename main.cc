// Test shortest-path algorithms on Project Euler problem 83
// (https://projecteuler.net/problem=83).

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "graph.h"
#include "dijkstra.h"

using namespace std;

vector<string> read_lines(const string& path) {
  vector<string> lines;
  ifstream fs(path);
  for (string line; getline(fs, line);) {
    lines.push_back(line);
  }
  return lines;
}

// Split string by single character delimiter.
// https://stackoverflow.com/a/46931770
vector<string> split(const string &s, char delim) {
  vector<string> result;
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
    result.push_back(item);
  }
  return result;
}

vector<vector<int>> parse_matrix(const vector<string> &lines) {
  vector<vector<int>> matrix;
  for (const auto line : lines) {
    vector<int> row;
    for (const auto s : split(line, ',')) {
      row.push_back(stoi(s));
    }
    matrix.push_back(row);
  }
  return matrix;
}

int main() {
  // Read file to vector of strings.
  vector<string> lines = read_lines("matrix.txt");
  // vector<vector<int>> matrix = parse_matrix(lines);
  vector<vector<int>> matrix = parse_matrix(lines);

  // Build graph representation of the matrix.
  graph<int, int> g;
  for (uint i = 0; i < matrix.size(); i++) {
    for (uint j = 0; j < matrix[i].size(); j++) {
      const int u = 80 * i + j;
      g.add_vertex(u);

      if (i > 0) {
        const int v = 80 * (i-1) + j;
        g.add_edge(u, v, matrix[i][j], true);
        g.add_edge(v, u, matrix[i-1][j], true);
      }
      
      if (j > 0) {
        const int v = 80 * i + (j-1);
        g.add_edge(u, v, matrix[i][j], true);
        g.add_edge(v, u, matrix[i][j-1], true);
      }
    }
  }

  // Compute the solution via Dijkstra's algorithm.
  const int src = 0;
  const int dest = 80 * 79 + 79;
  auto path = dijkstra::shortest_path(g, src, dest);

  // Compute path sum.
  int sum = matrix[0][0];
  cout << sum << endl;
  for (const auto e : path) {
    sum += matrix[e.v2 / 80][e.v2 % 80];
  }
  cout << sum << endl;
}
