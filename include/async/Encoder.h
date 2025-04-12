#include <async/Task.h>
#include <async/Chain.h>
#include <async/Button.h>
#include <async/Interrupt.h>

namespace async {
    typedef Function<void(int, bool)> encoderCallback;

    class Encoder {
        private:
        Interrupt * first;
        Interrupt * second;
        Interrupt * button;
        Button * butt;
        volatile int count;
        bool reverse;
        
        public:
        Encoder(Interrupt * first, Interrupt * second, bool reverse = false, Interrupt * button = nullptr): 
            first(first), second(second), reverse(reverse), button(button), count(0) {
            
            if(button != nullptr) {
                this->butt = new Button(button);
            }
        }

        Tick * onRotate(encoderCallback callback) {
            Semaphore * waiter = new Semaphore(1,1);

            return first->onInterrupt([this, callback]() {
                int firstVal = first->getValue();
                int secondVal = second->getValue();

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

        Tick * onPress(voidCallback callback) {
            return this->butt->onPress(callback);
        }

        Tick * onLongPress(voidCallback callback) {
            return this->butt->onLongPress(callback);
        }
    };
}