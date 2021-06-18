
from typing import Optional
from library import *
import math
import config

def enum(**enums):
    return type('Enum', (), enums)

def name(unit):
    """ Returns the name of a unit or unit_type without 'TERRAN_'"""
    if isinstance(unit, Unit):
        return unit.unit_type.name.replace("TERRAN_", "")
    return unit.name.replace("TERRAN_", "")

def print_unit_overview(agent):
        """ Prints an overview of how many units are doing what """
        agent.map_tools.draw_text_screen(0.01, 0.01,
                "UNIT ASSIGNMENT", config.COLOR)
        agent.map_tools.draw_text_screen(0.01, 0.02,
                "---------------", config.COLOR)
        assignments = agent.workerHandler.get_assignment_numbers()

        y = 0.04
        for assignment, number in assignments.items():
            text = assignment + ": " + str(number)
            agent.map_tools.draw_text_screen(0.01, y, text, config.COLOR)
            y += 0.02
        if config.SQUAD_OUTPUT:
            rows = agent.armyHandler.info_squads()
            for row in rows:
                agent.map_tools.draw_text_screen(0.01, y, row, config.COLOR)
                y += 0.02

def print_debug(agent):
    for i, unit in enumerate(agent.get_my_units()):
        agent.map_tools.draw_text(unit.position, f"{unit.unit_type} id: {unit.id}  i: {i}")

    base_location = agent.base_location_manager.get_player_starting_base_location(PLAYER_SELF)

    for i, mineral_field in enumerate(base_location.mineral_fields):
        agent.map_tools.draw_text(mineral_field.position, f"{mineral_field.unit_type} id: {mineral_field.id}  i: {i}")

    for i, geyser in enumerate(base_location.geysers):
        agent.map_tools.draw_text(geyser.position, f"{geyser.unit_type} id: {geyser.id}  i: {i}")


def can_afford(agent, unit_type: UnitType):
    """ Returns True if there are an sufficient amount of minerals, gas and supply to build the given unit_type, False otherwise """
    return agent.minerals >= unit_type.mineral_price\
            and agent.gas >= unit_type.gas_price\
            and (agent.max_supply - agent.current_supply) >= unit_type.supply_required

def get_my_producers(agent, unit_type: UnitType):
    """ Returns a list of units which can build or train units of type unit_type """
    producers = []
    type_data = agent.tech_tree.get_data(unit_type)
    what_builds = type_data.what_builds

    for unit in agent.get_my_units(): # TODO: fix to get_worker or similar
        if unit.unit_type in what_builds:
            producers.append(unit)

    return producers


def get_my_workers(agent):
    workers = []
    for unit in agent.get_my_units():
        if unit.unit_type.is_worker:
            workers.append(unit)

    return workers

def squared_distance(p1, p2):
    return (p1.x - p2.x)**2 + (p1.y - p2.y)**2

def distance(p1, p2):
    return math.sqrt(squared_distance(p1, p2))
