function [clus_tuning,gt_tuning,clus_test,gt_test,positions] = tuning_test(clusters,num_clusters,tam_clusters,ground_truth)


tam_tuning = round(tam_clusters/2);
n=1;
t=1;
tam = size(clusters)
positions = zeros(tam(1),1);
for i=1:tam(1)
    valor = clusters(i);
   if(tam_tuning(valor) ~= 0)
      clus_tuning(n) = clusters(i);
      gt_tuning(n) = ground_truth(i);
      tam_tuning (valor) = tam_tuning(valor)-1;
      n=n+1;
   else
      clus_test(t) = clusters(i);
      gt_test(t) = ground_truth(i);
      t=t+1;
      positions(i,1) = 1;
   end
end
clus_test = clus_test';
gt_test = gt_test';
clus_tuning = clus_tuning';
gt_tuning = gt_tuning';
 
