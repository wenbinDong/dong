function [ rmse ] = nihecha( p,r,cap )
%UNTITLED2 �˴���ʾ�йش˺�����ժҪ
%   �˴���ʾ��ϸ˵��
%   x:Ϊ��϶���ʽϵ�� y:Ϊԭʼ����
t1=length(r);
t2=length(cap);
s=0;
if t1==t2
for i=1:t1
    z1(i)=p(1).*r(i)+p(2);%��ϵ�
%     z3(i)=cap(i)-z1(i);
    z2(i)=(cap(i)-z1(i)).^2;
    s=s+z2(i);
end
s=s/(t1-2);
rmse=sqrt(s);
else
    %����ά�Ȳ�һ�±���
end

end

