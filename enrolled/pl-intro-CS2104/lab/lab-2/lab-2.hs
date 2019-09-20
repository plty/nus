module Lab02 where
import           Control.Monad (join)
import qualified Data.List     as L
import           Prelude       hiding (product, reverse)
import Debug.Trace (trace)
{-|

   Please complete `Lab02.hs` and submit it to LumiNUS
   by 11pm, 1st October 2019 (Tues)

-}
{-|
   Q1:
   Consider the following implementation of reverse using 'foldr'. It contains a
   bug that causes it to return the wrong result. Can you fix it so that you get
   the following correct outcome?
   > reverse [1, 2, 3]
   [3, 2, 1]
   > reverse [1, 2]
   [2, 1]
   > reverse []
   []
-}
reverse :: [a] -> [a]
reverse xs = foldr (\x acc -> acc ++ [x]) [] xs
{-|
   Q2: List Operaions
   Using only 'map', 'filter', 'foldl' and 'join', write code for the following list
   operations. You may use let constructs to name intermediate computations
-}
{-|
   (a)
   Compute the cross product of 2 lists that returns an element from each list as
   a pair (a, b) using foldl.
   > product [1, 2, 3] ['a', 'b']
   [(1, 'a'), (1, 'b'), (2, 'a'), (2, 'b'), (3, 'a'), (3, 'b')]
-}
product :: [a] -> [b] -> [(a, b)]
product xs ys = foldl (\acc x -> acc ++ (map (\y -> (x, y)) ys)) [] xs
{-|
   (b)
   Compute the cross product (without using foldl) of 2 lists that returns an integer from each list as
   a pair (a, b), such that a*b > a+b.
   > product2 [1, 2, 3] [2, 7]
   [(3,2),(2,7),(3,7)]
-}
product2 :: [Int] -> [Int] -> [(Int, Int)]
product2 xs ys = filter (\(x, y) -> x * y > x + y) (product xs ys)
{-|
   (c)
   Compute the divisor from the product of 2 lists, but only if the second
   element is non-zero.
   > divisorProd [5, 9, 4] [2, 0, 3]
   = [5/2,9/2,4/2,5/3,9/3,4/3]
   = [2, 4, 2, 1, 3, 1]
-}
divisorProd :: [Int] -> [Int] -> [Int]
--divisorProd xs ys = map (\(x, y) -> (div x y)) (filter (\(_, y) -> y /= 0) (product xs ys))
divisorProd xs ys = map (\(y, x) -> (div x y)) (filter (\(y, _) -> y /= 0) (product ys xs))
{-|
   Q3:
   Write a function that could count the number of positive, negative, and zero
   elements in a list of numbers;
   (i)  using only 'foldr'
   (ii) using only 'filter' and 'length'
-}
countNums1 :: [Int] -> (Int, Int, Int)
countNums1 xs = foldr (\x (p, q, r) -> ((if x > 0 then p + 1 else p), (if x < 0 then q + 1 else q), (if x == 0 then r + 1 else r))) (0, 0, 0) xs
countNums2 :: [Int] -> (Int, Int, Int)
countNums2 xs = (length (filter (\x -> x > 0) (xs)), length (filter (\x -> x < 0) (xs)), length (filter (\x -> x == 0) (xs)))
{-|
   Q4: Higher-Order functions for trees
   We can implement 2 common higher-order functions, mapTree and foldTree, on
   simple binary trees, as shown below. Use these two higher order functions to
   complete the subsequent questions on binary trees.
-}
data Tree a
   = Leaf a
   | Node a (Tree a) (Tree a)
   deriving Show
t1 :: Tree Int
t1 = Node 3 (Leaf 1) (Leaf 2)
t2 :: Tree Int
t2 = Node 4 t1 (Leaf 6)
t3 :: Tree Int
t3 = Node 5 t2 (Leaf 3)
instance Functor Tree where
   fmap f t =
     case t of
       Leaf v ->
         Leaf (f v)
       Node v leftTree rightTree ->
         Node (f v) (fmap f leftTree) (fmap f rightTree)
mapTree :: (a -> b) -> Tree a -> Tree b
mapTree = fmap
foldTree :: (a -> b) -> (a -> b -> b -> b) -> Tree a -> b
foldTree fLeaf _ (Leaf v) = fLeaf v
foldTree fLeaf fNode (Node v leftTree rightTree) =
   fNode v leftRes rightRes
   where
     leftRes = foldTree fLeaf fNode leftTree
     rightRes = foldTree fLeaf fNode rightTree
{-|
   (a)
   Write a function that would add n to every element of a tree.
   > t1
   Node 3 (Leaf 1) (Leaf 2)
   > addN 3 t1
   Node 6 (Leaf 4) (Leaf 5)
-}
addN ::Int -> Tree Int -> Tree Int
addN n = mapTree (+ n)
{-|
   (b)
   Write a function that would return the left most element of a tree
   > t2
   Node 4 (Node 3 (Leaf 1) (Leaf 2)) (Leaf 6)
   > leftMost t2
   1
   > t3
   Node 5 (Node 4 (Node 3 (Leaf 1) (Leaf 2)) (Leaf 6)) (Leaf 3)
   > leftMost t3
   1
-}
leftMost :: Tree a -> a
leftMost = foldTree (\(Leaf x) -> x) (\(Node _ l _) -> l)
{-|
   (c)
   Write a function that would mirror a tree aound its root element, i.e., a
   tree with its left and right subtrees recursively flipped.
   > t2
   Node 4 (Node 3 (Leaf 1) (Leaf 2)) (Leaf 6)
   > mirrorTree t2
   Node 4 (Leaf 6) (Node 3 (Leaf 2) (Leaf 1))
-}
mirrorTree :: Tree a -> Tree a
mirrorTree = foldTree (\x -> x) (\(Node v l r) -> Node v r l)
t4 :: Tree Char
t4 = Node 'a' (Leaf 'b') (Node 'c' (Leaf 'e') (Leaf 'f'))
{-|
   (d)
   Write a function that would tag each element of a tree with the size of its
   subtree.
   > t4
   Node 'a' (Leaf 'b') (Node 'c' (Leaf 'e') (Leaf 'f'))
   > addSize t4
   Node (5, 'a') (Leaf (1, 'b')) (Node (3, 'c') (Leaf (1, 'e')) (Leaf (1, 'f')))
-}
getValue :: Tree a -> a
getValue Leaf v -> v
getValue Node v _ _ -> v

