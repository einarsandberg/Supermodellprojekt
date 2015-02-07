
function vdot = funct(t,v)

airCoeff=1.28;
dens=1.2041; %densitet
area=0.0025; %tvärsnittsarea i m^2
mass=0.005;
g=9.82;

vdot=g-(0.5*airCoeff*dens*area*v^2)/mass;

