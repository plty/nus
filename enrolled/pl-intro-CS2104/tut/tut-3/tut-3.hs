-- Efficient Recursion and Higher-Order Functions.
-- Please submit your Tut3 solution by 20 Sept 6pm
-- on IVLE for participation mark

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

--factlist_tup :: (Eq t, Num t) => t -> (t,[t])
factlist_tup 0 = (1, [1])
factlist_tup n = (\(a, b) -> (a * n, a * n:b)) (factlist_tup (n - 1))


{-
    Q2.
    (a)  Guess the types for each of the functions below.
    (b)  Write two examples of each of the functions below.
    (c)  Write a tail recursive version of (|>>|), calling it |>>>|.
         Is it helpful for this function to be tail-recursive?
         You may use the rev method that can help reverse a list
            rev :: [t] -> [t]

-}

(|>>|) :: (t -> a) -> [t] -> [a]
(|>>|) _ []     = []
(|>>|) f (x:ys) = f x : f |>>| ys

fold_right :: (t1 -> t2 -> t2) -> [t1] -> t2 -> t2
fold_right _ [] a     = a
fold_right f (x:ys) a = f x (fold_right f ys a)

map2 :: (t1 -> t2 -> a) -> [t1] -> [t2] -> [a]
map2 _ [] []         = []
map2 _ _ []          = error "Length of two lists are not equal"
map2 _ [] _          = error "Length of two lists are not equal"
map2 f (x:xs) (y:ys) = f x y : map2 f xs ys


(|>>>|) :: (a -> b) -> [a] -> [b]
(|>>>|) f xs =
    let aux [] zx     = zx
        aux (x:ys) zx = aux ys (zx ++ [f x])
    in
        aux xs []

{-
    EXAMPLES
-}
prodlist = map2 (*)
timestwo = (|>>>|) (*2)
{-
    END EXAMPLES
-}

{-
    Q3. Use fold_right to implement each of
    the following functions.

-}

len :: Num p => [a] -> p
len xs = fold_right (\_ y -> y + 1) xs 0

rev :: [a] -> [a]
rev xs = fold_right (\x y -> y ++ [x]) xs []

sumlen :: (Num a, Num b) => [a] -> (a, b)
sumlen xs = fold_right (\x (p, q) -> (p + x, q + 1)) xs (0, 0)

app :: [a] -> [a] -> [a]
app xs ys= fold_right (\x y -> x: y) xs ys

filter2 :: (a -> Bool) -> [a] -> [a]
filter2 f xs = fold_right (\x y -> if f x then (x:y) else y) xs []


{-
    Q4. Can you rewrite dropWhile2 below to use fold_right.
    This is a bit more challenging

    Your first attempt may involve:

    dropWhile2 f xs = 
      fold_right (\x r -> if f x then r else ?) xs []

   However, the remaining list "xs" is not avaialble
   in r. Hint: You need to think about how this may be added.

-}

dropWhile2 :: (a -> Bool) -> [a] -> [a]
dropWhile2 f xs = (\(_, q) -> q) (fold_right (\x (p, q) -> if f x then (x:p, q) else ([], x:(p ++ q))) xs ([], []))


{-
    Q5. Can you write a function 'for_all' that takes a function 'f' and a list '[x1, x2, ...]' as parameter
        and return true if and only if 'f x1 & f x2 & ... is true'
    Write this as a recursive function.
    After that, implement it using fold_right.
-}

for_all :: (a -> Bool) -> [a] -> Bool
for_all f xs = fold_right (\x y -> f x && y) xs True


{-
Q6. Implement a higher-order function that would apply
  a given function n-times:
  iter n f x = f (f ...(f x) ..)

Can you re-implement this using fold_right together
with the help of other auxiliary functions?

-}

iter :: Int -> (a->a) -> a -> a
iter n f x = fold_right (\_ y -> f y) [1..n] x


{-
Q7. Implement iter in terms of 
    of the composition operator (.)
    where f is applied n times
	

	What is the inferred type of iter2

-}

iter2 :: (Num t1, Enum t1) => t1 -> (b -> b) -> b -> b
iter2 n f = fold_right(\_ y -> f . y) [1..n] (\x -> x)


{-
Q8. Consider the function that applies the map function twice.
    Rewrite this function using only a single map.
-}

double_map f g xs = map f (map g xs)

double_map2 f g xs = map (f . g) xs

main = putStrLn  "Tutorial 3"
