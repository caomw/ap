function[conditional_entropy]=conditionalentropy2(ground_truth,idx)

tam=size(idx);
for i=1:tam(1)
    conditional_entropy(i) = conditionalentropy(ground_truth,idx(i,:));
end


