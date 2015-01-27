load c:\sim??.asc
x=sim?? (:,1);
y=sim?? (:,2);
z = x + i*y;
B=abs(z);
k=length(B);
d=1:k;
bar(d,B,'b')
