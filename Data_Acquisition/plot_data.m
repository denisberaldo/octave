# Clear old variables
clear

# Variables initialization
p1_x = 0;
p1_y = 0;
p2_x = 0;
p2_y = 0;
lineTime = 1;
lineCurr = 1;
lineEnergy = 1;
lineVoltage = 1;

meanCount = 0;
meanPos = 1;
meanData = zeros(10,1);
meanSize = 20

ftime = fopen('data_time.txt', "r");
fcurr = fopen('data_current.txt', "r");
fenergy = fopen('data_energy.txt', "r");
#fvoltage = fopen('data_voltage.txt', "r");

% skipe some lines
%fskipl(fcurr, 2)

# Gets line by line until file isnt EOF
# Convert ASCII to Octave number format
# Increment line counter
while (!feof(ftime))
    TimeData(lineTime, 1) = str2num(fgetl(ftime));
    lineTime++;
endwhile

while (!feof(fcurr))
    CurrentData(lineCurr, 1) = str2num(fgetl(fcurr));

    if (meanCount == meanSize)
        meanCount = 0;
        meanData(meanPos) = mean(CurrentData((lineCurr-meanSize):lineCurr, 1), 1);
        meanPos++;
    endif

    lineCurr++;
    meanCount++;
endwhile

while (!feof(fenergy))
    EnergyData(lineEnergy, 1) = str2num(fgetl(fenergy));
    lineEnergy++;
endwhile

#while (!feof(fvoltage))
#  VoltageData(lineVoltage, 1) = str2num(fgetl(fvoltage));
#  lineVoltage++;
#endwhile

subplot (2, 1, 1);
# Plot logarithmic Y axis
# Convert time from us to ms
semilogy ((TimeData/1000), CurrentData);
grid();
title ("Current");
xlabel ("Aquisition time [ms]");
ylabel ("Current [nA]");

subplot (2, 1, 2);
# Convert Energy from nJ to uJ
%plot ((TimeData/1000), (EnergyData/1000));   # plot energy graphic
semilogy (meanData);                          # plot average current

grid();
set(gca,'xticklabel', num2str(get(gca,'xtick')'));
set(gca,'yticklabel', num2str(get(gca,'ytick')'));
title ("Energy");
xlabel ("Aquisition time [ms]");
ylabel ("Energy [uJ]");

# Put text in the figure
meanCount = 0;
while (meanCount < length(meanData))
    meanCount++;
    if (rem(meanCount, 50) == 0)
        if ((meanData(meanCount) > 1000) && (meanData(meanCount) < 1000000))
            text(meanCount, meanData(meanCount), strcat(num2str(meanData(meanCount)/1000), "uA"));
        elseif (meanData(meanCount) >= 1000000)
            text(meanCount, meanData(meanCount), strcat(num2str(meanData(meanCount)/1000000), "mA"));
        else
            text(meanCount, meanData(meanCount), strcat(num2str(meanData(meanCount)), "nA"));  
        endif
    endif
endwhile

# Dummy line, only to create a valid handle
h = line ([0 0], [0 0], "linestyle", "-", "color", "b");

while(1)
    [p1_x p1_y] = ginput(1);
    [p2_x p2_y] = ginput(1);
    delete(h);
    xlabel (strcat("x: ", num2str(p1_x - p2_x), "\ny: ", num2str(p1_y - p2_y)));
    h = line ([p1_x p2_x], [p1_y p2_y], "color", "b", "marker", "s");
endwhile

#subplot (3, 1, 3)
#plot (VoltageData);

fclose(fcurr);
fclose (fenergy);
#fclose (fvoltage);