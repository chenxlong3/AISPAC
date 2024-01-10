# Link Recommendation to Augment Influence Diffusion
This repo tries to augment the influence diffusion by the seed set by recommending links.

# File Structure
```
.
|-- CMakeLists.txt
|-- README.md
|-- build
|   |-- CMakeCache.txt
|   |-- CMakeFiles
|   |-- IMA_only
|   |-- MCGreedy_only
|   |-- Makefile
|   |-- RW
|   |-- cmake_install.cmake
|   |-- eval_by_mc
|   `-- format_graph
|-- data
|   |-- DBLP
|   |-- Epinions
|   |-- GRQC
|   |-- LiveJournal
|   |-- nethept
|   |-- orkut
|   |-- twitter
|   `-- wiki_vote
|-- data_process.ipynb
|-- exec_files
|   |-- IMA_only.cpp
|   |-- MCGreedy_only.cpp
|   |-- RW.cpp
|   |-- evaluate.cpp
|   `-- main.cpp
|-- include
|   |-- Algs.h
|   |-- CommonFuncs.h
|   |-- Experiments.h
|   |-- GraphBase.h
|   |-- IOcontroller.h
|   |-- InfGraph.h
|   |-- SFMT
|   |-- Timer.h
|   |-- headers.h
|   `-- serialize.h
`-- scripts
    |-- IMA_grqc.sh
    |-- IMA_nethept.sh
    |-- MCG_epinion.sh
    |-- MCG_grqc.sh
    |-- MCG_nethept.sh
    |-- clear_build.sh
    |-- epinions_whole.sh
    |-- format_graphs.sh
```


# Usage
## Cmake Compile
```
cd ./scripts
./clear_build
```
## Format Graph
In the folder of one dataset, two files should be included - `attr.txt` and `edgelist_ic.txt`.
`attr.txt` gives the number of nodes and edges in the following format.

`edgelist_ic.txt` stores the edge list of the graph. An example is shown below.
```
3466	937 0.1
3466	5233    0.1
3466	826 0.1
```

```
cd ../build
./format_graph -dataset ../data/slashdot -filename edgelist_ic.txt
```