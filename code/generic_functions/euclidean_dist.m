function [e_dist] = euclidean_dist(X,Y)

tam_X = size(X);
tam_Y = size(Y);
l = max(tam_X(2),tam_Y(2));

for i=1:tam_X(1);
    aux_X = X(i,:);
    for j=1:tam_Y(1)
        aux_Y = Y(j,:);
          if(l==tam_X(2))
            resta = aux_X-aux_Y;
          else
            resta = aux_Y-aux_X;
          end
          e_dist(i,j) = sqrt(sum(resta.*resta));
        if(e_dist(i,j) == 0)  %the same vector
          e_dist(i,j) = 100000;
        end
    end
end

        
