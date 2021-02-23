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

count = 1;
fd = fopen('C:\Denis\Acq\data\acq2.txt', "r");
while (!feof(fd))
    data2(count, 1) = str2num(fgetl(fd));
    count++;
endwhile
fclose(fd);

count = 1;
fd = fopen('C:\Denis\Acq\data\acq3.txt', "r");
while (!feof(fd))
    data3(count, 1) = str2num(fgetl(fd));
    count++;
endwhile
fclose(fd);


Fs = 420;            % Sampling frequency                    
T = 1/Fs;            % Sampling period       
L = 4000;            % Length of signal
t = (0:L-1)*T;       % Time vector
f = Fs*(0:(L/2))/L;

Ya  = fft(data);
Pa2 = abs(Ya/L);
Pa1 = Pa2(1:L/2+1);
Pa1(2:end-1) = 2*Pa1(2:end-1);

Yb  = fft(data2);
Pb2 = abs(Yb/L);
Pb1 = Pb2(1:L/2+1);
Pb1(2:end-1) = 2*Pb1(2:end-1);

Yc  = fft(data3);
Pc2 = abs(Yc/L);
Pc1 = Pc2(1:L/2+1);
Pc1(2:end-1) = 2*Pc1(2:end-1);

#plot(f,Pa1);
plot(f,Pa1,'-',f,Pb1,'-',f,Pc1,'-');


title('Single-Sided Amplitude Spectrum of X(t)')
xlabel('f (Hz)')
ylabel('P1')

resdc=Pa1(2)
res60=Pa1(576)

resdc=Pb1(2)
res60=Pb1(576)

resdc=Pc1(2)
res60=Pc1(576)


axis([0 200 0 8]);
grid minor;
