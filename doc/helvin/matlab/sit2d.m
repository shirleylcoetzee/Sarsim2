load h:\sim10.asc
x=sim10 (:,1);
y=sim10 (:,2);
z = x + i*y;
B=abs(z);
k=length(B);
d=1:k;
bar(d,B,'b')
