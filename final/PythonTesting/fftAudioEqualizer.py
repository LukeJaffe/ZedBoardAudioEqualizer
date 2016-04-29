import sys
import wave
import struct
import numpy as np
from scipy.signal import firwin 
import matplotlib.pyplot as plt

from scipy.signal import butter, lfilter

# usage:  python audioEqualizer.py <wave_file_name.wav> <lowpass|highpass>

bufferWidth = 1024

def fftFreqScale(dataArray, fs, scalingTuples):
    '''
    scalingTuples are all (frequency, factor)
    interpreted:  up to {frequency}, scale by {factor}
    '''

    fft = np.fft.fft(dataArray)

    freqArray = fs * np.fft.fftfreq(len(dataArray))

    halfScalingArray = np.zeros([len(dataArray) / 2], dtype=np.float)

    currentScalingIndex = 0

    for i in range(len(dataArray) / 2):
        if freqArray[i] > scalingTuples[currentScalingIndex][0]:
            currentScalingIndex+= 1

        if (currentScalingIndex == 0):
            halfScalingArray[i] = scalingTuples[currentScalingIndex][1]
        else:
            halfScalingArray[i] = scalingTuples[currentScalingIndex-1][1] + \
                    (scalingTuples[currentScalingIndex][1] - scalingTuples[currentScalingIndex-1][1]) * \
                    ((freqArray[i] - scalingTuples[currentScalingIndex-1][0]) / (scalingTuples[currentScalingIndex][0]))

    dummy = np.concatenate((halfScalingArray, np.array([halfScalingArray[-1]])))
    scalingArray = np.concatenate((dummy , halfScalingArray[:0:-1]))

    fft = fft * scalingArray

    return np.round(np.real(np.fft.ifft(fft)))

f = wave.open(sys.argv[1], 'r');
w = wave.open(sys.argv[1].split(".")[0] + "Output.wav", 'w');

w.setnchannels(f.getnchannels())
w.setsampwidth(f.getsampwidth())
w.setframerate(f.getframerate())
w.setcomptype(f.getcomptype(), f.getcompname())

preFilter = np.array([])
postFilter = np.array([])

oneChannelInput = np.zeros([bufferWidth], dtype=np.int16)
otherChannelInput = np.zeros([bufferWidth], dtype=np.int16)

oneChannelOutput = np.zeros([bufferWidth], dtype=np.int16)
otherChannelOutput = np.zeros([bufferWidth], dtype=np.int16)

if sys.argv[2] == 'highpass':
    attenTuples = [(150.0, 0), (160.0, 1.0),  (1e6, 1.0)]
elif sys.argv[2] == 'lowpass':
    attenTuples = [(150.0, 1.0), (160.0, 0), (1e6, 0)]
elif sys.argv[2] == 'custom1':
    attenTuples = [(33.0, 1), (65.0, .5), (131.0, .0), (262.0, .0), (523.0, 0.0), (1047.0, 0.0), (2093.0, 0.0), (4186.0, 0.0), (1e6, 0.0)]
else:
    attenTuples = [(1e6, 1.0)]

# Samples from previous frame to be present at beginning of next frame
oneChannelOverlapInput = np.zeros([bufferWidth / 2])
oneChannelOverlapOutput = np.zeros([bufferWidth / 2])
otherChannelOverlapInput = np.zeros([bufferWidth / 2])
otherChannelOverlapOutput = np.zeros([bufferWidth / 2])

while (f.tell() < f.getnframes() / 40):
    print f.tell()
    oneChannelInput[0:bufferWidth/2] = oneChannelOverlapInput
    otherChannelInput[0:bufferWidth/2] = otherChannelOverlapInput

    for i in range(bufferWidth/2, bufferWidth):
        frame = f.readframes(1)
        oneChannelInput[i] = struct.unpack('<h', frame[0:2])[0]
        otherChannelInput[i] = struct.unpack('<h', frame[2:4])[0]

    # Theoretically, do processing stuff here
    oneChannelOutput = fftFreqScale(oneChannelInput * np.hanning(bufferWidth), f.getframerate(), attenTuples)
    otherChannelOutput = fftFreqScale(otherChannelInput * np.hanning(bufferWidth), f.getframerate(), attenTuples)

    packedData = ''

    oneChannelFiltered = oneChannelOutput[0:bufferWidth/2] + oneChannelOverlapOutput
    otherChannelFiltered = otherChannelOutput[0:bufferWidth/2] + otherChannelOverlapOutput

    #oneChannelFiltered = lfilter(b, a, oneChannelFiltered)
    for i in range(len(oneChannelFiltered) - 1):
        oneChannelFiltered[i+1] = 0.5 * oneChannelFiltered[i] + 0.5 * oneChannelFiltered[i+1]
    #otherChannelFiltered = lfilter(b, a, otherChannelFiltered)

    # Drop all overlap samples when writing out data
    for i in range(bufferWidth / 2):

        packedData += struct.pack('<h', oneChannelFiltered[i])
        packedData += struct.pack('<h', otherChannelFiltered[i])

    '''
    plt.subplot(211)
    plt.plot(oneChannelInput[0:bufferWidth/2])
    plt.subplot(212)
    plt.plot(oneChannelFiltered)
    plt.show()
    '''
    #print oneChannelFiltered[0], oneChannelFiltered[-1]

    w.writeframesraw(packedData)

    preFilter = np.concatenate((preFilter, oneChannelInput[:bufferWidth/2]))
    postFilter = np.concatenate((postFilter, oneChannelFiltered))

    oneChannelOverlapInput = oneChannelInput[bufferWidth/2:bufferWidth]
    oneChannelOverlapOutput = oneChannelOutput[bufferWidth/2:bufferWidth]
    otherChannelOverlapInput = otherChannelInput[bufferWidth/2:bufferWidth]
    otherChannelOverlapOutput = otherChannelOutput[bufferWidth/2:bufferWidth]

plt.subplot(211)
plt.plot(preFilter[80000:90000])
plt.title("Unfiltered Audio Data");

plt.subplot(212)
plt.plot(postFilter[80000:90000])
plt.title("Audio with Filter: " + str(sys.argv[2]) +", with Windowing")
if (sys.argv[2] == 'highpass'):
    plt.ylim((-2000, 2000))
plt.show()

w.close()
