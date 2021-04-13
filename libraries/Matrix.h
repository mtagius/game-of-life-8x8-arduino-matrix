int animationFrames = 1;

int animationDelays[] = { 500 };

// Animation is designed for 8x8 pixels
uint8_t animation[][16] = {
  { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
};

// A number indicating when to advance to the next frame
unsigned long nextImage = 0;
// 8x8 Point temporary array
byte brightnesses[64];
// Matrix image frame for 8x8 LED
int M[8];

void shiftIt(byte dataOut) {
  // Shift out 8 bits LSB first, on rising edge of clock
  boolean pinState;

  //clear shift register read for sending data
  digitalWrite(dataPin, LOW);
  // for each bit in dataOut send out a bit
  for (int i=0; i<8; i++) {
    //set clockPin to LOW prior to sending bit
    digitalWrite(clockPin, LOW);
    // if the value of DataOut and (logical AND) a bitmask
    // are true, set pinState to 1 (HIGH)
    if ( dataOut & (1<<i) ) {
      pinState = HIGH;
    }
    else {
      pinState = LOW;
    }
    //sets dataPin to HIGH or LOW depending on pinState
    digitalWrite(dataPin, pinState);
    //send bit out on rising edge of clock
    digitalWrite(clockPin, HIGH);
    digitalWrite(dataPin, LOW);
  }
  digitalWrite(clockPin, LOW); //stop shifting
}

void screenUpdate(unsigned long frametime) 
{ // function to display image

    unsigned long startime=millis();
  while(millis()-startime<frametime)
  {
    byte row = B10000000; // row 1 
    for (byte k = 0; k < 8; k++) 
    {
      digitalWrite(latchPin, LOW); // open latch ready to receive data
      shiftIt(~row); // row binary number
      shiftIt(M[k]); // LED array (inverted) 

      // Close the latch, sending the data in the registers out to the matrix
      digitalWrite(latchPin, HIGH);
      row = row>> 1; // bitshift right
    }
  } 
}

//I made this to convert the 8x8 bool array into the weird [0][16] array
//that drives the matrix.  It aint pretty
void updateMatrix() {
	int matrixIndex = 0;

  for(int i = 0; i < 8; i++) {
    if(matrix[i][0]) {
      if(matrix[i][1]) {
        if(matrix[i][2]) {
          if(matrix[i][3]){
            animation[0][matrixIndex] = 0xff;
          } else {
            animation[0][matrixIndex] = 0x3f;
          }
        } else {
          if(matrix[i][3]){
            animation[0][matrixIndex] = 0xcf;
          } else {
            animation[0][matrixIndex] = 0xf;
          }
        }
      } else {
        if(matrix[i][2]) {
          if(matrix[i][3]) {
            animation[0][matrixIndex] = 0xf3;
          } else {
            animation[0][matrixIndex] = 0x33;
          }
        } else {
          if(matrix[i][3]) {
            animation[0][matrixIndex] = 0xc3;
          } else {
            animation[0][matrixIndex] = 0x3;
          }
        }
      }
    } else {
      if(matrix[i][1]) {
        if(matrix[i][2]) {
          if(matrix[i][3]){
            animation[0][matrixIndex] = 0xfc;
          } else {
            animation[0][matrixIndex] = 0x3c;
          }
        } else {
          if(matrix[i][3]){
            animation[0][matrixIndex] = 0xcc;
          } else {
            animation[0][matrixIndex] = 0xc;
          }
        }
      } else {
        if(matrix[i][2]) {
          if(matrix[i][3]) {
            animation[0][matrixIndex] = 0xf0;
          } else {
            animation[0][matrixIndex] = 0x30;
          }
        } else {
          if(matrix[i][3]) {
            animation[0][matrixIndex] = 0xc0;
          } else {
            animation[0][matrixIndex] = 0x0;
          }
        }
      }
    }
    

    if(matrix[i][4]) {
      if(matrix[i][5]) {
        if(matrix[i][6]) {
          if(matrix[i][7]){
            animation[0][matrixIndex + 1] = 0xff;
          } else {
            animation[0][matrixIndex + 1] = 0x3f;
          }
        } else {
          if(matrix[i][7]){
            animation[0][matrixIndex + 1] = 0xcf;
          } else {
            animation[0][matrixIndex + 1] = 0xf;
          }
        }
      } else {
        if(matrix[i][6]) {
          if(matrix[i][7]) {
            animation[0][matrixIndex + 1] = 0xf3;
          } else {
            animation[0][matrixIndex + 1] = 0x33;
          }
        } else {
          if(matrix[i][7]) {
            animation[0][matrixIndex + 1] = 0xc3;
          } else {
            animation[0][matrixIndex + 1] = 0x3;
          }
        }
      }
    } else {
      if(matrix[i][5]) {
        if(matrix[i][6]) {
          if(matrix[i][7]){
            animation[0][matrixIndex + 1] = 0xfc;
          } else {
            animation[0][matrixIndex + 1] = 0x3c;
          }
        } else {
          if(matrix[i][7]){
            animation[0][matrixIndex + 1] = 0xcc;
          } else {
            animation[0][matrixIndex + 1] = 0xc;
          }
        }
      } else {
        if(matrix[i][6]) {
          if(matrix[i][7]) {
            animation[0][matrixIndex + 1] = 0xf0;
          } else {
            animation[0][matrixIndex + 1] = 0x30;
          }
        } else {
          if(matrix[i][7]) {
            animation[0][matrixIndex + 1] = 0xc0;
          } else {
            animation[0][matrixIndex + 1] = 0x0;
          }
        }
      }
    }
    matrixIndex += 2;
  }
  
  
    //load Delay time for Image
    nextImage = animationDelays[0];

    //load image(converted)
    for(int i=0; i<64; i++) {
      brightnesses[i] = (animation[0][i/4] >> (i%4*2)) & B00000001;
      M[i/8] |= (brightnesses[i] << (i%8)) ;
    }

    //Update Image
    screenUpdate(nextImage);

    //clear M[]
    for(int i=0; i<(8); ++i) {
      M[i]=0;
    }
	
}

