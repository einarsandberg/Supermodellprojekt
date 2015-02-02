% mg-ks(t)=ms'(t)
m = 0.01;
k = 10;
g = 9.82;


syms s(t)
s(t) = dsolve(m*g - k*s == m*diff(s), s(0)==0)

%hmmm plot

x=1:0.01:10;
y = (491/500)*(1 - exp(-x));
plot(x,y); % s beroende på t