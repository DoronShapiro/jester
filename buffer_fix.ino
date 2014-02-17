/* serious shit */

//Random compiler stuff
//int serialData;

//#include <SoftwareSerial.h>

//SoftwareSerial impSerial(5, 7); // RX on 5, TX on 7 

// pin numbers
const int x = A3; //white
const int y = A2; //red
const int z = A1; //black
//int light = A5;//fix later please ***************************************************************
//int lightVal = 0;
int serialData = 0;

// constants (could be changed)
const int buffsize = 20;
const int numcap = 500;

// thresholds to be tinkered with
const int threshold = 150;
const int closeEnoughThreshold = 10;  // TOTALLY MADE THIS UP

// are we reading?
boolean reading = false;

// iterator
int numloop = 0;

// holds the data we have collected
int buffers[3][buffsize];

// holding extremas
int mins[] = {0, 0, 0};
int maxs[] = {0, 0, 0};

// not really sure...
int oldValues[] = {0, 0, 0};

// shifts buffer and queues a new element
void shiftBuff(int front, int buffer[]) {
    for (int i = buffsize - 1; i > 0; i--) {
        buffer[i] = buffer[i - 1]; 
    }
    buffer[0] = front;
}

// takes weighted average of a buffer
int weightedAvg(int buffer[]) {
    double weighted = 0;
    int j = buffsize;
    for (int i = 0; i < buffsize; i++) {
        weighted = weighted + (buffer[i] * j);
        j--;
    } 
    return (int) (weighted / 210);
}

// find the Reimenn Sum
int riemSum(int a, int b) {
    return (a + b) / 2; 
}

// if val is more extreme, it becomes the number one champion
void updateExtrema(int val, int axis) {
    if (val > maxs[axis]) maxs[axis] = val; 
    if (val < mins[axis]) mins[axis] = val;
}  

// if we've reached a severity of movements, return true
boolean reachedThresh(int axis) {
  Serial.print("here");
    return maxs[axis] - mins[axis] >= threshold;
}

/*int encodeGesture(int x[]){
  return 1 * (x[0] + 1) + 10 * (x[1] + 1) + 100 * (x[2] + 1);
}*/

// to be done later.
void sendToImp(int data[]){
    // TODO @Max: implement me 
       Serial.print(data[0]);
       Serial.print(","); 
              Serial.print(data[1]);
       Serial.print(","); 
              Serial.print(data[2]);
       Serial.println("");
    
      // Send data from the hardware serial
  /*if (Serial.available())
    impSerial.write(encodeGesture(data));  // to the software serial*/
}

// check if we've almost reached a maximum
boolean closeEnough(int axis) {
    return (maxs[axis] - mins[axis]) >= threshold - closeEnoughThreshold;
}

//boolean litUp() {
  //return lightVal < 50;
//}

void setup() {
    Serial.begin(9600);
    
     // Open the hardware serial port
  //Serial.begin(19200);
  
  // set the data rate for the SoftwareSerial port
  //impSerial.begin(19200);
  
  
}

void loop() {
    //lightVal = analogRead(light);
    //if (litUp()) {
       numloop = 0;
    //}
    
       if (0 < Serial.available()) {
        serialData = Serial.read();
        if (serialData == 98) {
            reading = true;
        }
        else if (serialData == 101) {
            reading = false;
           // Serial.println("END");
        }
    }
    if (reading && numloop < numcap) {
        int trap[3];
        int new_data[] = { analogRead(x), analogRead(y), analogRead(z) };
        
        for (int i = 0; i < 3; i++) {
            int old_value = weightedAvg(buffers[i]);
            shiftBuff(new_data[i], buffers[i]);
            trap[i] = riemSum(old_value, weightedAvg(buffers[i]));
            Serial.print("trap ");
            Serial.print(trap[i]);
            
            // reset when 10
            if (numloop == 10) {
                mins[i] = trap[i];
                maxs[i] = trap[i];
            } else if (numloop > 10) {
                int dir[] = { 0, 0, 0 };
                dir[i] = trap[i] - oldValues[i] > 0 ?  1 :
                         trap[i] - oldValues[i] < 0 ? -1 :
                                                       0 ;
                updateExtrema(trap[i], i);

                // if we reached a threshold...
                if (reachedThresh(i)) {
                  Serial.print("here");
                    for (int j = 0; j < 3; j++) {
                        if (closeEnough(j)) dir[j] = trap[j] - oldValues[j] > 0 ?  1 :
                                                     trap[j] - oldValues[j] < 0 ? -1 :
                                                                                   0 ;
                    }
                    // send to the interwebs
                    sendToImp(dir);

                    for (int j = 0; j < 3; j++) {
                        if (dir[j] == 1) {
                            mins[j] = trap[j];
                        } else {
                            maxs[j] = trap[j];
                        }
                    }
                }
            }
        }

        //give to csv file
        if (numloop < numcap - 10 && numloop > 10) {
            // Set laggging values
            oldValues[0] = trap[0];
            oldValues[1] = trap[1];
            oldValues[2] = trap[2];
        }
        
        numloop++;
        delay(10);
    }
}
