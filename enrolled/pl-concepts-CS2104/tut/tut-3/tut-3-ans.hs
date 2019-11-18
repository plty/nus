-- Recursion and Higher-Order Functions.

import           Data.Array
import           Debug.Trace (trace)

{-
  stack ghci
  Prelude> :l Tut3.hs
-}

{-
    Q1: You have been asked to implement a list of factorials.
	A naive way of implementing it is given below. The naive
	algorithm has O(n^2) complexity.

    Can you write a more efficient tupled recursive method, call
    it factlist_tup, to do this in O(n) time?

-}


fact :: (Eq t, Num t) => t -> t
fact 0 = 1
fact n = n * fact (n-1)

factlist :: (Eq t, Num t) => t -> [t]
factlist 0 = []
factlist n = fact n : factlist (n-1)

factlist_tup :: (Eq t,Num t) => t ->(t,[t])
--factlist_tup n = error "To be implemented using tupled recursion and Integer"
-- computing a tuple outcome below
-- factlist_tup n = (fact n, factlist n)
-- using recursion to achieve efficient computation
factlist_tup 0 = (1,[])
factlist_tup n = let (a,b)=factlist_tup (n-1) in
                 let f = n * a in
                 (f,f:b)

{-
    Q2.
    (a)  Guess the types for each of the functions below.
    (b)  Write two examples of each of the functions below.
    (c)  Write a tail recursive version of (|>>|), calling it |>>>|.
         Is it helpful for this function to be tail-recursive?
         You may use the rev method that can help reverse a list
            rev :: [t] -> [t]

-}

(|>>|) :: (a -> b) -> [a] -> [b]
(|>>|) _ []     = []
(|>>|) f (x:ys) = f x : f |>>| ys

fold_right :: (a -> b -> b) -> [a] -> b -> b
fold_right _ [] a     = a
fold_right f (x:ys) a = f x (fold_right f ys a)

map2 :: (a -> b -> c) -> [a] -> [b] -> [c]
map2 _ [] []         = []
map2 _ _ []          = error "Length of two lists are not equal"
map2 _ [] _          = error "Length of two lists are not equal"
map2 f (x:xs) (y:ys) = f x y : map2 f xs ys


-- tail-recursion leads to constant stack space usage
-- however, for this case, there is an extra cost of
-- list reversal which negates the benefit of tail-recursion

(|>>>|) :: (a -> b) -> [a] -> [b]
(|>>>|) f xs =
    let aux [] zx     = reverse zx
        aux (x:ys) zx = aux ys (f x : zx)
    in
        aux xs []

{-
    Q3. Use fold_right to implement each of
    the following functions.

-}

len [] = 0
len (x:xs) = 1 + (len xs)

len2 xs = fold_right (\ x r -> 1+r) xs 0

rev [] = []
rev (x:xs) = rev xs ++ [x]

rev2 xs = fold_right (\ x r -> r++[x]) xs []

sumlen [] = (0,0)
sumlen (x:xs) = let (a,b) = sumlen xs in (x+a,1+b)

sumlen2 xs = fold_right (\ x (a,b) -> (x+a,1+b)) xs (0,0)


app [] ys = ys
app (x:xs) ys = x:(app xs ys)

app2 xs ys = fold_right (\ x r -> x:r) xs ys


filter2 f [] = []
filter2 f (x:xs) = 
  if f x then x:(filter2 f xs)
  else filter f xs

filter3 f xs = fold_right (\ x r -> if f x then x:r else r) xs []


{-
    Q4. Can rewrite dropWhile2 below to use fold_right.
    This is a bit more challenging

    Your first attempt may involve:

    dropWhile2 f xs = 
      fold_right (\x r -> if f x then r else ?) xs []
   However, the remaining list "xs" is not available
   in r. You need to think about how this may be added.

Ans : To make "xs" available, let us consider
a more general function which computes two results
namely:
  aux2 f xs = (dropWhile2 f xs,xs)
We can thus implement the above usign fold_right, as follows:

-}

aux2 f xs = fold_right (\ x (r1,r2) ->
     if f x then (r1,x:r2)
     else (r2,x:r2)
     ) xs ([],[]) 

dropWhile2 f [] = []
dropWhile2 f (x:xs) = 
  if f x then dropWhile2 f xs
  else xs

dropWhile3 f xs = fst (aux2 f xs)


{-
    Q4. Can you write a function 'for_all' that takes a function 'f' and a list '[x1, x2, ...]' as parameter
        and return true if and only if 'f x1 & f x2 & ... is true'
    Write this as a recursive function.
    After that, implement it using fold_right.
-}

for_all :: (a -> Bool) -> [a] -> Bool
for_all f = error "TBI - to be implemented"

for_all2 f [] = True
for_all2 f (x:xs) = (f x) && (for_all2 f xs)

for_all3 f (xs) = fold_right (\ x r -> (f x) && r) xs True


{-
Q5. Implement a higher-order function that would apply
  a given function n-times:
  iter n f x = f (f ...(f x) ..)

What is the type of this 
-}

iter::Int -> (a->a) -> a -> a

iter n f x = error "TBI"

iter_r 0 f x = x
iter_r n f x = iter_r (n-1) f (f x)

{-
Q6. Implement iter in terms of 
    of the composition operator (.)
    where f is applied n times

-}

iter2 n f = error "TBI"

iter2r 0 f = \ x -> x
iter2r n f = (iter2r (n-1) f) . f


{-
Q7. Consider the function that applies the map function twice.
    Rewrite this function using only a single map.
-}

dmap f g xs = map f (map g xs)

dmapr f g xs = map (f . g) xs


