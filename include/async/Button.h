#pragma once

#include <async/Task.h>
#include <async/Chain.h>
#include <async/Pin.h>
#include <async/Log.h>

namespace async {
    typedef Function<void(uint64_t)> TimeCallback;

    class Button: public Tick {
        private:
        Pin * pin;
        Task * task;
        Executor executor;
        
        public:
        Button(Pin * pin): pin(pin) {}

        bool start() override {
            return executor.start();
        }

        Pin * getPin() {
            return pin;
        }

        void onPress(VoidCallback callback) {
            Semaphore * waiter = new Semaphore(1,1);

            executor.add(chain()
                ->interrupt(pin, pin->getMode() == INPUT_PULLUP ? FALLING : RISING)
                ->semaphoreSkipToStartIfNotAcquired(waiter)
                ->delay(5)
                ->then([this, waiter, callback]() {
                    auto read = pin->digitalRead();

                    // double check
                    if(read == (pin->getMode() == INPUT_PULLUP ? LOW : HIGH)) {
                        callback();
                    }
                })
                ->then([this, waiter, callback]() {
                    waiter->release();
                })
                ->loop());
        }

        void onLongPress(int time, VoidCallback callback) {
            Semaphore * waiter = new Semaphore(1,1);

            executor.add(chain<uint32_t>(0)
                ->interrupt(pin, pin->getMode() == INPUT_PULLUP ? FALLING : RISING)
                ->semaphoreSkipToStartIfNotAcquired(waiter)
                ->then([](uint32_t ms) {
                    return millis();
                })
                ->delay(10)
                ->interrupt(pin, pin->getMode() == INPUT_PULLUP ? RISING : FALLING, time)
                ->delay(10)
                ->then([this, time, waiter, callback](uint32_t ms) {
                    //Serial.println("123");
                    
                    // check fast press and double check
                    if(millis() - ms >= time && pin->digitalRead() == (pin->getMode() == INPUT_PULLUP ? LOW : HIGH)) {
                        callback();
                    }

                    waiter->release();
                    return ms;
                })
                ->loop());
        }

        void onTimePress(TimeCallback callback) {
            Semaphore * waiter = new Semaphore(1,1);

             executor.add(chain(0)
                ->interrupt(pin, pin->getMode() == INPUT_PULLUP ? RISING : FALLING)
                ->semaphoreSkipToStartIfNotAcquired(waiter)
                ->delay(10)
                ->then([](uint64_t ms) {
                    return millis();
                })
                ->again([this, waiter](uint64_t ms) {
                    // double check
                    bool result = pin->digitalRead() != (pin->getMode() == INPUT_PULLUP ? LOW : HIGH);
                    
                    if(result) {
                        waiter->release();
                    }
                    
                    return result;
                })
                ->cycle([this](uint64_t ms) {
                    if(pin->digitalRead() != (pin->getMode() == INPUT_PULLUP ? LOW : HIGH)) {
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
                ->loop());
        }

        bool tick() override {
            return executor.tick();
        }
    };
}