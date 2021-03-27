problem=load('F:\sem6\hpc\LP_MATLAB\adlittle.mat')
A=full(problem.A)
n=size(A,1)
T=table([A eye(n,n)]);
writetable(T,'A_pilot_ja.txt','Delimiter',',','WriteRowNames',false,'WriteVariableNames',0);
writetable(T,'A.txt','Delimiter',',','WriteRowNames',false,'WriteVariableNames',0);

b=full(problem.b)
T=table([b]);
writetable(T,'b_pilot_ja.txt','Delimiter',',','WriteRowNames',false,'WriteVariableNames',0);
writetable(T,'b.txt','Delimiter',',','WriteRowNames',false,'WriteVariableNames',0);

c=full(-1*problem.c)
T=table([c' zeros(1,n)]);
writetable(T,'c_pilot_ja.txt','Delimiter',',','WriteRowNames',false,'WriteVariableNames',0);
writetable(T,'c.txt','Delimiter',',','WriteRowNames',false,'WriteVariableNames',0);
