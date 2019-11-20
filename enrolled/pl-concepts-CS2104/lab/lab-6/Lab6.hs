{-
    Lab6 - Grading your mid-term with Haskell 

    This is an optional lab assignment that will not
    be graded!

    Lab6 is an attempt at automatic lab grading with Haskell.
    A skeleton code is provided for below.
    cs2104-2019.csv contains your captured answers.
    answer-2019.txt contains a description of
    the expected answers. Some data needs to
    be cleaned up, and we will check them manually later.

    Each correct answer will carry 1 mark, while incorrect answers
    will not be penalized. Some answers are partially correct, and
    may be awarded 0.5 marks. This can be determined from
    answer-2019.txt. Please see tip-2014.txt for how
    you may write your grading code.

    You are to revise the Haskell program to obtain
    the following outcomes.

    Output Format:
    ===================
    Score (Sorted by ID):
    <ID>, <Score>
    ...
    <ID>, <Score>

    Score (Sorted by Score):
    <ID>, <Score>
    ...
    <ID>, <Score>

    Min:XX,Max:XX,Median:XXXX,Average:XX.XXX

    Students who may answer Q41 correctly (based on correct answer for Q39)
    <Id>
    ..
    <Id>

    Example:
    ===================
    Given Answers:
    A000001U, A,A,A,A,B,A,A,A,A,B,A,A,A,A,B,A,A,A,A,B,A,A,A,A,B,A,A,A,A,B,A,A,A,A,B
    A000000U, A,A,A,A,B,A,A,A,A,B,A,A,A,A,B,A,A,A,A,B,A,A,A,A,B,A,A,A,A,B,A,A,A,A,B

    Score (Sorted by ID):
    A000000U, 10
    A000001U, 12

    Score (Sorted by Score):
    A000001U, 12
    A000000U, 10

    Min:10,Max:12,Median:11.0,Average:11.000

    Students who may answer Q41 correctly (based on correct answer for Q39)
    A000008U, 13


-}

import Data.List
import Text.Parsec
import Text.ParserCombinators.Parsec.Combinator

type GivenAnswers = [(String, Int, String, [Char])]

type SParsec = Parsec String ()

answer = "ECBEDBBECDECCECBBECBCEECDCCAEDACBAEBBEED"

main = do
    dd <- read_file "cs2104-2019.csv"
    -- putStrLn "parseFile "    
    let data' = extract_data dd
    grade_quiz2 data'
    return ()


read_file :: FilePath -> IO [String]
read_file x =
  do
    content <- readFile x
    let llcontent = lines content
        lz = length llcontent
     in do
          return llcontent

whitespaces :: SParsec [Char]
whitespaces = many (tab <|> space)

comma = whitespaces >> char ',' >> whitespaces

parseSqBr :: SParsec a -> SParsec a
parseSqBr x = between (char '[') (char ']') x

parseParen :: SParsec a -> SParsec a
parseParen x = between (char '(') (char ')') x

parseCrBr :: SParsec a -> SParsec a
parseCrBr x = between (char '{') (char '}') x

commaSep :: SParsec a -> SParsec [a]
commaSep p  = p `sepBy` comma

parseStr :: SParsec String
parseStr = many parsechr

parsechr :: SParsec Char 
parsechr = noneOf ",\n"

parseCSV :: SParsec a -> SParsec [a]
parseCSV p = p `sepBy` comma

parseLn :: SParsec [String]
parseLn = parseCSV parseStr

extract_data :: [String] -> GivenAnswers
extract_data all_lines = map parseLine all_lines

parseLine :: String -> (String, Int, String, [Char])
parseLine a_line = 
  let ans = parse parseLn "" a_line in
  case ans of {
        Left e -> ("",0,"","ERROR");
        Right a -> ("",0,"",intercalate "-" a)
        } 

grade_quiz2 :: GivenAnswers -> IO ()
grade_quiz2 ll = foldr (\ (_,_,_,ln) r -> putStrLn ln >> r) (return ()) ll
