function [size_clusters] = obtain_size_clusters(num_clusters,clusters)

for i=1:num_clusters  %number of elements in each cluster
    aux = (clusters==i);
    size_clusters(i) = sum(aux);
end