

// IR Sensors
int sensor1 = 2;      // Left most sensor
int sensor2 = 3;
int sensor3 = 4;
int sensor4 = 11; 
int sensor5 = 12;      // Right most sensor

//SWITCH
int sensorSw = 13;
int buttonState = 0;
int lastButtonState = 0;
int pressCount = 0;

// Initial Values of IR Sensors
int sensor[5] = {0, 0, 0, 0,0};

// Motor Variables
int ENA = 5; //Left Motor Speed Control
int motorInput1 = 6; //Left Motor Speed Control
int motorInput2 = 7; //Right Motor
int motorInput3 = 8; //Right Motor
int motorInput4 = 9; //Right Motor
int ENB = 10; //Right Motor Speed Control

//Initial Speed of Motor
int initial_motor_speed = 110;


// PID
float Kp = 20;
float Ki = 0;
float Kd = 5;

float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;
int flag = 0;

//Vision Serial
int command = 0;
int status;
int prev_Status;

//ESP32->ARDUINO COMM
const int array0_esp= A0;
const int array1_esp= A1;
const int array2_esp= A2;
const int array3_esp= A3;
const int array4_esp= A4;
const int array5_esp= A5;
int autoManual;

void setup(){
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);

  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
  pinMode(motorInput3, OUTPUT);
  pinMode(motorInput4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(array0_esp, INPUT);
  pinMode(array1_esp, INPUT);
  pinMode(array2_esp, INPUT);
  pinMode(array3_esp, INPUT);
  pinMode(array4_esp, INPUT);
  pinMode(array5_esp, INPUT);

  pinMode(sensorSw, INPUT_PULLUP);
  Serial.begin(9600);                  //Baud rate 9600
  delay(500);
  Serial.println("Started !!");
}
void loop(){
      Serial.print(array0_esp);
      Serial.print(array1_esp);
      Serial.print(array2_esp);
      Serial.print(array3_esp);
      Serial.print(array4_esp);
      Serial.println(array5_esp);
      Serial.print("autoManual");
      Serial.println(autoManual);
      Serial.print("command");
      Serial.println(command);
      switchOn_Off();
      read_sensor_values();
      vision();
      cmd();
      switch(autoManual){
        case 1:
        vision();
          switch (command) {
            case 1:
            vision();
              
              if (status != 104 && status != 105){  
                  read_sensor_values();
              }
              // Make left turn
              if (status == 100) {
                  prev_Status = status;                
                  do {
                    Serial.print("\t");
                    Serial.println("Left");
                    analogWrite(ENA, 20); //Left Motor Speed
                    analogWrite(ENB, 120); //Right Motor Speed
                    sharpLeftTurn();
                    cmd();
                    vision();
                    read_sensor_values();
                    
                    
                    
                  } while (status !=99 && status != 101 && status != 103 && command != 0 && command != 2);

              // Make right turn      
              } else if (status == 101) {    
                  prev_Status = status;      
                
                  do {
                    Serial.print("\t");
                    Serial.println("Right");
                    read_sensor_values();
                    analogWrite(ENA, 120); //Left Motor Speed
                    analogWrite(ENB, 20); //Right Motor Speed
                    sharpRightTurn();
                    cmd();
                    vision();
                    read_sensor_values();
                    
                  } while (status != 99  && status != 100 && status != 103 && command != 0 && command != 2) ;

              // Make left turn untill it detects straight path     
              } else if (status == 102) {
                  if (prev_Status != 101 ){       
                    Serial.print("\t");
                    Serial.println("Sharp Left Turn");
                    do {
                      Serial.print("prev_Statux");
                      Serial.println(prev_Status);
                      analogWrite(ENA, 20); //Left Motor Speed
                      analogWrite(ENB, 110); //Right Motor Speed
                      sharpLeftTurn();
                      cmd();
                      vision();
                      read_sensor_values();
                      prev_Status = status;

                      if (status == 99 && error == 0) {
                        stop_bot();
                        delay(200);
                        read_sensor_values();
                      }
                    } while (status != 99 && status != 101 && status != 100 && status != 103 && command != 0 && command != 2 ) ;
                  }
                  else if (prev_Status == 101 ){     
                    Serial.print("prev_Statuf");
                    Serial.println(prev_Status);  
                    Serial.print("\t");
                    Serial.println("Sharp Right Turn");
                    do {
                      analogWrite(ENA, 110); //Left Motor Speed
                      analogWrite(ENB, 20); //Right Motor Speed
                      sharpRightTurn();
                      vision();
                      cmd();
                      read_sensor_values();
                      prev_Status = status;
                      if (status == 99 && error == 0) {
                        stop_bot();
                        delay(200);
                        read_sensor_values();
                      }
                    } while (status != 99 && status != 101 && status != 100 && status != 103  ) ;
                  }


              //Stop Motor    
              } else if (status == 103) {
                  // Serial.print("\t");
                  // Serial.println("STOP");
                  // stop_bot();
                  // delay(1000);
                  // read_sensor_values();
                  
                  // command = 0;
                //Slight Right
              } else if (status == 104){
                  Serial.println("Slight Right");
                  stop_bot();
                  delay(1000);
                  //SLIGHT RIGHT
                  analogWrite(ENA, 150); //Left Motor Speed
                  analogWrite(ENB, 20); //Right Motor Speed
                  forward(); 
                  delay(800);
                  //SLIGHT LEFT
                  do{ 
                    analogWrite(ENA, 30); //Left Motor Speed
                    analogWrite(ENB, 150); //Right Motor Speed
                    forward();
                    read_sensor_values();
                  }while (status != 99);

                //Slight Left
              } else if (status == 105){
                  Serial.println("Slight LEFT");
                  stop_bot();
                  delay(800);
                  //SLIGHT LEFT
                  analogWrite(ENA, 20); //Left Motor Speed
                  analogWrite(ENB, 150); //Right Motor Speed
                  forward(); 
                  delay(800);
                  //SLIGHT RIGHT
                  do{
                    analogWrite(ENA, 150); //Left Motor Speed
                    analogWrite(ENB, 30); //Right Motor Speed
                    forward();
                    read_sensor_values();
                  }while (status != 99);

              //Forward calculate by the line   
              }else {
                  Serial.print("\t");
                  Serial.println("FOR");
                  calculate_pid();
                  motor_control();
                  cmd();
                  vision();
                  prev_Status = 99;
              }
              break;

            case 0:
              Serial.println("STOP BECAUSE VIS");
              stop_bot();
              break;

            default: 
              break;
          }
        break;

        case 0:
          remote();
          break;
        
        default:
        break;


        }  

    }

