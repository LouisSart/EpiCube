# EpiCube

A Rubik's Cube c++ library for manipulating cubes on the cubie level


## Multistep solver

 Usual steps for the Rubik's Cube involve recursively bringing the cube state into some smaller subgroups until the goal state is reached. Thistlethwaite and Kociemba's algorithms are probably the most powerful of this kind but you could come up with any kind of step-by-step method as long as solving step k is relatively easy once step k - 1 is solved. Fewest Moves solvers sometimes use blockbuilding to find short solutions, by solving a 2x2x2 block, then a 2x2x3 and then the first two layers of the cube for example. These algorithms are useful when you want to find solutions that are "humanly findable" (e.g. that can be broken down into human steps that you can learn from), or when the puzzle space size is too big to build an optimal solver that has a chance of returning in a reasonable amount of time. Multistep solvers serve as a tradeoff between optimality and execution speed. As an FMCer I am interested in building various kinds of multiphase solvers to watch well known (or obscure) methods be applied by a computer, and be amazed by the results.

## Joba's Cubelib solver

 One other possible approach for building a multistep solver is used in the [cubelib](https://github.com/Jobarion/cubelib) program by Joba : first generate all step 1 solutions that fit into a given move budget. For each of these solutions, recursively generate all step 2, 3, ... n that still fit into the move budget, until solutions are reached. If no solutions are found, increase the move budget and start over. The advantage of such strategy is that it will theoretically always hit the optimal solution for the given scrambled state, but the problem is that as the move budget increases, exponentially more work is done at low level steps. In practice this will lead to a memory overflow before solutions are even reached. What Joba does to prevent this is to use a search cap to each step, which limits the number of intermediate solutions. If too many solutions are found in spite of the search cap, he forces the breadth of search at each step with a custom parameter. His program is very useful for FMC solvers because they can tune the different search caps themselves to fit their own standards as competitors and thus watch the code generate solutions that they should be able to find. The web version of cubelib can be found at [mallard](https://joba.me/mallard/).


# Jaap's multiphase strategy

As much as cubelib/Mallard can find very nice solutions that can be exploited for competitive solving, it is unfortunatly limited by the need for various parameters, and hence wont find very short solutions in a fair amount of time unless you are experienced with its method and know what parameters to use. What if instead we just solved every step optimally ? Well, this will work reasonably well if your method is good, but on average solving every step optimally usually doesn't guarantee that you will find an overall short solution. 

The multistep solver in EpiCube is an implementation of Jaap Sherphuis [ideas](https://www.jaapsch.net/puzzles/compcube.htm). It starts with a family of n functions that can each solve a step, either optimally or with some "slackness". Adding s moves of slackness to one of the step solvers means allowing this step to be s more moves than optimal. Then the idea behind Jaap's solver is to introduce slackness moves one by one, here is how it goes :

First solve every step optimally. This will generate our first solution references, which we will try to beat afterwards.
Then allow the (n - 1) first step solvers one move of slackness (increasing the last step length will not yield better solutions so we exclude the nth step). If shorter solutions have been found, update the search cap so we don't expand too many positions that will not lead to better solutions.
When these options have been searched, add another move of slackness to one of the (n - 1) first steps. That will allow either one of the steps to use two more moves than optimal, or two different steps to use one more move than optimal.
More and more slackness moves can be added this way until the solutions found are good enough. Theoretically, when the slackness plus the optimal length of the first step equal the length of the solutions found, the algorithm has found an optimal solution.

What I like about this solver is that it doesn't require any knowledge about the method you are using, or computational safeguards that you need to add. It will simply generate the best multiphase solutions given only the slackness value that you are willing to give it .