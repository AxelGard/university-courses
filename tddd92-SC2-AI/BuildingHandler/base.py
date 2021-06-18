class Base():
    def __init__(self, agent, base_location):
        self.base_location = base_location
        self.agent = agent
        self.mineral_fields = []
        self.geysers = []
        self.compute_mineral_fields()
        self.compute_geysers()

    def get_mineral_fields(self):
        return self.mineral_fields

    def get_mineral_field(self):
        return self.mineral_fields[0]

    def get_geysers(self):
        return self.geysers

    # TODO: combine the computes

    def compute_mineral_fields(self): # WARNING: use with caution, very slow
        self.mineral_fields = []
        for mineral_field in self.base_location.mineral_fields:
            for unit in self.agent.get_all_units():
                if unit.unit_type.is_mineral \
                        and mineral_field.tile_position.x == unit.tile_position.x\
                        and mineral_field.tile_position.y == unit.tile_position.y:
                    self.mineral_fields.append(unit)


    def compute_geysers(self): # WARNING: use with caution, very slow
        self.geysers = []
        for geyser in self.base_location.geysers:
            for unit in self.agent.get_all_units():
                if unit.unit_type.is_geyser \
                        and geyser.tile_position.x == unit.tile_position.x \
                        and geyser.tile_position.y == unit.tile_position.y:
                    self.geysers.append(unit)