void remote(){
  
        if (digitalRead(array1_esp) == 1 ) {
          stop_bot();
        }
        else if (digitalRead(array2_esp) == 1 ){ 
          forward();
        }
        else if (digitalRead(array3_esp) == 1 ){
          reverse();
        }
        else if (digitalRead(array4_esp) == 1 ){
          left();
        }
        else if (digitalRead(array5_esp) == 1 ){
          right();
        }
}    
void cmd(){

    if (digitalRead(array0_esp) == 1) { 
      autoManual = 0;
    }else if (digitalRead(array0_esp) == 0) {
      autoManual = 1;
    } 
}

void vision(){
  if (Serial.available() > 0) {
      // Read the incoming data
      char received = Serial.read();
      // Convert the received character to an integer
      if (received  == '4' ) {//Motor Start 
          command = 1;
        }
        else if (received == '5'  ){ //Motor Stop
          command = 0;
        }
        else if (received == '1' ){
          command = 1;// There is object on left side, turn slight right
          status = 104;
        }
        else if (received == '3' ){
          command = 1;// There is object on right side, turn slight left
          status = 105;
        }
  }
}
void read_sensor_values(){ //Calculate position with 5 IR Sensor
  sensor[0] = !digitalRead(sensor1);
  sensor[1] = !digitalRead(sensor2);
  sensor[2] = !digitalRead(sensor3);
  sensor[3] = !digitalRead(sensor4);
  sensor[4] = !digitalRead(sensor5);

  
    Serial.print(sensor[0]);
    Serial.print(",");
    Serial.print(sensor[1]);
    Serial.print(",");
    Serial.print(sensor[2]);
    Serial.print(",");
    Serial.print(sensor[3]);
    Serial.print(",");
    Serial.println(sensor[4]);
    Serial.print("error ");
    Serial.println(error);

    // Declare the error due to the position  (-4,-3,-2,-1,0,1,2,3,4) and (100,101,102,103)
    if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0)){
        status = 99;
        error = 4;}
    else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0)){
        status = 99;
        error = 3; }
    else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0)){
        status = 99;
        error = 2;}  
    else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0)){
        status = 99;
        error = 1;}  
    else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0)){
        status = 99;
        error = 0; }  
    else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0)){
        status = 99;
        error = -1; }   
    else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 0)){
        status = 99;
        error = -2; }   
    else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1)){
        status = 99;
        error = -3;}  
    else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1)){
        status = 99;
        error = -4;}  
    else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0 )){ // Turn robot left side
        status = 100;}  
    else if ((sensor[0] == 0) && (sensor[1] == 1) &&(sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1)){ // Turn robot right side
        status = 101;}
    else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 1)){ // Turn robot right side
        status = 101; }
    else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0)){ // Turn robot left side
        status = 100;}
    else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1)){ // Turn robot right side
        status = 101;}    
    else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0)){ // Make U turn
        status = 102;}
    else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 1)){ // stop
        status = 103;}

}

void switchOn_Off(){
 buttonState = digitalRead(sensorSw);  // Read the button state

  // Detect button press (LOW state due to pull-up resistor)
  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50);  // Debounce delay
    if (digitalRead(sensorSw) == LOW) {
      pressCount++;  // Increment press count
      if (pressCount == 1) {
        command = 1;  // First press: command = 1
      } else if (pressCount == 2) {
        command = 0;  // Second press: command = 0
        pressCount = 0;  // Reset press count for the nextcycle
      }
      Serial.println(command);  // Print the value of x for debugging
    }
  }

  lastButtonState = buttonState;  // Update the last button state
}
void calculate_pid(){ // CLassic PID Calculation
  if (prev_Status > 99){
    previous_error = 0;
  }

  P = error;
  I = previous_error + error;
  D = error - previous_error;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  previous_error = error;
}

void motor_control(){
  // Calculating the effective motor speed:
  int left_motor_speed = initial_motor_speed - PID_value;
  int right_motor_speed = initial_motor_speed + PID_value;

  // The motor speed should not exceed the max PWM value
  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);

    Serial.print("PID_value");
    Serial.print(PID_value);
    Serial.print("\t");
    Serial.print("LS:");
    Serial.print(left_motor_speed);
    Serial.print("\t");
    Serial.print("RS:");
    Serial.println(right_motor_speed);

  analogWrite(ENA, left_motor_speed); //Left Motor Speed
  analogWrite(ENB, right_motor_speed ); //Right Motor Speed

  forward();
}

void forward(){
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}
void reverse(){
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}
void right(){
  digitalWrite(motorInput1, LOW);//LEFT
  digitalWrite(motorInput2, HIGH);//LEFT
  digitalWrite(motorInput3, LOW);//RIGHT
  digitalWrite(motorInput4, LOW);//RIGHT
}
void left(){
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}
void sharpRightTurn() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}
void sharpLeftTurn() {
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}
void stop_bot(){
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
}


