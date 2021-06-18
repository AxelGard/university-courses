from typing import Optional
from library import *
from enum import IntEnum

from .squad import Squad
import util
from handler import Handler
import positions
import config




class Role(IntEnum):
    DEFENSE = 0
    OFFENSE = 1


class ArmyHandler(Handler):
    def __init__(self, agent):
        super().__init__(agent)
        self.squads = {
            Role.OFFENSE: [],
            Role.DEFENSE: []
        } #id => unit (maybe squad => {id => unit})
        self.assigned = set() #id's
        self.unit = util.enum(
            MARINE = UnitType(UNIT_TYPEID.TERRAN_MARINE, agent),
            SIEGETANK = UnitType(UNIT_TYPEID.TERRAN_SIEGETANK, agent),
            SIEGETANKSIEGED = UnitType(UNIT_TYPEID.TERRAN_SIEGETANKSIEGED, agent),
        )
        self.chokepoints = []

    def on_game_start(self):
        self.chokepoints = positions.positions[self.agent.base_position]["chokepoints"]
        self.create_offensive_squad()

    def on_step(self):
        if min(len(self.agent.get_bases()), len(self.chokepoints)) > len(self.squads[Role.DEFENSE]):
            self.squads[Role.DEFENSE].append(Squad(self, Role.DEFENSE, self.chokepoints[len(self.squads[Role.DEFENSE])]))

        for squad in self.squads[Role.DEFENSE] + self.squads[Role.OFFENSE]:
            for unit_type, units in squad.units.items():
                for unit in units:
                    if not unit.is_alive:
                        squad.units[unit_type].pop(squad.units[unit_type].index(unit))
                        squad.full = False
                    self.agent.map_tools.draw_text(unit.position, squad.role.name + str(squad.id), config.COLOR)

        for squad in self.squads[Role.DEFENSE]:
            if not squad.full:
                squad.train()
            squad.move_to_hold_point()

        for squad in self.squads[Role.OFFENSE]:
            if not squad.full and not squad.on_mission:
                squad.train()
                squad.move_to_hold_point()
            else:
                base = self.agent.base_location_manager.get_player_starting_base_location(PLAYER_ENEMY)
                squad.attack_move(base.position)
                if not squad.on_mission:
                    squad.on_mission = True
                    self.create_offensive_squad()
                if squad.on_mission and squad.is_empty():
                    self.squads[Role.OFFENSE].remove(squad)

    def create_offensive_squad(self):
        self.squads[Role.OFFENSE].append(Squad(self, Role.OFFENSE, positions.positions[self.agent.base_position]["offensive_hold"]))

    def info_squads(self):
        rows = []
        for squad in self.squads[Role.DEFENSE] + self.squads[Role.OFFENSE]:
            rows.append(squad.role.name + " " + str(squad.id) + " : ")
            for unit_type, units in squad.units.items():
                rows.append("  - " + util.name(unit_type) + " : " + str(len(units)))
        return rows


    def defense_squads_full(self):
        number = 0
        for squad in self.squads[Role.DEFENSE]:
            if squad.full:
                number += 1
        return number


    def composition(self, role):
        if role == Role.DEFENSE:
            return {self.unit.MARINE: 8, self.unit.SIEGETANK: 2}
        elif role == Role.OFFENSE:
            return {self.unit.MARINE: 8, self.unit.SIEGETANK: 2}


    def include_unit(self, unit):
        if unit.id not in self.assigned:
            for squad in self.squads[Role.DEFENSE] + self.squads[Role.OFFENSE]:
                if squad.composition[unit.unit_type] > len(squad.units[unit.unit_type]) and not squad.on_mission:
                    squad.assign(unit)
                    self.assigned.add(unit.id)
                    break

    def train(self, unit_type, amount = 1):
        trained = 0
        if util.can_afford(self.agent, unit_type):
            for building in util.get_my_producers(self.agent, unit_type):
                if not building.is_training and building.unit_type.is_building:
                    building.train(unit_type)
                    trained += 1
                    if trained == amount:
                        break
