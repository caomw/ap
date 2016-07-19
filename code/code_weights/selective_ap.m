function [w,SAP,AP] = selective_ap(C)
%Selective AP model
%Version 5.2
% Incorporates local seach refinement for the clustering aggregation
%Input:
%  C -> codebooks (weak input partitions)
%Output:
%  SAP-> Selective AP (final partition)
%  AP -> final partition without weights
%  w -> vector of weights


%m number of clusterings and np number of vectors
[np,m] = size(C);

%Number of clusterigs for each optimization m-1
mo = m-1;

%Define max number of iterations
T = m;

%Select the partitions randomly
it_index = randperm(m);

%Declare W matrix
W = zeros(T,m);

for t=1:T
	%Select one clustering (input weak partition) as ground-truth
	ci = it_index(t);
    
    %Extract clustering ci from Ct
    Ct = C; Ct(:,ci) = [];
    Cgt = C(:,ci);
    
    %Compute conditional entropy between the clusters in C and Cgt
    DC = conditionalentropy2(Cgt',Ct');
    
    
    %% Optimize weights for a minimum Ce of the solution with Cgt
    
    %Declare fitness function
    fitnessFcn = @(x) min_ce(x,Ct,Cgt,DC);

    %Define weights (m-1)
    numberOfVariables = mo;
    %Define bounds
    %binary problem
    minimun=0; maximun=mo;
    LB = minimun*ones(1,numberOfVariables); %min
    UB = maximun*ones(1,numberOfVariables); %max
    Bound = [LB;UB];
    psize = min((2^mo),256); 
    
    % Create an options structure to be passed to GA
    % Three options namely 'CreationFcn', 'MutationFcn', and
    % 'PopInitRange' are required part of the problem.
    options = gaoptimset('CreationFcn',@int_pop,'MutationFcn',@int_mutation, ...
        'PopInitRange',Bound,'Display','iter','StallGenL',5,'Generations',1, ...
        'PopulationSize',psize,'UseParallel','always','Vectorized', 'off','PlotFcns',{@gaplotbestf,@gaplotbestindiv});
    
    %Call optimization function
    [x,fval] = ga(fitnessFcn,numberOfVariables,options);    
    %fval - condition entropy
    %x - weights for the current iteration
    W(t,:) =[x(1:ci-1) 0 x(ci:end)];   
    
    
end

%Give the final solution
w = mode(W); 

%check that we don't obtain sum(w) = 0
if sum(w) == 0
w = w+1; % all wegihts to one
end

%Obtain the definitive clustering (according to the weights)
[CAW,SAP,y,new_num_clusters] = compute_bestoption(w,C,m,np);

%% Compute the standard version where all clusterings have weight 1
%Usefull for further comparisons
[foo,AP,y,new_num_clusters2] = compute_bestoption(ones(1,m),C,m,np);



%---------------------------------------------------
% Mutation function to generate childrens satisfying the range and integer
% constraints on decision variables.
function mutationChildren = int_mutation(parents,options,GenomeLength, ...
    FitnessFcn,state,thisScore,thisPopulation)
shrink = .01; 
scale = 1;
scale = scale - shrink * scale * state.Generation/options.Generations;
range = options.PopInitRange;
lower = range(1,:);
upper = range(2,:);
scale = scale * (upper - lower);
mutationPop =  length(parents);
% The use of ROUND function will make sure that childrens are integers.
mutationChildren =  repmat(lower,mutationPop,1) +  ...
    round(repmat(scale,mutationPop,1) .* rand(mutationPop,GenomeLength));
% End of mutation function

%---------------------------------------------------
function Population = int_pop(GenomeLength,FitnessFcn,options)
totalpopulation = sum(options.PopulationSize);
range = options.PopInitRange;
lower= range(1,:);
span = range(2,:) - lower;
% The use of ROUND function will make sure that individuals are integers.
Population = repmat(lower,totalpopulation,1) +  ...
    round(repmat(span,totalpopulation,1) .* rand(totalpopulation,GenomeLength));
