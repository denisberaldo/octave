# Clear old variables
clear
count = 1;
fd = fopen('C:\Denis\Acq\data\acq1.txt', "r");

# Gets line by line until file isnt EOF, convert ASCII to Octave number format
while (!feof(fd))
    data(count, 1) = str2num(fgetl(fd));
    count++;
endwhile
fclose(fd);

Fs = 24000;          % Sampling frequency
T = 1/Fs;            % Sampling period
L = 10000;            % Length of signal
t = (0:L-1)*T;       % Time vector
f = Fs*(0:(L/2))/L;

Ya  = fft(data);
Pa2 = abs(Ya/L);
Pa1 = Pa2(1:L/2+1);
Pa1(2:end-1) = 2*Pa1(2:end-1);

plot(f,Pa1);

title('Single-Sided Amplitude Spectrum of X(t)')
xlabel('f (Hz)')
ylabel('P1')

grid minor;
