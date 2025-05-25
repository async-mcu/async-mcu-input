#include <Arduino.h>
#include <async/Executor.h>
#include <async/Button.h>
#include <async/Chain.h>
#include <async/Pin.h>
#include <async/Encoder.h>
#include <async/Matrix3x4.h>

using namespace async;

Executor executor;

// Interrupt pin23(23, FALLING);
// Interrupt pin25(25, RISING);
Pin pin23(23, INPUT_PULLUP);
Pin pin25(25, INPUT);

Button button2(&pin23);
Button button3(&pin25);

Pin pin5(5, INPUT, RISING);
Pin pin19(19, INPUT, RISING);
Pin pin21(21, INPUT, FALLING);

Encoder encoder(&pin5, &pin19, false, &pin21);


Pin keyCol1(32, INPUT_PULLUP);
Pin keyCol2(33, INPUT_PULLUP);
Pin keyCol3(15, INPUT_PULLUP);

Pin keyRow1(2, OUTPUT, LOW);
Pin keyRow2(14, OUTPUT, LOW);
Pin keyRow3(27, OUTPUT, LOW);
Pin keyRow4(0, OUTPUT, LOW);

Matrix3x4 keyboard(&keyCol1, &keyCol2, &keyCol3, &keyRow1, &keyRow2, &keyRow3, &keyRow4);

void setup() {
  Serial.begin(115200);
    Serial.println("setup");
    // add interrupts
    executor.start();
    executor.add(&pin5);
    executor.add(&pin19);
    executor.add(&pin21);
    executor.add(&pin23);
    executor.add(&pin25);
    executor.add(&button2);
    executor.add(&button3);

    
    executor.add(&keyCol1);
    executor.add(&keyCol2);
    executor.add(&keyCol3);
    executor.add(&keyRow1);
    executor.add(&keyRow2);
    executor.add(&keyRow3);
    executor.add(&keyRow4);
    executor.add(&keyboard);

    //executor.add(&encoder);

    keyboard.onPress([](int val) {
      Serial.print("keyboard.onPress ");
      Serial.println(val);
    });

    encoder.onPress([]() {
      Serial.println("encoder.onPress");
    });

    encoder.onLongPress(1000, []() {
      Serial.println("encoder.onLongPress");
    });

    encoder.onRotate([](int count, bool right) {
      Serial.println("encoder.onRotate");

      Serial.print("count ");
      Serial.print(count);
      Serial.print(", direction ");
      Serial.println(right);
    });

    button2.onPress([]() {
      Serial.println("button2.onPress");
    });

    button2.onLongPress(1000, []() {
      Serial.println("button2.onLongPress");
    });

    button3.onPress([]() {
      Serial.println("button3.onPress");
    });

    button3.onPress([]() {
      Serial.println("button3.onPress another");
    });

    button3.onLongPress(1000, []() {
      Serial.println("button3.onLongPress");
    });

    button3.onTimePress([](uint64_t ms) {
      Serial.print("encoder.onTimePress value: ");
      Serial.println(ms);
    });
}

void loop() {
    executor.tick();
}