% End of creation function



%---------------------------------------------------
%Minimize the ce of the partition
function  y = min_ce(x,Ct,Cgt,DC)

if (sum(x) == 0)
    y = 1000; %return a high value to discard this option of weights
    return;
end
%x encodes the weights of the clusterings

%1)Build Ct
Cext = [];
[npoints,N_clusterings] = size(Ct);

for k=1:N_clusterings
    %build Ct for this iteration
    Cext =[Cext repmat(Ct(:,k),1,x(k))];
end
%update number of clusterings
N_clusterings = sum(x);

%2)Compute the new distances matrix for the clustering aggregation
Xuv = compute_distances_c(npoints,double(Cext));
%Matrix of distances (square)
D=squareform(Xuv);

%Save temp file with distances
tmp_file_distances = tempname;
tmp_file_out = tempname;

%Speed up writing to disk
[rows cols] = size(D);
pattern = repmat('%d ',1,(cols-1));
fid =fopen(tmp_file_distances,'w');
fprintf(fid,'%d\n',npoints);
fprintf(fid,[pattern,'%d\n'],D');
fclose(fid);

%free memory
clear D;
clear Xuv;

%Run clustering aggregation
command =['export LD_LIBRARY_PATH=/usr/lib; ../../../../code/aggregation/ca_code/./aggregated ' tmp_file_distances ' A a O ' tmp_file_out];
[status,result]=system(command);

%Read results
CA=dlmread([tmp_file_out '.out']);

%free disk
delete(tmp_file_distances,[tmp_file_out '.out'],[tmp_file_out '.log']);

%Compute ce (Cgt,CA)
ce = conditionalentropy(Cgt',CA');

%Output combination
%ce + norm + loss_function
y = ce + sum(abs(x)) + lossf(x,DC);
% End of the function

%New lossfunction
function l = lossf(w,DC)

l = 0;
%Enforce sparsity of more than one term
if (sum(w) < 2) %penalize
    l = sum(DC);
end


function [CA,new_CA,y,new_num_clusters] = compute_bestoption(x,C,N_clusterings,npoints)

%1)Build Ct
Ct = [];
for k=1:N_clusterings
    %build Ct for this iteration
    Ct =[Ct repmat(C(:,k),1,x(k))];
end
%update number of clusterings
N_clusterings = sum(x);

%2)Compute the new distances matrix for the clustering aggregation
Xuv = compute_distances_c(npoints,double(Ct));
%Matrix of distances (square)
D=squareform(Xuv);

%Save temp file with distances
tmp_file_distances = tempname;
tmp_file_out = tempname;
tmp_file_ls = tempname;
dlmwrite(tmp_file_distances,npoints);
dlmwrite(tmp_file_distances,D,'-append','delimiter', ' ');

%free memory
clear D;
clear Xuv;

%Run aggregation
command =['export LD_LIBRARY_PATH=/usr/lib; ../../../../code/aggregation/ca_code/./aggregated ' tmp_file_distances ' A b 0.4 O ' tmp_file_out];
[status,result]=system(command);
CA = dlmread([tmp_file_out '.out']);
CA = CA+1;
doc_tmp = [tmp_file_out '.out'];
command2 = ['!../../../../code/aggregation/ca_code/./aggregated ' tmp_file_distances ' I ' doc_tmp ' L d O ' tmp_file_ls];
eval(command2);
new_CA = dlmread([tmp_file_ls '.out']);
new_CA = new_CA+1;
new_num_clusters = length(unique(new_CA));

%Compute d(C)
y = compute_score_ca(CA,C);


function d=compute_cd(C,Cgt)
[n,m] =size(C);

for xx=1:m    
    d(xx) = compute_score_ca(C(:,xx),Cgt);        
end
