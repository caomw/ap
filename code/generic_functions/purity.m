function [pur] = purity(X,Y)

l = length(X);
pur = 0;
for m=(min(Y):max(Y))       %for every obtained cluster label
    mx = 0;   
    y = Y == m;     %images with label m
    for n=(min(X):max(X))       %for every cluster label in ground truth
        x = X == n;     %images with label n
        pxy = sum(and(x,y))/l;      %probability of intersection
        if (pxy > mx)       %biggest probability
            mx = pxy;
        end
    end
    pur = pur + mx;
end
