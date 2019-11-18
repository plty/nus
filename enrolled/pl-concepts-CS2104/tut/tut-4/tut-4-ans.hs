import System.IO.Unsafe
import Data.Array

{-
    Tutorial 4 : Laziness, List Comprehension, Arrays


   Try complete this tutorial and submit in LumiNUS for participation
   mark by 23Sep2019. It will NOT be graded.
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

-}

sieve (x:xs) = x:(sieve [y | y <-xs, (mod y x) /= 0])


{-
Q2 : Can you think of a way to generate
     an infinite list of prime numbers using
     the sieve method?

     Consider two methods sel_drop and sel_take.
     Write the types for these two functions.
     How are they related to take/drop?

     How many prime numbers are there between 10000 to 20000?
     Write a function to compute this with the help of sel_drop/sel_take?

-}

-- primes = error "TBI: please use the sieve method over infinite numbers"
primes = sieve [2..]

-- q2 = error "TBI: number of primes between 10000 to 20000 inclusive"
sel_drop f (ys @ (x:xs))  =
    if (f x) then ys
    else sel_drop f xs

sel_take f (x:xs)  =
    if (f x) then []
    else x:(sel_take f xs)


-- q2 = error "TBI: number of primes between 10000 to 20000"
q2 = let xs = sel_drop (\x-> x>=10000) primes in
     let ys = sel_take (\x-> x>20000) xs in
     length ys



{-
Q3 : Write a boolean function that
     would check if a number is prime or otherwise.

     Provide three ways to write this isPrime function.

     (i) Implement this using foldl.

     (ii) Implement this using recursion.

     (ii) Implement this using infinite list of primes

-}

-- isPrime1 n = error "TBI: is n prime? (use foldl)"
for_all f xs = foldl (\ b x -> (f x) && b) True xs
    
isPrime1 n =
  for_all (\x -> mod n x /= 0) [2..(n-1)] 


-- isPrime2 n = error "TBI: is n prime? (use recursion"

-- isPrime3 n = error "TBI: is n prime? (use primes)"
isPrime3 n =
  let x:_ = sel_drop (\a -> a>=n) primes in
  x==n


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

fact 0 = 1
fact n = n * (fact (n-1))

infFacts  = [fact a | a <- [0..]]

--infFacts2 = error "TBI : efficient inf list of factorials using factEff"
fact_eff n = infFacts !! n
infFacts2 = 1:[a * fact_eff (a-1) | a <-[1..]]

-- infFacts3 = error "TBI : efficient inf list of factorials using zip"
-- we are zipping two list of numbers to mulltiple, we need a base case 1:
-- otherwise we will go into a loop
infFacts3 = 1:map (\(x,y)-> x*y) (zip [1..] infFacts3)

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

mult_table n m = [(a,b,a*b)| a <- [1..n],b <-[1..m]]


--mult_table2 n m = error "implement mult_table using map,filter and concatMap"
mult_table2 n m = (concatMap (\i -> map (\j -> (i,j,i*j)) [1..m]) [1..n])

-- mult_table3 n m = error "implement mult_table using Array (Int,Int) Int"
-- take note to specify lower corner (1,1) and upper corner (n,m)
mult_table3 n m = array ((1,1),(n,m)) [((i,j),i*j)| i<-[1..n],j<-[1..m]]

{-
Q6 : Consider a tree data structure and
     its automatic derivation for Eq and Show classes.

What method in EQ class is available?
Ans: (==) and (/=)
class Eq a where
  (==) :: a -> a -> Bool
  (/=) :: a -> a -> Bool
  {-# MINIMAL (==) | (/=) #-}
  
The default show method for t1 would return:
"Node 3 (Leaf 1) (Node 4 (Leaf 0) (Leaf 9))"

Change the implementation of Show to provide
the following indented printing instead:

Node 3
  1
  Node 4
    0
    9

You may test using (print t1)
-}

data Tree a = Leaf a | Node a (Tree a) (Tree a)
          deriving Eq
          --deriving (Eq,Show)

t1 = Node 3 (Leaf 1) (Node 4 (Leaf 0) (Leaf 9))

instance Show a => Show (Tree a) where
 showsPrec _ t s =
   let rep n = concat (replicate n " ") 
       aux t n s = case t of
         Leaf v -> (rep n)++(show v)++"\n"++s
         Node v lt rt ->
           (rep n)++("Node "++(show v)++"\n")
           ++(aux lt (n+2) (aux rt (n+2) s))
   in aux t 0 s




{-

Q7: Harder.

    Say you wish to implement an infinite list of multiplication table.
    Explain why [(Int,Int,Int)] is not the right data structure to use.
    Neither is Array (Int,Int) Int.

    How would you then implement a working
    infinite list of multiplication table?
-}

-- inf_mult_table n m = error "implement a workable infinite mult_table"
-- I guess one could use list of list [[(Int,Int,Int]]
-- which would allow us to access infinite list of infinite sub-lists.

inf_mult_table = [[(i,j,i*j)|j<-[1..]]|i<-[1..]]
inf_mult n m = (inf_mult_table !! (n-1)) !! (m-1)

