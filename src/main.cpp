#include <Arduino.h>
#include <async/Executor.h>
#include <async/Button.h>
#include <async/Chain.h>
#include <async/Interrupt.h>
#include <async/Encoder.h>

using namespace async;

Executor executor;

// Interrupt pin23(23, FALLING);
// Interrupt pin25(25, RISING);
Interrupt pin23(23, FALLING);
Interrupt pin25(25, RISING);

Button button2(&pin23);
Button button3(&pin25);

Interrupt pin5(5, RISING);
Interrupt pin19(19, RISING);
Interrupt pin21(21, FALLING);

Encoder encoder(&pin5, &pin19, false, &pin21);

Semaphore sema(1,1);

void setup() {
  Serial.begin(115200);
    Serial.println("setup");
    // add interrupts
    executor.add(&pin5);
    executor.add(&pin19);
    executor.add(&pin21);
    executor.add(&pin23);
    executor.add(&pin25);

    executor.add(encoder.onPress([]() {
      Serial.println("encoder.onPress");
    }));

    executor.add(encoder.onLongPress([]() {
      Serial.println("encoder.onLongPress");
    }));


    executor.add(encoder.onRotate([](int count, bool right) {
      Serial.println("encoder.onRotate");

      Serial.print("count ");
      Serial.print(count);
      Serial.print(", direction ");
      Serial.println(right);
    }));

    executor.add(button2.onPress([]() {
      Serial.println("button2.onPress");
    }));

    executor.add(button2.onLongPress([]() {
      Serial.println("button2.onLongPress");
    }));

    executor.add(button3.onPress([]() {
      Serial.println("button3.onPress");
    }));

    executor.add(button3.onPress([]() {
      Serial.println("button3.onPress another");
    }));

    executor.add(button3.onLongPress([]() {
      Serial.println("button3.onLongPress");
    }));

    executor.add(button3.onTimePress([](uint64_t ms) {
      Serial.print("encoder.onTimePress value: ");
      Serial.println(ms);
    }));
}

void loop() {
    executor.tick();
}