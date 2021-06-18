from typing import Optional
from library import *
from enum import IntEnum

class BasePosition(IntEnum):
    UPPERLEFT = 0
    LOWERRIGHT = 1

# TODO: find a better way of doing positions then hard coded
positions = {
    BasePosition.UPPERLEFT: {
        "chokepoints": [Point2D(34, 122),
                        Point2D(45, 107),
                        Point2D(64, 121)],
        "offensive_hold": Point2D(34, 110),
        "military_production": Point2DI(42, 140),
        "building_positions": {
                                "SUPPLYDEPOT": [
                                    Point2DI(16, 134),
                                    Point2DI(15, 136),
                                    Point2DI(17, 136),
                                    Point2DI(15, 138),
                                    Point2DI(17, 138),
                                    Point2DI(15, 140),
                                    Point2DI(17, 140),
                                    Point2DI(16, 142),
                                    Point2DI(18, 142),
                                    Point2DI(16, 144),
                                    Point2DI(18, 144),
                                    Point2DI(17, 146),
                                    Point2DI(19, 146),
                                    Point2DI(21, 146),
                                    Point2DI(23, 145),
                                    Point2DI(19, 148),
                                    Point2DI(21, 148),
                                    Point2DI(23, 149),
                                    Point2DI(25, 149),
                                    Point2DI(27, 149),
                                    Point2DI(23, 147),
                                    Point2DI(25, 147),
                                    Point2DI(27, 147),
                                    Point2DI(29, 148),
                                    Point2DI(31, 148),
                                    Point2DI(33, 148),
                                    Point2DI(37, 150),
                                    Point2DI(39, 150)
                                ],
                                "BARRACKS": [
                                    Point2DI(35, 144),
                                    Point2DI(35, 141),
                                    Point2DI(35, 138),
                                    Point2DI(35, 135),
                                    Point2DI(35, 132)
                                ],
                                "FACTORY": [
                                    Point2DI(42, 144),
                                    Point2DI(42, 141),
                                    Point2DI(42, 138)
                                ]
                            }
    },
    BasePosition.LOWERRIGHT: {
        "chokepoints": [Point2D(114.5, 44.5),
                         Point2D(108, 62),
                         Point2D(88, 47)],
        "offensive_hold": Point2D(117, 57),
        "military_production": Point2DI(96, 20),
        "building_positions": {
            "SUPPLYDEPOT": [
                            Point2DI(136, 35),
                            Point2DI(136, 33),
                            Point2DI(135, 31),
                            Point2DI(137, 31),
                            Point2DI(135, 29),
                            Point2DI(137, 29),
                            Point2DI(135, 27),
                            Point2DI(137, 27),
                            Point2DI(134, 25),
                            Point2DI(136, 25),
                            Point2DI(134, 23),
                            Point2DI(132, 23),
                            Point2DI(130, 23),
                            Point2DI(128, 23),
                            Point2DI(134, 21),
                            Point2DI(132, 21),
                            Point2DI(130, 21),
                            Point2DI(128, 21),
                            Point2DI(126, 21),
                            Point2DI(124, 21),
                            Point2DI(129, 19),
                            Point2DI(127, 19),
                            Point2DI(125, 19),
                            Point2DI(122, 20),
                            Point2DI(120, 20),
                            Point2DI(115, 18),
                            Point2DI(113, 18),
                            Point2DI(111, 18)
                        ],
            "BARRACKS": [
                            Point2DI(115, 35),
                            Point2DI(115, 32),
                            Point2DI(115, 29),
                            Point2DI(115, 26),
                            Point2DI(115, 23)
                       ],
            "FACTORY": [
                            Point2DI(107, 28),
                            Point2DI(107, 25),
                            Point2DI(107, 22)
                       ]

        }
    }
}
