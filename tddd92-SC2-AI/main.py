import os
import socket

from typing import Optional
from library import *
import config
from local_config import *
import util
import positions

# TODO: config import for AI

class MyAgent(IDABot):
    def __init__(self):
        IDABot.__init__(self)
        self.workerHandler = WorkerHandler(self)
        self.buildingHandler = BuildingHandler(self)
        self.armyHandler = ArmyHandler(self)
        self.scoutHandler = ScoutHandler(self)
        self.base_position = None

    def on_game_start(self):
        IDABot.on_game_start(self)

        base = self.base_location_manager.get_player_starting_base_location(PLAYER_SELF)
        if self.map_tools.get_ground_distance(Point2D(34, 122), base.position) < self.map_tools.get_ground_distance(Point2D(128, 46), base.position):
            self.base_position = positions.BasePosition.UPPERLEFT
        else:
            self.base_position = positions.BasePosition.LOWERRIGHT

        self.workerHandler.on_game_start()
        self.buildingHandler.on_game_start()
        self.armyHandler.on_game_start()
        self.scoutHandler.on_game_start()


    def on_step(self):
        IDABot.on_step(self)
        #util.print_debug(self)
        self.workerHandler.on_step()
        self.buildingHandler.on_step()
        self.armyHandler.on_step()
        self.scoutHandler.on_step()
        util.print_unit_overview(self)

        for unit in self.get_my_units():
            if unit.unit_type.is_worker:
                self.workerHandler.include_worker(unit)
            elif unit.unit_type.is_building:
                self.buildingHandler.include_building(unit)
            elif unit.unit_type.is_combat_unit:
                self.armyHandler.include_unit(unit)

    def get_workers_building(self, building):
        return self.workerHandler.get_workers_building(building)

    def get_mineral_field(self, base):
        return base.get_mineral_field()

    def get_scouts(self):
        return self.workerHandler.get_scouts()

    def get_bases(self):
        return self.buildingHandler.bases

    def get_starting_base_location(self):
        return self.base_location_manager.get_player_starting_base_location(PLAYER_SELF)

    def get_base_locations(self):
        return self.base_location_manager.get_occupied_base_locations(PLAYER_SELF)

    def get_worker(self):
        return self.workerHandler.get_worker()

    def get_worker_near(self, position):
        return self.workerHandler.get_worker_near(position)

    def get_worker_by_id(self, id):
        return self.workerHandler.get_worker_by_id(id)




def main():
    coordinator = Coordinator(GAME_PATH)

    bot1 = MyAgent()

    participant_1 = create_participants(config.RACE, bot1)

    if (config.OPPONENT_IS_COMPUTER):
        participant_2 = create_computer(config.ENEMY_RACE, config.DIFFICULTY)
    else:
        bot2 = MyAgent()
        participant_2 = create_participants(Race.Terran, bot2)

    coordinator.set_real_time(config.REAL_TIME)
    coordinator.set_participants([participant_1, participant_2])
    coordinator.launch_starcraft()

    path = os.path.join(os.getcwd(), "maps", "InterloperTest.SC2Map")
    coordinator.start_game(path)

    while coordinator.update():
        pass


if __name__ == "__main__":
    main()
