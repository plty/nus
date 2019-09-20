import System.IO.Unsafe
import Data.Array

{-
    Tutorial 4 : Laziness, List Comprehension, Show Class and Arrays
	
   This tutorial rely on some topics that will be covered during
   lecture on 18Sept.

   Try complete this tutorial and submit in LumiNUS for participation
   mark by 29Sep2019. It will NOT be graded.
   
-}

{-
Q1
    Consider the following sieve method
    that can be used to generate a list of
    numbers without their multiples.

    Explain what happens when a finite list
    of numbers such as [3..100] is given.

    What should you do to avoid the
    "non-exhaustive patterns exception"
    for finite list of this method?

Take note that the same sieve method could also be written more
succinctly using list comprehension, as follows:
  sieve (x:xs) = x:(sieve [y | y <- xs, (mod y x) /= 0])
List comprehension are syntactic sugar for map, filter and
concatMap methods (see coming Lecture). They are analogous
to ZF set comprehension notation which you may be familiar
     https://en.wikipedia.org/wiki/Set-builder_notation

-}

sieve :: Integral a => [a] -> [a]
sieve (x:xs) = x:(sieve (filter (\y -> (mod y x) /= 0) xs))


{-
Q2 : Can you think of a way to generate
     an infinite list of prime numbers using
     the sieve method? Use this method to return the
     first 100 prime numebrs.

     Consider two methods sel_drop and sel_take.
     Write the types for these two functions.
     How are they related to take/drop?

     How many prime numbers are there between 10000 to 20000?
     Write a function to compute this with the help of sel_drop/sel_take?

-}

primes :: Integral a => [a]
primes = error "TBI: please use the sieve method over infinite numbers"

sel_drop f (ys @ (x:xs))  =
    if (f x) then ys
    else sel_drop f xs

sel_take f (x:xs)  =
    if (f x) then []
    else x:(sel_take f xs)


q2 = error "TBI: number of primes between 10000 to 20000 inclusive"



{-
Q3 : Write a boolean function that
     would check if a number is prime or otherwise.

     Provide three ways to write this isPrime function.

     (i) Implement this using foldl.

     (ii) Implement this using recursion.

     (ii) Implement this using infinite list of primes

-}

isPrime1 :: Int -> Bool
isPrime1 n = error "TBI: is n prime? (use foldl)"

isPrime2 n = error "TBI: is n prime? (use recursion"

isPrime3 n = error "TBI: is n prime? (use primes)"


{-
Q4 : Consider the following function
     to return an infinite list of factorials.

     Explain if there are redundant calls present in
     this computation of infinite list?

     Improve this in infFacts2 by removing redundant calls
     present in the computation through the use of fact_eff.
     Explain why this implementation may not be quite optimal

     Improve this further in inf_facts_3 by using zip.
-}

fact :: (Eq p, Num p) => p -> p
fact 0 = 1
fact n = n * (fact (n-1))

infFacts :: [Integer]
infFacts  = [fact a | a <- [0..]]

infFacts2 :: [Integer]
infFacts2 = error "TBI : efficient inf list of factorials"
fact_eff :: Int -> Integer
fact_eff n = infFacts2 !! n

infFacts3 :: [Integer]
infFacts3 = error "TBI : efficient inf list of factorials using zip"
-- see how infinite list of fibonacci was efficiently computed

{-

Q5:
(i) Use list comprehension to build a n x m
    multiplication table with type Int -> Int -> [(Int,Int,Int)]

*Main> mult_table 5 5
[(1,1,1),(1,2,2),(1,3,3),(1,4,4),(1,5,5),(2,1,2),(2,2,4),(2,3,6),(2,4,8),(2,5,10),(3,1,3),(3,2,6),(3,3,9),(3,4,12),(3,5,15),(4,1,4),(4,2,8),(4,3,12),(4,4,16),(4,5,20),(5,1,5),(5,2,10),(5,3,15),(5,4,20),(5,5,25)]

(ii) Re-implement mult_table2 using map, filter and concatMap

(iii) Re-implement mult_table3 to return the multiplication
      table as an Array (Int,Int) Int instead
-}

mult_table :: (Num c, Enum c) => c -> c -> [(c, c, c)]
mult_table n m = [(a,b,a*b)| a <- [1..n],b <-[1..m]]

mult_table2 :: (Num c, Enum c) => c -> c -> [(c, c, c)]
mult_table2 n m = error "implement mult_table using map,filter and concatMap"

mult_table3 :: Int -> Int -> Array (Int,Int) Int
mult_table3 n m = error "implement mult_table using Array (Int,Int) Int"

{-
Q6 : Consider a tree data structure and
     its automatic derivation for Eq and Show classes.

What method in EQ class is available?

The default show method for t1 would return:
"Node 3 (Leaf 1) (Node 4 (Leaf 0) (Leaf 9))"

Change the implementation of Show to provide
the following indented printing instead:

Node 3
  1
  Node 4
    0
    9

You may make use of the code in comment.
-}


data Tree a = Empty | Leaf a | Node a (Tree a) (Tree a)
          deriving (Eq,Show)

t1 = Node 3 (Leaf 1) (Node 4 (Leaf 0) (Leaf 9))

{-
instance Show a => Show (Tree a) where
 showsPrec _ t s =
   let rep n = concat (replicate n " ") 
       aux t n s = case t of
         Empty -> (rep n)++"#"++"\n"++s
         Leaf v -> (rep n)++(show v)++"\n"++s
         Node v lt rt ->
           (rep n)++("Node "++(show v)++"\n")
           ++(aux lt (n+2) (aux rt (n+2) s))
   in aux t 0 s
-}

{-

Q7. Arrays may be built monolithically using the following method:
      array :: Ix i => (i, i) -> [(i, e)] -> Array i e
(i) Write a method which would return an array of factorials
(ii) Given an array e of bounds (l,h) of values (a_1,a_2,a_3..,a_h)
     (prefixSum e) will compute a new array based on prefix sum with values
	    (a_1,a_1+a_2,a_1+a_2+a_3 ..., a_1+a_2+a+3+..+a_h)
	 Try to compute it in the most efficient manner.
	 Implement your method to works for arrays of arbitrary bounds

-}

factArr :: Int -> Array Int Int
factArr n = error "TBI : an array of factorial"

prefixSum :: Array Int Int -> Array Int Int
prefixSum e = error "TBI : prefix sum of array e"


{-

Q8: Harder.

    Say you wish to implement an infinite list of multiplication table.
    Explain why [(Int,Int,Int)] is not the right data structure to use.
    Neither is Array (Int,Int) Int.

    How would you then implement a working
    infinite list of multiplication table?
-}

inf_mult_table n m = error "implement a workable infinite mult_table"

main = putStrLn "Tut 4"

