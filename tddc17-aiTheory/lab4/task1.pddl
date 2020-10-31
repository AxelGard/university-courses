
(define (domain shakeysWorld)
    (:requirements :strips :equality :adl)

    (:predicates
        (shakey ?s)
        (room ?r)
        (door ?d)
        (object ?o)
        (lightswitch ?l ?r)
        (box ?b)
        (wide ?door)
        (carryingRight )
        (carryingLeft )
        (carrying ?object)
        (small ?object)
        (lightOn ?room)
        (in ?object ?room)
        (isBelow ?box ?lightswitch)
        (between ?room1 ?room2 ?door)
    )

    (:action move
        :parameters (?s ?r1 ?r2 ?d)
        :precondition (and (shakey ?s) (room ?r1) (room ?r2) (door ?d) (in ?s ?r1) (between ?r1 ?r2 ?d))
        :effect (and (not(in ?s ?r1)) (in ?s ?r2))
    )

    (:action pickupRight
        :parameters (?s ?o ?r)
        :precondition (and (shakey ?s) (object ?o) (room ?r) (small ?o) (lightOn ?r) (not(carryingRight)) (in ?o ?r) (in ?s ?r))
        :effect (and (carryingRight) (carrying ?o) (not(in ?o ?r)))
    )

    (:action putdownRight
        :parameters (?s ?o ?r)
        :precondition (and (shakey ?s) (object ?o) (room ?r) (carryingRight) (carrying ?o) (lightOn ?r) (in ?s ?r))
        :effect (and (not(carryingRight)) (not(carrying ?o)) (in ?o ?r))
    )

    (:action pickupLeft
        :parameters (?s ?o ?r)
        :precondition (and (shakey ?s) (object ?o) (room ?r) (small ?o) (lightOn ?r) (not(carryingLeft)) (in ?o ?r) (in ?s ?r))
        :effect (and (carryingLeft) (carrying ?o) (not(in ?o ?r)))
    )

    (:action putdownLeft
        :parameters (?s ?o ?r)
        :precondition (and (shakey ?s) (object ?o) (room ?r) (carrying ?o) (carryingLeft) (lightOn ?r) (in ?s ?r))
        :effect (and (not(carryingLeft)) (not(carrying ?o)) (in ?o ?r))
    )

    (:action pushbelow
        :parameters (?s ?b ?l ?r)
        :precondition (and (shakey ?s) (lightswitch ?l ?r) (box ?b) (in ?b ?r) (in ?s ?r))
        :effect (and (isBelow ?b ?l))
    )

    (:action pushThrough
        :parameters (?s ?b ?r1 ?r2 ?d ?l)
        :precondition (and (shakey ?s) (box ?b) (room ?r1) (room ?r2) (door ?d) (lightswitch ?l ?r1) (in ?b ?r1) (between ?r1 ?r2 ?d) (wide ?d) (in ?s ?r1))
        :effect (and (not(in ?b ?r1)) (in ?b ?r2) (not(isBelow ?b ?l)) (not(in ?s ?r1)) (in ?s ?r2))
    )

    (:action turnLightOn
        :parameters (?s ?r ?l ?b)
        :precondition (and (shakey ?s) (room ?r) (lightswitch ?l ?r) (box ?b) (isBelow ?b ?l) (in ?s ?r))
        :effect (and (lightOn ?r))
    )

)
