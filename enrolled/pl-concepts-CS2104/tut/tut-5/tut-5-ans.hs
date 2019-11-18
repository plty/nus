{-
    Tutorial 5/6 : Monads and IO

    We will discuss properties of the class Monad m and some useful monads.
    We will use the monads [], Maybe, IO, and (Either e) in this discussion.


-}

{-
    'Monad m' is a class with at least the following members.
    (>>=)  :: m a -> (a -> m b)    -> m b
    return ::   a                  -> m a

    In addition to implementing these class functions, 
    all instance of Monad are expected to obey the following
    properties or 'Monad Law'

    return a >>= k                 = k a               (Left Identity)
    m        >>= return            = m
    m        >>= (\x -> k x >>= h) = (m >>= k) >>= h
-}

{-
    TRY creating some monadic values in ghci.
-}

import System.IO.Unsafe

x1 = return 42 :: Maybe Int
x2 = return 42 :: [Int]
x3 = return 42 :: IO Int

{-
x1
x2
x3
:t x3
-}

{-
    Q2. Let us compose these monads which contain a number 42
        with an increment operation.
-}

increment x = return (succ x)

y1 = x1 >>= increment
y2 = x2 >>= increment
y3 = x3 >>= increment

{-
    We may also compose other monads with increment, 
    as shown below. Guess their effects, especially y7

-}

y4 = (Nothing >>= increment) :: Maybe Int
y5 = ([] >>= increment) :: [Int]
y6 = ([1,2,3] >>= increment) :: [Int]
y7 = (getChar >>= increment) :: IO Char

{-

Type y4,y5,y6,y7 in ghci
Explain the effect of each of the monadic operation.

-}

{-
    Q3. Let us write a monad to perform an operation twice.
    What is its type?

-}
perform_twice m = 
  do
    x1 <- m
    x2 <- m
    return (x1,x2)

{-
Execute this method with

> perform_twice x1  -- x1=(Just 42)
    x1 is performed twice successfully returning
    42 each time
> perform_twice x2  -- x2=[42]
    x1 is performed twice successfully non-deterministically
    returning [42] each time; at the end [(42,42)] is returned
> perform_twice y7  -- y7::IO Char
    y1 is reading a char and returning the next char;
   this will be performed twice
> perform_twice [1,9]

Explain the effect of each of these executions.
What is the type of the last operation?

-}


{-
   Q5

    Re-implement perform_twice using >>=
    instead of the do notation.

-}

perform_twice' :: Monad m => m t -> m (t, t)
-- perform_twice' m = fail "to be implemented using >>="
perform_twice' m = m >>= \ t1 ->
                   m >>= \ t2 ->
                   return (t1,t2)

{-
   Q6

    Implement a more general method performN
    which will perform a given monadic computation
    a fixed number of times.

    Test this method with (performN 5 getChar)

-}

performN :: Monad m => Int -> m t -> m [t]
performN 0 m = return []
performN n m = 
  do x1 <- m
     x2 <- performN (n-1) m
     return (x1:x2) --(x1:(performN (n-1) x2))

{-
   Q7

   Consider another method which is able to 
   repeatedly perform the same monadic computation
   as long as some filter condition is true .

  (i) use perform_filter to test the method getDigits
  (ii) Can you rewrite perform_filter method to use an aux
      method instead?


-}

perform_filter :: Monad m => (t->Bool) -> m t -> m [t]
perform_filter f m = 
  do x1<-m
     if not(f x1) then return []
     else do x2<-perform_filter f m
             return (x1:x2)

perform_filter' f m =
  let aux () = do 
        x1 <- m
        if not(f x1) then return [] else do 
            x2 <- aux ()
            return (x1:x2)
  in aux ()
-- the main advantage of using aux is that the unchanged
-- parameters need not be passed around in the auxiliary recursive
-- function

getDigits :: IO String
getDigits = 
    do
      xs <- perform_filter (\n -> elem n ['0'..'9']) getChar
      putStrLn "\nThe sequence of digits you just typed is:"
      print xs
      return xs

