import pygame
import objects
pygame.init() #Start Pygame

background_color = (121, 187, 210) # light blue
screen = pygame.display.set_mode((800,600)) #Start the screen
clock = pygame.time.Clock()

player = objects.Player(100, 100, 50, 50)
ground = objects.Platform(0, 500, 800, 100)
top = objects.Platform(0, -100, 800, 100)
left_side = objects.Platform(-10, -10*100, 10, 10*100 + 600)
right_side = objects.Platform(800, -10*100, 10, 10*100 + 600)
platforms = [objects.Platform(200, 400, 400, 25), objects.Platform(500, 100, 200, 25)]
game_objects = [ground, left_side, right_side, top] + platforms
blackholes = [objects.BlackHole(300, 300, 10, 10), objects.BlackHole(400, 200, 10, 10)]

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: # The user closed the window!
            running = False # Stop running

    screen.fill(background_color)
    for object in game_objects + [player]:
        object.on_step(screen, game_objects, blackholes)
    for blackhole in blackholes:
        blackhole.draw(screen)


    pygame.display.flip()

    clock.tick(60)
pygame.quit() # Close the window



"""
eq:
F = -kx  # fjärderkraft
U = 1/2 * kx^2  # potentiell energi fjäder
F = ma => F = mg # gravitasion
K = mv^2/2 # kinetisk energi
U = mgh # potentiell energi
F = u_k*F_n # Friktion

Leapfrog:
välj xi(t = 0) och vxi(t = -1/2)

(1) axi(t) = Fxi(t) / mi # kraft vid tids
(2) vxi(t + ½ ∆t) = vxi(t –½ ∆t) + axi(t)·∆t # hastighet vid tid
(3) xi(t+∆t) = xi(t) + vxi(t+½ ∆t)·∆t # postion

"""
