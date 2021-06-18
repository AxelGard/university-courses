from queue import PriorityQueue
from library import *
from ScoutHandler import aStarNode
import util
import math


def a_star(start, goal, time_now):
    """ A* impl for moving and avoiding obsticals and potesnial """
    if start == goal:
        return []
    open_set = PriorityQueue()
    nodes = {} # position -> Node
    start_h = start.heuristic(goal, time_now)
    open_set.put(start.item)
    best_goal = None
    COST_WIGHT = 0 # 0 is fastest but has its own problems like getting stuck in walls 
    count = 0
    while not open_set.empty():
        current = open_set.get()[1]
        count += 1
        if current.is_close(goal.position, 2): # or count == 6000:
            #print("count = " + str(count))
            return current.reconstructed_path()

        for neighbour in current.neighbours(nodes):
            cost = current.cost + neighbour.distance(goal.position)
            if cost < neighbour.cost:
                wight = (neighbour.potensial+1)**2 # TODO: https://www.movingai.com/astar-var.html
                neighbour.fx = COST_WIGHT*cost + wight*neighbour.heuristic(goal, time_now) # TODO: add wight for better preformence
                neighbour.cost = cost
                open_set.put(neighbour.item)
    print("no plan found for scout A*")
    return []
