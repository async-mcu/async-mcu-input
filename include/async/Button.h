#pragma once

#include <async/Task.h>
#include <async/Chain.h>
#include <async/Interrupt.h>

namespace async {
    typedef Function<void(uint64_t)> TimeCallback;

    class Button {
        private:
        Interrupt * interrupt;
        
        public:
        Button(Interrupt * interrupt): interrupt(interrupt) {

        }

        Tick * onPress(VoidCallback callback) {
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

        Tick * onLongPress(VoidCallback callback) {
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

        Tick * onTimePress(TimeCallback callback) {
            Semaphore * waiter = new Semaphore(1,1);

            return chain(0)
                ->interrupt(interrupt)
                ->semaphoreSkip(waiter)
                ->delay(10)
                ->then([](uint64_t ms) {
                    return millis();
                })
                ->again([this, waiter](uint64_t ms) {
                    // double check
                    bool result = interrupt->getValue() != (interrupt->getMode() == FALLING ? LOW : HIGH);
                    
                    if(result) {
                        waiter->release();
                    }
                    
                    return result;
                })
                ->cycle([this](uint64_t ms) {
                    if(interrupt->getValue() != (interrupt->getMode() == FALLING ? LOW : HIGH)) {
                        return (uint64_t) nullptr;
                    }

                    return ms;
                })
                ->then([this, waiter, callback](uint64_t ms) {
                    waiter->release();

                    // double check
                    callback(millis() - ms);

                    return ms;
                })
                ->loop();
        }
    };
}