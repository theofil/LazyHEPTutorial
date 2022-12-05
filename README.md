# LazyHEPTutorial

This is meant to be an introduction on how to analyze data from LHC experiments.

![imageLT](https://theofil.web.cern.ch/theofil/images/imageLT.png)

There are plenty of tutorials based on [CMS open data](http://opendata.cern.ch/docs/about-cms), what's special here ?

The short answer, is that there is nothing really special other than you can run an 
[A-Z analysis solely on Google Colab](https://github.com/theofil/LazyHEPTutorial/blob/main/code/python/LazyHEPTutorialColab.ipynb), without installing anything in your computer.  

The [LazyHEPTutorial](https://github.com/theofil/LazyHEPTutorial) is using the CMS open data files from the [HEP Tutorial](http://opendata.cern.ch/record/50), written by C. Sander & A. Schmidt,
but is entirely rewritten so that it can be run in Google Colab using [uproot](https://uproot.readthedocs.io/en/latest/) and [awkward](https://awkward-array.readthedocs.io/en/stable/index.html) arrays.
For comparison, a very minimal C/C++ version of a simple event-loop analysis using ROOT is provided in the code folder. 

Which computing language you need to master if you want to work for CERN ? Both C/C++ and python are mandatories nowadays.


## Introduction
In this tutorial, we will make use of a [pocket-size data sample](http://theofil.web.cern.ch/theofil/cmsod/files/) that comes with the [HEP Tutorial](http://opendata.cern.ch/record/50), comprising of just a small fraction of pp collisionn data $(50fb^{-1})$ where at least one muon is present in the collision debris (single muon trigger),  suitable for doing everything with your laptop or even with your phone if you can connect a keyboard to it :-). But before going on technicalities of how we do a data analysis, few general things to see and appreciate.

* [Standard Model](https://en.wikipedia.org/wiki/Standard_Model)
* [LHC video, absolute must see!](https://www.youtube.com/watch?v=pQhbhpU9Wrg)
* [CMS video, all what you need to know for getting started](https://www.youtube.com/watch?v=S99d9BQmGB0)


The [LHC](https://www.home.cern/science/accelerators/large-hadron-collider) experiments use [ROOT](https://root.cern) to analyze and **store** the information recorded during hadronic collisions.
So, if you would want to study the interactions taking during pp collisions, you need to learn how to open and read the ROOT files produced by the experiments.

There are many ways to open a ROOT file, the most popular are:
1. Install [ROOT](https://root.cern). 
2. Install [uproot](https://uproot.readthedocs.io/en/latest/) and [awkward](https://awkward-array.readthedocs.io/en/stable/index.html) arrays, use Python ecosystem for your analysis. If the line `pip install uproot awkward` doesn't mean anything to you, then you might consider installing [Anaconda](https://anaconda.org) and install the needed packages via the anaconda environment.

This tutorial offers in addition a third way:

3. Use Google Colab suite, [see the analysis example in Google Colab]((https://github.com/theofil/LazyHEPTutorial/blob/main/code/python/LazyHEPTutorialColab.ipynb)).

The 1st solution is most probably what you will end up sooner or later, the 2nd is what you will probably need to learn if you would want to bring in at CERN developments happening elsewhere 
(not at CERN), most famous usecase is machine learning (e.g., keras, tensorflow, pytorch ...).

, following the instructions from the [ROOT](https://root.cern), use C/C++ and Python ecosystems to write up analysis code.
