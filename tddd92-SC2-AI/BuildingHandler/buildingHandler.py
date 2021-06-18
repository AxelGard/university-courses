from typing import Optional
from library import *
from .base import Base
from handler import Handler
import util
import positions


class BuildingHandler(Handler):
    def __init__(self, agent):
        super().__init__(agent)
        self.buildings = {} # key = building enum (library.UnitType), value = set of buildings (library.Unit)
        self.construction = {} # key = UnitType, value = buildings (list)
        self.unit_types = {}
        self.bases = []
        self.building = util.enum(
            SUPPLYDEPOT = UnitType(UNIT_TYPEID.TERRAN_SUPPLYDEPOT, agent),
            SUPPLYDEPOTLOWERED = UnitType(UNIT_TYPEID.TERRAN_SUPPLYDEPOTLOWERED, agent),
            REFINERY = UnitType(UNIT_TYPEID.TERRAN_REFINERY, agent),
            BARRACKS = UnitType(UNIT_TYPEID.TERRAN_BARRACKS, agent),
            COMMANDCENTER = UnitType(UNIT_TYPEID.TERRAN_COMMANDCENTER, agent),
            FACTORY = UnitType(UNIT_TYPEID.TERRAN_FACTORY, agent),
            FACTORYTECHLAB = UnitType(UNIT_TYPEID.TERRAN_FACTORYTECHLAB, agent)
        )
        self.buildings[self.building.REFINERY] = set()
        self.buildings[self.building.BARRACKS] = set()
        self.building_commandcenter = False
        self.previous_expansion = None


    def on_game_start(self):
        self.bases.append(Base(self.agent, self.agent.get_starting_base_location()))
        self.previous_expansion = self.agent.get_starting_base_location()
        self.building_positions = positions.positions[self.agent.base_position]["building_positions"] # TODO: Make less shit
        self.building_positions[self.building.SUPPLYDEPOT] = self.building_positions["SUPPLYDEPOT"]
        self.building_positions[self.building.BARRACKS] = self.building_positions["BARRACKS"]
        self.building_positions[self.building.FACTORY] = self.building_positions["FACTORY"]
        del self.building_positions["SUPPLYDEPOT"]
        del self.building_positions["BARRACKS"]
        del self.building_positions["FACTORY"]


    def build_addon(self, unit_type):
        if util.can_afford(self.agent, unit_type):
            for building in util.get_my_producers(self.agent, unit_type):
                if not building.is_training:
                    building.train(unit_type)


    def on_step(self):
        if self.construction_amount(self.building.COMMANDCENTER) == 1:
            self.building_commandcenter = True
        elif self.construction_amount(self.building.COMMANDCENTER) == 0 and self.building_commandcenter == True:
            self.building_commandcenter = False
            self.bases.append(Base(self.agent, self.previous_expansion))
            for base in self.bases:
                base.compute_mineral_fields()
                base.compute_geysers()

        if len(self.get_buildings(self.building.FACTORY)) > 0:
            self.build_addon(self.building.FACTORYTECHLAB)

        if (self.agent.max_supply <= self.agent.current_supply * 1.25) and self.construction_amount(self.building.SUPPLYDEPOT) == 0:
            self.build(self.building.SUPPLYDEPOT)

        if len(self.get_buildings(self.building.REFINERY)) < 1*len(self.bases) and self.construction_amount(self.building.REFINERY) == 0:
            self.build(self.building.REFINERY)

        if len(self.get_buildings(self.building.BARRACKS)) < 3 and self.construction_amount(self.building.BARRACKS) == 0:
            self.build(self.building.BARRACKS)

        if len(self.get_buildings(self.building.FACTORY)) < 2 and self.construction_amount(self.building.FACTORY) == 0:
            self.build(self.building.FACTORY)

        if len(self.get_buildings(self.building.COMMANDCENTER)) < min(self.agent.armyHandler.defense_squads_full() + 1, 3) and self.construction_amount(self.building.COMMANDCENTER) == 0:
            self.build(self.building.COMMANDCENTER)

    def construction_amount(self, building):
        return len(self.agent.get_workers_building(building))

    def include_building(self, building):
        if not self.buildings.get(building.unit_type, False):
            self.buildings[building.unit_type] = set()
        if (building.id == 0): # BUG: Seems like a bug in lib
            return
        if building.unit_type == self.building.SUPPLYDEPOT:
            building.morph(self.building.SUPPLYDEPOTLOWERED) # WARNING: is still stored as SUPPLYDEPOT in buildings
        self.buildings[building.unit_type].add(building)


    def get_buildings(self, building_type):
        return self.buildings.get(building_type, set())

    def get_refinery(self, geyser: Unit) -> Optional[Unit]:
        """ Returns: A refinery which is on top of unit `geyser` if any, None otherwise """
        for building in self.buildings[self.building.REFINERY]:
            if building.unit_type.is_refinery and util.squared_distance(building.position, geyser.position) < 1:
                return building

        return None

    def build_refinery(self):
        """ builds one refinery at the next base when given a refinery unit"""
        for base in self.bases:
            for geyser in base.get_geysers():
                if not self.get_refinery(geyser):
                    worker = self.agent.get_worker_near(geyser.position)
                    if worker:
                        worker.stop()
                        worker.build_target(self.building.REFINERY, geyser)
                        break


    def build(self, building, space = 2):
        if not util.can_afford(self.agent, building):
            return
        if building == self.building.REFINERY: # special case for refineries
            self.build_refinery()
            return

        if building == self.building.COMMANDCENTER: # special case for commandcenter
            base_location = self.agent.base_location_manager.get_next_expansion(PLAYER_SELF)
            self.previous_expansion = base_location
            cord = base_location.depot_position
        else:
            point = self.building_positions[building][len(self.get_buildings(building))]
            cord = self.agent.building_placer.get_build_location_near(point, building, space)

        worker = self.agent.get_worker_near(cord)
        if worker:
            worker.stop()
            worker.build(building, cord)
        else:
            self.building_positions[building].push(point)
