syms s(t);
airCoeff=1.28;
dens=1.2041; %densitet
area=0.0025; %tv�rsnittsarea i m^2
mass=0.005;
g=9.82;
x=1:10
Ds = diff(s)

hej = dsolve(Ds==g-(0.5*airCoeff*dens*area*s)/mass, s(0)==0)

%h = 25.48584 - 25.48584*exp(-0.3853*t);




 
clear all;



