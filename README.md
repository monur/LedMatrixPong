LedMatrixPong
=============

Pong Game using Arduino Uno + 8x8 Led Matrix 


Classical Pong game implemented on an Arduino Uno and 8x8 mono-color led matrix.
Player sticks are controlled by a potentiometer for each player. If you want the stick to go left, just turn the potentiometer left.
The game speed increases by time. After each game, the score is shown on the screen for 2 seconds and then a new game starts.
There are simple audio effects like ball bouncing and a game over melody.
You can see the game in action in this short video.

Tech details: Considering the fact that Arduino Uno has only 14 digital output pins, I decided to use a 74HC154 4-to-16 decoder to control 8x8 Led Matrix. While 8 digital pins directly control 8 rows of the led matrix(anode), 4 digital pins select the column over 74HC154 decoder(cathode).

I tried to draw the circuit schema using Fritzing, but I gave up realizing that Fritzing doesn't have built-in 8x8 Led Matrix and 4-to-16 decoder tools yet.
