class Handler:
    def __init__(self, agent):
        self.agent = agent

    def on_game_start(self):
        raise NotImplementedError(self.__class__.__name__ + " needs to implement on_game_start")

    def on_step(self):
        raise NotImplementedError(self.__class__.__name__ + " needs to implement on_step")
