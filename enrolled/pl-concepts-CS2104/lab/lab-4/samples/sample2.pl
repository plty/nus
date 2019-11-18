grandparent(X,Y) :- parent(X,Z),parent(Z,Y). % Parsed: Program [Rule (Rel "grandparent" [X,Y]) [[Rel "parent" [X,Z]],[Rel "parent" [Z,Y]]]]