addSize :: Tree a -> Tree (Int, a)
addSize = foldTree (\(Leaf x) -> Leaf (x, 1)) (\(Node v l@(Tree (_, a)) r@(Tree (_, b))) -> Node (v, a + b) l r)
{-|
   (e)
   Write a function to check if a tree of integers is a binary search tree, i.e.,
   a tree where all the elements in the left subtree are strictly smaller than
   the root node, which is, in turn, smaller than or equal to all the elements
   of the right subtree.
   > t1
   Node 3 (Leaf 1) (Leaf 2)
   > checkBST t1
   False
   > t5
   Node 2 (Leaf 1) (Leaf 3)
   > checkBST t5
   True
-}
t5 :: Tree Int
t5 = Node 2 (Leaf 1) (Leaf 3)
checkBST :: Tree Int -> Bool
checkBST = error "'checkBST' - To be implemented"
{-|
   Q5:
   The 'foldTree' operation uses tree recursion.
   Let us write a different tree folding operation that works with the help of
   an accumulating parameter that would be similar to 'foldl'.
   An example of this which uses postorder traversal is given below.
   (a) Count the number of elements in a tree using 'foldTreePostorder'
   > t4
   Node 'a' (Leaf 'b') (Node 'c' (Leaf 'e') (Leaf 'f'))
   > countTree t4
   5
   (b) Compare 'foldTreePostorder' with 'foldTree'. Can one be implemented in
       terms of the other, or are they incomparable?
-}
foldTreePostorder :: Show a => (a -> b -> b) -> Tree a -> b -> b
foldTreePostorder f (Leaf v) acc = f v acc
foldTreePostorder f (Node v leftTree rightTree) acc =
   foldTreePostorder f leftTree z2
   where
     z1 = foldTreePostorder f rightTree acc
     z2 = f v z1
countTree :: Tree a -> Int
countTree = error "'countTree' - To be implemented"
{-|
   Q6: Pretty printers
   Consider the binary tree defined earlier.
   You have been given a higher-order printer, 'showTree', that returns a tree as
   a string (prints a tree) in prefix form. For example, the tree 't2' would be:
   Node 4
   Node 3
   Leaf 1
   Leaf 2
   Node 3
   Leaf 1
   Leaf 2
   (i) The above pretty printing is not very readable; provide a neater pretty
        printer in 'showTree2' that would provide space indentation to represent
        the depth of each subtree.
        > showTree2 t2
        Node 4
          Node 3
            Leaf 1
            Leaf 2
          Node 3
            Leaf 1
            Leaf 2
   (ii) We can also print a tree in infix form. Complete the 'showTreeInfix'
        function to allow binary trees to be printed in infix order.
         > showTreeInfix t2
             Leaf 1
           Node 3
             Leaf 2
         Node 4
             Leaf 1
           Node 3
             Leaf 2
-}
showTree :: Show a => Tree a -> String
showTree (Leaf v) = "Leaf " ++ show v ++ "\n"
showTree (Node v leftTree rightTree) =
   "Node " ++ show v ++ "\n" ++
   showTree leftTree ++
   showTree rightTree
showTree2 :: Show a => Tree a -> String
showTree2 = error "'showTree2' - To be implemented - neat tree with nested indentation"
showTreeInfix :: Show a => Tree a -> String
showTreeInfix = error "'showTreeInfix' - To be implemented - neat tree with nested indentation in infix form"
{-|
   Q7: Numbered Lists
   We have our own printer for lists in 'showList', which prints a list as a
   comma separated string of its elements, surrounded by square brackets
   > showList ls
   "[\"This\", \"is\", \"a\", \"numbered\", \"list\"]"
   Give a list printer that would number each element of a given list and print
   it.
   > showListNum ", " ls
   "[(1)\"This\", (2)\"is\", (3)\"a\", (4)\"numbered\", (5)\"list\"]"
   You may make use of the addNum function below which adds a number to each
   element of a list.
-}
showList :: Show a => [a] -> String
showList xs = "[" ++ L.intercalate ", " (map show xs) ++ "]"
addNum :: [a] -> [(Int, a)]
addNum xs =
   let
     aux [] _       = []
     aux (y : ys) n = (n, y) : aux ys (n + 1)
   in
     aux xs 1
ls :: [String]
ls = ["This", "is", "a", "numbered", "list"]
showListNum :: Show a => String -> [a] -> String
showListNum separator = error "'showListNum' - To be implemented"
