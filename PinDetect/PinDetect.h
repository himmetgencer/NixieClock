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

#ifndef AJK_PIN_DETECT_H
#define AJK_PIN_DETECT_H

#ifndef MBED_H
    #include "mbed.h"
#endif

#ifndef PINDETECT_PIN_ASSERTED
    #define PINDETECT_PIN_ASSERTED   1
#endif

#ifndef PINDETECT_SAMPLE_PERIOD
    #define PINDETECT_SAMPLE_PERIOD 20000
#endif

#ifndef PINDETECT_ASSERT_COUNT
    #define PINDETECT_ASSERT_COUNT  1
#endif

#ifndef PINDETECT_HOLD_COUNT
    #define PINDETECT_HOLD_COUNT    50
#endif

namespace AjK {

class PinDetect {
  public:
    friend class Ticker;


    /** PinDetect constructor
     *
     * By default the PinMode is set to PullDown.
     *
     * @see http://mbed.org/handbook/DigitalIn
     * @param p PinName is a valid pin that supports DigitalIn
     */
    PinDetect(PinName p);

    /** PinDetect constructor
     *
     * @see http://mbed.org/handbook/DigitalIn
     * @param PinName p is a valid pin that supports DigitalIn
     * @param PinMode m The mode the DigitalIn should use.
     */
    PinDetect(PinName p, PinMode m);

    /** PinDetect destructor
     */
    virtual ~PinDetect(void);

    /** Set the sampling time in microseconds.
     *
     * @param int The time between pin samples in microseconds.
     */
    void setSampleFrequency(int i = PINDETECT_SAMPLE_PERIOD);

    /** Set the value used as assert.
     *
     * Defaults to 1 (ie if pin == 1 then pin asserted).
     *
     * @param int New assert value (1 or 0)
     */
    void setAssertValue(int i = PINDETECT_PIN_ASSERTED);

    /** Set the number of continuous samples until assert assumed.
     *
     * Defaults to 1 (1 * sample frequency).
     *
     * @param int The number of continuous samples until assert assumed.
     */
    void setSamplesTillAssert(int i);

    /** Set the number of continuous samples until held assumed.
     *
     * Defaults to 50 * sample frequency.
     *
     * @param int The number of continuous samples until held assumed.
     */
    void setSamplesTillHeld(int i);

    /** Set the pin mode.
     *
     * @see http://mbed.org/projects/libraries/api/mbed/trunk/DigitalInOut#DigitalInOut.mode
     * @param PinMode m The mode to pass on to the DigitalIn
     */
    void mode(PinMode m);

    void attach_asserted(Callback<void()> function);

    void attach_deasserted(Callback<void()> function);

    void attach_asserted_held(Callback<void()> function);

    void attach_deasserted_held(Callback<void()> function);

    /** operator int()
     *
     * Read the value of the pin being sampled.
     */
    operator int() {
        return _in->read();
    }

  protected:
    DigitalIn   *_in;
    Ticker      *_ticker;
    int         _prevState;
    int         _currentStateCounter;
    int         _sampleTime;
    int         _assertValue;
    int         _samplesTillAssertReload;
    int         _samplesTillAssert;
    int         _samplesTillHeldReload;
    int         _samplesTillHeld;
    Callback<void()> _callbackAsserted;
    Callback<void()> _callbackDeasserted;
    Callback<void()> _callbackAssertedHeld;
    Callback<void()> _callbackDeassertedHeld;

    /** The Ticker periodic callback function
      */
    void isr(void);

    /** initialise class
     *
     * @param PinName p is a valid pin that supports DigitalIn
     * @param PinMode m The mode the DigitalIn should use.
     */
    void init(PinName p, PinMode m);
};

}; // namespace AjK ends.

using namespace AjK;

#endif
