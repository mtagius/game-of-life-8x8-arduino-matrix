//  DM11A88(Matrix 8x8 LED)

//some library the manufacturers need
#include <TimerOne.h>

//these pins are labeled on the PCB so, hook them up to the arduino
int latchPin = 10;
int clockPin = 12;
int dataPin = 11;

//this 8x8 array represents the matrix. True means the LED will light up, False means it wont
bool matrix[8][8];

//this mess of an H file contaitns the nonsense that enables the matrix to be used with the matrix array
#include <Matrix.h>

#define SIZE 8

byte world[SIZE][SIZE][2];
long density = 20;

bool change = false;
bool predeterminedDeath = false;
int iterations = 0;
int threeCells = 0;

bool mode = false;

int neighbours(int x, int y) {
  int numberOfNeighbours = 0;
  //if the borders overlap to the other side of the matrix
  if(mode) {
    numberOfNeighbours = world[(x + 1) % SIZE][y][0] + 
         world[x][(y + 1) % SIZE][0] + 
         world[(x + SIZE - 1) % SIZE][y][0] + 
         world[x][(y + SIZE - 1) % SIZE][0] + 
         world[(x + 1) % SIZE][(y + 1) % SIZE][0] + 
         world[(x + SIZE - 1) % SIZE][(y + 1) % SIZE][0] + 
         world[(x + SIZE - 1) % SIZE][(y + SIZE - 1) % SIZE][0] + 
         world[(x + 1) % SIZE][(y + SIZE - 1) % SIZE][0]; 
  } else  { 
  //if the borders are solid
    if(x != 7) {
      numberOfNeighbours += world[x + 1][y][0];
    }
    if(y != 7) {
      numberOfNeighbours += world[x][y + 1][0];
    }
    if(x != 0) {
      numberOfNeighbours += world[x - 1][y][0];
    }
    if(y != 0) {
      numberOfNeighbours += world[x][y - 1][0];
    }
    if(x != 7 && y != 7) {
      numberOfNeighbours += world[x + 1][y + 1][0];
    }
    if(x != 7 && y != 0) {
      numberOfNeighbours += world[x + 1][y - 1][0];
    }
    if(x != 0 && y != 7) {
      numberOfNeighbours += world[x - 1][y + 1][0];
    }
    if(x != 0 && y != 0) {
      numberOfNeighbours += world[x - 1][y - 1][0];
    }
  }
  return numberOfNeighbours; 
}

void worldCreation() {
   for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (random(100) < density) {
        world[i][j][0] = 1;
      }
      else {
        world[i][j][0] = 0;
      }
      world[i][j][1] = 0;
    }
  }
}

void setup() {
  //This is the delay for how often the code checks if matrix[][] has been updated
  animationDelays[0] = 1500;

  //never really figured out if bool[][] initializes to false by default
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      matrix[i][j] = false;
    }
  }

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT); 

  randomSeed(analogRead(5));

  pinMode(2, INPUT);

  if(digitalRead(2)) {
    mode = true;
  } else {
    mode = false;
  }

  worldCreation();

  /*
  //Glider
  world[1][0][0] = 1;
  world[2][1][0] = 1;
  world[0][2][0] = 1;
  world[1][2][0] = 1;
  world[2][2][0] = 1;
 */ 
}

void loop() {
  // Display current generation
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      matrix[i][j] = world[i][j][0];
    }
  }

  updateMatrix();

  change = false;
  int liveCells = 0;

  // Birth and death cycle 
  for (int x = 0; x < SIZE; x++) { 
    for (int y = 0; y < SIZE; y++) {
      // Default is for cell to stay the same
      world[x][y][1] = world[x][y][0];
      int count = neighbours(x, y); 
      if (count == 3 && world[x][y][0] == 0) {
        // A new cell is born
        world[x][y][1] = 1; 
        change = true;
      } 
      if ((count < 2 || count > 3) && world[x][y][0] == 1) {
        // Cell dies
        world[x][y][1] = 0; 
        change = true;
      }
    }
  }
  
  // Copy next generation into place
  for (int x = 0; x < SIZE; x++) { 
    for (int y = 0; y < SIZE; y++) {
      world[x][y][0] = world[x][y][1];
      if(world[x][y][0]) {
        liveCells++;
      }
    }
  }

  if(change == false && predeterminedDeath == false) {
    predeterminedDeath = true;
    //if everything stops moving the grid will reset after 1 iteration (no cells alive) or 6 iterations (some cells alive)
    if(liveCells == 0) {
      iterations = 99;
    } else {
      iterations = 94;
    }
  }

  if(change == true && predeterminedDeath == false) {
    if(liveCells == 3) {
      threeCells++;
    }
    if(threeCells >= 5) {
      threeCells = 0;
      predeterminedDeath = true;
      iterations = 96;
    }
  }

  //forced reset
  if(iterations >= 100) {
    worldCreation();
    iterations = 0;
    predeterminedDeath = false;
  } else {
    iterations++;
  }
}


