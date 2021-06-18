from library import IDABot, Unit, UnitType, TypeData, Point2DI, PLAYER_SELF
from typing import Optional


def has_addon(bot: IDABot, candidate: Unit, addon_type: UnitType):
    """
    Looks through all units and looks if there is an addon of the type addon_type nearby to the supplied candidate.

    :return: True if the unit "candindate" has an addon of the type "addon_type"
    """
    for unit in bot.get_my_units():
        if unit.unit_type.is_addon and unit.is_alive and unit.is_completed \
                and unit.unit_type == addon_type \
                and unit.position.distance(candidate.position) < 3:
            return True

    return False


def find_producer(bot: IDABot, unit_type: UnitType) -> Optional[Unit]:
    """
    Goes through all units and tries to find a unit which can produce the given unit_type at this very moment. Ignores
    units which non-idle.

    :return: A unit which can build unit_type, None if there is no such unit
    """
    data = bot.tech_tree.get_data(unit_type)  # type: TypeData

    if data.required_addons:
        addon = data.required_addons[0]
    else:
        addon = None

    for candidate in bot.get_my_units():  # type: Unit
        if candidate.unit_type in data.what_builds:
            if addon and not has_addon(bot, candidate, addon):
                continue
            elif candidate.unit_type.is_building and candidate.is_training:
                continue
            elif not candidate.is_completed:
                continue
            elif candidate.unit_type.is_building and candidate.is_flying:
                continue
            elif candidate in bot.building_assignment:
                continue
            else:
                return candidate
    return None


def exists_producer_for(bot: IDABot, unit_type: UnitType) -> bool:
    """
    A faster version of the function find_producer, it only looks if there is a unit which can build the given
    unit_type. It does not check if it is available or even done constructing

    :return: True if there is a unit which might eventually build this unit, False otherwise.
    """
    what_builds = bot.tech_tree.get_data(unit_type).what_builds
    for unit in bot.get_my_units():  # type: Unit
        if unit.is_alive and unit.unit_type in what_builds:
            return True
    return False


def find_refinery_position(bot: IDABot) -> Optional[Point2DI]:
    """
    Goes through each occupied base and checks if there are refineries at each location. If it finds a free spot it
    returns its location, None otherwise.
    """

    refineries = filter(lambda u: u.unit_type.is_refinery, bot.get_my_units())

    for base_location in bot.base_location_manager.get_occupied_base_locations(PLAYER_SELF):
        for geyser in base_location.geysers:  # type: Unit
            if not bot.refinery_at_position(geyser.position, refineries) \
                    and not bot.refinery_being_built_at_position(geyser.position):
                return geyser.tile_position

    return None
