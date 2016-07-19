function S = compute_bow_similarity(bow,method);

%Compute similarity

switch method
    case 'hik'
        %Histogram intersection kernel
        fprintf('Computing HIK\n');
        S = hist_isect_c(bow,bow);
    case 'chi2'
        %Chi^2 distance
        fprintf('Computing Chi^2\n');
        D = chi_squared_c(bow,bow);
        a=mean(mean(D));
        S=exp((-1/(2*a^2))*D);
        clear D;
    case 'l2'
        %Gaussian Kernel with L2 norm
        fprintf('Computing Gaussian-L2\n');
        D = pdist2(bow,bow);
        a=mean(mean(D));
        S=exp((-1/(2*a^2))*D);
        clear D;
    case 'cosine'
        fprintf('Computing Cosine Similarity\n');
        max = size(bow,1);
        for a = 1:max
            for b = a:max
                val1 = dot(bow(a,:),bow(b,:));

                val2 = sqrt(sum(bow(a,:).^2)) * sqrt(sum(bow(b,:).^2));

                S(a,b) = val1/val2;
                S(b,a) = val1/val2;             
            end         
        end        
end