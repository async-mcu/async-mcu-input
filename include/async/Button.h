#pragma once

#include <async/Task.h>
#include <async/Chain.h>
#include <async/Interrupt.h>

namespace async {

    class Button {
        private:
        Interrupt * interrupt;
        
        public:
        Button(Interrupt * interrupt): interrupt(interrupt) {

        }

        Tick * onPress(voidCallback callback) {
            Semaphore * waiter = new Semaphore(1,1);

            return chain()
                ->interrupt(interrupt)
                ->semaphoreSkip(waiter)
                ->delay(5)
                ->then([this, waiter, callback]() {
                    waiter->release();

                    // double check
                    if(interrupt->getValue() == (interrupt->getMode() == FALLING ? LOW : HIGH)) {
                        callback();
                    }
                })
                ->loop();
        }

        Tick * onLongPress(voidCallback callback) {
            Semaphore * waiter = new Semaphore(1,1);

            return chain<uint32_t>(0)
                ->interrupt(interrupt)
                ->semaphoreSkip(waiter)
                ->then([](uint32_t ms) {
                    return millis();
                })
                ->delay(10)
                ->interrupt(interrupt, 1000)
                ->delay(10)
                ->then([this, waiter, callback](uint32_t ms) {
                    waiter->release();
                    
                    // check fast press and double check
                    if(millis() - ms > 900 && interrupt->getValue() == (interrupt->getMode() == FALLING ? LOW : HIGH)) {
                        callback();
                    }

                    return ms;
                })
                ->loop();
        }
    };
}