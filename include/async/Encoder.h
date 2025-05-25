#include <async/Task.h>
#include <async/Chain.h>
#include <async/Button.h>
#include <async/Pin.h>

namespace async {
    typedef Function<void(int, bool)> EncoderCallback;

    class Encoder {
        private:
        Pin * first;
        Pin * second;
        Pin * button;
        Button * butt;
        volatile int count;
        bool reverse;
        
        public:
        Encoder(Pin * first, Pin * second, bool reverse = false, Pin * button = nullptr): 
            first(first), second(second), reverse(reverse), button(button), count(0) {
            
            if(button != nullptr) {
                this->butt = new Button(button);
            }
        }

        void onRotate(EncoderCallback callback) {
            Semaphore * waiter = new Semaphore(1,1);

            first->onInterrupt(first->getMode() == INPUT_PULLUP ? FALLING : RISING, [this, callback]() {
                int firstVal = first->digitalRead();
                int secondVal = second->digitalRead();

                if(firstVal = HIGH && secondVal == HIGH && !reverse) {
                    callback(++count, true);
                }
                else {
                    callback(--count, false);
                }
            });

            // return chain()
            //     ->semaphoreSkip(waiter)
            //     ->interrupt(first)
            //     ->then([this, waiter, callback]() {
            //         int val = first->getValue();
            //         int val2 = second->getValue();

            //         Serial.print("first ");
            //         Serial.print(val);
            //         Serial.print(", second ");
            //         Serial.println(val2);

            //         waiter->release();

            //         // double check
            //         //if(interrupt->getValue() == (interrupt->getMode() == FALLING ? LOW : HIGH)) {
            //         //    callback();
            //        // }
            //     })
            //     ->loop();
        }

        void onPress(VoidCallback callback) {
            this->butt->onPress(callback);
        }

        void onLongPress(int time, VoidCallback callback) {
            this->butt->onLongPress(time, callback);
        }
    };
}