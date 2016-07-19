function [idx] = k_means_rep(vectors,rep_kmeans,k)
idx = [];
for i=1:rep_kmeans
	fprintf('Building weak input partition %d/%d \n',i,rep_kmeans)
	idx(i,:)=kmeans(vectors,k,'emptyaction','singleton');	
end

        
