#include <Servo.h>
#define SERVO_PIN 9
#define SW_PIN 8
#define INIT_DEG 10
#define UP_DEG 110

// global variable
Servo serv;

int deg = INIT_DEG;
int push_count = 0;
char debug_buffer[20];

// flags
int over_turn = 0;
int is_nomad = 0;

void debug_delay(char* tag, char* act, int delay_time) {
  Serial.print("[");
  Serial.print(tag);
  Serial.print("] is ");
  Serial.print(act);
  Serial.print(" at delay: ");
  Serial.println(delay_time);
}

void debug_message(char* tag, char* body) {
  Serial.print("[");
  Serial.print(tag);
  Serial.print("] ");
  Serial.println(body);
}

void debug_variable(char* tag, char* body, int variable) {
  Serial.print("[");
  Serial.print(tag);
  Serial.print("] ");
  Serial.print(body);
  Serial.print(" ");
  Serial.println(variable);
}

void setup() {
  Serial.begin(9600);
  Serial.println();
  debug_message("*", "Initialize Switch.");
  pinMode(SW_PIN, INPUT_PULLUP);
  
  debug_message("*", "Initialize Servo");
  serv.attach(7);
  pinMode(SERVO_PIN, OUTPUT);
  debug_message("*", "Initialize Servo Degree.");
  serv.write(INIT_DEG);
  deg = INIT_DEG;
  
  debug_message("*", "Useless Box Initialize Complete.");
}

void pull_action(int wait_time, int up_time, int stop_time, int up_deg) {
  delay(random(wait_time));
  debug_delay("#", "WAIT", wait_time);
  while (digitalRead(SW_PIN) == HIGH || is_nomad) {
    if (deg > up_deg) {
      debug_message("!", "over turn");
      over_turn = 1;
      return;
    }
    serv.write(++deg);
    delay(up_time);
  }
  debug_delay("#", "UP", up_time);
  if (!over_turn) delay(stop_time);
  debug_delay("#", "STOP", stop_time);
}

void reset_action(int down_time, int reset_deg) {
  // down action
  debug_delay("#", "DOWN", down_time);
  while (deg > reset_deg) {
    if(digitalRead(SW_PIN) == HIGH && deg < 90) {
      debug_message("!", "Switch Interrupt.");
      return;
    }
    serv.write(--deg);
    if (is_nomad || over_turn) {
      down_time = 2;
    }

    delay(down_time);
  }
  serv.write(reset_deg);
  deg = reset_deg;
}

void loop() {
  ////////////////////////////// INITIALIZE ZONE //////////////////////////////
  // normal setting
  // delay with some magic number
  int down_time = random(2, 20);
  int up_time = random(2, 7);
  int stop_time = random(20, 1000);
  int wait_time = random(20, 2000);
  // nomad setting
  int nomad_condition = random(5,10);
  int nomad_time = random(2,8);

  over_turn = 0;
  /////////////////////////////////////////////////////////////////////////////

  // RESET arm to INIT_DEG
  if (deg != INIT_DEG) {
    reset_action(down_time, INIT_DEG);
  }

  // PUSH switch back
  if (digitalRead(SW_PIN) == HIGH) {
    if (!is_nomad) push_count++;

    if (push_count % nomad_condition == 0) {
      push_count++;
      is_nomad = 1;
      Serial.println("\n############## NOMAD mode ##############");
    }

    debug_variable("#", "NOMAD COND is", nomad_condition);

    // normal pull action
    debug_variable("!", "is PULLed #", push_count);

    if (is_nomad) {
      // nomad mode
      for (int i = 0; i < nomad_time; i++) {
        // nomad with some magic degree and delay
        pull_action(0, 2, 0, 130);
        reset_action(2, 80);
      }
      is_nomad = 0;

      Serial.println("########################################\n");
    } else pull_action(wait_time, up_time, stop_time, UP_DEG);
  }
}
