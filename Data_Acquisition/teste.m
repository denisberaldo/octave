#x = -10:0.1:10;
#plot (x, sin (x));
#plot (x, cos (x));
#h = line ([1 1], [1.8 0.8], "linestyle", "-", "color", "b");
#[cur_x cur_y] = ginput(1);
#delete(h);

#hold ("off");

x2 = 3*pi/4;
y2 = sin(3*pi/4);
txt2 = '\leftarrow sin(3\pi/4) = 0.71';
text(x2,y2,txt2)