
from typing import Optional
from library import *
from .worker import Worker
from .assignment import Assignment
from handler import Handler
import util
import config

class WorkerHandler(Handler):
    def __init__(self, agent):
        super().__init__(agent)
        self.workers = {} #key = id (int), value = worker (library.Unit)

    def on_game_start(self):
        pass

    def on_step(self):
        if (len(self.get_workers_assigned_to(Assignment.MINERALS)) + len(self.get_workers_assigned_to(Assignment.BUILDING)) < 16*len(self.agent.get_bases())):
            self.train_worker()
        for id, worker in self.workers.items():
            if not worker.is_alive:
                del self.workers[id]
                break
            self.agent.map_tools.draw_text(worker.position, worker.assignment.name, config.COLOR)
            if worker.is_idle or (worker.is_collecting_gas(self.agent) and worker.assignment.BUILDING):
                worker.set_assignment(Assignment.IDLE)
            if worker.assignment == Assignment.IDLE:
                if len(self.get_workers_assigned_to(Assignment.SCOUT)) < 1:
                    worker.set_assignment(Assignment.SCOUT)
                else:
                    base = self.find_base(worker.position)
                    worker.right_click(self.agent.get_mineral_field(base))
                    worker.assignment = Assignment.MINERALS
                    worker.assignment_details["base"] = base

        self.assign_workers_to_gas()

    def find_base(self, position):
        for base in self.agent.get_bases():
            if len(self.get_base_mineral_workers(base)) < 16:
                return base
        return self.agent.get_bases()[0] # Maybe use something like get_closest_base()

    def get_scouts(self):
        return self.get_workers_assigned_to(Assignment.SCOUT)

    def get_base_mineral_workers(self, base):
        base_mineral_workers = []
        for id, worker in self.workers.items():
            if worker.assignment_details.get("base", False) == base:
                base_mineral_workers.append(worker)
        return base_mineral_workers

    def assign_workers_to_gas(self):
        current_workers = self.get_workers_assigned_to(Assignment.GAS)
        assigned_workers = {} #refinery id : [] workers
        for worker in current_workers:
            refinery_id = worker.assignment_details["refinery_id"]
            if assigned_workers.get(refinery_id, None) == None:
                assigned_workers[refinery_id] = [worker]
            else:
                assigned_workers[refinery_id].append(worker)
        for refinery in self.agent.buildingHandler.get_buildings(self.agent.buildingHandler.building.REFINERY):
            if not refinery.is_completed:
                continue
            for _ in range(3 - len(assigned_workers.get(refinery.id, []))):
                worker = self.get_worker_near(refinery.position)
                worker.right_click(refinery)
                worker.set_assignment(Assignment.GAS)
                worker.assignment_details["refinery_id"] = refinery.id


    def include_worker(self, worker):
        if self.workers.get(worker.id, None) == None:
            worker_ = Worker(worker)
            self.workers[worker_.id] = worker_

    def get_worker(self):
        for id, worker in self.workers.items():
            if worker.assignment == Assignment.IDLE:
                return worker
        for id, worker in self.workers.items():
            if worker.assignment == Assignment.MINERALS:
                return worker
        # TODO: find more workers and handel None
        return None

    def get_workers_assigned_to(self, assignment):
        workers = []
        for id, worker in self.workers.items():
            if worker.assignment == assignment:
                workers.append(worker)
        return workers

    def get_workers_as_list(self):
        workers = []
        for id, worker in self.workers.items():
            workers.append(worker)
        return workers

    def get_workers_building(self, building):
        workers = []
        for id, worker in self.workers.items():
            if worker.assignment == Assignment.BUILDING and\
               worker.assignment_details["building_type"] == building:
                workers.append(worker)
        return workers

    def get_worker_near(self, position):
        """ Returns the closest worker to the given position
        that is collecting minerals """
        def compare(worker):
            """ sort first based on assignment then based on distance to position """
            return (int(worker.assignment), util.squared_distance(position, worker.position))

        workers = self.get_workers_as_list()
        if not workers:
            return None
        workers.sort(key = compare)
        return workers[0]

    def get_worker_by_id(self, id):
        return self.workers.get(id, None)

    def get_assignment_numbers(self):
        assignment_numbers = {}
        for id, worker in self.workers.items():
            if worker.assignment == Assignment.BUILDING:
                assignment = worker.assignment_details["building_type"].name.replace("TERRAN_", "")
                assignment_numbers[assignment] = assignment_numbers.get(assignment, 0) + 1
            else:
                assignment_numbers[worker.assignment.name] = assignment_numbers.get(worker.assignment.name, 0) + 1
        return assignment_numbers

    def train_worker(self):
        SCV = UnitType(UNIT_TYPEID.TERRAN_SCV, self.agent)
        if util.can_afford(self.agent, SCV):
            for building in util.get_my_producers(self.agent, SCV):
                if not building.is_training:
                    building.train(SCV)
