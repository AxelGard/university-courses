from typing import Optional
from library import *
import math
import copy
import random
from enum import IntEnum
import numpy as np

from handler import Handler
import config
import util
import positions
from ScoutHandler import potentialMatrix
from ScoutHandler import aStar
from ScoutHandler.aStarNode import Node as aStarNode

from ScoutHandler.PotentialFieldsDebug import heatmap

# TODO list:
# Getting stuck when getting huted by enemie soldier 


class ScoutHandler(Handler):
    def __init__(self, agent):
        super().__init__(agent)

        self.pot_mat = None

        self.time = 0
        self.scout = None
        self.hp_last_step = 45.0 # SCV hp value

        self.spoted_units = {} # {unit.id:{"unit": unit, "time":time}}
        self.spoted_units_strat = {}  # {unit.id:{"unit": unit_name, "time":time}}
        self.base_locations = [] # [BaseLocation]
        self.location_to_scout = None
        self.pos_last_step = None
        self.drawings = [] # [Point2DI]

        #A* // safe pasage cach
        self.pot_path = []
        self.path_cache = []
        self.path_time_cache = 0
        self.partcial_path = Point2DI(0,0)


    def on_game_start(self):
        self.pot_mat = potentialMatrix.PotentialMatrix(self.agent.map_tools)
        self.pos_last_step = Point2DI(self.pot_mat.width//2, self.pot_mat.height//2)
        self.pot_mat.set_nonvalids(self.agent)
        self.time = self.agent.time

        starting_base = self.agent.get_starting_base_location()
        self.pot_mat.set_bases(self.get_bases(), starting_base)

        for base_location in self.get_bases():
            #self.pot_mat.update(base_location.position, -1, self.time)
            self.base_locations.append(base_location)

        self.location_to_scout = self.base_locations[0].position
        self.pot_mat.enemie_base = self.base_locations[0].position

        if config.DEBUG_SCOUT:
            heatmap.debugger(self.pot_mat)


    def on_step(self):
        self.time = self.agent.time
        self.pot_mat.time = self.time
        scouts = self.agent.get_scouts()
        if len(scouts) > 0:
            if len(scouts) > 1:
                scouts.pop()
            self.scout = scouts[0]

            position = self.scout.position
            self.pot_mat.scout_position = position
            radius = 10
            self.pot_mat.set_time_area(position, 3)
            if util.distance(position, self.location_to_scout) < (radius//2):
                self.pot_mat.set_time(self.location_to_scout, self.time)
            enemies = self.close_enemies(radius)
            if enemies:
                potential_pos = self.potential_filed(enemies=enemies)
                self.drawings.append(potential_pos)
                if util.distance(position, self.location_to_scout) < 5*radius:
                    self.pot_mat.set_time(self.location_to_scout, self.time)
                self.location_to_scout = self.pot_mat.lowest_potential_point(self.scout.position,radius=2*radius) #util.opposite_direction(position, potential_pos)
                self.move(self.location_to_scout)
                self.pot_path = []

            else:
                self.location_to_scout = self.pot_mat.highest_intrest_point(self.get_bases(), self.time)
                if config.SAFE_PASSAGE and not self.pot_path:
                    enemies = self.close_enemies(radius*2)
                    if not enemies:
                        self.pot_path = self.safe_passage_move(self.scout.position, self.location_to_scout)
                        if len(self.pot_path) > 6:  # BUG: some times it gets stuck due to a building being on top of point. only in start of game 
                            self.pot_path = self.pot_path[:-5]
                        self.pot_mat.debug_lst = self.pot_path
                        self.path_time_cache = self.time
                        self.partcial_path = self.pot_path.pop(-1)
                elif config.SAFE_PASSAGE and self.pot_path:
                    self.move(self.partcial_path)
                    if self.close_location(self.partcial_path,self.scout.position) and self.pot_path:
                        self.partcial_path = self.pot_path.pop(-1)
                else:
                    self.move(self.location_to_scout)

            self.potesnial_to_buildings()

            if config.DEBUG_SCOUT:
                self.draw_potentsial() # draws pot in game
            self.hp_last_step = self.scout.hit_points
            self.pos_last_step = position


    def include_enemy(self, enemy_unit):
        """ adds new units to trackt enemies """
        self.spoted_units[enemy_unit.id] = {"unit": enemy_unit, "time":self.time}

    def include_enemy_strategy(self, enemy_unit):
        """ adds new units to trackt enemies """
        self.spoted_units[enemy_unit.id] = {"unit": enemy_unit, "time": self.time}

        if (enemy_unit.unit_type.is_building and
                not enemy_unit.unit_type.is_combat_unit and
                enemy_unit.hit_points > 0 and
                "SUPPLYDEPOT" not in enemy_unit.unit_type.name):
            if self.spoted_units_strat.get(enemy_unit.id, None) is None:
                self.spoted_units_strat[enemy_unit.id] = {"unit": enemy_unit.unit_type.name, "time": self.time}


    def potential_filed(self, enemies): # BUG: local minimum gets stuck
        """ calc the potenstial fileds from the given list """
        max_pos = self.pot_mat.MID_POINT
        max_v = -1*math.inf
        for pos in [enemie.position for enemie in enemies]:
            v = 0
            for enemie in enemies:
                wight = self.get_weight(enemie)
                v = v + wight * self.repulsive_potential(enemie)
                self.pot_mat.update(pos, v, self.time)
                if v > max_v:
                    max_pos = pos
                    max_v = v
        return max_pos


    def close_enemies(self, radius):
        """ looks for enimeis close to scout and 
        if they were spoted with in a time  """
        TIME_DELAY = 400
        enemies = []
        for id, dict in self.spoted_units.items():
            if util.distance(self.scout.position, dict["unit"].position) <= radius and \
                    dict["unit"].unit_type.is_combat_unit and self.time - dict["time"] < TIME_DELAY:
                enemies.append(dict["unit"])
        return enemies

    def close_location(self, p1, p2):
        """ returns bool if closer then radius """
        radius = 3
        return util.distance(p1=p1,p2=p2) < radius


    def repulsive_potential(self, enemie):
        """ Repulsive obsticale potensial, for obstical avoidence """
        return 1 / (1+2*(util.distance(self.scout.position, enemie.position)**2))


    def force_repulsive_potential(self, enemie):
        """ calc the potesnial repulsive force created from a enemie unit """
        return -1 / (1+(util.distance(self.scout.position, enemie.position)**3))


    def get_weight(self, unit):
        """" calc the weight of a unit type, used for potensial fileds """
        return unit.unit_type.attack_range


    def move(self, pos):
        """ moves the scout to the given postition, 
        ensures that the given postition is valid """
        if self.is_valid(pos):
            self.scout.move(pos)


    def safe_passage_move(self, start_pos, goal_pos):
        """ takes a starting and a goal position and 
        uses A* to create a path that is using pot """
        start = aStarNode(start_pos, self.pot_mat, cost=0)
        goal = aStarNode(goal_pos, self.pot_mat)
        return aStar.a_star(start, goal, self.time)


    def is_valid(self, pos):
        """ checks if the position is a valid postion,
        according to the pot_mat matrix """
        return self.pot_mat.is_valid(pos)


    def potential_move(self):
        """ moves the scout acording to the potensial fileds
        with in its sight range """
        radius = self.scout.unit_type.sight_range
        enemies = self.close_enemies(radius)
        potential_pos = self.potential_filed(enemies=enemies)
        pos = self.pot_mat.lowest_potential_point(self.scout.position, radius)
        self.move(pos=pos)


    def draw_potentsial(self):
        """ draws a circle at saved postions """
        for pos in self.drawings:
            util.draw_circle(self.agent, pos, 3)


    def get_bases(self):
        """ returns all the base locations on the map  """
        starting_base = self.agent.get_starting_base_location()
        base_locations = self.agent.base_location_manager.base_locations
        base_locations.sort(key=lambda base: -util.squared_distance(starting_base.position, base.position))
        return base_locations


    def potesnial_to_buildings(self):
        """ sets a potensial for each seen enemie building """
        for id, dict in self.spoted_units.items():
            if dict["unit"].unit_type.is_building:
                self.pot_mat.set_potensial(dict["unit"].position, dict["unit"].unit_type.mineral_price)


    def get_enemie_bases(self):
        """ retruns a list of enime base position """
        bases = []
        for base in self.get_bases():
            if base.is_occupied_by_player(PLAYER_ENEMY):
                bases.append(base.position)
        return bases
