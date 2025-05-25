#pragma once

#include <async/Task.h>
#include <async/Chain.h>
#include <async/Log.h>
#include <async/Pin.h>
#include <async/Button.h>

namespace async {
    typedef Function<void(int)> MatrixCallback;

    class Matrix3x4: public Tick {
        private:
            Button * c1;
            Button * c2;
            Button * c3;
            Pin * r1;
            Pin * r2;
            Pin * r3;
            Pin * r4;
            MatrixCallback callback;

        public:
            Matrix3x4(Pin * c1, Pin * c2, Pin * c3, Pin * r1, Pin * r2, Pin * r3, Pin * r4)
            : c1(new Button(c1)), c2(new Button(c2)), c3(new Button(c3)), 
              r1(r1), r2(r2), r3(r3), r4(r4) { }

        void onPress(MatrixCallback callback) {
            this->callback = callback;
        }

        bool start() override {
            c1->onPress([this]() {
                r1->digitalWrite(HIGH);
                if(c1->getPin()->digitalRead() == 1) this->callback(1);
                r1->digitalWrite(LOW);

                r2->digitalWrite(HIGH);
                if(c1->getPin()->digitalRead() == 1) this->callback(4);
                r2->digitalWrite(LOW);

                r3->digitalWrite(HIGH);
                if(c1->getPin()->digitalRead() == 1) this->callback(7);
                r3->digitalWrite(LOW);

                r4->digitalWrite(HIGH);
                if(c1->getPin()->digitalRead() == 1) this->callback(-1);
                r4->digitalWrite(LOW);
            });

            c2->onPress([this]() {
                r1->digitalWrite(HIGH);
                if(c2->getPin()->digitalRead() == 1) this->callback(2);
                r1->digitalWrite(LOW);

                r2->digitalWrite(HIGH);
                if(c2->getPin()->digitalRead() == 1) this->callback(5);
                r2->digitalWrite(LOW);

                r3->digitalWrite(HIGH);
                if(c2->getPin()->digitalRead() == 1) this->callback(8);
                r3->digitalWrite(LOW);

                r4->digitalWrite(HIGH);
                if(c2->getPin()->digitalRead() == 1) this->callback(0);
                r4->digitalWrite(LOW);
            });

            c3->onPress([this]() {
                r1->digitalWrite(HIGH);
                if(c3->getPin()->digitalRead() == 1) this->callback(3);
                r1->digitalWrite(LOW);

                r2->digitalWrite(HIGH);
                if(c3->getPin()->digitalRead() == 1) this->callback(6);
                r2->digitalWrite(LOW);

                r3->digitalWrite(HIGH);
                if(c3->getPin()->digitalRead() == 1) this->callback(9);
                r3->digitalWrite(LOW);

                r4->digitalWrite(HIGH);
                if(c3->getPin()->digitalRead() == 1) this->callback(-2);
                r4->digitalWrite(LOW);
            });

            return true;
        }

        bool tick() override {
            c1->tick();
            c2->tick();
            c3->tick();

            return true;
        }
    };
}


