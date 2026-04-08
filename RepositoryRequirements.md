# Repository Requirements

## Engine Requirements
- [ ] There are two projects in your repository: the game engine (static/dynamic library) and the game (executable).
- [ ] Level layout/setup is read from a file, you decide which format.
- [x] Make sure the project builds for all build targets at warning level 4 and warnings flagged as errors.
- [ ] Some functionality in your game or engine must make use of threading.
- [ ] Apply design patterns:
    - [x] Game Loop.
    - [x] Update method.
    - [x] Command.
    - [x] Observer and/or EventQueue.
    - [x] Component.
    - [ ] State.

## Game Requirements (Galaga)
Galaga is a fixed shooter. The player mans a lone starfighter at the bottom of the screen, which must prevent the Galaga forces from destroying all of mankind. The following is a description of the gameplay elements you must implement, inspired by the original game.

The objective of each stage is to defeat all of the Galaga aliens, which will fly into formation from the top and sides of the screen. Aliens will dive towards the player while shooting down projectiles; colliding with either projectiles or aliens will result in a life being lost.

- [ ] The player has 4 lives.
- [ ] You can fire up to 2 missiles on to the screen at a time.
- [ ] We will implement 3 stages, each stage must have a different formation at the top.
    - [ ] F1 is a button to skip levels.

- [ ] Aliens fly into the level in a formation.
- [ ] Then some of them fly down at the player while shooting.

There are 3 types of Aliens.
- [ ] Bees, or Zako. The front line of the Galaga army.
    - [ ] Bees make up the bottom two rows of the formation at the top of the screen.
    - [ ] They will occasionally break from the formation and go on a bombing run, attempting to shoot you or crash in to you.
    - [ ] After their initial dive, they may make a round swoop back upwards, attempting to hit the fighter from behind.
    - [ ] They are worth more if shot down during a bombing run (100) than when shot in formation (50).
- [ ] Butterflies, or Goei.
    - [ ] The Butterflies occupy the two rows above the Bees.
    - [ ] They will break from the formation as well to go on bombing runs like the Bees and have a tendency to quickly steer left and right while diving, making them harder to hit.
    - [ ] They are worth more if shot down during a bombing run (160) than when shot in formation (80).
- [ ] The Boss Galagas resemble birds.
    - [ ] Unlike the rest of the Galaga army, they must be shot twice in order to be destroyed. The first shot merely changes their color from green to purple.
    - [ ] In either color, they will do one of two things. Either they will go on a typical bombing run, or they will initiate a tractor beam run.
        - [ ] When a Boss Galaga starts a tractor beam run, it brings no Butterflies along.
        - [ ] It loops just once at the top of the formation. It slides down to about mid-way along the screen, and starts the tractor beam.
        - [ ] If your fighter is caught in the beam, it will turn red and be captured and brought up to the top of the screen, the player loses a life and the fighter starts shooting at the player.
    - [ ] They are worth more if shot down during a bombing run (400) than when shot in formation (150).

- [ ] There must be a single player, a co-op and a versus mode
    - [ ] Co-op - two players can play with a fighter.
    - [ ] Versus - One player plays as a Boss Galaga - he cannot fly with the ship, but he can shoot and decide when he wants to fly towards the player and use the tractor beam.
    - [ ] The single player mode must be completely playable with both keyboard and gamepad, including the menu's.
    - [ ] The 2 player modes support two gamepads.
    - [ ] The 2 player modes support keyboard and gamepad.

- [ ] Music & sound effects must play.
    - [ ] F2 is a button to mute & unmute the sound.

- [ ] Upon game over the player gets a screen showing his results
    - [ ] Shots fired
    - [ ] Number of hits
    - [ ] Hit/miss ratio

- [ ] Next the player's score and a highscore list are displayed.
    - [ ] This highscore list must be permanent in between sessions.
    - [ ] Highscores must be associated with a name.
    - [ ] Entering names must be supported by gamepad.

# Tips
- Have simple visuals - we don't care about the looks of your game, it's **only the code that counts**. It must however function properly of course.
- Consider using a text format for your levels (like json or xml) while debugging. For "release" a binary format could be a better option.