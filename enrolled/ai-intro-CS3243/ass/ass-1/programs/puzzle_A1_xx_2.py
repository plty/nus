from heapq import heappush, heappop
import os
import sys

class PriorityQueue:
    heap = []
    def push(self, item):
        return heappush(self.heap, item)

    def pop(self):
        top = self.top()
        heappop(self.heap)
        return top

    def top(self):
        return self.heap[0]

    def size(self):
        return len(self.heap)

class Puzzle:
    def __init__(self, init_state, goal_state):
        self.init_state = tuple(map(tuple, init_state))
        self.goal_state = tuple(map(tuple, goal_state))

    def actions(self, state):
        moves = [(0, 1), (1, 0), (0, -1), (-1, 0)]
        r, c = [(i, j) for i in range(3) for j in range(3) if state[i][j] == 0][0]
        return [(i, j) for (i, j) in moves if 0 <=r + i < 3 and 0 <=c + j < 3]

    def heuristic(self, state):
        return sum([abs((state[i][j] - 1) // 3 - i) + abs((state[i][j] - 1) % 3 - j) for i in range(3) for j in range(3) if state[i][j] != 0])

    def move(self, state, action):
        dr, dc = action
        r, c = [(i, j) for i in range(3) for j in range(3) if state[i][j] == 0][0]
        s = list(map(list, state))
        s[r][c], s[r + dr][c + dc] = s[r + dr][c + dc], s[r][c]
        return tuple(map(tuple, s))

    def solve(self):
        frontier = PriorityQueue()
        # explored double as known distance to a state.
        explored = {}
        goal_distance = None

        frontier.push((self.heuristic(self.init_state), 0, self.init_state))
        while frontier.size() > 0:
            prio, distance, state = frontier.pop()
            if state in explored:
                continue
            explored[state] = distance
            if state == self.goal_state:
                goal_distance = distance
                break

            states = [(distance + 1 + self.heuristic(s), distance + 1, s) 
                    for s in [self.move(state, action) for action in self.actions(state)] 
                    if s not in explored]

            for s in states:
                frontier.push(s)

        if goal_distance == None:
            return ["UNSOLVABLE"]

        current = self.goal_state
        moves = []
        while current != self.init_state:
            _, action, current = min([(explored[s], a, s)
                    for a, s in [(action, self.move(current, action)) for action in self.actions(current)]
                    if s in explored])
            moves += [action]
        action_name = {(0, 1): "RIGHT", (1, 0): "DOWN", (0, -1): "LEFT", (-1, 0): "UP"}
        return map(lambda x: action_name[x], reversed(moves))


if __name__ == "__main__":
    # do NOT modify below
    if len(sys.argv) != 3:
        raise ValueError("Wrong number of arguments!")

    try:
        f = open(sys.argv[1], 'r')
    except IOError:
        raise IOError("Input file not found!")

    init_state = [[0 for i in range(3)] for j in range(3)]
    goal_state = [[0 for i in range(3)] for j in range(3)]
    lines = f.readlines()

    i,j = 0, 0
    for line in lines:
        for number in line:
            if '0'<= number <= '8':
                init_state[i][j] = int(number)
                j += 1
                if j == 3:
                    i += 1
                    j = 0

    for i in range(1, 9):
        goal_state[(i-1)//3][(i-1)%3] = i
    goal_state[2][2] = 0

    puzzle = Puzzle(init_state, goal_state)
    ans = puzzle.solve()

    with open(sys.argv[2], 'a') as f:
        for answer in ans:
            f.write(answer+'\n')
