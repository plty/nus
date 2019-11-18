-- Tutorial 1 : Introduction to Haskell
-- Use GHCi (ghci in command line);  You can load the entire file
-- into interpreter by using :load Tut1ans.hs
-- The purpose of this tutorial is to be familiar with Haskell


main = putStrLn "Hello to Lab1/Tutorial1"
--------------------------------------------
-- Q1 (Scope). Consider the expression below

{-
let x = 2

let y = let x=3 in x+1

let z = x+5
-}

-- What are the values of x,y and z after the definition of z?
{- Ans: x = 2, y = 4 (based on local x=3 binding), z = 7 (based on outer x=2 binding)
-}
-- How many let definitions are there, and what are their scopes?
-- Ans: 4, 3 global (x, y, z), 1 local to y (x = 3)

{-
let x = x * 4 in x + y
-}

-- Which x is being referenced in the last line?
-- Ans: x + y -> local let binding of x
--      x * 4 -> recursive x = x*4 binding 

-- What is the behavior of the last execution? Why?
{- Ans: Infinite loop, x = x * 4 is a recursive definition that's infinitely
        unfolded in x + y
-}


---------------------------------------------
-- Q2 (Type). Consider the functions below

goo1 x y = x
goo2 x = \ y -> x
goo3 (x,y) = x

-- What are the types of these three functions?
{- Ans :
   goo1 : a -> b -> a
   goo2 : a -> b -> a
   goo3 : a * b -> a
-}
-- You can check the type of expression in ghci by :type goo1 or :t goo1.
-- Are these types general?
-- Ans: Yes
-- Give some examples of calls to these functions?  goo1 1 "h" --> 1 ; goo3 ("h",1) --> "h"
-- Describe in words what are the intensions of these functions? Ans: getting the first element of a pair of arguments
-- How are these functions related to each other? Ans: The first 2 are the same, the 3rd is slightly different but isormorphic

---------------------------------------------
-- Q3 (Type). Consider the function below

foo1 x = x + 1
foo2 x = x ++ " there"

-- What are the types of these two function?
{- Ans:
   foo1 : Num a => a -> a
   foo2 : [Char] -> [Char] / List Char -> List Char
-}
-- Give some examples of valid arguments that can
--  be used by foo1 and foo2 methods?
--  e.g foo1 2.5, foo1 4, foo1 "hello"
-- What is [Char]? Is this the same as String? Ans: List of Char is the same as String in Haskell
-- You may get same information about type t using :info t

---------------------------------------------
-- Q4 (Type). Consider this set of mutual recursive functions
-- Take note that multi-line commands are entered in ghci with
-- the help of commands :{ and  :}
{-
let even :: Int -> Bool
    even n = if n==0 then True else odd (n - 1)
    odd :: Int -> Bool
    odd n = if n==0 then False else even (n - 1)
-}

-- Illustrate some valid calls to these functions, such as odd 3 --> True, even 3 --> False
-- Explain what happens when you have negative inputs to these functions? Ans: Infinite loop or stack overflow
-- What are the types of these functions? Ans: most general type even :  
--   Ans : As declared, namely Int -> Bool
-- Explain what happens when type declarations of form e::t
--   are not provided. Ans: the most general types of even and odd will be inferred
--   namely (Eq a, Num a) => a -> Bool,

------------------------------------------
-- Q5. Consider the maxlist function below for a non-empty list of numbers.
-- take note that

maxlist [] = error "empty list";
maxlist (x:[]) = x;
maxlist (x:ys) = max x (maxlist ys)

-- maxlist [3,16,1]

-- (i) What is the type of this function? Ans: maxlist : [a] -> a / List a -> a
-- (ii) Explain what happen when an empty list is supplied as the input? Ans: A runtime error (exception)
-- (iii) Is it possible for recursive maxlist ys call to throw an exception? 
--       Ans: The recursive call in the third definition cannot throw an exception
--            since its input ys is definitely non-empty.

------------------------------------------
-- Q6. Rewrite maxlist function to use an auxiliary recursive
--     function, as follows:

maxlist2 xs =
  let aux xs sofar =
        case xs of
          [] -> sofar
          y:ys -> if y > sofar then aux ys y else aux ys sofar
  in
   case xs of
     [] -> error "empty list"
     (x:ys) -> aux ys x

-- maxlist2 [3,16,1]

-- How is this function different from the version in Q7
-- Is it any better? Ans: it is tail recursive & it has more efficient pattern-matching
--                        with two cases ratherm than 3 cases in maxlist

