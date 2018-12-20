#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the assignment problem of matching W workers to J jobs with the minimum cost
// where each worked can only be assigned to 1 job and each job can only be assigned to 1 worker
// Time Complexity: O(max(W, J)^3)
// Memory Complexity: O(max(W, J)^2)
template <const int MAXWJ, class unit> struct HungarianAlgorithm {
    unit INF, EPS; HungarianAlgorithm(unit INF, unit EPS) : INF(INF), EPS(EPS) {}
    int rows, cols, dim; bool committedWorkers[MAXWJ];
    unit matrix[MAXWJ][MAXWJ], costMatrix[MAXWJ][MAXWJ], labelByWorker[MAXWJ], labelByJob[MAXWJ], minSlackValueByJob[MAXWJ], totalCost;
    int minSlackWorkerByJob[MAXWJ], matchJobByWorker[MAXWJ], matchWorkerByJob[MAXWJ], parentWorkerByCommittedJob[MAXWJ];
    void match(int w, int j) { matchJobByWorker[w] = j; matchWorkerByJob[j] = w; }
    void updateLabeling(unit slack) {
        for (int w = 0; w < dim; w++) if (committedWorkers[w]) labelByWorker[w] += slack;
        for (int j = 0; j < dim; j++) {
            if (parentWorkerByCommittedJob[j] != -1) labelByJob[j] -= slack;
            else minSlackValueByJob[j] -= slack;
        }
    }
    void reduce() {
        for (int w = 0; w < dim; w++) {
            unit minCost = INF;
            for (int j = 0; j < dim; j++) minCost = min(minCost, costMatrix[w][j]);
            for (int j = 0; j < dim; j++) costMatrix[w][j] -= minCost;
        }
        for (int j = 0; j < dim; j++) {
            unit minCost = INF;
            for (int w = 0; w < dim; w++) minCost = min(minCost, costMatrix[w][j]);
            for (int w = 0; w < dim; w++) costMatrix[w][j] -= minCost;
        }
    }
    void computeInitialSolution() {
        for (int j = 0; j < dim; j++) labelByJob[j] = INF;
        for (int w = 0; w < dim; w++) for (int j = 0; j < dim; j++)
            if (costMatrix[w][j] < labelByJob[j]) labelByJob[j] = costMatrix[w][j];
    }
    void greedyMatch() {
        for (int w = 0; w < dim; w++) for (int j = 0; j < dim; j++)
            if (matchJobByWorker[w] == -1 && matchWorkerByJob[j] == -1 && costMatrix[w][j] - labelByWorker[w] - labelByJob[j] <= EPS)
                match(w, j);
    }
    int getUnmatchedWorker() {
        for (int w = 0; w < dim; w++) if (matchJobByWorker[w] == -1) return w;
        return dim;
    }
    void initialize(int w) {
        for (int i = 0; i < dim; i++) { committedWorkers[i] = false; parentWorkerByCommittedJob[i] = -1; }
        committedWorkers[w] = true;
        for (int j = 0; j < dim; j++) {
            minSlackValueByJob[j] = costMatrix[w][j] - labelByWorker[w] - labelByJob[j];
            minSlackWorkerByJob[j] = w;
        }
    }
    void execute() {
        while (true) {
            int minSlackWorker = -1, minSlackJob = -1; unit minSlackValue = INF;
            for (int j = 0; j < dim; j++) {
                if (parentWorkerByCommittedJob[j] == -1) {
                    if (minSlackValueByJob[j] < minSlackValue) {
                        minSlackValue = minSlackValueByJob[j]; minSlackWorker = minSlackWorkerByJob[j]; minSlackJob = j;
                    }
                }
            }
            if (minSlackValue > EPS) updateLabeling(minSlackValue);
            parentWorkerByCommittedJob[minSlackJob] = minSlackWorker;
            if (matchWorkerByJob[minSlackJob] == -1) {
                int committedJob = minSlackJob, parentWorker = parentWorkerByCommittedJob[committedJob];
                while (true) {
                    int temp = matchJobByWorker[parentWorker]; match(parentWorker, committedJob); committedJob = temp;
                    if (committedJob == -1) break;
                    parentWorker = parentWorkerByCommittedJob[committedJob];
                }
                return;
            } else {
                int worker = matchWorkerByJob[minSlackJob]; committedWorkers[worker] = true;
                for (int j = 0; j < dim; j++) {
                    if (parentWorkerByCommittedJob[j] == -1) {
                        unit slack = costMatrix[worker][j] - labelByWorker[worker] - labelByJob[j];
                        if (minSlackValueByJob[j] > slack) { minSlackValueByJob[j] = slack; minSlackWorkerByJob[j] = worker; }
                    }
                }
            }
        }
    }
    void init() { for (int i = 0; i < MAXWJ; i++) fill(matrix[i], matrix[i] + MAXWJ, 0); }
    unit run(int workers, int jobs) {
        dim = max(workers, jobs); rows = workers; cols = jobs;
        for (int i = 0; i < dim; i++) {
            labelByWorker[i] = labelByJob[i] = minSlackValueByJob[i] = 0;
            minSlackWorkerByJob[i] = 0;
            matchJobByWorker[i] = matchWorkerByJob[i] = parentWorkerByCommittedJob[i] = -1;
            committedWorkers[i] = false;
            for (int j = 0; j < dim; j++) costMatrix[i][j] = matrix[i][j];
        }
        reduce(); computeInitialSolution(); greedyMatch();
        int w = getUnmatchedWorker();
        while (w < dim) { initialize(w); execute(); w = getUnmatchedWorker(); }
        for (w = 0; w < rows; w++) if (matchJobByWorker[w] >= cols) matchJobByWorker[w] = -1;
        for (int j = 0; j < cols; j++) if (matchWorkerByJob[j] >= rows) matchWorkerByJob[j] = -1;
        totalCost = 0;
        for (int w = 0; w < rows; w++) if (matchJobByWorker[w] != -1) totalCost += matrix[w][matchJobByWorker[w]];
        return totalCost;
    }
};
