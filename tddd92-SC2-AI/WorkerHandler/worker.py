from .assignment import Assignment
import util

class Worker():
    def __init__(self, unit):
        self.unit = unit
        self.assignment = Assignment.IDLE
        self.assignment_details = {}

    def __getattr__(self, key):
        """ If attribute does not exist in Worker class ask
        library.Unit class for it"""
        return self.unit.__getattribute__(key)

    def set_assignment(self, assignment):
        self.assignment = assignment
        self.assignment_details = {}

    def build(self, building, cord):
        self.assignment = Assignment.BUILDING
        self.assignment_details["building_type"] = building
        self.unit.build(building, cord)

    def build_target(self, building, target):
        self.assignment = Assignment.BUILDING
        self.assignment_details["building_type"] = building
        self.unit.build_target(building, target)

    def is_collecting_gas(self, agent):
        """ Returns: True if Worker is collecting gas, False otherwise """
        for refinery in agent.buildingHandler.get_buildings(agent.buildingHandler.building.REFINERY):
            if refinery.is_completed and util.squared_distance(self.position, refinery.position) < 3:
                return True
        return False
