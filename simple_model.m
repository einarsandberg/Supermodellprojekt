clear all
timeRange=[0 10];
v_initial=0;
[t,v]=ode45(@funct,timeRange,v_initial);
plot(t,v)


