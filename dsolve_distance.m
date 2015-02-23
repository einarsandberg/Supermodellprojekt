syms s(t);
airCoeff=1.28;
dens=1.2041; %densitet
area=0.0025; %tv�rsnittsarea i m^2
mass=0.005;
g=9.82;
x=1:10
Ds = diff(s)
Ds_2=diff(s,2)
hej = dsolve(Ds_2==g-(0.5*airCoeff*dens*area*(Ds^2))/mass, s(0)==0, Ds(0)==0)


