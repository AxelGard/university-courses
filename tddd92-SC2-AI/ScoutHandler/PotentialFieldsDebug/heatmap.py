import pygame
from library import *
import util
import math
import threading 

def debugger(pot_mat):

    game = threading.Thread(target=heat_map, args=(pot_mat,))
    game.start()

def heat_map(pot_mat):
    
    EXPLORDING = False

    pygame.init() #Start Pygame
    SCALLER = 4
    HEIGHT = pot_mat.height*SCALLER
    screen = pygame.display.set_mode((pot_mat.width*SCALLER, HEIGHT)) #Start the screen
    clock = pygame.time.Clock()
    background_color = (0, 0, 0)

    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT: #The user closed the window!
                running = False #Stop running
        ground = []
        potensial = []
        intrest = []
        explored = []
       
        radius = 5

        for y in range(pot_mat.height):
            for x in range(pot_mat.width):
                if pot_mat.is_valid(Point2DI(x,y)):
                    ground.append(pygame.Rect(x*SCALLER, HEIGHT - y*SCALLER,SCALLER,SCALLER))
                if EXPLORDING and pot_mat.map_tools.is_explored(Point2DI(x,y)) and pot_mat.is_valid(Point2DI(x,y)):
                    explored.append(pygame.Rect(x*SCALLER, HEIGHT - y*SCALLER,SCALLER,SCALLER)) # map_tools.get_least_recently_seen_tile              
                if pot_mat.get_potensial(Point2DI(x,y)) > 0:
                    p = pot_mat.get_potensial(Point2DI(x,y))
                    intnes = min(max(int(math.sqrt(p)), 60), 255) # upper lower bound
                    potensial.append((pygame.Rect(x*SCALLER, HEIGHT - y*SCALLER,radius + SCALLER,radius + SCALLER), intnes))

        screen.fill(background_color)
        for rec in ground:
            pygame.draw.rect(screen, (0,0, 255), rec)
        if EXPLORDING:
            for rec in explored:
                pygame.draw.rect(screen, (0, 150, 150), rec)
        for tup in potensial:
            pygame.draw.rect(screen, (255, 255-tup[1], 255-tup[1]), tup[0])


        for pos in pot_mat.debug_lst: 
            rec = pygame.Rect(pos.x*SCALLER, HEIGHT - pos.y*SCALLER,SCALLER,SCALLER)
            pygame.draw.rect(screen, (0,255, 255), rec)

        max_intrest_rec = pygame.Rect(pot_mat.intrest_point.x*SCALLER, HEIGHT - pot_mat.intrest_point.y*SCALLER,radius + SCALLER,radius + SCALLER)
        pygame.draw.rect(screen, (255,255, 0), max_intrest_rec)

        scout = pygame.Rect(pot_mat.scout_position.x*SCALLER, HEIGHT - pot_mat.scout_position.y*SCALLER,radius + SCALLER,radius + SCALLER)
        pygame.draw.rect(screen, (0,255, 0), scout)

        pygame.display.flip()
        
        
        # Logic goes here
    pygame.quit() #Close the window




