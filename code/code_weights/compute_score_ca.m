function sca = compute_score_ca(C,Cm)

%Compute D(C) =  sum_i d_V(Cm_i,C)

[nv m] = size(Cm);
%m = number of clustering
%nv = number of elements

%initialize cost
sca = 0;
for x = 1:m
    sca = sca + d_V(C,Cm(:,x));
end

%Normalize with the number of clusterings?
sca = sca/m;
end

%Number of pairs of objects on wich the two clusterings disagree
function dv = d_V(C1,C2)

[npoints c] = size(C1);
dv = 0;
for n=1:npoints-1
    for m=n+1:npoints       
        if( ((C1(n) == C1(m)) & (C2(n) ~= C2(m))) | ((C1(n) ~= C1(m)) & (C2(n) == C2(m))) )
            dv = dv + 1;
        end            
    end
end

%normalize d_V?
K = nchoosek(npoints,2);
dv = dv/K;
end
