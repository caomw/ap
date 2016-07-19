function[new_clusters,new_num_clusters,new_tam_clusters] = sing2notsing(bow,num_clusters,clusters)
  
for i=1:num_clusters  %number of elements in each cluster
    aux = (clusters==i);
    tam_cluster(i) = sum(aux);
end

n_clusters = num_clusters;    
v_centroid = centroid(bow,clusters,num_clusters);  %centroids of the clusters

n_clusters = num_clusters;
for i=1:num_clusters %look for single clusters
   if(tam_cluster(i)==1);
       sing_clus = (clusters == i);
       v_singleton = bow(sing_clus,:);
       dist = euclidean_dist(v_singleton,v_centroid);
       [mini,pos] = min(dist);
       clusters(sing_clus) = pos;  
       tam_cluster(pos) = tam_cluster(pos)+1;
       tam_cluster(i) = tam_cluster(i)-1;
   end
end
aux = (tam_cluster ~= 0);
n_clusters = sum(aux);
flag = 1;
n=num_clusters-1;
for i=1:n_clusters
    if(tam_cluster(i) == 0)
      while(flag)
        for j=i:n
            valor = (clusters==(j+1));
            clusters(valor) = j;
            tam_cluster(j) = tam_cluster(j+1);
            tam_cluster(j+1);
        end
        n=n-1;
        num_clusters = num_clusters-1;
        flag = 0;
        if(tam_cluster(i) == 0)
            flag = 1;
        end
      end
      flag = 1;
    end
    
end
new_clusters = clusters;
new_num_clusters = n_clusters;
new_tam_clusters = tam_cluster(1:n_clusters);

