function [ rmse ] = nihecha( p,r,cap )
%UNTITLED2 此处显示有关此函数的摘要
%   此处显示详细说明
%   x:为拟合多项式系数 y:为原始数据
t1=length(r);
t2=length(cap);
s=0;
if t1==t2
for i=1:t1
    z1(i)=p(1).*r(i)+p(2);%拟合点
%     z3(i)=cap(i)-z1(i);
    z2(i)=(cap(i)-z1(i)).^2;
    s=s+z2(i);
end
s=s/(t1-2);
rmse=sqrt(s);
else
    %数据维度不一致报错
end

end