-------------------------------------------
-- Q7 : The maxlist function currently has type: [Int] -> Int.
--      Rewrite it to a version that is based on the following Maybe type::
--      data Maybe a = Nothing | Just a

maxlist3 xs =
  let aux xs sofar =
        case xs of
          [] -> sofar
          y:ys -> if y > sofar then aux ys y else aux ys sofar
  in
   case xs of
     [] -> Nothing 
     (x:ys) -> Just (aux ys x)

-- above definition is similar to maxlist2, except for returning Maybe result type

maxlist3a xs =
  case xs of
    [] -> Nothing
    (x:[]) -> Just x
    (x:ys) ->
      case (maxlist3a ys) of
        Just y -> Just (max x y)
        Nothing -> Just x    -- this branch cannot occur since ys is non-empty

-- above definition is similar to maxlist, except for returning Maybe result type

-- The function 'show' converts a number into a string or [Char]
{-
print ("Q7a Ans (maxlist3 [3,16,1] = " ++ (show (maxlist3 [3,16,1])))
print ("Q7a Ans (maxlist3 [3,16,1] = " ++ (show (maxlist3 [3, 16, 1])))
-}

maxlist4 xs =
     let aux xs sofar =
           case xs of
             [] -> sofar
             y : ys -> if y > sofar then aux ys y 
                       else aux ys sofar
     in
      case xs of
        [] -> Nothing
        (x:ys) -> Just (aux ys x)

-- print ("Q7b Ans (maxlist4 [3,16,1] = " ++ (show (maxlist4 [3,16,1])))
-- print ("Q7b Ans (maxlist4 [3,16,1] = " ++ (show(maxlist4 [])))

-------------------------------------------
-- Q8 : Using function for abstraction.
-- Consider a simple code to compute the sum of
-- the areas for three circles.
--
-- How would could you generalize the code
-- to make it more abstract
-- so that it could compute the sum for an
-- arbitrary number of shapes instead.
-- Hint : Use a list & use functions!

{-
let pi = 3.14159
let r1 = 1.2
let r2 = 3.888
let r3 = 10.0
let three_circle = pi*r1*r1 + pi*r2*r2 * pi*r3*r3
-}

-- If a list of arbitary number of circles, given their radii

circlesR = [1.2, 3.888, 10.0]
totalAreas' circles =
  case circles of
    [] -> 0
    x : xs -> pi * x * x + totalAreas' xs

-- If a list of arbitary number of all the same shape

totalAreasAny :: Num n => (s-> n) -> [s] -> n
totalAreasAny findArea shapes =
  case shapes of
    [] -> 0
    x : xs -> (findArea x) + (totalAreasAny findArea xs)


-------------------------------------------
-- Q9 : Implement the Babylonian method for square root in
-- Haskell without using loops! Please use a recursive method
-- instead. What would be its expected type? 
-- Ans: Float -> Float / (Fractional t, Ord t) => t -> t

squareRoot n = 
  let
     aux x y e =
      if (x - y) > e then
        let
          x' = ((x + y) / 2)
          y' = (n / x')
        in
          aux x' y' e
      else
        x
     in aux n 1 0.000001

{-
Babylonian method for square root
Algorithm:
This method can be derived from (but predates) Newton–Raphson method.

1 Start with an arbitrary positive start value x (the closer to the
   root, the better).
2 Initialize y = 1.
3. Do following until desired approximation is achieved.
  a) Get the next approximation for root using average of x and y
  b) Set y = n/x
Implementation:

/*Returns the square root of n. Note that the function */
float squareRoot(float n)
{
  /*We are using n itself as initial approximation
   This can definitely be improved */
  float x = n;
  float y = 1;
  float e = 0.000001; /* e decides the accuracy level*/
  while(x - y > e)
  {
    x = (x + y)/2;
    y = n/x;
  }
  return x;
}

/* Driver program to test above function*/
int main()
{
  int n = 50;
  printf ("Square root of %d is %f", n, squareRoot(n));
  getchar();
}
Run on IDE
Example:

n = 4 /*n itself is used for initial approximation*/
Initialize x = 4, y = 1
Next Approximation x = (x + y)/2 (= 2.500000),
y = n/x  (=1.600000)
Next Approximation x = 2.050000,
y = 1.951220
Next Approximation x = 2.000610,
y = 1.999390
Next Approximation x = 2.000000,
y = 2.000000
Terminate as (x - y) > e now.
-}

