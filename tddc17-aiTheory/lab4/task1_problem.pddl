(define (problem simple)
    (:domain shakeysWorld)
    (:objects
     shakey room1 room2 room3 lightSwitch1 lightSwitch2 lightSwitch3 door1 door2 door3 box toyDigger toyTractor)
    (:init
        (shakey shakey)
        (in shakey room2)
        (room room1) (room room2) (room room3)
        (lightswitch lightSwitch1 room1) (lightswitch lightSwitch2 room2) (lightswitch lightSwitch3 room3)
        (door door1) (door door2) (door door3)
        (box box)
        (object toyDigger) (object toyTractor)
        (small toyDigger) (small toyTractor)
        (wide door1) (wide door3)
        (in box room1) (in toyDigger room2) (in toyTractor room3)
        (between room1 room2 door1) (between room2 room3 door2) (between room2 room3 door3)
        (between room2 room1 door1) (between room3 room2 door2) (between room3 room2 door3) ; define two-way door
    )
    (:goal (and (in toyDigger room1) (in toyTractor room1))
    )
)
; working planners = LAMA, FF
; /courses/TDDC17/bin/downward/src/lama task1.pddl task1_problem.pddl
; cat sas_plan.l
; /courses/TDDC17/bin/ff -o task1.pddl -f task1_problem.pddl
