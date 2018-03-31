#ifndef ALGORITHMS_GRAPH_MATCHING_HUNGARIANALGORITHM_H_
#define ALGORITHMS_GRAPH_MATCHING_HUNGARIANALGORITHM_H_

#include <bits/stdc++.h>
using namespace std;

typedef double unit;
const unit INF = numeric_limits<double>::infinity();
const unit EPS = 1e-9;

/**
 * The Hungarian algorithm solves the assignment problem of matching
 * N workers to M jobs with the minimum cost. Each worker can only be
 * assigned to 1 job and each job can only be assigned to 1 worker.
 *
 * Time complexity: O(max(N, M)^3)
 */
class HungarianAlgorithm {
private:
    unit **costMatrix;
    int rows, cols, dim;
    unit *labelByWorker, *labelByJob;
    int *minSlackWorkerByJob;
    unit *minSlackValueByJob;
    int *matchJobByWorker, *matchWorkerByJob, *parentWorkerByCommittedJob;
    bool *committedWorkers;
    unit totalCost;

    inline void match(int w, int j) {
        matchJobByWorker[w] = j;
        matchWorkerByJob[j] = w;
    }

    void updateLabeling(unit slack) {
        for (int w = 0; w < dim; w++) {
            if (committedWorkers[w]) labelByWorker[w] += slack;
        }
        for (int j = 0; j < dim; j++) {
            if (parentWorkerByCommittedJob[j] != -1) labelByJob[j] -= slack;
            else minSlackValueByJob[j] -= slack;
        }
    }

    void reduce() {
        for (int w = 0; w < dim; w++) {
            unit minCost = INF;
            for (int j = 0; j < dim; j++) {
                minCost = min(minCost, costMatrix[w][j]);
            }
            for (int j = 0; j < dim; j++) {
                costMatrix[w][j] -= minCost;
            }
        }
        for (int j = 0; j < dim; j++) {
            unit minCost = INF;
            for (int w = 0; w < dim; w++) {
                minCost = min(minCost, costMatrix[w][j]);
            }
            for (int w = 0; w < dim; w++) {
                costMatrix[w][j] -= minCost;
            }
        }
    }

    void computeInitialSolution() {
        for (int j = 0; j < dim; j++) {
            labelByJob[j] = INF;
        }
        for (int w = 0; w < dim; w++) {
            for (int j = 0; j < dim; j++) {
                if (costMatrix[w][j] < labelByJob[j]) {
                    labelByJob[j] = costMatrix[w][j];
                }
            }
        }
    }

    void greedyMatch() {
        for (int w = 0; w < dim; w++) {
            for (int j = 0; j < dim; j++) {
                if (matchJobByWorker[w] == -1 && matchWorkerByJob[j] == -1 && costMatrix[w][j] - labelByWorker[w] - labelByJob[j] <= EPS) match(w, j);
            }
        }
    }

    int getUnmatchedWorker() {
        for (int w = 0; w < dim; w++) {
            if (matchJobByWorker[w] == -1) return w;
        }
        return dim;
    }

    void initialize(int w) {
        for (int i = 0; i < dim; i++) {
            committedWorkers[i] = false;
            parentWorkerByCommittedJob[i] = -1;
        }
        committedWorkers[w] = true;
        for (int j = 0; j < dim; j++) {
            minSlackValueByJob[j] = costMatrix[w][j] - labelByWorker[w] - labelByJob[j];
            minSlackWorkerByJob[j] = w;
        }
    }

    void execute() {
        while (true) {
            int minSlackWorker = -1;
            int minSlackJob = -1;
            unit minSlackValue = INF;
            for (int j = 0; j < dim; j++) {
                if (parentWorkerByCommittedJob[j] == -1) {
                    if (minSlackValueByJob[j] < minSlackValue) {
                        minSlackValue = minSlackValueByJob[j];
                        minSlackWorker = minSlackWorkerByJob[j];
                        minSlackJob = j;
                    }
                }
            }
            if (minSlackValue > EPS) updateLabeling(minSlackValue);
            parentWorkerByCommittedJob[minSlackJob] = minSlackWorker;
            if (matchWorkerByJob[minSlackJob] == -1) {
                int committedJob = minSlackJob;
                int parentWorker = parentWorkerByCommittedJob[committedJob];
                while (true) {
                    int temp = matchJobByWorker[parentWorker];
                    match(parentWorker, committedJob);
                    committedJob = temp;
                    if (committedJob == -1) break;
                    parentWorker = parentWorkerByCommittedJob[committedJob];
                }
                return;
            } else {
                int worker = matchWorkerByJob[minSlackJob];
                committedWorkers[worker] = true;
                for (int j = 0; j < dim; j++) {
                    if (parentWorkerByCommittedJob[j] == -1) {
                        unit slack = costMatrix[worker][j] - labelByWorker[worker] - labelByJob[j];
                        if (minSlackValueByJob[j] > slack) {
                            minSlackValueByJob[j] = slack;
                            minSlackWorkerByJob[j] = worker;
                        }
                    }
                }
            }
        }
    }

public:
    HungarianAlgorithm(unit **matrix, int workers, int jobs) {
        dim = max(workers, jobs);
        rows = workers;
        cols = jobs;
        costMatrix = new unit*[dim];
        for (int w = 0; w < dim; w++) {
            if (w < rows) {
                costMatrix[w] = new unit[dim];
                for (int j = 0; j < dim; j++) {
                    if (j < cols) costMatrix[w][j] = matrix[w][j];
                    else costMatrix[w][j] = 0;
                }
            } else {
                costMatrix[w] = new unit[dim];
                for (int j = 0; j < dim; j++) {
                    costMatrix[w][j] = 0;
                }
            }
        }
        labelByWorker = new unit[dim];
        labelByJob = new unit[dim];
        minSlackWorkerByJob = new int[dim];
        minSlackValueByJob = new unit[dim];
        matchJobByWorker = new int[dim];
        matchWorkerByJob = new int[dim];
        parentWorkerByCommittedJob = new int[dim];
        committedWorkers = new bool[dim];
        for (int i = 0; i < dim; i++) {
            labelByWorker[i] = 0;
            labelByJob[i] = 0;
            minSlackWorkerByJob[i] = 0;
            minSlackValueByJob[i] = 0;
            matchJobByWorker[i] = -1;
            matchWorkerByJob[i] = -1;
            parentWorkerByCommittedJob[i] = -1;
            committedWorkers[i] = false;
        }
        reduce();
        computeInitialSolution();
        greedyMatch();
        int w = getUnmatchedWorker();
        while (w < dim) {
            initialize(w);
            execute();
            w = getUnmatchedWorker();
        }
        for (w = 0; w < rows; w++) {
            if (matchJobByWorker[w] >= cols) matchJobByWorker[w] = -1;
        }
        for (int j = 0; j < cols; j++) {
            if (matchWorkerByJob[j] >= rows) matchWorkerByJob[j] = -1;
        }
        totalCost = 0;
        for (int w = 0; w < rows; w++) {
            if (matchJobByWorker[w] != -1) totalCost += matrix[w][matchJobByWorker[w]];
        }
    }

    int getJobForWorker(int w) {
        return matchJobByWorker[w];
    }

    int getWorkerForJob(int j) {
        return matchWorkerByJob[j];
    }

    unit getTotalCost() {
        return totalCost;
    }
};

#endif /* ALGORITHMS_GRAPH_MATCHING_HUNGARIANALGORITHM_H_ */
