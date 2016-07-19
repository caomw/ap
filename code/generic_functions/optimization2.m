function [x,fval] = optimization2(ground_truth,date,num_clus_date,min,max)
% Fitness function and numver of variables

fitnessFcn = @(x) min_ce(x,ground_truth,date);
numberOfVariables = num_clus_date;

% decision variables. 
LB = min*ones(1,numberOfVariables); %min
UB = max*ones(1,numberOfVariables); %max
Bound = [LB;UB]; 

% Create an options structure to be passed to GA
% Three options namely 'CreationFcn', 'MutationFcn', and
% 'PopInitRange' are required part of the problem.
options = gaoptimset('CreationFcn',@int_pop,'MutationFcn',@int_mutation, ...
    'PopInitRange',Bound,'Display','iter','StallGenL',20,'Generations',200,...
    'PopulationSize',500,'UseParallel','always','Vectorized', 'off','PlotFcns',{@gaplotbestf,@gaplotbestindiv});

[x,fval] = ga(fitnessFcn,numberOfVariables,options);

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

function  y = min_ce(x,gt,dat)
new_data = x(dat);   
y = conditionalentropy(gt',new_data);
% End of the function
