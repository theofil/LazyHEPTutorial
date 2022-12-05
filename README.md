# LazyHEPTutorial

This is meant to be an introduction on how to analyze data from LHC experiments.

![imageLT](https://theofil.web.cern.ch/theofil/images/imageLT.png)

There are plenty of tutorials based on [CMS open data](http://opendata.cern.ch/docs/about-cms), what's special here ?

The short answer, is that there is nothing really special other than you can run an 
[A-Z analysis solely on Google Colab](https://github.com/theofil/LazyHEPTutorial/blob/main/code/python/LazyHEPTutorialColab.ipynb), without installing anything in your computer.  

The [LazyHEPTutorial](https://github.com/theofil/LazyHEPTutorial) is using the CMS open data files from the [HEP Tutorial](http://opendata.cern.ch/record/50), written by C. Sander & A. Schmidt,
but is entirely rewritten so that it can be run in Google Colab using [uproot](https://uproot.readthedocs.io/en/latest/) and [awkward](https://awkward-array.readthedocs.io/en/stable/index.html) arrays.
For comparison, a very minimal C/C++ version of a simple event-loop analysis using ROOT is provided in the code folder. 

Which computing language you need to master if you want to work for CERN ? Both C/C++ and python are mandatory nowadays.


## Introduction
In this tutorial, we will make use of a [pocket-size data sample](http://theofil.web.cern.ch/theofil/cmsod/files/) that 
comes with the [HEP Tutorial](http://opendata.cern.ch/record/50), comprising of just a small fraction of pp collision data $(50pb^{-1})$ where at least one muon is present in the collision debris (single muon trigger),  suitable for doing everything with your laptop or even with your phone if you can connect a keyboard to it :-). But before going on technicalities of how we do a data analysis, few general things to see and appreciate.

* [Standard Model](https://en.wikipedia.org/wiki/Standard_Model)
* [LHC video, absolute must see!](https://www.youtube.com/watch?v=pQhbhpU9Wrg)
* [CMS video, all what you need to know for getting started](https://www.youtube.com/watch?v=S99d9BQmGB0)


The [LHC](https://www.home.cern/science/accelerators/large-hadron-collider) experiments use [ROOT](https://root.cern) to analyze and **store** the information recorded during hadronic collisions.
So, if you would want to study the interactions taking during pp collisions, you need to learn how to open and read the ROOT files produced by the experiments.

There are many ways to open a ROOT file, the most popular are:
1. Install [ROOT](https://root.cern). 
2. Install [uproot](https://uproot.readthedocs.io/en/latest/) and [awkward](https://awkward-array.readthedocs.io/en/stable/index.html) arrays.

This tutorial offers in addition a third way:

3. Use Google Colab suite, [see the analysis example in Google Colab](https://github.com/theofil/LazyHEPTutorial/blob/main/code/python/LazyHEPTutorialColab.ipynb).

The 1st solution is most probably what you will end up doing, sooner or later. 
The 2nd is what you will probably need to learn if you would want to bring in to your analysis developments happening elsewhere 
(not at CERN), most famous use-case is machine learning (e.g., keras, tensorflow, pytorch ...). 
The 3rd solution is what you should try out in any case, since there are no prerequisites and technical competences you need to have other than a gmail/android account.

At this point, I should say that the three solutions listed above are with inverted order in terms of difficulty. 
For the 1st, even if you go for the python version of ROOT (pyROOT) you better know also some C++ if you want to get benefit of the ROOT performance, especially recent developments like RDataFrame.
For the 2nd, if the line `pip install uproot awkward` doesn't mean anything to you then you might consider installing first [Anaconda](https://anaconda.org) and then installing 
the needed packages for [uproot](https://uproot.readthedocs.io/en/latest/) and [awkward](https://awkward-array.readthedocs.io/en/stable/index.html) arrays, inside the anaconda enviroment.


## Physics Analysis
The most basic quantity we are interested in particle physics is called [cross section](https://en.wikipedia.org/wiki/Cross_section_(physics)) $(\sigma)$ for a particular particle 
interaction to take place.
You could think the cross section of a process as the analogous of the probability for that process to take place, 
but instead being a pure number it is measured in units of area, [1 barn](https://en.wikipedia.org/wiki/Barn_(unit)) $= 10^{-28}m^{2}$.
The sample size of the LHC pp collision data is quantified by what is known as luminosity measurement (L) and has 
units of inverse area $(fb^{-1})$, where $f$ stands for the femto $=10^{-15}$ order of magnitude. 
More pp collisions on tape, means more $fb^{-1}$'s, i.e., more L. 

### Master Equation
The number of events $(N)$ we expect for a specific process  with known cross section $(\sigma)$ in a data sample of with luminosity $(L)$ is: 

$N = \epsilon \sigma L$  

where $\epsilon$ is the detector efficiency for recording this process. 

### Physics Processes
While we have some control of the initial state, e.g., the center of mass energy of the colliding protons, we don't really control what comes out in the final state.
Provided that there is sufficient energy in the initial state, all possible paths (particle interactions) will be taken by nature with probabilities that governed (we believe)
by the laws of quantum mechanics. In fact, we can't do anything to stop:
*  $4000 W^{\pm}$ / sec 
*  $1200 Z^{0}$' / sec
*  $17 t \bar{t}$ / sec
*  $1 h^{0}$ / sec
from being produced during LHC Run II, with $\sqrt{s} = 13$ TeV and $20 nb^{-1)/s}$ instantenous luminosity of pp collisions.  
However, the particle detectors don't detect directly the very short lived particles listed above, but rather detect their decay products. 
By applying selection criteria (analysis cuts) on the pp data, one can increase the efficiency of selecting a specific process (call it signal: S) against other processes (call them backgrounds: B) 
that will also satisfy the applied criteria. 
Ideally, we would want the signal efficiency to be 100% while the backgrounds to have 0% efficiency.
However, this is almost never the case and there is always some background contribution in the sub-sample of data we selected to focus our attention.
The amount of background events in our signal-enriched sample has to be estimated and MC simulation might be used for that purpose. 
It is therefore typical that together with the MC simulation of the signal we do also consider the background simulation, which is usually much more difficult to get correctly. 
We compare the data yield versus the MC simulated S+B sample in designated control regions selected in data, 
for which we expect the modeling of the data provided by the MC simulation to be sufficiently good.
An example of this type of comparison is show below:

![LazyHEPtutorial_j2.png](cern.ch/theofil/images/LazyHEPtutorial_j2.png)




