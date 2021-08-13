HOMEWORK 6: CROSSWORD BLACKOUT

This HW was rough so Imma take the license to be as humorous as I want on this README

NAME:  Abby Itty



COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

Just about everyone helped with this one: Cplusplus reference, geeks for geeks, Discord (#ds-questions, #data-stuffs on the CSCI server), 
Nicolette Glut (a friend who took the class last semester and walked me through logic), Navya Ajay (fellow classmate and hype queen),
My Uncle Roy (who is a CS person and helped me debug my initial functions), my dad (walked through logic-he's my proverbial rubber duck),
my mom/sister (mostly I just vented to them haha), God (is vent-praying a thing? if so, I 100% did this God is definitely tired of me
ranting about data structures)

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < like a bajillion (actually about 30?) >



ANALYSIS OF PERFORMANCE OF YOUR ALGORITHM:
(running time order notation & concise paragraph, < 200 words)
The dimensions of the board (w and h) 
The number of words in the dictionary (d)? 
The total number of spaces with a letter (l)?
The total number of blacked out spaces (b)? 
The number of constraints (c)? 
Etc. 

A lot of the running times in findAllMatches depend on the number of words found in each solution and the number of characters in
each of the words in the solution. Other functions like emptyBoard only depend on the size of the board (both w and h). I think most
of the running time comes from findOneSolution, which is like O(d*w*h) at least. It goes through every character, and every word in the
dictionary for every character, so I think it's the most costly function.

DESCRIPTION OF YOUR ADDTIONAL PUZZLES/DICTIONARIES:
Briefly explain the design of the puzzles or dictionaries and what
makes them "interesting" test cases (or why they aren't interesting).


SUMMARY OF PERFORMANCE OF YOUR PROGRAM ON THE PROVIDED PUZZLES AND 
ON YOUR ADDITIONAL PUZZLES/DICTIONARIES:
# of solutions & approximate wall clock running time for different
puzzles / dictionaries for a single solution or all solutions. Be
specific about which puzzle/dictionary/parameters you were timing.



MISC. COMMENTS TO GRADER:  
please examine your consicence before assigning this hw again next year XD XD XD
haha kidding
mostly


