# Clear old variables
clear

[data]=importdata('C:\Denis\Acq\data\capture.txt');
    
Fs = 24000;           % Sampling frequency                    
T = 1/Fs;            % Sampling period       
L = 2000;            % Length of signal
#t = (0:L-1)*T;      % Time vector
t = (0:L-1)/1;
f = Fs*(0:(L/2))/L;

#Y  = fft(data(:, 2));
#P2 = abs(Y/L);

P2a = abs(fft(data(:, 2))/L);
P1a = P2a(1:L/2+1);
P1a(2:end-1) = 2*P1a(2:end-1);

P2b = abs(fft(data(:, 3))/L);
P1b = P2b(1:L/2+1);
P1b(2:end-1) = 2*P1b(2:end-1);

P2c = abs(fft(data(:, 4))/L);
P1c = P2c(1:L/2+1);
P1c(2:end-1) = 2*P1c(2:end-1);

#FFT
#plot(f,P1);
plot(f,P1a,'-',f,P1b,'-',f,P1c,'-');
title('Single-Sided Amplitude Spectrum of X(t)');
xlabel('f (Hz)');
ylabel('P1');

#Timewave
#plot(t,data(:, 2),'-',t,data(:, 3));

grid minor;
