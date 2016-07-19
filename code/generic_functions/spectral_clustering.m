% this function implement the normalized similaity, spectral clustering,
% and evaluation

function [sc]=spectral_clustering(W,K,rep_sc)

D =sum(W,1);
D=D.^(-1/2);
D = diag(D);
L=D*W*D;
clear D W
opts.tol = 1e-3; 
opts.maxit=300;
[U,V]=eigs(L,K, 'LR',opts);

% Normalizing the row 
U=Normalized_Vector(U);
[m n]=size(U);

%-----------------------------------------------------------------------
for i=1:rep_sc
    fprintf('Building weak input partition %d/%d \n',i,rep_sc)
	sc(i,:)=kmeans(U,K,'emptyaction','singleton');
end





function T=Normalized_Vector(U)

P=U*U';
P1=diag(P);
clear P 
 
P2=P1.^(-1/2);
clear P1

P21=diag(P2);
clear P2 
 
T=P21*U;
