
from enum import IntEnum

class Assignment(IntEnum):
    """ ordered based on priority when asking for a worker """
    IDLE = 0
    MINERALS = 1
    GAS = 2
    SCOUT = 3
    BUILDING = 4
    ATTACKING = 5
