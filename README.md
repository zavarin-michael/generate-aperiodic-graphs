# Generate / Analyze synchronized  graphs and automatas

![6 vertexes case](/assets/volkov6.svg)

## Motivation

Researching task of finding minimal repaintings to find synchronized automata requires computer computing

There are projects that iterate through graphs with up to 10 vertices

In this repo priority was given to ease of expansion and viewing rather than performance - I could reach only 6 vertexes

Would be glad if someone helps me with performance)

### Representation of recolors in the form of a Hasse diagram for [4 vertexes graph](https://dreampuf.github.io/GraphvizOnline/?engine=dot#digraph%20G%20%7B%0D%0A0%20%5Bfillcolor%3D%22%22%2C%20style%3Dfilled%5D%3B%0D%0A1%20%5Bfillcolor%3D%22%22%2C%20style%3Dfilled%5D%3B%0D%0A2%20%5Bfillcolor%3D%22%22%2C%20style%3Dfilled%5D%3B%0D%0A3%20%5Bfillcolor%3D%22%22%2C%20style%3Dfilled%5D%3B%0D%0A0-%3E1%20%3B%0D%0A0-%3E0%20%3B%0D%0A1-%3E2%20%3B%0D%0A1-%3E2%20%3B%0D%0A2-%3E3%20%3B%0D%0A2-%3E0%20%3B%0D%0A3-%3E2%20%3B%0D%0A3-%3E2%20%3B%0D%0A%7D%0D%0A)

![6 vertexes case](/assets/hasse4.svg)

## Сontent

There are some utils for convenient way to analyze graphs with small quantity of vertexes and edges

There are two main steps: 
1. **generation** scripts
   1. generate-graphs
   2. generate-automatas
   3. generate-biset-graphs
2. **analyzing** scripts
    1. analyze-automatas

You can store intermediate results in [grapviz](https://graphviz.org/) format and use it further

See your results by copying graphs to [graphiz online](https://dreampuf.github.io/GraphvizOnline/?engine=dot#digraph%20P%20%7B%0D%0A%20%20%20%20subgraph%20G1%20%7B%0D%0A%20%20%20%200%3B%0D%0A%20%20%20%201%3B%0D%0A%20%20%20%202%3B%0D%0A%20%20%20%203%3B%0D%0A%20%20%20%200-%3E3%20%20%5Blabel%3Db%5D%3B%0D%0A%20%20%20%200-%3E3%20%20%5Blabel%3Da%5D%3B%0D%0A%20%20%20%201-%3E3%20%20%5Blabel%3Da%5D%3B%0D%0A%20%20%20%201-%3E2%20%20%5Blabel%3Db%5D%3B%0D%0A%20%20%20%202-%3E1%20%20%5Blabel%3Da%5D%3B%0D%0A%20%20%20%202-%3E0%20%20%5Blabel%3Db%5D%3B%0D%0A%20%20%20%203-%3E1%20%20%5Blabel%3Da%5D%3B%0D%0A%20%20%20%203-%3E0%20%20%5Blabel%3Db%5D%3B%0D%0A%20%20%20%20%7D%0D%0A%7D%0D%0Agraph%20G%20%7B%0D%0A%20%20%20%200%20%5Bfillcolor%3Dgreen%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%201%20%5Bfillcolor%3D%22%22%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%202%20%5Bfillcolor%3Dgreen%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%203%20%5Bfillcolor%3D%22%22%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%204%20%5Bfillcolor%3D%22%22%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%205%20%5Bfillcolor%3Dgreen%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%206%20%5Bfillcolor%3D%22%22%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%207%20%5Bfillcolor%3Dgreen%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%208%20%5Bfillcolor%3Dgreen%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%209%20%5Bfillcolor%3D%22%22%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%2010%20%5Bfillcolor%3Dgreen%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%2011%20%5Bfillcolor%3D%22%22%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%2012%20%5Bfillcolor%3D%22%22%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%2013%20%5Bfillcolor%3Dgreen%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%2014%20%5Bfillcolor%3D%22%22%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%2015%20%5Bfillcolor%3Dgreen%2C%20style%3Dfilled%5D%3B%0D%0A%20%20%20%200--1%20%3B%0D%0A%20%20%20%200--2%20%3B%0D%0A%20%20%20%200--4%20%3B%0D%0A%20%20%20%200--8%20%3B%0D%0A%20%20%20%201--3%20%3B%0D%0A%20%20%20%201--5%20%3B%0D%0A%20%20%20%201--9%20%3B%0D%0A%20%20%20%202--3%20%3B%0D%0A%20%20%20%202--6%20%3B%0D%0A%20%20%20%202--10%20%3B%0D%0A%20%20%20%203--7%20%3B%0D%0A%20%20%20%203--11%20%3B%0D%0A%20%20%20%204--5%20%3B%0D%0A%20%20%20%204--6%20%3B%0D%0A%20%20%20%204--12%20%3B%0D%0A%20%20%20%205--7%20%3B%0D%0A%20%20%20%205--13%20%3B%0D%0A%20%20%20%206--7%20%3B%0D%0A%20%20%20%206--14%20%3B%0D%0A%20%20%20%207--15%20%3B%0D%0A%20%20%20%208--9%20%3B%0D%0A%20%20%20%208--10%20%3B%0D%0A%20%20%20%208--12%20%3B%0D%0A%20%20%20%209--11%20%3B%0D%0A%20%20%20%209--13%20%3B%0D%0A%20%20%20%2010--11%20%3B%0D%0A%20%20%20%2010--14%20%3B%0D%0A%20%20%20%2011--15%20%3B%0D%0A%20%20%20%2012--13%20%3B%0D%0A%20%20%20%2012--14%20%3B%0D%0A%20%20%20%2013--15%20%3B%0D%0A%20%20%20%2014--15%20%3B%0D%0A%20%20%20%20%7D)

Basic entities:

1. **Filters** - filter graph collection by some parameters
2. **Generators** - generate collection of graphs or automatas based on graphs
3. **Readers** - read graph collection from files
4. **Recorders** - write graph to file
5. **Selectors** - configration scripts for user input

# TODO

1. Multiple format support
2. Performance improvement
3. Integrations with nauty scripts