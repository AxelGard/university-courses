from queue import PriorityQueue
from library import *
from ScoutHandler import aStarNode
import util
import math

def position_string(pos):
    return str(pos.x) + ":" + str(pos.y)

class Node:
    def __init__(self, position, pot_mat, parent=None, **kwargs):
        self.pot_mat=pot_mat
        self.parent = parent
        self.position = Point2DI(int(position.x), int(position.y))
        self.fx = kwargs.get("fx", math.inf)
        self.cost = kwargs.get("cost", math.inf)
        self.potensial = 0

    def reconstructed_path(self):
        """ recursevly creates a path by going to parrent node """
        if self.parent is None:
            return []
        return [self.position] + self.parent.reconstructed_path();


    def neighbours(self, nodes):
        """ creates neighboring nodes and retruns a list with them """
        neighbours_ = []
        radius = 1
        for i in range(-radius, radius+1):
            for j in range(-radius, radius+1):
                pos = Point2DI(int(self.position.x)+j, int(self.position.y)+i)
                if self.pot_mat.is_valid(pos) and pos != self.position and\
                        (self.parent is None or pos != self.parent.position):
                    if nodes.get(position_string(pos)):
                        neighbours_.append(nodes.get(position_string(pos)))
                    else:
                        cur_neighbour = Node(pos, self.pot_mat, self)
                        if self.pot_mat.get_potensial(pos) > 0:
                            cur_neighbour.potensial = self.pot_mat.potential_in_area(pos, 3)
                        nodes[position_string(pos)] = cur_neighbour
                        neighbours_.append(cur_neighbour)
        return neighbours_


    def distance(self, point):
        """ ruturns the distance between node and a point + the potensial at that point """
        if not self.pot_mat.is_valid(self.position):
            return math.inf
        return util.distance(self.position, point) + (self.pot_mat.get_potensial(point))**4

    def heuristic(self, goal, time_now):
        """ heuristic for A* """
        dt = time_now - self.pot_mat.get_time(self.position)
        p = self.pot_mat.get_potensial(self.position)
        if not self.pot_mat.is_valid(self.position):
            return math.inf
        return util.distance(self.position, goal.position) #( (1/(1+self.distance(node.position))) - (1/(1+(dt * p))) )**2

    def is_close(self, point, radius):
        """ checks if self is with in the radius of the point"""
        return util.distance(self.position, point) < radius

    @property
    def item(self):
        return (self.fx, self)

    def __eq__(self, other):
        return self.position.x == other.position.x and self.position.y == other.position.y

    def __repr__(self):
        return f"Node({self.position}, {self.cost})"

    def __lt__(self, other):
        return self.cost < other.cost
