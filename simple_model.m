% mg-ks(t)=ms'(t)
c = 1.7; %luftmotst�ndskoefficient
d = 0.4; %densitet
a = 0.0025; %tv�rsnittsarea i m^2
v = s'; %hastighet
m = 0.005; %massa i kg
k = (1/2)*c*d*a*v^2; %luftmotst�nd
g = 9.82;


syms s(t)
s(t) = dsolve(m*g - k*s == m*diff(s), s(0)==0)

%hmmm plot

x=1:0.01:10;
y = (491/500)*(1 - exp(-x));
plot(x,y); % s beroende p� t