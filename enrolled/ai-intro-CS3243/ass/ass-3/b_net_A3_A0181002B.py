from itertools import product
import sys
import json

def merge_dicts(*dicts):
    res = dict()
    for d in dicts:
        res.update(d)
    return res

class BayesianNetwork:
    def __init__(self, structure, values, queries):
        self.variables = structure["variables"]
        self.dependencies = structure["dependencies"]
        self.conditional_probabilities = values["conditional_probabilities"]
        self.prior_probabilities = values["prior_probabilities"]
        self.queries = queries
        self.answer = []
        self.queries = queries

    def construct(self):
        self.domains = self.variables
        self.deps = merge_dicts(
                dict((k, []) for k in self.domains.keys()),
                self.dependencies)
        # self.prob[v][k][(t_0, t_1, ...)] denotes P(v = k | deps[v][0] = t_0, deps[v][1] = t_1, ...)
        self.prob = dict((k, dict((v, {}) for v in self.domains[k])) for k in self.domains.keys())
        for k, probs in self.prior_probabilities.items():
            self.prob[k] = dict((v, {(): p}) for v, p in probs.items())
        for v, entries in self.conditional_probabilities.items():
            for e in entries:
                self.prob[v][e["own_value"]][tuple(e[d] for d in self.deps[v])] = e["probability"]

    def _calc_probability(self, values):
        probability = 1.0
        for k, v in values.items():
            probability *= self.prob[k][v][tuple(values[d] for d in self.deps[k])]
        return probability

    def _and(self, values):
        values = dict((k, [v]) for k, v in values.items())
        domains = merge_dicts(self.domains, values)

        probability = 0.0
        for v in product(*domains.values()):
            probability += self._calc_probability(dict(zip(domains.keys(), v)))
        return probability

    def _infer(self, tofind, given):
        for k, v in tofind.items():
            if v != given.get(k, v):
                return 0.0

        return self._and(merge_dicts(tofind, given)) / self._and(given)

    def infer(self):
        return [{
            "index": q["index"], 
            "answer": self._infer(q["tofind"], q["given"])
            } for q in self.queries]

def main():
    # STRICTLY do NOT modify the code in the main function here
    if len(sys.argv) != 4:
        print ("\nUsage: python b_net_A3_xx.py structure.json values.json queries.json \n")
        raise ValueError("Wrong number of arguments!")

    structure_filename = sys.argv[1]
    values_filename = sys.argv[2]
    queries_filename = sys.argv[3]

    try:
        with open(structure_filename, 'r') as f:
            structure = json.load(f)
        with open(values_filename, 'r') as f:
            values = json.load(f)
        with open(queries_filename, 'r') as f:
            queries = json.load(f)

    except IOError:
        raise IOError("Input file not found or not a json file")

    # testing if the code works
    b_network = BayesianNetwork(structure, values, queries)
    b_network.construct()
    answers = b_network.infer()


if __name__ == "__main__":
    main()

