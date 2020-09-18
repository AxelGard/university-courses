import os
from random import choice
from typing import Optional
from library import *
import time


class MyAgent(IDABot):
    def __init__(self):
        IDABot.__init__(self)
        """ global var """
        self.workers_jobbs = {}
        self.building_dictionary = 0
        self.army_dic = {}
        self.tank_dic = {}
        self.SCV_dic = {}
        self.move_list = []
        self.base_dic = {}
        self.worker_jobbs_2 = {}
        self.bunk_lst = []
        self.scouts = []
        self.im_attacking = []
        self.squad_attacking = []
        self.counter = 0
        self.count = 0
        self.count_2 = 0
        self.black_listed_bases = []
        self.squad_moved_out = []


    def on_game_start(self):
        """ starts the bot  """
        IDABot.on_game_start(self)

    # Get-funktioner:

    def get_all_my_units(self, type='All'):
        """ Gets all available units of chosen type and
            return them in a enumerated tuple -> (nr, unit) """
        unit_lst = []
        units_var = []
        if not type == 'All':
            for unit in self.get_my_units():
                if unit.unit_type == type:
                    units_var.append(unit)
        else:
            units_var = self.get_my_units()
        for unit in units_var:
            unit_lst.append([unit.id, unit])
        unit_sorted_lst = list(sorted(unit_lst))
        for unit in range(len(unit_sorted_lst)):
            unit_sorted_lst[unit] = unit_sorted_lst[unit][1]
        return enumerate(unit_sorted_lst)


    def get_base_minerals(self):
        """ Returns resources close to main base in a enumerate tuple """
        base_lst = []
        base = self.base_location_manager.get_player_starting_base_location(
        PLAYER_SELF)
        for resources in base.minerals:
            base_lst.append(resources)
        base_lst = enumerate(base_lst)
        return base_lst


    def get_base_geysers(self):
        """ Returns geyser close to main base in a enumerate tuple """
        geyser_lst = []
        base = self.base_location_manager.get_player_starting_base_location(
        PLAYER_SELF)
        for geyser_resources in base.geysers:
            geyser_lst.append([geyser_resources.id, geyser_resources])
        geyser_sorted_lst = list(sorted(geyser_lst))
        for geyser in range(len(geyser_sorted_lst)):
            geyser_sorted_lst[geyser] = geyser_sorted_lst[geyser][1]
        return enumerate(geyser_sorted_lst)


    def get_my_refineries(self):
        """ Returns a list of all refineries (list of Unit) """
        refineries = []
        for unit in self.get_my_units():
            if unit.unit_type.is_refinery:
                refineries.append(unit)
        return refineries


    def get_my_refineries_enum(self):
        """ finds all refineries and returns
        them in a list enumerate list"""
        ref_enum_list = []
        for ref in self.get_all_my_units():
            if ref[1].unit_type.is_refinery:
                for geys in self.get_base_geysers():
                    if self.get_refinery(geys[1]):
                        if self.get_refinery(geys[1]) == ref[1]:
                            ref_enum_list.append((geys[0], ref[1]))
        return (ref_enum_list)


    def get_my_refineries_1(self):
        """ returns refineries in base 1 in enumerate list """
        ref_enum_list = []
        for ref in self.get_all_my_units():
            if ref[1].unit_type.is_refinery:
                for geys in self.get_geysers(self.base_dic[1]):
                    if self.get_refinery(geys[1]):
                        if self.get_refinery(geys[1]) == ref[1]:
                            ref_enum_list.append((geys[0], ref[1]))
        return (ref_enum_list)


    def get_my_refineries_enum_2(self):
        """ returns refineries in base 2 in enumerate list """
        ref_enum_list = []
        for ref in self.get_all_my_units():
            if ref[1].unit_type.is_refinery:
                for geys in self.get_geysers(self.base_dic[2]):
                    if self.get_refinery(geys[1]):
                        if self.get_refinery(geys[1]) == ref[1]:
                            ref_enum_list.append((geys[0], ref[1]))
        return (ref_enum_list)


    def get_my_producers(self, unit_type: UnitType):
        """ Returns a list of units which can build or
        train units of type unit_type """
        producers = []
        type_data = self.tech_tree.get_data(unit_type)
        what_builds = type_data.what_builds
        for unit in self.get_my_units():
            if unit.unit_type in what_builds:
                producers.append(unit)
        return producers

    # worker uppdrag

    def build(self, build_type):
        """ a general build building function """
        # refinary
        ref_id = UNIT_TYPEID.TERRAN_REFINERY
        if build_type == ref_id:
            """ When called starts takes a worker
            to build a refinery on a gaser """
            miners = []
            for workers in self.workers_jobbs:
                if self.workers_jobbs[workers] == "mining":
                    miners.append(workers)
            ref_type = UnitType(UNIT_TYPEID.TERRAN_REFINERY, self)
            chosen_worker = choice(miners)
            for geys in self.get_base_geysers():
                if not self.get_refinery(geys[1]):
                    self.workers_jobbs.update({chosen_worker: ("building_ref",
                    geys[0])})
                    chosen_worker[1].build_target(ref_type, geys[1])
                    break
        else:
            miners = []
            for workers in self.workers_jobbs:
                if self.workers_jobbs[workers] == "mining":
                    miners.append(workers)
            building = UnitType(build_type, self)
            chosen_worker = choice(miners)
            base = self.base_location_manager.get_player_starting_base_location(
            PLAYER_SELF)
            build_loc = self.building_placer.get_build_location_near(
            base.depot_position, building)
            if self.can_afford(UnitType(build_type,
                    self)) and self.building_placer.can_build_here_with_spaces(
                    build_loc.x,
                    build_loc.y,
                    building, 2):
                chosen_worker[1].move(build_loc)
                chosen_worker[1].build(building, build_loc)
                self.workers_jobbs[chosen_worker] = "building_" + str(build_type)


    def build_bunker(self):
        """ takes a scv and build bunker on
        pre selected on map """
        unit_lst = []
        for unit in self.get_my_units():
            unit_lst.append(str(unit))
        p_base = self.start_location
        p1 = Point2DI(38, 125)
        p2 = Point2DI(114, 43)
        p2south = Point2DI(115, 56)
        p2north = Point2DI(41, 109)
        miners = []
        for workers in self.workers_jobbs:
            if self.workers_jobbs[workers] == "mining":
                miners.append(workers)
        chosen_worker = choice(miners)
        if (abs(p1.x - p_base.x) ** 2 + abs(p1.y - p_base.y) ** 2) ** 0.5 > (
                abs(p2.x - p_base.x) ** 2 + abs(p2.y - p_base.y) ** 2) ** 0.5:
            first = p2
            second = p2south
        else:
            first = p1
            second = p2north
        for squad in self.army_dic:
            if len(self.army_dic[squad]) > 1:
                if squad == 1 and "one" not in self.bunk_lst:
                    chosen_worker[1].build(UnitType(UNIT_TYPEID.TERRAN_BUNKER,
                    self), first)
                    self.bunk_lst.append("one")
                    self.workers_jobbs[chosen_worker] = "Building_bunk"
                elif squad == 2 and "two" not in self.bunk_lst:
                    chosen_worker[1].build(UnitType(UNIT_TYPEID.TERRAN_BUNKER,
                    self), second)
                    self.bunk_lst.append("two")
                    self.workers_jobbs[chosen_worker] = "Building_bunk"


    def enter_bunk(self):
        """ takes marines and puts in a bunker """
        bunk = list(self.get_all_my_units(UnitType(UNIT_TYPEID.TERRAN_BUNKER,
        self)))
        if len(bunk) > 0:
            if "one" in self.bunk_lst and bunk[0][1].is_completed and \
            "one_done" not in self.bunk_lst:
                for marines in self.army_dic[1]:
                    if str(marines[1]) == "<Unit of type: 'TERRAN_MARINE'>" \
                    and self.count < 4:
                        marines[1].right_click(bunk[0][1])
                        self.count += 1
                if not self.count < 4:
                    self.bunk_lst.append("one_done")

        if "two" in self.bunk_lst and "two_done" not in self.bunk_lst:
            if len(bunk) > 1:
                if bunk[0][1].is_completed and bunk[1][1].is_completed:
                    self.count_2 = 0
                    for marines in self.army_dic[2]:
                        if str(marines[1]) == "<Unit of type: 'TERRAN_MARINE'>" \
                        and self.count_2 < 4:
                            marines[1].right_click(bunk[1][1])
                            self.count_2 += 1
                    if not self.count_2 < 4:
                        self.bunk_lst.append("two_done")


    def get_gas_workers(self, geyser_chosen, enumerate_value):
        """ finds all gas workers """
        for worker in self.workers_jobbs:
            if self.workers_jobbs[worker] == 'mining':
                worker[1].right_click(geyser_chosen)
                self.workers_jobbs.update({worker: ("collecting_gas",
                enumerate_value)})
                break


    def ref_need_workers(self):
        """ looks if ref's needs more workers """
        refineries = self.get_my_refineries_1()
        for i in range(10):
            ref_work_lst = []
            for worker in self.workers_jobbs:
                if self.workers_jobbs[worker] == ("collecting_gas", i):
                    ref_work_lst.append(worker)
            if len(ref_work_lst) < 3:
                for unit in refineries:
                    if unit[0] == i:
                        if unit[1].build_percentage == 1.0:
                            self.get_gas_workers(unit[1], i)


    def ref_need_workers_2(self):
        """ sends worker to refinery in base 2"""
        refineries = self.get_my_refineries_enum_2()
        for i in range(10):
            ref_work_lst = []
            for worker in self.worker_jobbs_2:
                if self.worker_jobbs_2[worker] == ("collecting_gas", i):
                    ref_work_lst.append(worker)
            if len(ref_work_lst) < 3:
                for unit in refineries:
                    if unit[0] == i:
                        if unit[1].build_percentage == 1.0:
                            self.get_gas_workers_2(unit[1], i)


    def get_gas_workers_2(self, geyser, enum):
        """ gets the SCV in base 2 and sends them to collect gas """
        for worker in self.worker_jobbs_2:
            if self.worker_jobbs_2[worker] == "mining":
                worker[1].right_click(geyser)
                self.worker_jobbs_2.update({worker: ("collecting_gas", enum)})
                break


    def mine_minerals(self):
        """starts workers to mine resource dose not return anything"""
        for base in self.base_dic:
            mineral_fileds = self.get_mineral_fields(self.base_dic[base])
            if base in self.SCV_dic:
                for workers in self.SCV_dic[base]:
                    workers = (workers[1].id, workers[1])
                    for mineral in mineral_fileds:
                        if workers[1].is_idle and workers in self.SCV_dic[base] \
                        and workers not in self.scouts:
                            if base == 1:
                                self.workers_jobbs.update({(workers[1].id,
                                workers[1]): "mining"})
                                workers[1].right_click(mineral)
                            elif base == 2:
                                self.worker_jobbs_2.update({(workers[1].id,
                                workers[1]): "mining"})
                                workers[1].right_click(mineral)


    def mine_single_worker(self, worker):
        """ sends singel worker to mine """
        mineral = choice(list(self.get_base_minerals()))
        self.workers_jobbs.update({worker: "mining"})
        worker[1].right_click(mineral[1])


    def mine_single_worker_2(self, worker):
        """ sends singel worker to mine ate base 2"""
        mineral = choice(list(self.get_mineral_fields(self.base_dic[2])))
        self.worker_jobbs_2.update({worker: "mining"})
        worker[1].right_click(mineral)


    def train_unit(self, type_of_unit, base="either"):
        """ builds a new worker dose not return anything """
        ccs = []
        all_prod = list(self.get_my_producers(UnitType(type_of_unit, self)))
        for producers in all_prod:
            if str(producers) == "<Unit of type: 'TERRAN_FACTORY'>":
                all_factorys = list(self.get_all_my_units(UnitType(
                UNIT_TYPEID.TERRAN_FACTORY, self)))
                for i in all_factorys:
                    ccs.append(i[1])

        if len(ccs) == 0:
            ccs = list(self.get_my_producers(UnitType(type_of_unit, self)))

        base_lst = []
        if len(ccs) > 0:
            if base == "either":
                for trainer in ccs:
                    if not trainer.is_training:
                        trainer.train(UnitType(type_of_unit, self))
            elif base == 2:
                for bases in self.get_all_my_units(UnitType(
                        UNIT_TYPEID.TERRAN_COMMANDCENTER, self)):
                    if not base_lst:
                        base_lst.append(bases)
                    elif self.base_dic[2].get_ground_distance(
                            bases[1].position) < self.base_dic[2].get_ground_distance(
                            base_lst[0][1].position):
                        base_lst = []
                        base_lst.append(bases)

                if not base_lst[0][1].is_training and base_lst[0][1].is_completed:
                    base_lst[0][1].train(UnitType(UNIT_TYPEID.TERRAN_SCV, self))


    def build_ref_2(self):
        """ Builds refinery in expansion base. """
        miners = []
        for workers in self.worker_jobbs_2:
            if self.worker_jobbs_2[workers] == "mining":
                miners.append(workers)
        ref_type = UnitType(UNIT_TYPEID.TERRAN_REFINERY, self)
        chosen_worker = choice(miners)
        for geys in self.get_geysers(self.base_dic[2]):
            if not self.get_refinery(geys[1]):
                self.worker_jobbs_2.update({chosen_worker: ("building_ref",
                                                            geys[0])})
                chosen_worker[1].build_target(ref_type, geys[1])
                break


    def build_supply_depo(self):
        """ builds a supply depo in vicinity of base  """
        for workers in self.workers_jobbs:
            if self.workers_jobbs[workers] == "building_depo":
                return
        miners = []
        for workers in self.workers_jobbs:
            if self.workers_jobbs[workers] == "mining":
                miners.append(workers)
        ccs = self.get_my_producers(UnitType(UNIT_TYPEID.TERRAN_SCV, self))
        depo_type = UnitType(UNIT_TYPEID.TERRAN_SUPPLYDEPOT, self)
        chosen_worker = choice(miners)
        self.workers_jobbs[chosen_worker] = "building_depo"
        building_pos = self.building_placer.get_build_location_near(
            ccs[0].tile_position, depo_type, 1)
        if self.building_placer.can_build_here(building_pos.x,
                                               building_pos.y, depo_type):
            chosen_worker[1].build(depo_type,
                                   building_pos)


    def squad_handler(self, dic, type, nr_of_units,
                      build_unit=False, unit_typeid=None, defence=False):
        """ Take arguments in order  to handl squads in dictenary in.
        dic structrue : dic = {squad_nr:[(unit.id, unit),(unit.id, unit)]"""

        wanted_unit_lst = []
        unit_lst = []
        for unit in self.get_my_units():
            unit_lst.append(str(unit))
            if str(unit) == type:
                wanted_unit_lst.append((unit.id, unit))


        squad_in_need = []
        for squad in dic:
            in_squad = 0
            for unit in dic[squad]:
                if str(unit[1]) == type or \
                    (str(unit[1]) == "<Unit of type: 'TERRAN_SIEGETANKSIEGED'>" \
                    and type == "<Unit of type: 'TERRAN_SIEGETANK'>"):
                    in_squad += 1
            if in_squad < nr_of_units:
                if defence:
                    if squad == 1 or squad == 2:
                        pass
                    else:
                        squad_in_need.append(squad)
                else:
                    squad_in_need.append(squad)

        for squad in dic:
            if squad not in self.squad_moved_out:
                for worker in dic[squad]:
                    if not worker[1].is_alive:
                        dic[squad].remove(worker)

        for wanted_unit in wanted_unit_lst:
            unit_in_squad = []
            for squad in dic:
                for worker in dic[squad]:
                    unit_in_squad.append(worker)
            if wanted_unit not in unit_in_squad:
                if len(squad_in_need) == 0:
                    dic[len(dic) + 1] = [(wanted_unit)]
                else:
                    dic[sorted(squad_in_need)[0]] += [(wanted_unit)]
    # Informativa funktioner


    def get_refinery(self, geyser: Unit) -> Optional[Unit]:
        """
        Returns: A refinery which is on top of unit `geyser`
        if any, None otherwise
        """

        def squared_distance(p1: Point2D, p2: Point2D) -> float:
            return (p1.x - p2.x) ** 2 + (p1.y - p2.y) ** 2

        for unit in self.get_my_units():
            if unit.unit_type.is_refinery and squared_distance(unit.position,
            geyser.position) < 1:
                return unit
        return None


    def can_afford(self, unit_type: UnitType):
        """ Returns True if there are an sufficient amount of minerals,
            gas and supply to build the given unit_type, False otherwise """
        return self.minerals >= unit_type.mineral_price \
            and self.gas >= unit_type.gas_price \
            and (self.max_supply - self.current_supply) >= unit_type.supply_required


    # Dictionary hjälpfunktion
    def worker_in_dic(self):
        """ looks for miners in worker dict """
        for unit in self.SCV_dic[1]:
            dic_lst = []
            for key in self.workers_jobbs:
                if key[0] == unit[0]:
                    dic_lst.append(unit)
            if len(dic_lst) == 0:
                self.workers_jobbs.update({unit: 'mining'})


    # debug funktioner
    def print_debug(self):
        """ Early way of showing info on units """
        # units
        enumerate_unit_lst = self.get_all_my_units()
        for unit in enumerate_unit_lst:
            self.map_tools.draw_text(unit[1].position,
             str(unit[1].unit_type
            ) + "id :" + str(unit[1].id) + "i:" + str(unit[0]),
             Color(255, 255, 255))
        # min
        for base_info in self.get_base_minerals():
            self.map_tools.draw_text(base_info[1].position,
             str(base_info[1]) + "id :" + str(
                 base_info[1].id) + "i:" + str(base_info[0]),
             Color(255, 255, 255))
        # gey
        for gey_info in self.get_base_geysers():
            self.map_tools.draw_text(gey_info[1].position,
             str(gey_info[1]) + "id :" + str(
                 gey_info[1].id) + "i:" + str(gey_info[0]),
             Color(255, 255, 255))


    def print_unit_info(self):
        """  Prints unit assignment at unit position. """
        scv_lst = []
        scv_lst_2 = []
        for worker in self.workers_jobbs:
            scv_lst.append(worker)
        for unit in scv_lst:
            self.map_tools.draw_text(unit[1].position,
            str(self.workers_jobbs[unit]), Color(255, 255, 255))

        for work in self.worker_jobbs_2:
            scv_lst_2.append(work)
        for unit_2 in scv_lst_2:
            self.map_tools.draw_text(unit_2[1].position,
            str(self.worker_jobbs_2[unit_2]), Color(255, 255, 255))

        for squad in self.army_dic:
            for unit in self.army_dic[squad]:
                if unit[1].is_alive:
                    if squad < 3:
                        army_output = "defending" + str(squad)
                    elif squad >= 3:
                        army_output = "attacking" + str(squad)
                    self.map_tools.draw_text(unit[1].position,
                    str(army_output), Color(255, 255, 255))


    def print_unit_overview_new(self):
        """ Prints unit assignments in the left upper corner of the screen. """
        # Base 1 assignments.
        self.map_tools.draw_text_screen(0.01, 0.01, "Unit assignments: ", Color(255, 255, 255))
        self.map_tools.draw_text_screen(0.01, 0.03, "------------------", Color(255, 255, 255))
        self.map_tools.draw_text_screen(0.01, 0.05, "mining:                   {}".format
                                        (sum(1 for i in self.workers_jobbs if self.workers_jobbs[i] == "mining")))
        self.map_tools.draw_text_screen(0.01, 0.07, "building_depo:            {}".format
                                        (sum(1 for i in self.workers_jobbs if
                                             self.workers_jobbs[i] == "building_depo")))
        self.map_tools.draw_text_screen(0.01, 0.09, "building_ref:             {}".format
        (sum(1 for i in self.workers_jobbs if self.workers_jobbs[i] ==("building_ref", 0)
             or self.workers_jobbs[i] == ("building_ref", 1))))
        self.map_tools.draw_text_screen(0.01, 0.11, "collecting_gas:           {}".format
        (sum(1 for i in self.workers_jobbs if self.workers_jobbs[i] ==("collecting_gas", 0)
             or self.workers_jobbs[i] == ("collecting_gas", 1))))
        self.map_tools.draw_text_screen(0.01, 0.13, "building_barracks         {}".format
        (sum(1 for i in self.workers_jobbs if self.workers_jobbs[i] == "building_UNIT_TYPEID.TERRAN_BARRACKS")))
        self.map_tools.draw_text_screen(0.01, 0.15, "scouting:                 {}".format
        (sum(1 for i in self.workers_jobbs if self.workers_jobbs[i] == "scouting")))
        self.map_tools.draw_text_screen(0.01, 0.17, "building_expansion:       {}".format                          (
        sum(1 for i in self.workers_jobbs if self.workers_jobbs[i] == "building_expansion")))

        # Base 2 assignments
        if len(self.worker_jobbs_2) > 0:
            self.map_tools.draw_text_screen(0.01, 0.21, "Unit assignments 2: ", Color(255, 255, 255))
            self.map_tools.draw_text_screen(0.01, 0.23, "------------------", Color(255, 255, 255))
            self.map_tools.draw_text_screen(0.01, 0.25, "mining:                   {}".format
            (sum(1 for i in self.worker_jobbs_2 if self.worker_jobbs_2[i] == "mining")))
            self.map_tools.draw_text_screen(0.01, 0.27, "building_ref:             {}".format
            (sum(1 for i in self.worker_jobbs_2 if self.worker_jobbs_2[i] == ("building_ref", 0)
                 or self.worker_jobbs_2[i] == ("building_ref", 1))))
            self.map_tools.draw_text_screen(0.01, 0.29, "collecting_gas:           {}".format
            (sum(1 for i in self.worker_jobbs_2 if self.worker_jobbs_2[i] == ("collecting_gas", 0)
                 or self.worker_jobbs_2[i] == ("collecting_gas", 1))))

        # Soldier assignments
        if len(self.army_dic) > 0:
            defend = 0
            attack = 0
            for squad in self.army_dic:
                if squad < 3:
                    defend += len(self.army_dic[squad])
                elif squad > 2:
                    attack += len(self.army_dic[squad])

            self.map_tools.draw_text_screen(0.01, 0.33, "Soldier assignments: ", Color(255, 255, 255))
            self.map_tools.draw_text_screen(0.01, 0.35, "--------------------", Color(255, 255, 255))
            self.map_tools.draw_text_screen(0.01, 0.37, "Defending: {}".format(defend), Color(255, 255, 255))
            self.map_tools.draw_text_screen(0.01, 0.39, "Attacking: {}".format(attack), Color(255, 255, 255))


    def base_expansion(self):
        """ Builds an expansion base. """
        expansion_base = self.base_location_manager.get_next_expansion(
        PLAYER_SELF)
        expansion_location = expansion_base.depot_position

        miners = []
        for workers in self.workers_jobbs:
            if self.workers_jobbs[workers] == "mining":
                miners.append(workers)
        rnd_worker = choice(miners)
        rnd_worker[1].move(expansion_location)
        rnd_worker[1].build(UnitType(UNIT_TYPEID.TERRAN_COMMANDCENTER, self),
        expansion_location)

        self.base_dic[len(self.base_dic) + 1] = expansion_base
        self.workers_jobbs[rnd_worker] = "building_expansion"


    def get_mineral_fields(self, base_location: BaseLocation):
        """ Given a base_location, this method will find and return a
        list of all mineral fields (Unit) for that base """
        mineral_fields = []
        for mineral_field in base_location.mineral_fields:
            for unit in self.get_all_units():
                if unit.unit_type.is_mineral \
                        and mineral_field.tile_position.x == unit.tile_position.x \
                        and mineral_field.tile_position.y == unit.tile_position.y:
                    mineral_fields.append(unit)
        return mineral_fields


    def get_geysers(self, base_location: BaseLocation):
        """ gets all geysers and returns them in a enumerated list """
        geysers = []
        for geyser in base_location.geysers:
            for unit in self.get_all_units():
                if unit.unit_type.is_geyser \
                        and geyser.tile_position.x == unit.tile_position.x \
                        and geyser.tile_position.y == unit.tile_position.y:
                    geysers.append((unit.id, unit))

        sorted_gey = sorted(geysers)
        for gey in range(len(sorted_gey)):
            sorted_gey[gey] = sorted_gey[gey][1]
        return enumerate(sorted_gey)


    def start_mining(self):
        """ makes miners start mining in the begning """
        # 1
        if len(self.SCV_dic) == 0:
            self.SCV_dic.update({1: []})
            self.squad_handler(self.SCV_dic, "<Unit of type: 'TERRAN_SCV'>", 22)
        self.base_dic[1] = self.base_location_manager.\
            get_player_starting_base_location(PLAYER_SELF)

        self.mine_minerals()


    def starting_base_train_scv(self):
        """ start train scv for first base """
        # Gammal train worker
        if self.can_afford(UnitType(UNIT_TYPEID.TERRAN_SCV, self)):
            if len(self.workers_jobbs) < 21:
                self.train_unit(UNIT_TYPEID.TERRAN_SCV)


    def new_train_scv(self):
        """ for training scv and add them to working dict"""
        # Ny train workers
        if len(list(self.get_all_my_units(
                UnitType(UNIT_TYPEID.TERRAN_COMMANDCENTER, self)))) > 1:
            if len(self.worker_jobbs_2) < 22:
                self.train_unit(UNIT_TYPEID.TERRAN_SCV, 2)
        if len(self.worker_jobbs_2) > 5:
            if self.can_afford(UnitType(UNIT_TYPEID.TERRAN_REFINERY,
                                        self)) and len(
                    self.get_my_refineries_enum_2()) < 2:
                for workers in self.worker_jobbs_2:
                    if self.worker_jobbs_2[workers][0] == 'building_ref':
                        return
                self.build_ref_2()

            self.ref_need_workers_2()


    def refaineris_onstep(self):
        """ looks for condition for when to build
        and collect gas and ref"""
        # ref
        if ('building_ref', 1) in self.worker_jobbs_2.values() or \
        ('building_ref', 0) in self.worker_jobbs_2.values():
            my_refineries = self.get_my_refineries_enum_2()
            for worker in self.worker_jobbs_2:
                if self.worker_jobbs_2[worker][0] == 'building_ref':
                    for ref in my_refineries:
                        if ref[0] == self.worker_jobbs_2[worker][1]:
                            if ref[1].is_completed:
                                self.mine_single_worker_2(worker)

        if self.can_afford(UnitType(UNIT_TYPEID.TERRAN_REFINERY,
        self)) and len(self.get_my_refineries()) < 2:
            builder_lst = []
            for workers in self.workers_jobbs:
                if self.workers_jobbs[workers][0] == 'building_ref':
                    builder_lst.append(True)
            if len(builder_lst) == 0:
                ref_id = UNIT_TYPEID.TERRAN_REFINERY
                self.build(ref_id)
        # get gass workers
        if ('building_ref', 1) in self.workers_jobbs.values():

            my_refineries_e = self.get_my_refineries_enum()
            for worker in self.workers_jobbs:
                if self.workers_jobbs[worker][0] == "building_ref":
                    for ref in my_refineries_e:
                        if ref[0] == self.workers_jobbs[worker][1]:
                            if ref[1].is_completed:
                                self.workers_jobbs[worker] = ("collecting_gas", 1)

        elif ('building_ref', 0) in self.workers_jobbs.values():
            my_refineries_e = self.get_my_refineries_enum()

            for worker in self.workers_jobbs:
                if self.workers_jobbs[worker][0] == "building_ref":
                    for ref in my_refineries_e:
                        if ref[0] == self.workers_jobbs[worker][1]:
                            if ref[1].is_completed:
                                self.workers_jobbs[worker] = ("collecting_gas", 0)


    def onstep_marines(self):
        """ comands for when to do things with army """
        # marines
        unit_lst = []
        marine_lst = []
        barrack_lst = []
        for unit in self.get_my_units():
            unit_lst.append(str(unit))
            if str(unit) == "<Unit of type: 'TERRAN_MARINE'>":
                marine_lst.append((unit.id, unit))

            if str(unit) == "<Unit of type: 'TERRAN_BARRACKS'>":
                barrack_lst.append(unit)

        barrack_type = UNIT_TYPEID.TERRAN_BARRACKS
        if self.can_afford(UnitType(barrack_type,self)) and \
        "building_UNIT_TYPEID.TERRAN_BARRACKS" not in self.workers_jobbs.values()\
        and len(barrack_lst) < 3 and len(self.base_dic) > 1:
            if list(self.get_all_my_units(UnitType(UNIT_TYPEID.TERRAN_SUPPLYDEPOT,
            self)))[0][1].is_completed:
                self.build(barrack_type)

        build_marines = self.can_afford(
            UnitType(UNIT_TYPEID.TERRAN_MARINE, self)) and\
            "<Unit of type: 'TERRAN_BARRACKS'>" in unit_lst
        marine_typeid = UNIT_TYPEID.TERRAN_MARINE

        # len(list(self.get_all_my_units(
        # UnitType(UNIT_TYPEID.TERRAN_COMMANDCENTER, self)))) >= 2 and

        squad_size = 0
        for squad in self.army_dic:
            if squad == 1 or squad == 2:
                if not len(self.army_dic[squad]) == 10:
                    squad_size += 1
        if squad_size == 0 and len(self.army_dic) > 2:
            self.squad_handler(self.army_dic,
            "<Unit of type: 'TERRAN_MARINE'>", 12,
            build_marines, marine_typeid, True)

        else:
            self.squad_handler(self.army_dic,
            "<Unit of type: 'TERRAN_MARINE'>", 8,
            build_marines, marine_typeid)

        self.ref_need_workers()
        self.worker_in_dic()


    def build_tanks(self):
        """ function uses factorry build tanks and
        puts wanted a mount tanks in squads """
        unit_lst = []
        for unit in self.get_my_units():
            unit_lst.append(str(unit))
        if "<Unit of type: 'TERRAN_FACTORY'>" in unit_lst:

            if self.can_afford(UnitType(UNIT_TYPEID.TERRAN_SIEGETANK, self)):

                marine_typeid = UNIT_TYPEID.TERRAN_SIEGETANK
                build_tank = False
                squad_len = 10
                for squad in list(self.army_dic):
                    squad_tank_lst = []
                    squad_marines = []
                    fac = list(self.get_my_producers(UnitType(marine_typeid, self)))
                    for unit in self.army_dic[squad]:
                        if str(unit[1]) == "<Unit of type: 'TERRAN_MARINE'>":
                            squad_marines.append(unit)
                        if str(unit[1]) == "<Unit of type: 'TERRAN_SIEGETANK'>" or str(
                                unit[1]) == "<Unit of type: 'TERRAN_SIEGETANKSIEGED'>":
                            squad_tank_lst.append(unit)

                    if len(squad_tank_lst) < 2 and squad <= 2 \
                    and not fac[0].is_training and len(squad_marines) == 8:
                        self.train_unit(marine_typeid)
                        squad_len = 2

                    elif len(squad_tank_lst) < 4 and squad > 2 \
                    and not fac[0].is_training and len(squad_marines) == 12:
                        self.train_unit(marine_typeid)
                        squad_len = 2

        squad_size = 0
        for squad in self.army_dic:
            if squad == 1 or squad == 2:
                if not len(self.army_dic[squad]) >= 10:
                    squad_size += 1
        if squad_size == 0 and len(self.army_dic) > 2:
            self.squad_handler(self.army_dic, "<Unit of type: 'TERRAN_SIEGETANK'>",
            4, False, UNIT_TYPEID.TERRAN_SIEGETANK, True)
        else:
            self.squad_handler(self.army_dic, "<Unit of type: 'TERRAN_SIEGETANK'>",
            2, False, UNIT_TYPEID.TERRAN_SIEGETANK)


    def move_army(self, unit_type_id, p1, p2, p2south, p2north):
        """ function for moving soldiers to points """
        p_base = self.start_location
        if (abs(p1.x - p_base.x) ** 2 + abs(p1.y - p_base.y) ** 2) ** 0.5 > (
                abs(p2.x - p_base.x) ** 2 + abs(p2.y - p_base.y) ** 2) ** 0.5:
            army = list(self.get_all_my_units(UnitType(unit_type_id, self)))
            for unit in army:
                unit = (unit[1].id, unit[1])
                if len(self.army_dic) > 1:
                    if unit not in self.move_list:
                        if unit in self.army_dic[1]:
                            unit[1].move(p2)

                            self.move_list.append(unit)

                        elif unit in self.army_dic[2]:
                            unit[1].move(p2south)

                            self.move_list.append(unit)
                else:
                    if unit not in self.move_list:
                        if unit in self.army_dic[1]:
                            unit[1].move(p2)
                            self.move_list.append(unit)
        else:
            army = list(self.get_all_my_units(UnitType(unit_type_id, self)))
            for unit in army:
                unit = (unit[1].id, unit[1])
                if len(self.army_dic) > 1:
                    if unit not in self.move_list:
                        if unit in self.army_dic[1]:
                            unit[1].move(p1)

                            self.move_list.append(unit)
                        elif unit in self.army_dic[2]:

                            unit[1].move(p2north)
                            self.move_list.append(unit)
                else:
                    if unit not in self.move_list:
                        if unit in self.army_dic[1]:
                            unit[1].move(p1)
                            self.move_list.append(unit)


    def move_marines_to_points(self):
        """ function moves units to points for defence """
        # marines
        p1 = Point2D(33, 120)
        p2 = Point2D(119, 47)
        p2south = Point2D(115, 56)
        p2north = Point2D(38, 110)

        self.move_army(UNIT_TYPEID.TERRAN_MARINE, p1, p2, p2south, p2north, )

        # move tanks
        p1 = Point2D(40, 121)
        p2 = Point2D(112, 47)
        p2south = Point2D(115, 56)
        p2north = Point2D(38, 110)

        self.move_army(UNIT_TYPEID.TERRAN_SIEGETANK, p1, p2, p2south, p2north, )

        corect_pos_lst = [p1, p2, p2north, p2south]

        for squad in self.army_dic:
            if squad == 1 or squad == 2:
                for unit in self.army_dic[squad]:
                    for point in corect_pos_lst:
                        aprox_dis = self.map_tools.get_ground_distance(unit[1].position, point)
                        if str(unit[1]) == "<Unit of type: 'TERRAN_SIEGETANK'>" \
                        and aprox_dis <= 10 and unit[1].is_idle:
                            unit[1].morph(UnitType(
                            UNIT_TYPEID.TERRAN_SIEGETANKSIEGED, self))


    def build_more_bases(self):
        """ changes base 2 to be a new base at new location """
        if len(self.base_dic) > 1:
            if len(self.get_mineral_fields(self.base_dic[2])) == 0:
                self.base_dic[2] = self.base_location_manager.get_next_expansion(PLAYER_SELF)
                self.base_expansion()


    def build_Factory(self):
        """ function to tell a worker to build a factory and
        build a lab in order to build tanks  """
        unit_lst = []
        factory_lst = []
        for unit in self.get_my_units():
            unit_lst.append(str(unit))
            if str(unit) == "<Unit of type: 'TERRAN_FACTORY'>":
                factory_lst.append(unit)
        if "<Unit of type: 'TERRAN_BARRACKS'>" in unit_lst and \
        self.can_afford(UnitType(UNIT_TYPEID.TERRAN_FACTORY, self)) and len(
            factory_lst) < 2 and "building_UNIT_TYPEID.TERRAN_FACTORY" not \
                in self.workers_jobbs.values():
            for barracks in self.get_all_my_units(UnitType(UNIT_TYPEID.TERRAN_BARRACKS, self)):
                if barracks[1].is_completed:
                    self.build(UNIT_TYPEID.TERRAN_FACTORY)

        if "<Unit of type: 'TERRAN_FACTORY'>" in unit_lst and self.can_afford(
                UnitType(UNIT_TYPEID.TERRAN_TECHLAB, self)):
            for i in range(len(factory_lst)):
                list(self.get_all_my_units(UnitType(UNIT_TYPEID.TERRAN_FACTORY, self)))[i][1].train(
                    UnitType(UNIT_TYPEID.TERRAN_FACTORYTECHLAB, self))


    def scout_bases(self):
        """ takes a singel worker and sends him to scout the map """
        scout = []
        p9 = Point2D(30, 133)
        p1 = Point2D(120, 36)
        p2 = Point2D(118, 56)
        p8 = Point2D(32, 109)

        p_base = self.start_location
        base1 = (abs(p9.x - p_base.x) ** 2 + abs(p9.y - p_base.y) ** 2) ** 0.5 > \
                (abs(p1.x - p_base.x) ** 2 + abs(p1.y - p_base.y) ** 2) ** 0.5

        for workers in self.workers_jobbs:
            if self.workers_jobbs[workers] == 'mining':
                scout.append(workers)
        chosen_worker = choice(scout)

        if base1:
            chosen_worker[1].move(p2)
            self.workers_jobbs[chosen_worker] = "scouting"
            self.scouts.append(chosen_worker)
            self.scouts += [str(Point2D(118, 56))]

        if not base1:
            chosen_worker[1].move(p8)
            self.workers_jobbs[chosen_worker] = "scouting"
            self.scouts.append(chosen_worker)
            self.scouts += [str(Point2D(32, 109))]


    def scout_map(self):
        """ moves a scaut to difrent points for map surveillance """
        p1 = Point2D(120, 36)
        p2 = Point2D(118, 56)
        p3 = Point2D(93, 44)
        p4 = Point2D(119, 82)
        p5 = Point2D(33, 82)
        p6 = Point2D(63, 121)
        p7 = Point2D(87, 134)
        p8 = Point2D(32, 109)
        p9 = Point2D(30, 133)
        p10 = Point2D(26, 32)
        p11 = Point2D(59, 34)
        p12 = Point2D(69, 55)
        p13 = Point2D(89, 72)
        p14 = Point2D(85, 112)
        p15 = Point2D(125, 134)
        p16 = Point2D(63, 95)

        coords = [p9, p8, p5, p10, p11, p12, p13, p3, p1, p2, p4, p16, p14, p15, p7, p6]

        for coord in range(len(coords)):
            if self.scouts[1] == (str(coords[coord])) and \
                    ((self.scouts[0][1].position.x - coords[coord].x) ** 2 +
                     (self.scouts[0][1].position.y - coords[coord].y) ** 2) ** 0.5 < 3:
                if coord == 15:
                    self.scouts[0][1].move(coords[0])
                    self.scouts[1] = str(coords[0])

                else:
                    self.scouts[0][1].move(coords[coord + 1])
                    self.scouts[1] = str(coords[coord + 1])

        if not self.scouts[0][1].is_alive:
            self.workers_jobbs.pop(self.scouts[0])
            self.scouts = []


    def offensive(self):
        """ function for attacking the enemy """
        target = []
        for units in self.get_all_units():
            if str(units.player) == "1":
                target.append(units)
        enemy_bases = list(
        self.base_location_manager.get_occupied_base_locations(PLAYER_ENEMY))
        for key in self.army_dic:
            if len(self.army_dic[key]) > 15:
                if key > 2:
                    for soldier in self.army_dic[key]:
                        if soldier not in self.squad_attacking:
                            for bases in enemy_bases:
                                soldier[1].attack_move(bases.position)
                                self.im_attacking.append((soldier, bases.position))
                                self.squad_attacking.append(soldier)
                                self.squad_moved_out.append(key)
                                break

        for i in range(len(self.im_attacking)):
            if self.im_attacking[i][0][1].is_idle and \
            (self.im_attacking[i][0][1].position.x - self.im_attacking[i][1].x
            ) ** 2 + (self.im_attacking[i][0][1].position.y - self.im_attacking[i][1].y) ** 2 < 3:
                self.black_listed_bases.append(self.im_attacking[i][1])
                for bases in enemy_bases:
                    #print(bases)
                    #print(self.black_listed_bases)
                    if bases.position not in self.black_listed_bases:
                        self.im_attacking[i][0][1].attack_move(bases.position)
                        self.im_attacking[i] = (self.im_attacking[i][0], bases.position)
                        #print("går mot ny bas som inte är blacklisted")
                        break
            elif self.im_attacking[i][0][1].is_idle:
                self.im_attacking[i][0][1].attack_move(self.im_attacking[i][1])
                            # on step (duh)


    def put_me_in_squad_plz(self):
        """ puts a marine in a squad that dose not in one """
        squad_in_need = []
        for squad in self.army_dic:
            marines_in_squad = 0
            if squad < 3:
                for marines in squad:
                    if str(marines) == "<Unit of type: 'TERRAN_MARINE'>":
                        marines_in_squad += 1
                if marines_in_squad < 8:
                    squad_in_need.append(squad)

        for marines in self.get_all_my_units(UnitType(UNIT_TYPEID.TERRAN_MARINE, self)):
            apperance = 0
            for squads in self.army_dic:
                if marines in self.army_dic[squads]:
                    apperance += 1
            if apperance > 0:
                pass


    def siege_mode(self):
        """ Puts tanks in siege mode when close to enemies. """

        def squared_distance(unit_1, unit_2):
            p1 = unit_1.position
            p2 = unit_2.position
            return ((p1.x - p2.x) ** 2 + (p1.y - p2.y) ** 2)**0.5

        enemies = []
        tanks = []

        for unit in self.get_all_units():
            if unit.player == PLAYER_ENEMY:
                enemies.append(unit)
        for tank in self.get_all_my_units(UnitType(UNIT_TYPEID.TERRAN_SIEGETANK, self)):
            tanks.append(tank)

        for siege in tanks:
            for unit in enemies:
                if squared_distance(tank[1], unit) <= 7:
                    siege[1].morph(UnitType(UNIT_TYPEID.TERRAN_SIEGETANKSIEGED, self))


    def on_step(self):
        """ functions that will be run evry tick """
        IDABot.on_step(self)
        self.counter += 1

        self.print_unit_info()
        self.print_unit_overview_new()
        #print(self.base_dic)
        self.refaineris_onstep()

        if self.counter % 10 == 0:

            self.start_mining()

            if len(self.workers_jobbs) < 21:
                self.starting_base_train_scv()

            if self.current_supply >= self.max_supply - 5 and self.can_afford(
                    UnitType(UNIT_TYPEID.TERRAN_SUPPLYDEPOT, self)):
                self.build_supply_depo()

            self.onstep_marines()

            self.move_marines_to_points()



            if len(self.base_dic) >= 2 and len(list(self.get_all_my_units(UnitType(
                    UNIT_TYPEID.TERRAN_COMMANDCENTER, self)))) < 2 and \
                    "building_expansion" not in self.workers_jobbs.values():
                del(self.base_dic[2])

            self.build_tanks()
            if self.minerals > 400 and len(self.base_dic) < 2:
                self.base_expansion()
            self.squad_handler(self.SCV_dic, "<Unit of type: 'TERRAN_SCV'>", 23)

            if len(self.workers_jobbs) > 20 and self.can_afford(
                    UnitType(UNIT_TYPEID.TERRAN_BUNKER, self)):
                self.build_bunker()

            if len(self.bunk_lst) > 0:
                self.enter_bunk()

            self.build_Factory()

            self.build_more_bases()

            self.new_train_scv()

            if len(self.scouts) == 0:
                self.scout_bases()
            self.ref_need_workers()
            self.worker_in_dic()
            self.scout_map()
            unit_lst = []
            tank_lst = []
            marine_lst = []
            for unit in self.get_my_units():
                unit_lst.append(str(unit))
                if str(unit) == "<Unit of type: 'TERRAN_MARINE'>":
                    marine_lst.append(unit)
                elif str(unit) == "<Unit of type: 'TERRAN_SIEGETANK'>" or str(
                        unit) == "<Unit of type: 'TERRAN_SIEGETANKSIEGED'>":
                    tank_lst.append(unit)
            if len(tank_lst) == 0:
                tank_lst.append("decoy")
            if len(marine_lst) == 0 or len(marine_lst) / len(tank_lst) < 3:
                self.train_unit(UNIT_TYPEID.TERRAN_MARINE)
            self.train_unit(UNIT_TYPEID.TERRAN_SIEGETANK)
            # print(self.army_dic)

        if self.counter % 30 == 0:
            if 4 in self.army_dic:
                self.offensive()


def main():
    """ the main function that starts the game and runs the bot """
    coordinator = Coordinator(r"D:\starcraft\StarCraft II\StarCraft II\Versions\Base63454\SC2_x64.exe")
    bot1 = MyAgent()
    bot2 = MyAgent()
    participant_1 = create_participants(Race.Terran, bot1)
    #participant_2 = create_participants(Race.Terran, bot2)
    participant_2 = create_computer(Race.Random, Difficulty.Easy)
    # coordinator.set_real_time(True)
    coordinator.set_participants([participant_1, participant_2])
    coordinator.launch_starcraft()
    path = os.path.join(os.getcwd(), "maps", "InterloperTest.SC2Map")
    coordinator.start_game(path)
    while coordinator.update():
        pass


if __name__ == "__main__":
    main()
"""
-------------
BUG list :
-------------

------------
To do lst
------------

    * pep8

"""
