unsigned long lastRefreshTime = 0;
int refreshInterval = 1;
unsigned long lastMoveTime = 0;
int moveInterval;
int rowPins[8] = {0,1,2,3,4,5,6,7};
int colPins[3] = {8,9,10};
int speakerPin = 11;
unsigned long now = 0;
unsigned long overTime = 100000000;
int currentColumn = 0;
int ballX;
int ballY;
int player1Score, player2Score;
int player1Position, player2Position;
boolean ballMovingUp = true;
boolean ballMovingLeft = true;
boolean isGameOn = true;
int shape[8][8] = {
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0}
};

void setup(){
  for(int i = 0; i < 13; i++){
    pinMode(i, OUTPUT); 
  }
  delay(2000);
  //Serial.begin(9600);
  player1Score = 0; 
  player2Score = 0;
  restartGame();
}
 
void loop(){
  now = millis();
  if(isGameOn){
    update();
  }else{
    updateScore(); 
  }
  draw();
}

void gameOver(){
    isGameOn = false;
    overTime = now;
    player1Score %= 7;
    player2Score %= 7;
    clearPins();
    int note[] = {700, 600, 500, 400, 300, 200};
    for(int i = 0; i < 6; i++){
      tone(speakerPin, note[i], 150);
      delay(200);
    }
   
}

void clearPins(){
  for(int i = 0; i < 11; i++){
    digitalWrite(i, 0);
  }
}

void restartGame(){
  moveInterval = 700;
  ballX = now % 8;
  ballY = 6;
  ballMovingUp = true;
  ballMovingLeft = true;
  isGameOn = true;
}

void updateBall(){
  if(ballMovingLeft)
    ballX--;
  else
    ballX++;
  if(ballX == 0)
    ballMovingLeft = false;
  else if (ballX == 7)
    ballMovingLeft = true;
    
  if(ballMovingUp)
    ballY--;
  else
    ballY++;
  if(ballY == 0){
    player1Score++;
    gameOver();
  }else if (ballY == 7){
    player2Score++;
    gameOver();
  }
  if(ballY == 1 && ballX >= player2Position && ballX < player2Position + 3){
    ballMovingUp = false;
    moveInterval -= 20;
    buzz();
  }
  else if(ballY == 6 && ballX >= player1Position && ballX < player1Position + 3){
    ballMovingUp = true;
    moveInterval -= 20;
    buzz();
  }
      
}

void buzz(){
  tone(speakerPin, 300, 20); 
}

void update(){
  //clear table;
  if(now - lastMoveTime > moveInterval){
    for(int i = 0; i < 8; i++){
      for(int j = 0; j < 8; j++){
        shape[i][j] = 0;
      }
    }
    
    //update player positions
    int player1PotansValue = analogRead(A0);
    int player2PotansValue = analogRead(A1);
    //Serial.println(player2PotansValue);
    player1Position = player1PotansValue * 6 / 1024;
    player2Position = player2PotansValue * 6 / 1024;
    for(int i = 0; i < 8; i++){
      if(i >= player1Position && i < player1Position + 3){
        shape[7][i] = 1;
      }else{
        shape[7][i] = 0;
      }
      if(i >= player2Position && i < player2Position + 3){
        shape[0][i] = 1;
      }else{
        shape[0][i] = 0;
      }
    }
    //update the ball
    updateBall();
    //draw the ball
    shape[ballY][ballX] = 1;
    lastMoveTime = now;
  }
}

void draw(){
  if(now - lastRefreshTime >= refreshInterval){
    digitalWrite(12, HIGH); // A03 of decoder
    
    for(int i = 0; i < 8; i++){
      digitalWrite(rowPins[i], shape[i][currentColumn]==1 ? HIGH:LOW); 
    }
    
    switch(currentColumn){
      case 0:
         digitalWrite(colPins[0], LOW);
         digitalWrite(colPins[1], LOW);
         digitalWrite(colPins[2], LOW); 
         break;
      case 1:
        digitalWrite(colPins[0], HIGH);
        digitalWrite(colPins[1], LOW);
        digitalWrite(colPins[2], LOW); 
        break;
      case 2:
        digitalWrite(colPins[0], LOW);
        digitalWrite(colPins[1], HIGH);
        digitalWrite(colPins[2], LOW); 
        break;
      case 3:
        digitalWrite(colPins[0], HIGH);
        digitalWrite(colPins[1], HIGH);
        digitalWrite(colPins[2], LOW); 
        break;
      case 4:
        digitalWrite(colPins[0], LOW);
        digitalWrite(colPins[1], LOW);
        digitalWrite(colPins[2], HIGH); 
        break;
      case 5:
        digitalWrite(colPins[0], HIGH);
        digitalWrite(colPins[1], LOW);
        digitalWrite(colPins[2], HIGH);
        break;
      case 6:
        digitalWrite(colPins[0], LOW);
        digitalWrite(colPins[1], HIGH);
        digitalWrite(colPins[2], HIGH);
        break;
      case 7:
        digitalWrite(colPins[0], HIGH);
        digitalWrite(colPins[1], HIGH);
        digitalWrite(colPins[2], HIGH); 
        break;
    }
    
    digitalWrite(12, LOW); // A03 of decoder
    currentColumn++;
    currentColumn %= 8;
    lastRefreshTime = now; 
  }
}

void updateScore(){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      shape[i][j] = 0;
    }
  }
  
  for(int i = 0; i < player2Score; i++){
    for(int j = 0; j < i + 1; j++){
      shape[j][6 - i] = 1;
    }
  }
  
  for(int i = 0; i < player1Score; i++){
    for(int j = 0; j < i + 1; j++){
      shape[7 - j][i + 1] = 1;
    }
  }
  if(now - overTime > 3000)
    restartGame();
}

