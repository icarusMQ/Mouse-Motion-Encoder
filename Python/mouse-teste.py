import pygame
import serial

# Initialize Pygame
pygame.init()

# Constants for the screen dimensions
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600

# Constants for the dot dimensions and initial position
DOT_SIZE = 300  # Increase this value to make the dot bigger
DOT_COLOR = (0, 0, 0)  # Black
DOT_SPEED = 5

# Create the screen and set the background color to white
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
screen.fill((255, 255, 255))  # White background
pygame.display.set_caption("Dot Controller")

# Initialize the Serial connection to the Arduino
ser = serial.Serial('COM6', 38400)  # Replace 'COMX' with your Arduino's COM port

# Initialize dot position
dot_x = SCREEN_WIDTH // 2
dot_y = SCREEN_HEIGHT // 2

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Read data from the Arduino
    data = ser.readline().decode().strip().split()
    if len(data) == 2:
        x, y = map(int, data)

        # Update dot position based on Arduino data
        dot_x += x * DOT_SPEED
        dot_y += y * DOT_SPEED

        # Limit dot movement to within the screen
        dot_x = max(min(dot_x, SCREEN_WIDTH - DOT_SIZE), 0)
        dot_y = max(min(dot_y, SCREEN_HEIGHT - DOT_SIZE), 0)

        # Clear the screen
        screen.fill((255, 255, 255))  # White background

        # Draw the dot
        pygame.draw.rect(screen, DOT_COLOR, (dot_x, dot_y, DOT_SIZE, DOT_SIZE))

        # Update the screen
        pygame.display.flip()

# Close the Serial connection and quit Pygame
ser.close()
pygame.quit()
