function[v_centroid] = centroid(bow,Cx,num_clusters)

for i=1:num_clusters
    aux_Cx=(Cx==i);
    img_class = bow(aux_Cx,:);
    tam_class = size(img_class);
    
    if tam_class(1) > 1
    v_centroid(i,:) = sum(img_class)/tam_class(1);
    else
        v_centroid(i,:) = img_class;
    end
    
end

    
        
    
    