{-
   Q8

   Using perform_filter, write three IO operations
   which would read and print
     (i) binary number
     (ii) octal number
     (iii) hexadecimal number
   from the terminal. 

   For hexadecimal, take note that "A" denotes 11
   and "E" denotes 15. The conv_base method
   would have to be suitably modified to
   compute the correct values for A..Z.

-}


-- this method takes a base and converts a list of digits
-- into its corresponding number of the given base
--   base=2 gives binary number
--   base=8 gives octal number
-- conv_base :: Enum a => Int -> [a] -> Int
conv_base :: Int -> [Char] -> Int
conv_base base lst =
  let aux xs n = case xs of 
        [] -> 0
        y:ys -> let r = 
                      if elem y ['A'..'F'] then 10+(fromEnum y)-(fromEnum 'A') 
                      else (fromEnum y)-(fromEnum '0') 
                in r*n + (aux ys (n*base))
  in aux (reverse lst) 1

-- Try use >>= and return only
-- To read a binary number, and return its decimal equivalent
getBin :: IO Int
getBin = 
  perform_filter (\n -> elem n ['0'..'1']) getChar >>=
    \ xs ->
    let ans = conv_base 2 xs in return ans
  --error "to be completed using perform_filter and conv_base"


-- Try use do notation in your implementation
-- To read an octal number, and return its decimal equivalent
getOct :: IO Int
getOct = 
  do
    xs <- perform_filter (\n -> elem n ['0'..'7']) getChar
    let ans = conv_base 8 xs in return ans
  -- error "to be completed using perform_filter and conv_base"


-- Choose either do notation or >>= 
-- To read a hexadecimal number, and return its decimal equivalent
getHex :: IO Int
getHex = 
  do
    xs <- perform_filter (\n -> elem n (['0'..'9']++['A'..'F'])) getChar
    let ans = conv_base 16 xs in return ans
  --error "to be completed using perform_filter and conv_base"
  -- we need to revise comv_base to handle A..F
    

{-

  Q9: DISCUSSION

 Monadic IO as a pure value

 Consider foo1 and foo2.
  (i) Is "io" a pure value?
  (ii) Are foo1 and foo2 equivalent?
      + Is this due to the fact that "io" is a pure value.
  (iii) Pure values has the "referential transparency" property
  which states that you can always replace a (pure) expression
  by its equivalent expression. This "referential transparency"
  property does not hold for impure codes.
 Alternatively, "referential transparency" aslo
 means an expression always evaluates to the same result in any context.

-}

foo1 =
      let io = print "hello" 
      in io >> io

foo2 =
      (print "hello") >> (print "hello")

-- yes, they are "exactly equivalent" and have
-- the same missing. the same cannot be said for
-- impure IO values whose duplication  results in 
-- multiple effects

{-

  Q10: Let us compare to the corresponding pseudo 
  C program of the followng:
  foo3 () {
    io = println "hello"
    return (io,io)
  }
  foo4 () {
    return (println "hello",println "hello")
  }
  Are these two methods equivalent?
  

The corresponding impure IO operation in Haskell
  is implemented as:
      unsafePerformIO (print "hello")
  as illustrated below.
  However, you are discouraged from
  using these since it introduces side-effects and impurity.
  In case you wish to use it, please do so with care!

-}

foo3 =
      let io = unsafePerformIO (print "hello")
      in io `seq` io `seq` ()

foo4 =
      (unsafePerformIO (print "hello")) `seq` 
      (unsafePerformIO (print "hello")) `seq`
      ()
-- foo3 perform printing only once
-- while foo4 performs it wtowce

{-

  Q11: What are the corresponding Haskell counterpart
  in pure functions for foo3 and foo4? 

-}

foo3pure :: IO ()
foo3pure = error "to be completed as a pure function"

foo4pure :: IO ()
foo4pure = error "to be completed as a pure function"


-- see Q10, and the values are identical..