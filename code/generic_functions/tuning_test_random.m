function [clus_tuning,gt_tuning,clus_test,gt_test,positions] = tuning_test_random(clusters,num_clusters,tam_clusters,ground_truth)

[num_vectors foo] = size(clusters);
positions = zeros(1,num_vectors);

for i = 1:num_clusters  
tam_aux = tam_clusters(i);
rand = randperm(tam_aux);
test = rand(1:round(tam_aux/2));

pos = find(clusters ==i);
datos = zeros(1,tam_aux);
datos(test) = 1;

for t =1:tam_aux
    if(datos(t) ==1)
        index = pos(t);
        positions(index) = 1;
    end
end
end

clus_test = clusters(positions == 1);
gt_test = ground_truth(positions == 1);
clus_tuning = clusters(positions == 0);
gt_tuning = ground_truth(positions == 0);






