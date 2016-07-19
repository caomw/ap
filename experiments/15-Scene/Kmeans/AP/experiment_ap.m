%15-Scene
%Description of the experiment
% AP experiment:
%   -Kmeans to build weak input partitions


addpath(genpath('../../../../code/'));

%% Configure experiment
data_path = '../../../../data/';
%Create dir to save experiments
exp_name = 'AP-15-Scene-kmeans';
results_path = ['../../../../results/' exp_name '/'];

%Do you want to measure the conditional entropy or purity of the final
%partition?
do_eval = 1; %0 or 1

mkdir(results_path);
%Number of classes to discover
k=15;
%Read ground truth labels for the 15-scene dataset
disp('Reading ground truth data and descriptors')
cl = load([data_path 'ground_truth_scene.txt']);
ground_truth = cl;
cl = cl';



%Load precomputed descriptors


%Select the descriptor you want to load (uncomment the corresponding lines)

%SP-BOW
descriptors = load([data_path 'descriptors/PYRAMID_scene.txt']);

%GIST
%descriptors = load([data_path 'descriptors/GIST_scene.txt']);

%PHOG
%descriptors = load([data_path 'descriptors/PHOG_scene.txt']);

%LBP
%descriptors = load([data_path 'descriptors/LBP_scene.txt']);

%LLC
%descriptors = load([data_path 'descriptors/LLC_scene.txt']);

%ALL: SP-BOW+GIST+PHOG+LLC+LBP
%BOW_vector = load([data_path 'descriptors/PYRAMID_scene.txt']);
%LLC_vector = load([data_path 'descriptors/LLC_scene.txt']);
%GIST_vector = load([data_path 'descriptors/GIST_scene.txt']);
%PHOG_vector = load([data_path 'descriptors/PHOG_scene.txt']);
%LBP_vector = load([data_path 'descriptors/LBP_scene.txt']);
%descriptors = [BOW_vector,GIST_vector,PHOG_vector,LBP_vector,LLC_vector];



%% 1- K-means
disp('Computing weak input partitions - [Running]')
%We compute 100 weak input partitions using Kmeans
num_partitions = 100;

%Run clustering
idx = k_means_rep(descriptors,num_partitions,k);
clusters.idx = idx;

%Compute baseline CE and Purity
ce.initial_partition = conditionalentropy2(ground_truth',idx);
ce.initial_partition_mean = mean(ce.initial_partition);
pur.initial_partition = mean(purity2(ground_truth',idx));
pur.initial_partition_mean = mean(pur.initial_partition);


%% 2- Obtain the Aggregated Partition (AP) 
disp('Building AP via CLUSTERING AGGREGATION - [Running]')
mat_C = [idx];
mat_C = mat_C';
%Compute the distance matrix for the clustering aggregation algorithm
[npoints dim] = size(descriptors);
D = compute_distances(npoints,mat_C);
doc_dist = [results_path 'tmp_dist.txt'];

dlmwrite(doc_dist,npoints);
dlmwrite(doc_dist,D,'-append','delimiter', ' ');

doc_dest = [results_path 'tmp_agg'];
cmd = ['!../../../../code/aggregation/ca_code/./aggregated ' doc_dist ' A b 0.4 O ' doc_dest];
eval(cmd);


%Read the AP
AP = dlmread([doc_dest '.out']);
num_clusters_AP = length(unique(AP));
AP = AP+1;
clusters.AP = AP;
clusters.num_clus = num_clusters_AP;

%Refine the AP with the local search algorithm for clustering aggregation
doc_dest2 = [results_path 'tmp_agg_ls'];
doc_tmp = [doc_dest '.out'];
cmd = ['!../../../../code/aggregation/ca_code/./aggregated ' doc_dist ' I ' doc_tmp ' L d O ' doc_dest2];
eval(cmd);

%Read the definitive AP
new_AP = dlmread([doc_dest2 '.out']);
new_AP = new_AP+1;
new_num_clusters_AP = length(unique(new_AP));
clusters.num_clus_ls = new_num_clusters_AP;

%Note that new_AP represents the final Aggregated Partition detailed in our
%paper.


%% 3- Evaluation of the quality of the results
% Note that our AP approach may discover more clusters than is known in 
% the ground truth. Both the P and CE, get better and better when the number
% of discovered objects increases. But this is due to over-fitting rather 
%than discovering a good partition. In order to solve this problem, we
% strictly follow here the evaluation protocol described in [32]. It
% uses an oracle to assign each discovered topic to its best known class, 
% and then evaluates the resultant assignments using P or CE. 

if do_eval

    size_cluster = obtain_size_clusters(new_num_clusters_AP,new_AP);

    for t=1:5
        % Tuning and test
        [clus_tuning,gt_tuning,clus_test,gt_test,positions(t,:)] = tuning_test_random(new_AP,new_num_clusters_AP,size_cluster,ground_truth);
    
        %Learn the Oracle via optimization
        [x,fval] = optimization2(gt_tuning,clus_tuning,new_num_clusters_AP,1,k);
    
        %conditional entropy test
        d = x(clus_test);
        new_ce_agg(t) = conditionalentropy(gt_test',d);
        new_pur_agg(t) = purity(gt_test',d);
    
    
        d_agg(t,:) = x(new_AP);
    
    
        [tam foo] = size(idx);
        tam_test = sum(positions(t,:));
        test_AP = zeros(tam_test,tam);
        test_gt_AP = zeros(tam_test,tam);
        
        for n=1:tam
            %partition test
            partition_test = idx(n,:);
            partition_test = partition_test';
            ground_truth = cl';
            pos_test = (positions(t,:) ==1);
            test_AP(:,n) = partition_test(pos_test);
            test_gt_AP(:,n) = ground_truth(pos_test);
        
        end
    
        %measure conditional entropy
    
        for i=1:tam
            ce_partition_comp_agg(t,i) = conditionalentropy(test_gt_AP(:,i)',test_AP(:,i)');
        end
    
        mean_ce_partition_comp_agg(t) = mean(ce_partition_comp_agg(t,:));
    
        %measure purity
    
        for i=1:tam
            pur_partition_comp_agg(t,i) = purity(test_gt_AP(:,i)',test_AP(:,i)');
        
        end
    
        mean_pur_partition_comp_agg(t) = mean(pur_partition_comp_agg(t,:));
    
    end
    
    %% Final results
    
    %CE and Purity
    
    %For the AP
    ce.AP = new_ce_agg;
    ce.AP_mean = mean(new_ce_agg);
    pur.AP = new_pur_agg;
    pur.AP_mean = mean(new_pur_agg);
    
    %For the baselines
    ce.baseline = ce_partition_comp_agg;
    ce.baseline_mean = mean(mean_ce_partition_comp_agg);
    pur.baseline = pur_partition_comp_agg;
    pur.baseline_mean = mean(mean_pur_partition_comp_agg);
    
    %Save the results
    save([results_path 'results_experiment.mat'],'ce','pur','clusters');
    
end

%Finally save the Aggregated Partition with the categories discovered
save([results_path 'definitive_AP.mat'],'new_AP');


