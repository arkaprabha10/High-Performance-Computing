clc
clear all
%lpgen: Generate random solvable lp: min c’x s.t. Ax = b ;x>=0,
%Input: m,n,d(ensity); Output: A,b,c; (x, u): primal-dual solution
% m=input('Enter m:')256-2048
% n=input('Enter n:')256-4096
% d=input('Enter d:')0.1 0.50.9



%matrix of A will be of dimension mxn so vary that accordingly
%d is the density 
for d=[0.1,0.5,0.9]
    m=32;
    while m<257
        n=32;
        while n<257
            pl=inline('(abs(x)+x)/2');
            A=sprand(m,n,d);
            A=100*(A-0.5*spones(A)); x=sparse(10*pl(rand(n,1)));
            u=spdiags((sign(pl(rand(m,1)-rand(m,1)))),0,m,m)*(rand(m,1)-rand(m,1));
            b=A*x;
            c=A'*u+spdiags((ones(n,1)-sign(pl(x))),0,n,n)*10*ones(n,1);
            format short e;
            temp = [A eye(m,n) b; -1*c' zeros(1,n+1)];
            T=table(full(temp));
            file_name = strcat(strcat(strcat(strcat(strcat('str',num2str(d*10)),'_'),num2str(m)),'x'),num2str(n));
            writetable(T,file_name,'Delimiter',' ','WriteRowNames',false,'WriteVariableNames',0);
            n=n*2;m=m*2;
        end
        m=m*2;
        
    end
end
            
            
            % [norm(A*x-b), norm(pl(A'*u-c)), c'*x - b'*u]
