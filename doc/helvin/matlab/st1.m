load f:\radar\sim1.asc
x=sim1 (:,1);
y=sim1 (:,2);
Z = x + i*y;
%B = ABS(Z)
%k=length(Z);
%f = 1000*(1:k)/k;
%plot(f,Z(1:k))
%plot(Pbb(1:k))