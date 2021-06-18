from typing import Optional
from library import *
import math
import numpy as np

from ScoutHandler.mapNode import Node
import config
import util


class PotentialMatrix():
    def __init__(self, map_tools):
        self.map_tools = map_tools
        self.AXIS_LEN = 160
        self.height = self.map_tools.height
        self.width = self.map_tools.width
        self.time = 0
        self.matrix = np.zeros((self.width,self.height, 3)) # NOTE: a 160*160 matrix were evry point Node obj
                                                        # represnting all points in the game world
        self.MID_POINT = Point2DI(self.width//2, self.height//2)
        self.enemie_base = self.MID_POINT

        self.time_last_cache = 0
        self.cache_base_pot = {} # {Poit2DI:pot}
        self.intrest_point = self.MID_POINT

        self.debug_lst = []
        self.scout_position = self.MID_POINT

    def update(self, point, potential, time):
        """ update a given postion on the world map with potensail and at what time this happend on """
        if self.is_valid(point):    
            self.matrix[int(point.x), int(point.y), Node.POTENTIAL] = potential
            self.matrix[int(point.x), int(point.y), Node.TIME] = time


    def get_potensial(self, point):
        return self.matrix[int(point.x), int(point.y), Node.POTENTIAL]


    def set_potensial(self, point, potensial):
        self.matrix[int(point.x), int(point.y), Node.POTENTIAL] = potensial


    def set_time(self, point, time):
        self.matrix[int(point.x), int(point.y), Node.TIME] = time


    def get_time(self, point):
        return self.matrix[int(point.x), int(point.y), Node.TIME]


    def is_valid(self, point):
        """ checks if the given position is valid so not wall or out side the map """
        return point.x <= self.width and point.x >= 0 and \
                point.y <= self.height and point.y >= 0 and \
                self.matrix[int(point.x), int(point.y), Node.ISVALID] == 0


    def set_nonvalids(self, agent):
        """ gose over the map and sets points that is walkabele to not valid points """
        for x in range(self.width):
            for y in range(self.height):
                if not agent.map_tools.is_walkable(Point2DI(x,y)):
                    self.matrix[x, y, Node.ISVALID] = 1


    def set_bases(self, bases, starting_base):
        """ takes a list of base obj  """ 
        for base in bases:
            self.set_potensial(base.position, util.squared_distance(starting_base.position, base.position))
            self.cache_base_pot[base.position] = self.get_potensial(base.position)


    def highest_intrest_point(self, bases, time):
        """ goes over the given list of base obj's and looks for the one with the highes intrest """
        point = self.enemie_base
        highest_intrest = 0 
        radius = 20
        for base in bases:
            base_point = Point2DI(int(base.position.x), int(base.position.y))
            pot = self.cache_base_pot[base.position]
            if time - self.time_last_cache > 600: # renew cache
                pot = self.potential_in_area(base.position, radius=radius)
                self.cache_base_pot[base.position] = pot
                self.time_last_cache = time
            intrest =  pot * (time - self.get_time(base_point))
            if intrest > highest_intrest:
                point = base.position
                highest_intrest = intrest
        self.intrest_point = point
        return point 


    def lowest_potential_point(self, point, radius):
        """" finds point on the map were it has the lowset potensial """
        l_pos = None
        l_p = math.inf  
        for i in range(-radius,radius):
            for j in range(-radius,radius):
                temp_point = Point2DI(int(point.x + i), int(point.y + j))
                if self.get_potensial(temp_point) < l_p and \
                    self.is_valid(temp_point):
                    l_pos = temp_point
                    l_p = self.get_potensial(temp_point)
        return l_pos


    def highest_potential_point(self, point, radius):
        """ finds point on the map were it has the hihgest potensial """  
        h_pos = None
        h_p = -1*math.inf  
        for i in range(-radius,radius):
            for j in range(-radius,radius):
                temp_point = Point2DI(int(point.x + i), int(point.y + j))
                if self.get_potensial(temp_point) > h_p and \
                    self.is_valid(temp_point):
                    h_pos = temp_point
                    h_p = self.get_potensial(temp_point)
        return h_pos


    def potential_in_area(self, point, radius):
        """ finds point on the map were it has the hihgest potensial """ 
        pot = 0
        for i in range(-radius,radius):
            for j in range(-radius,radius):
                temp_point = Point2DI(int(point.x + i), int(point.y + j))
                pot += self.get_potensial(temp_point)
        return pot
    

    def world_highest_potential_point(self):
        """ finds the highst potensial point in the world """ # TODO: rewrite, find a better solution
        h_pos = self.MID_POINT
        h_p = 10**100 * -1 # -inf  
        for i in range(self.width):
            for j in range(self.height):
                temp_point = Point2DI(i,j)
                if self.get_potensial(temp_point) > h_p and \
                    self.is_valid(temp_point):
                    h_pos = temp_point
                    h_p = self.get_potensial(temp_point)
        return h_pos

    def set_time_area(self, point, radius):
        """ sets the time in radius  """
        for i in range(-radius,radius):
            for j in range(-radius,radius):
                temp_point = Point2DI(int(point.x + i), int(point.y + j))
                self.set_time(point, self.time)


