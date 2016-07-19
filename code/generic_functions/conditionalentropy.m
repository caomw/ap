function [cond_ent] = conditionalentropy(X,Y)

l = length(Y);
cond_ent = 0;
for m=(min(Y):max(Y))       %for every obtained cluster label
    y = Y == m;     %images with label m
    py = sum(y)/l;
    sum_aux = 0;
    if(py ~= 0)
        for n=(min(X):max(X))   %for every cluster label in ground truth
            x = X == n;     %images with label n
            pxy = (sum(and(x,y))/l)/py;     %probability of intersection / probability of y = P(X|Y)
            if (pxy ~= 0)
                sum_aux = sum_aux + pxy*log2(1/pxy);
            end
        end
        cond_ent = cond_ent + py*sum_aux;
    end
end