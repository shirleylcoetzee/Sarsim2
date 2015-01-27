load f:\RADAR\sim33.asc
x=sim33 (:,1);
y=sim33 (:,2);
X=(x-127)*9.73698-12;
Y=(y-127)*9.73698-12;
Z = X + i*Y;
B=abs(Z)
%k=length(C);
%d=1:k;
%xlabel('N');
%ylabel('Magnitude');
%bar(d,C,'b')
