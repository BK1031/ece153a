<div style="text-align:right;">Bharat Kathi</div>
<div style="text-align:right;">ECE 153A</div>
<div style="text-align:right;">11/15/23</div>

# HW 5

## Q1

**a)**
<p align="middle">
  <img src="https://github.com/BK1031/ece153a/blob/main/hw5/1a.png?raw=true" width="500" />
</p>

**b)** 

Here are the reconstructed frequencies for each of the input frequencies.
250: 250 Hz
333: 333.33 Hz
667: 333.33 Hz
750: 250 Hz
1250: 333.33 Hz
1333: 333.33 Hz

## Q2

**a)**

Period = 1/sampling freq = 1/44.1kHz = 22.68 microseconds

**b)**

Vrms = J * T = 1 * 10^-6 * 22.67 * 10^-6 = 22.68 * 10^-12 = 22.67 picovolts

**c)** 

Signal to noise ratio = 10 log(10) * ( 2V / 22.68 * 10^-12 ) = 109.45

Effective number of bits = ( SNR - 1.76 ) / 6.02 = 17.89 bits

## Q3

**a)**

50 kHz / 128 = 390.625 Hz

**b)**

50 kHz / 2 = 25 kHz

**c)**

If the sampling frequency is increased and the number of FFT bins remains constant, then the bin spacing would decrease.

**d)**

((x / 128) / 5) < 21

The largest sampling frequency that can be used with a 128-point FFT to allow 440 and 461 Hz to be distinguished is 13440 Hz.