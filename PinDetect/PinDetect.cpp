/*
    Copyright (c) 2010 Andy Kirkham

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "PinDetect.h"

namespace AjK {

PinDetect::PinDetect(PinName p) {
    init(p, PullDefault);
}

PinDetect::PinDetect(PinName p, PinMode m) {
    init(p, m);
}


PinDetect::~PinDetect(void) {
    if ( _ticker )  delete( _ticker );

    if ( _in )      delete( _in );
}

void PinDetect::init(PinName p, PinMode m) {
    _sampleTime              = PINDETECT_SAMPLE_PERIOD;
    _samplesTillAssert       = PINDETECT_ASSERT_COUNT;
    _samplesTillHeld         = 0;
    _samplesTillAssertReload = PINDETECT_ASSERT_COUNT;
    _samplesTillHeldReload   = PINDETECT_HOLD_COUNT;
    _assertValue             = PINDETECT_PIN_ASSERTED;

    _in = new DigitalIn(p);
    _in->mode(m);
    _prevState = _in->read();
    _ticker = new Ticker;

    _callbackAsserted = NULL;
    _callbackDeasserted = NULL;
    _callbackAssertedHeld = NULL;
    _callbackDeassertedHeld = NULL;
}

void PinDetect::setSampleFrequency(int i) {
    _sampleTime = i;
    _prevState  = _in->read();
    _ticker->attach_us(callback(this, &PinDetect::isr), _sampleTime);
}

void PinDetect::setAssertValue(int i) {
    _assertValue = i & 1;
}

void PinDetect::setSamplesTillAssert(int i) {
    _samplesTillAssertReload = i;
}

void PinDetect::setSamplesTillHeld(int i) {
    _samplesTillHeldReload = i;
}

void PinDetect::mode(PinMode m) {
    _in->mode(m);
}

void PinDetect::attach_asserted(Callback<void()> function) {
    core_util_critical_section_enter();
    _callbackAsserted = function;
    core_util_critical_section_exit();
}

void PinDetect::attach_deasserted(Callback<void()> function) {
    core_util_critical_section_enter();
    _callbackDeasserted = function;
    core_util_critical_section_exit();
}

void PinDetect::attach_asserted_held(Callback<void()> function) {
    core_util_critical_section_enter();
    _callbackAssertedHeld = function;
    core_util_critical_section_exit();
}

void PinDetect::attach_deasserted_held(Callback<void()> function) {
    core_util_critical_section_enter();
    _callbackDeassertedHeld = function;
    core_util_critical_section_exit();
}

void PinDetect::isr(void) {
    int currentState = _in->read();

    if ( currentState != _prevState ) {
        if ( _samplesTillAssert == 0 ) {
            _prevState = currentState;
            _samplesTillHeld = _samplesTillHeldReload;

            if ( currentState == _assertValue ) {
                if (_callbackAsserted) _callbackAsserted.call();

            } else {
                if (_callbackDeasserted) _callbackDeasserted.call();
            }

        } else {
            _samplesTillAssert--;
        }

    } else {
        _samplesTillAssert = _samplesTillAssertReload;
    }

    if ( _samplesTillHeld ) {
        if ( _prevState == currentState ) {
            _samplesTillHeld--;

            if ( _samplesTillHeld == 0 ) {
                if ( currentState == _assertValue ) {
                    if (_callbackAssertedHeld) _callbackAssertedHeld.call();

                } else {
                    if (_callbackDeassertedHeld) _callbackDeassertedHeld.call();
                }
            }

        } else {
            _samplesTillHeld = 0;
        }
    }
}
}; // namespace AjK ends.

using namespace AjK;

