load f:\radar\sim33.asc
x=sim33 (:,1);
y=sim33 (:,2);
X=(x-127)*9.73698e-9;
Y=(y-127)*9,73698e-9;
Z = X + i*Y;
B=abs(Z);
k=length(B);
d=1:k;
bar(d,B,'b')
