

clear all
syms teta(t);

timeRange=[0 10];
v_initial=0;
[t,v]=ode45(@funct,timeRange,v_initial);


airCoeff=1.28;
my = 1;
dens=1.2041; %densitet
area=0.0025; %tv�rsnittsarea i m^2

integrator=(sinh(my)*sin(teta)-cosh(my)*cos(teta));
mass=0.005;
l = 0.005;
h = 0.005;
g=-9.82;

integrated = int(integrator);


Dt = diff(teta);
t=0:0.01/50:10/50;
Solved_diff = dsolve(Dt==integrated,teta(0)==0);


vdot = v*dens*1*Solved_diff+1/4 * pi * dens*l*h*g;
fun = vdot (2);
for l = 1:89
    hold on;
    ezplot(vdot(l),[l-1 l])
    t = l;
    lol(l) = vpa(vdot(l));
end
plot(lol)
