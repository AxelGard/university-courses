from typing import Optional
from library import *
from handler import Handler
import util
import positions


class ScoutHandler(Handler):
    def __init__(self, agent):
        super().__init__(agent)
        self.scouted = []  # [ {"base_locs": BaseLocation, scouted: True, time: ...}] meta data
        self.hp_last_step = 45.0 # SCV hp value
        self.location_to_scout = None
        self.spoted_units = {} # {unit.id:{"unit": unit, "time":time}}
        self.time = 0

    def on_game_start(self):
        starting_base = self.agent.get_starting_base_location()
        base_locations = self.agent.base_location_manager.base_locations
        base_locations.sort(key=lambda base: -util.squared_distance(starting_base.position, base.position))
        for base_location in base_locations[:-3]:
            dict = {
                "base_location": base_location,
                "cord": base_location.position,
                "scouted": False,
                "dangerous": False,
            }
            self.scouted.append(dict)
        self.location_to_scout = self.scouted[0]

    def on_step(self):
        self.time = self.agent.time
        scouts = self.agent.get_scouts()
        if len(scouts) > 0:
            scout = scouts[0]

            if scout.hit_points < self.hp_last_step:
                self.location_to_scout["dangerous"] = True
            self.hp_last_step = scout.hit_points

            for dict in self.scouted:
                if util.squared_distance(scout.position, dict["cord"]) < 10:
                    dict["scouted"] = True
                if not dict["scouted"] and not dict["dangerous"]:
                    self.location_to_scout = dict
                    scout.move(dict["cord"])
                    break
            else: # NOTE: no-break => set scouted to False for all.
                for dict in self.scouted:
                    dict["scouted"] = False

    def include_enemy(self, enemy_unit):
        """ adds new units to trackt enemies """
        self.spoted_units[enemy_unit.id] = {"unit": enemy_unit, "time":self.time} 
