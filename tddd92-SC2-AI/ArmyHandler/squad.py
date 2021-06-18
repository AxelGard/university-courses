class Squad():
    def __init__(self, handler, role,  hold_point):
        self.handler = handler
        if len(handler.squads[role]) > 0:
            self.id = handler.squads[role][-1].id + 1
        else:
            self.id = 1
        self.units = {
            handler.unit.MARINE: [],
            handler.unit.SIEGETANK: []
        } # type : [unit]
        self.role = role
        self.composition = self.handler.composition(self.role)
        self.full = False
        self.hold_point = hold_point
        self.on_mission = False

    def assign(self, unit):
        self.units[unit.unit_type].append(unit)

    def train(self):
        self.full = True
        for unit_type, amount in self.composition.items():
            if len(self.units[unit_type]) < amount:
                self.full = False
                #print("Train " + unit_type.name)
                self.handler.train(unit_type, amount - len(self.units[unit_type]))

    def move_to_hold_point(self):
        self.move(self.hold_point)

    def move(self, pos):
        for _, units in self.units.items():
            for unit in units:
                if util.distance(unit.position, pos) > 3:
                    if unit.unit_type == self.handler.unit.SIEGETANKSIEGED:
                        unit.morph(self.handler.unit.SIEGETANK)
                    unit.move(pos)
                elif unit.unit_type == self.handler.unit.SIEGETANK:
                    unit.morph(self.handler.unit.SIEGETANKSIEGED)


    def attack_move(self, pos):
        for unit_type, units in self.units.items():
            for unit in units:
                if unit_type == self.handler.unit.SIEGETANK:
                    unit.morph(self.handler.unit.SIEGETANK)
                if unit.is_idle:
                    unit.attack_move(pos)

    def is_empty(self):
        return len(self.units[self.handler.unit.MARINE]) == 0 and len(self.units[self.handler.unit.SIEGETANK]) == 0
