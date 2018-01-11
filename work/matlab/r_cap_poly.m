

% i:直线
ri0=40:0.01:52;
ri1=38:0.01:47;
ri2=37:0.01:44;
ri3=36:0.01:42;
ri4=35:0.01:41;
ri5=35:0.01:40;
%j:分立点
% rj=35:0.5:52;
rj0=40:0.5:52;
rj1=38:0.5:47;
rj2=37:0.5:44;
rj3=36:0.5:42;
rj4=35:0.5:41;
rj5=35:0.5:40;

%一次 连续点
capi0=polyval(p0,ri0);
capi1=polyval(p1,ri1);
capi2=polyval(p2,ri2);
capi3=polyval(p3,ri3);
capi4=polyval(p4,ri4);
capi5=polyval(p5,ri5);

plot(r0,cap0,'o',ri0,capi0,'k');pause;
plot(r1,cap1,'o',ri1,capi1,'k');pause;
plot(r2,cap2,'o',ri2,capi2,'k');pause;
plot(r3,cap3,'o',ri3,capi3,'k');pause;
plot(r4,cap4,'o',ri4,capi4,'k');pause;
plot(r5,cap5,'o',ri5,capi5,'k');pause;

% 一次 分立点
%  capj0=polyval(p0,rj0);
%  capj1=polyval(p1,rj1);
%  capj2=polyval(p2,rj2);
%  capj3=polyval(p3,rj3);
%  capj4=polyval(p4,rj4);
%  capj5=polyval(p5,rj5);

% %二次 分立点
% capj0=polyval(q0,r0);
% capj1=polyval(q1,r1);
% capj2=polyval(q2,r2);
% capj3=polyval(q3,r3);
% capj4=polyval(q4,r4);
% capj5=polyval(q5,r5);