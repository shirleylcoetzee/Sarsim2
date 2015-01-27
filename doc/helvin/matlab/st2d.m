load 
x=       (:,1);
y=       (:,2);
z = x + i*y;
%A=[x,y];
B=fft(z);
k=length(B);
Pbb = B.*conj(B);
f = 1000*(1:k)/k;
plot(f,Pbb(1:k))
%plot(Pbb(1:k))