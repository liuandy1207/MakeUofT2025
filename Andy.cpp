// Joystick, white
#define JOYSTICK_X_PIN 34
#define JOYSTICK_Y_PIN 35
#define JOYSTICK_BUTTON_PIN 32

// Buzzer, Brown
#define BUZZER_PIN 27

// Buzzer Functions
void setBuzzerVolume(int volume) {
  volume = constrain(volume, 0, 255);
  ledcWrite(BUZZER_PIN, volume);
}

int playerX = 0, playerY = 0;  // character position, (0,0) for now
int gridSize = 5;
int monsterX = 4, monsterY = 4; // monster position, (4, 4) for now
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
      // Hit a wall, make noise
    } else {
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

void wander() { // ********** help
  int newX, newY;
  int validMove = 0;

  while (!validMove) {
    int randDir = random(0, 4); // Pick a random direction
    newX = monsterX + dx[randDir];
    newY = monsterY + dy[randDir];

    // Check if new position is within bounds and not a wall
    if (newX >= 0 && newX < gridSize && newY >= 0 && newY < gridSize && grid[newY][newX] != WALL) {
      moveMonster(randDir);
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
  printGrid();
  wander();
  delay(1000);
}



// char reverse_direction( char direction){
//   if (direction == 'U'){
//     return('D');
//   }
//   else if (direction == 'D'){
//     return('U');
//   }
//   else if ( direction == 'L'){
//     return('R');
//   }
//   else{
//     return('L');
//   }
// }

// void setup() {
//   Serial.begin(9600);
//   pinMode(X_axis, INPUT);
//   pinMode(Y_axis, INPUT);
//   pinMode(button, INPUT_PULLUP);

//   char path[25];
//   findShortestPath(0, 0, 4, 4, path);
//   Serial.println(path);  // Print shortest path to Serial Monitor
// }

// void loop() {
//   int xInput = analogRead(X_axis);
//   int yInput = analogRead(Y_axis);
//   int moveDelay = 300;

//   if (yInput < 1025) moveP(0, -1);
//   if (yInput > 3071) moveP(0, 1);
//   if (xInput < 1025) moveP(-1, 0);
//   if (xInput > 3071) moveP(1, 0);

//   char result[gridsize * gridsize];
//   if () {  // bool for loud ound && shortest path is <6...? or distance... hmmm

//     findShortestPath( monposX, monposY, currX, currY, result);
//     if (strlen(result) < 3) {
//       // do something
//     }
//     moveM(result[0]);
//     // TURN OFF BOOL FOR LOUD OUND
//   } else {
//     char direction = moves[rand() % 4];
//     while(reverse_direction(direction)!= prevMove){
//       moveM(direction);
//     }

//   }
//   delay(moveDelay);
// }
