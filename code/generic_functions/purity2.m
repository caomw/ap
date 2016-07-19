function[pur] = purity2(ground_truth,idx)

tamano = size(idx);

for i=1:tamano(1)
    pur(i) = purity(ground_truth,idx(i,:));
end

