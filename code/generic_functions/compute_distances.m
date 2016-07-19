function[D] = compute_distances (points_X,mat_C)

%This function compute the distances 
num_C=size(mat_C);
Xuv = zeros(1,points_X*(points_X-1)/2);
r=1;
for i=1:points_X-1
    for n=i+1:points_X
        for c=1:num_C(2)
            if(mat_C(i,c) ~= mat_C(n,c))
                Xuv(r)=Xuv(r)+1;
            end
        end
        Xuv(r)=Xuv(r)/num_C(2);
        r=r+1;
    end
end
D = squareform(Xuv);

