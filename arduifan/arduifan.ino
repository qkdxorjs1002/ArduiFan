#include <IRremote.h>

#define PIN_IR_RECEIVER 2
#define PIN_BUZZER      3
#define PIN_MOTER_1     5
#define PIN_MOTER_2     6
#define PIN_MOTER_3     7
#define PIN_MOTER_S     8
#define PIN_BTN_SPD     0
#define PIN_BTN_SWING   0
#define PIN_LED_SLEEP   0
#define PIN_LED_1  11
#define PIN_LED_2  12
#define PIN_LED_3  0
#define PIN_LED_4  0

#define STOP 4204738687 // Stop
#define SPED 2333313659 // On / Speed
#define SWNG 1380212731 // Swing
#define TIMR 57014751   // Timer
#define MODE 310427483  // Mode

int FAN_SPEED = 0;      // Off:0  Low:1  Mid:2  High:3
int FAN_MODE = 0;       // Normal:0  Natural:1  Sleep:2
int FAN_TIMER = 0;      // Off:0  30m:1  1h:2  2h:3  4h:4
bool FAN_SWING = false; // Swing Mode

IRrecv irrecv(PIN_IR_RECEIVER);

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_MOTER_1, OUTPUT);
    pinMode(PIN_MOTER_2, OUTPUT);
    pinMode(PIN_MOTER_3, OUTPUT);
    pinMode(PIN_MOTER_S, OUTPUT);
    pinMode(PIN_LED_1, OUTPUT);
    pinMode(PIN_LED_2, OUTPUT);

    stop_fan();
    
    Serial.begin(115200);

    irrecv.enableIRIn();
    tone(PIN_BUZZER, 1250, 100);
}

void stop_fan() {
    digitalWrite(PIN_MOTER_1, HIGH);
    digitalWrite(PIN_MOTER_2, HIGH);
    digitalWrite(PIN_MOTER_3, HIGH);
    digitalWrite(PIN_MOTER_S, HIGH);
    digitalWrite(PIN_LED_1, LOW);
    digitalWrite(PIN_LED_2, LOW);
    FAN_SPEED = 0;
    FAN_MODE = 0;
    FAN_TIMER = 0;
    FAN_SWING = false;
}

void change_speed() {
    ++FAN_SPEED %= 4;

    digitalWrite(PIN_MOTER_1, HIGH);
    digitalWrite(PIN_MOTER_2, HIGH);
    digitalWrite(PIN_MOTER_3, HIGH);

    switch(FAN_SPEED) {
        case 1:
            Serial.println("Moter Low");
            digitalWrite(PIN_MOTER_1, LOW);
            break;
        case 2:
            Serial.println("Moter Mid");
            digitalWrite(PIN_MOTER_2, LOW);
            break;
        case 3:
            Serial.println("Moter High");
            digitalWrite(PIN_MOTER_3, LOW);
            break;
        default :
            Serial.println("Fan Off");
            stop_fan();
            break;
    }
}

void toggle_swing() {
    FAN_SWING = !FAN_SWING;
    digitalWrite(PIN_MOTER_S, HIGH);
    digitalWrite(PIN_LED_1, LOW);
    digitalWrite(PIN_LED_2, LOW);

    if (FAN_SWING && FAN_SPEED != 0) {
        Serial.println("Swing On");
        digitalWrite(PIN_MOTER_S, LOW);
        digitalWrite(PIN_LED_1, HIGH);
    }
}

void loop() {
    decode_results results;

    if (irrecv.decode(&results)) {
        Serial.print("Input: ");
        Serial.println(results.value);

        switch(results.value) {
            case 4204738687:
                Serial.println("Stop Command");
                stop_fan();
                tone(PIN_BUZZER, 1250, 100);
                break;
            case 2333313659:
                Serial.println("Speed Command");
                change_speed();
                tone(PIN_BUZZER, 1250, 70);
                break;
            case 1380212731:
                Serial.println("Swing Command");
                toggle_swing();
                tone(PIN_BUZZER, 1250, 70);
                break;
            case 57014751:
                Serial.println("Timer Command");
                tone(PIN_BUZZER, 1250, 70);
                break;
            case 310427483:
                Serial.println("Mode Command");
                tone(PIN_BUZZER, 1250, 70);
                break;
        }
        
        irrecv.resume();
    }
}
