import pygame
import util
import math

class Physics_object(pygame.Rect):
    def __init__(self, left, top, width, height, color = (0,0,0), effected_by_gravity = True):
        super().__init__(left, top, width, height)
        self.x_pos = float(left)
        self.y_pos = float(top)
        self.color = color
        self.current_color = color
        self.acceleration_x = 0.0
        self.acceleration_y = 0.0
        self.velocity_x_before = 0.0
        self.velocity_x_after = 0.0
        self.velocity_y_before = 0.0
        self.velocity_y_after = 0.0
        self.effected_by_gravity = effected_by_gravity
        self.is_free_fall = True
        self.mass = util.PLAYER_MASS
        self.center_of_mass_x = self.x_pos + width//2
        self.center_of_mass_y = self.y_pos + self.height//2


    def on_step(self, screen, other_objects, blackholes):

        # leapfrog things
        self.force_sum(blackholes)  # first step in the leapfrog algorithm for both dimensions
        self.velocity_y_after = self.velocity_y_before + (self.acceleration_y * 1)  # second step in y-dimension
        self.y_pos += self.velocity_y_after  # third step in y-dimension
        self.velocity_y_before = self.velocity_y_after

        # same as above but for the x-dimension
        self.velocity_x_after = self.velocity_x_before + (self.acceleration_x * 1)
        self.x_pos += self.velocity_x_after
        self.velocity_x_before = self.velocity_x_after

        self.center_of_mass_x = self.x_pos + self.width//2
        self.center_of_mass_y = self.y_pos + self.height//2
        self.collision_move_to(self.x_pos, self.y_pos, other_objects)
        self.draw(screen)

    def draw(self, screen):
        """ draws the shape of the object on screen """
        pygame.draw.rect(screen, self.current_color, self)

    def colliding_with(self, other_objects):
        """ checks if any collisons has ocured with in the given list  """
        return self.collidelistall(other_objects)

    def move_ip(self, x_dist, y_dist):
        """ moves in place the obj with the given distances """
        super().move_ip(x_dist, y_dist)

    def collision_move_to(self, x, y, other_objects):
        """ handels collision when obj is moving,
        checks if moveing one more step givs collioson """
        x_dist = x - self.left
        y_dist = y - self.top
        self.move_ip(x_dist, 0)
        if (self.colliding_with(other_objects)):
            while self.colliding_with(other_objects):
                self.move_ip(-x_dist/abs(x_dist), 0)
                self.x_pos += -x_dist/abs(x_dist)
            self.velocity_x_before = 0

        self.move_ip(0, y_dist)
        if (self.colliding_with(other_objects)):

            while self.colliding_with(other_objects):
                self.move_ip(0, -y_dist/abs(y_dist))
                self.y_pos += -y_dist/abs(y_dist)

            if (y_dist > 0):
                #self.is_free_fall = False
                kinetik_energy = self.mass * self.velocity_y_before**2/2
                spring_potenital_energy = kinetik_energy - util.ENERGY_LOSS_SPRING * kinetik_energy
                self.velocity_y_before = -math.sqrt((spring_potenital_energy * 2) / self.mass)
                #K = mv^2/2 # kinetisk energi
                #self.velocity_y_before = -self.velocity_y_before
                if int(kinetik_energy) == 0:
                    self.is_free_fall = False
                    self.velocity_y_before = 0
                return
            self.velocity_y_before = 0


    def move_to(self, x, y):
        x_dist = x - self.left
        y_dist = y - self.top
        self.move_ip(x_dist, y_dist)

    def apply_friction(self):
        # F = u_k*F_n # Friktion
        if self.velocity_x_before < 0:
            self.acceleration_x = min(+util.FRICTION * util.GRAVITY, -self.velocity_x_before)
        elif self.velocity_x_before > 0:
            self.acceleration_x = max(-util.FRICTION * util.GRAVITY, -self.velocity_x_before)


    def apply_gravity(self, other):
        """ F = G * m_1*m_2 / r^2 """
        dist = self.get_distance_to(other)
        x_dif = self.center_of_mass_x - other.center_of_mass_x
        y_dif = self.center_of_mass_y - other.center_of_mass_y
        angle = math.acos(x_dif/dist)
        total_force = (util.GRAVITATIONAL_CONSTANT * self.mass * other.mass ) / dist**2
        self.acceleration_x -= math.cos(angle) * (total_force / self.mass) #(x_dif/abs(x_dif)) *
        self.acceleration_y -= (y_dif/abs(y_dif)) * math.sin(angle) * (total_force / self.mass)


    def get_distance_to(self, other):
        dist = math.sqrt((self.center_of_mass_x - other.center_of_mass_x)**2 + (self.center_of_mass_y - other.center_of_mass_y)**2)
        return max(dist, 5) #lower bound


    def force_sum(self, blackholes):
        self.acceleration_y = util.GRAVITY
        if not self.is_free_fall:
            self.apply_friction()
        else:
            self.acceleration_x = 0

        for blackhole in blackholes:
            self.apply_gravity(blackhole)



class Player(Physics_object):
    def __init__(self, x, y, width, height, color = (0,125,0)):
        super().__init__(x, y, width, height, color)
        self.charging = False

    def on_step(self, screen, other_objects, blackholes):
        """ inheritance the on step from Phy_obj
        but checks keyboard input first """
        keys = pygame.key.get_pressed()
        if keys[pygame.K_LEFT]:
            self.velocity_x_before = -util.X_SPEED
        if keys[pygame.K_RIGHT]:
            self.velocity_x_before = util.X_SPEED
        if keys[pygame.K_SPACE] and not self.is_free_fall:
            self.charging = True
            self.mass = min(self.mass+1, util.MAX_MASS)
            self.current_color = ((self.mass / util.MAX_MASS) * 255, self.current_color[1], self.current_color[2])
        if not keys[pygame.K_SPACE] and self.charging and not self.is_free_fall:
            self.charging = False
            self.current_color = self.color
            self.is_free_fall = True
            self.spring_jump()
        super().on_step(screen, other_objects, blackholes)

    def spring_jump(self):
        # F = -kx  # fjärderkraft --> x = F/-k
        # U = 1/2 * kx^2  =  mv^2/2 --> 1/2 * k(F/-k)^2  =  mv^2/2
        # => v = sqrt((2 * U) / m)
        force = (self.mass * util.GRAVITY) - (util.PLAYER_MASS * util.GRAVITY)
        x = force / -util.SPRING
        energy_in_spring = 1/2 * util.SPRING * x**2
        self.mass = util.PLAYER_MASS
        self.velocity_y_before = -math.sqrt(energy_in_spring * 2 / self.mass)


class BlackHole(pygame.Rect):
    def __init__(self, left, top, width, height):
        super().__init__(left, top, width, height)
        self.mass = util.BLACKHOLE_MASS
        self.x_pos = float(left)
        self.y_pos = float(top)
        self.color = (0,0,0)
        self.center_of_mass_x = self.x_pos + width//2
        self.center_of_mass_y = self.y_pos + height//2

    def draw(self, screen):
        pygame.draw.rect(screen, self.color, self)


class Platform(pygame.Rect):
    def __init__(self, left, top, width, height, color = (191, 159, 64)):
        super().__init__(left, top, width, height)
        self.color = color

    def on_step(self, screen, other_objects, blackholes):
        self.draw(screen)

    def draw(self, screen):
        pygame.draw.rect(screen, self.color, self)
