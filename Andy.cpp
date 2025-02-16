// Joystick, white
#define JOYSTICK_X_PIN 34
#define JOYSTICK_Y_PIN 35
#define JOYSTICK_BUTTON_PIN 32

// Joystick Functions
void valuesToMove(int xValue, int yValue) {
  int thresholdLow = 1025;   // Lower threshold for joystick movement
  int thresholdHigh = 3071;  // Upper threshold for joystick movement

  char move = '?';

  if (yValue != xValue) {
    if (yValue < thresholdLow) {
      move = 'U';
    } else if (yValue > thresholdHigh) {
      move = 'D';
    } else if (xValue < thresholdLow) {
      move = 'L';
    } else if (xValue > thresholdHigh) {
      move = 'R';
    }
  }

  // if valid move direction is detected, move player
  if (move != '?') {
    movePlayer(move);
  }
  Serial.println(move);
}

// Buzzer, Brown
#define BUZZER_PIN 27

// Buzzer Functions
void setBuzzerVolume(int volume) {
  volume = constrain(volume, 0, 255);
  ledcWrite(BUZZER_PIN, volume);
}

int playerX = 0, playerY = 0;  // character position, (0,0) for now
int gridSize = 5;
int monsterX = 4, monsterY = 4;                                   // monster position, (4, 4) for now
const char PLAYER = 'P', WALL = '#', EMPTY = '.', MONSTER = 'M';  // graph legend

char grid[5][5] = {
  { '.', '.', '#', '.', '.' },
  { '.', '#', '.', '#', '.' },
  { '.', '.', '.', '.', '.' },
  { '.', '#', '.', '#', '.' },
  { '.', '.', '#', '.', '.' }
};

// Map Functions
void setGrid() {
  grid[playerY][playerX] = PLAYER;
  grid[monsterY][monsterX] = MONSTER;
}

void printGrid() {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < gridSize; j++) {
      Serial.print(grid[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}

// // Directions: Up, Down, Left, Right
const int dx[4] = { 0, 0, -1, 1 };
const int dy[4] = { -1, 1, 0, 0 };
const char moves[4] = { 'U', 'D', 'L', 'R' };
char prevMove = '?';

void movePlayer(char direction) {
  int index = -1;
  for (int i = 0; i < 4; ++i) {
    if (moves[i] == direction) {
      index = i;
      break;
    }
  }
  int x = dx[index];
  int y = dy[index];
  if (playerX + x >= 0 && playerX + x < gridSize && playerY + y >= 0 && playerY + y < gridSize) {
    if (grid[playerY + y][playerX + x] == WALL) {
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      wallHit = true;
    } else {
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(buzzer, LOW);
      delay(200);
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(buzzer, LOW);

      grid[playerY][playerX] = EMPTY;
      playerX += x;
      playerY += y;
      grid[playerY][playerX] = PLAYER;
    }
  }
}

void moveMonster(char direction) {
  int index = -1;
  for (int i = 0; i < 4; ++i) {
    if (moves[i] == direction) {
      index = i;
      break;
    }
  }
  int x = dx[index];
  int y = dy[index];
  if (monsterX + x >= 0 && monsterX + x < gridSize && monsterY + y >= 0 && monsterY + y < gridSize) {
    if (grid[monsterY + y][monsterX + x] == WALL) {
      // Hit a wall, make noise
    } else {
      grid[monsterY][monsterX] = EMPTY;
      monsterX += x;
      monsterY += y;
      grid[monsterY][monsterX] = MONSTER;
      prevMove = moves[index];
    }
  }
}

void wander() {
  int newX, newY;
  int validMove = 0;

  while (!validMove) {
    int randIndex = random(0, 4);  // Pick a random direction
    newX = monsterX + dx[randIndex];
    newY = monsterY + dy[randIndex];

    // Check if new position is within bounds and not a wall
    if (newX >= 0 && newX < gridSize && newY >= 0 && newY < gridSize && grid[newY][newX] != WALL) {
      moveMonster(moves[randIndex]);
      validMove = 1;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Joystick
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);

  // Buzzer Set Up
  ledcAttach(BUZZER_PIN, 1000, 8);  // Channel 0, 1kHz frequency, 8-bit resolution
  // lower frequency, deeper sound

  // Map Set Up
  setGrid();
  printGrid();
  movePlayer('D');
  printGrid();
}


void loop() {
  // Joystick
  int xValue = analogRead(JOYSTICK_X_PIN);
  int yValue = analogRead(JOYSTICK_Y_PIN);
  // Serial.print("x: ");
  // Serial.println(xValue);
  // Serial.print("y: ");
  // Serial.println(yValue);
  int buttonState = digitalRead(JOYSTICK_BUTTON_PIN);

  char result[gridSize*gridSize];
  findShortestPath( monposX, monposY, currX, currY, result);
  if (strlen(result) < 3) {
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
  }
  if(wallHit){
    moveM(result[0]);
  }
  else{
    wander();


  valuesToMove(xValue, yValue);
  printGrid();
  delay(1000);
}
