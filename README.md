  

# LazyHEPTutorial

  
This is meant to be an introduction on how to analyze data from the LHC experiments.

  

*There are plenty of tutorials based on [CMS open data](http://opendata.cern.ch/docs/about-cms), what's special here ?*

  

The short answer, is that there is nothing really special other than you can run an [A-Z analysis template relying solely on Google Colab](https://github.com/theofil/LazyHEPTutorial/blob/main/code/python/LazyHEPTutorialColab.ipynb), without installing anything in your computer.

The [LazyHEPTutorial](https://github.com/theofil/LazyHEPTutorial) is using the CMS open data files from the [HEP Tutorial](http://opendata.cern.ch/record/50), written **by C. Sander & A. Schmidt**, but is entirely rewritten so that it can be run in Google Colab using [uproot](https://uproot.readthedocs.io/en/latest/) and [awkward](https://awkward-array.readthedocs.io/en/stable/index.html) arrays. In addition, a minimal C/C++ version (all-in-one-source-file) of a simple event-loop analysis relying on ROOT is provided in the code folder and will automatically fetch from the web all ROOT files needed to run the analysis.

  

*Which computing language you need to master if you want to work for CERN ?* 
Both C/C++ and python are mandatory nowadays.

  

Useful links:

* original tutorial [HEP Tutorial](http://opendata.cern.ch/record/50) from  **C. Sander & A. Schmid**

* my own backup of the original HEP Tutorial, together with the accompanying ROOT files,  C++ code and documentation, all bundled inside a single downloadable [HEPTutorial.tar.gz](http://theofil.web.cern.ch/theofil/cmsod/HEPTutorial.tar.gz)

* my own pythonic remake of the HEP Tutorial giving a [A-Z analysis template relying solely on Google Colab](https://github.com/theofil/LazyHEPTutorial/blob/main/code/python/LazyHEPTutorialColab.ipynb)

* my own [C++ remake](https://github.com/theofil/LazyHEPTutorial/tree/main/code/C) of the HEP Tutorial (automatically fetches from the web the needed ROOT files). 
  

# Introduction

  

In this tutorial, we will make use of a [pocket-size data sample](http://theofil.web.cern.ch/theofil/cmsod/files/) that comes with the [HEP Tutorial](http://opendata.cern.ch/record/50), comprising of just a small fraction of pp collision data $(50pb^{-1})$ where at least one muon is present in the collision debris (single muon trigger),  suitable for doing everything with your laptop or even with your phone if you can connect a keyboard to it :-). But before going on technicalities of how we do a data analysis, few general things to see and appreciate.

  

  

* [Standard Model](https://en.wikipedia.org/wiki/Standard_Model)

  

* [LHC video, absolute must see!](https://www.youtube.com/watch?v=pQhbhpU9Wrg)

  

* [CMS video, all what you need to know for getting started](https://www.youtube.com/watch?v=S99d9BQmGB0)

  

  
## Opening ROOT files
  

The [LHC](https://www.home.cern/science/accelerators/large-hadron-collider) experiments use [ROOT](https://root.cern) to analyze and **store** the information recorded during hadronic collisions.

  

So, if you would want to study the interactions taking during pp collisions, you need to learn how to open and read the ROOT files produced by the experiments.

  

  

There are many ways to open a ROOT file, the most popular are:

  

1. Install [ROOT](https://root.cern).

  

2. Install [uproot](https://uproot.readthedocs.io/en/latest/) and [awkward](https://awkward-array.readthedocs.io/en/stable/index.html) arrays.

  

  

This tutorial offers in addition a third way:

  

  

3. Use Google Colab suite, [see the analysis example in Google Colab](https://github.com/theofil/LazyHEPTutorial/blob/main/code/python/LazyHEPTutorialColab.ipynb), which relies on [uproot](https://uproot.readthedocs.io/en/latest/) and [awkward](https://awkward-array.readthedocs.io/en/stable/index.html) arrays but you don't have to install anything in your computer.

  

  

  

## Physics Analysis

  

The most basic quantity we are interested in particle physics is called [cross section](https://en.wikipedia.org/wiki/Cross_section_(physics)) $(\sigma)$ for a particular particle interaction to occur. You could think the cross section of a process as the analogous of the probability for that process to take place, but instead being a pure number it is measured in units of area, [1 barn](https://en.wikipedia.org/wiki/Barn_(unit)) $= 10^{-28}m^{2}$.

  

The sample size of the LHC pp collision data is quantified by what is known as luminosity measurement (L) and has units of inverse area $(fb^{-1})$, where $f$ stands for the femto $=10^{-15}$ order of magnitude. Smaller cross section area implies smaller chance for the interaction to occur. On the other hand, more pp collisions on tape, means more $fb^{-1}$'s, i.e., more L. So we can probe a process of small $\sigma$ if $L$ is sufficiently large, provided that we have a way to select $pp$ events enriched with the process of interest  (in most cases a $pp$ collision results in a boring final state) . The first level of selection is known in CMS as L1-trigger and is made by very fast algorithms encoded in FPGAs. The L1 algorithms have to be quicker than $3.2\mu s$ and do partial and coarse reconstruction of physics objects like (jets, e/γ, MET, μ, τ, b-jets ...) that are used to decide if the event is interesting enough to keep it on tape for offline analysis or not. Events that are not firing the L1-trigger, are lost for ever. Further qualification criteria are imposed by algorithms running on a computing farm, known as High Level Trigger. Events that have passed the two levels of triggering (L1 and HLT) are available for offline studies, where typically the person doing the analysis defines further (offline) criteria to define how the signal should look like.

  

### Master Equation

  

The number of events $(N)$ we expect for a specific process  with known cross section $(\sigma)$ in a data sample of known luminosity $(L)$ is:

  

  

$N = \epsilon \sigma L$

  

  

where $\epsilon$ is the detector efficiency for recording this process.

   

### Physics Processes

  

While we have some control of the initial state, e.g., the center of mass energy of the colliding protons, we don't really control what comes out in the final state.

  

Provided that there is sufficient energy in the initial state, all possible paths (particle interactions) will be taken by nature with probabilities that governed (we believe) by the laws of quantum mechanics. In fact, we can't do anything to stop:

  

*  $4000$ $W^{\pm}$ 

*  $1200$ $Z^{0}$ 

*  $17$ $t\bar{t}$ 

*  $1$ $h^{0}$ 

  

from being **produced every second** during LHC Run II,  for $\sqrt{s} = 13$ TeV and $20 nb^{-1}/s$ instantaneous luminosity. However, the particle detectors don't detect directly the very short lived particles listed above, but rather detect their decay products.

  

By applying selection criteria (analysis cuts) on the pp data, one can increase the efficiency of selecting a specific process (call it signal: S) against other processes (call them backgrounds: B) that will also satisfy the applied criteria mimicking the signal.

  

*Ideally, we would want the signal efficiency to be 100% while the backgrounds to have 0% efficiency.*

  

Unfortunately, this is almost never the case and there is always some background contribution in the sub-sample of data we selected to focus our attention. The amount of background events in our signal-enriched sample has to be estimated and MC simulation might be used for that purpose.

  

It is therefore typical that together with the MC simulation of the signal we do also consider the background simulation, which is usually much more difficult to get correctly (i.e., having larger uncertainty on the predicted event yields).

  

We compare the data yield versus the MC simulated S+B sample in designated control regions selected in data, for which we expect the modeling of the data provided by the MC simulation to be sufficiently good. Find out more below, in the physics analysis examples


  

### Cross Section Measurements

  

Perhaps the simplest and most fundamental type of an LHC physics analysis is a cross section measurement.

  

*This can be found turning around the **Master Equation**.*

  

We measure $L$ from data and estimate $\epsilon$ for the signal $(S)$ using MC simulation (sometimes corrected with data-driven scale factors).
  

Assuming that MC predicts with good accuracy the amount of background we expect in the signal region ($B$), the measured signal yield in data should be just $N - B$. In the ideal case (with no uncertainties of any type) we would expect by construction that $N = S + B$, with $N$ being the measured event counts in the signal region of data and $S$ and $B$ the expected signal and background in the signal region, which here we will get solely from MC simulation.

  

Dividing our signal candidate events in data $(N-B)$ by the factor $\epsilon L$, gives an estimate of signal's cross section in data, which could be compared with the $\sigma$ expected from theory.

  

In its simplest incarnation an LHC cross section measurement is as simple as an event counting experiment, provided that we know accurately $B$, $\epsilon$ and $L$.

  

The uncertainty of $L$ is at the level of $2-3$ percent, so all the **analysis challenge** boils down to estimate $\epsilon$ and $B$ in the signal region and find a way to define a suitable signal region such as the uncertainties are minimum.


# Analyses Examples
* [code/C/simple.C](https://github.com/theofil/LazyHEPTutorial/blob/main/code/C/simple.C) opens just the ``data.root`` file (without loading MC samples) and counts how many events in data satisfy the $N_\mathrm{\mu} = 2$ and $N_\mathrm{j} \ge 2$ selection cuts (see lines 133-155)
* [code/C/makePlot.C](https://github.com/theofil/LazyHEPTutorial/blob/main/code/C/makePlot.C) is a generic analysis template. You can recycle its code and modify it to adapt it to your desired analysis. It loads all data and MC samples and compares the distribution of $N_\mathrm{\mu}$ in data and MC simulation, for events that pass the  ``triggerIsoMu24`` trigger bit. <img src="https://github.com/theofil/LazyHEPTutorial/blob/main/code/C/NMuon.png?raw=true" width="500"> 
It also prints summary counting statistics for data, the sum of all MCs as well as each MC sample individually taking account the event weights $w =\sigma L/N_\mathrm{gen}^{\mathrm{tot}}$. (The ``EventWeight`` is trivially 1.0 for data and varies for different MCs according the cross section of the simulated process and the total number of events that have been generated.)  Lines 48-55 define the histogram range, binnning and axis titles. L162 is where the file loop indexed with ``i`` starts.  L191 is where the *event loop* starts, indexed with ``j``. L202-205 is essentially all the analysis i.e., the event selection applied as an ``if`` statement and the variable to be histogrammed, in our case the $N_\mathrm{\mu}$.   
* [C/bjet/makePlot.C](https://github.com/theofil/LazyHEPTutorial/blob/main/code/C/bjet/makePlot.C) is recycled version of the ``makePlot.C`` that has been copied into a new folder named``bjet`` and modified internally such as for each event it counts the number of jets that found to satisfy the ``b-tagging`` requirement, applied as cut [here](https://github.com/theofil/LazyHEPTutorial/blob/68c377b48a212838b04b65f1be1a40e8e0d3bc70/code/C/bjet/makePlot.C#L195). The analysis cuts are encoded [here](https://github.com/theofil/LazyHEPTutorial/blob/68c377b48a212838b04b65f1be1a40e8e0d3bc70/code/C/bjet/makePlot.C#L188) and select events that have at least one muon with $p_\mathrm{T}>25$ GeV and the ``triggerIsoMu24``  bit fired.   <img src="https://github.com/theofil/LazyHEPTutorial/blob/main/code/C/bjet/NBJet.png?raw=true" width =500>
* [C/os_dimuons/makePlot.C](https://github.com/theofil/LazyHEPTutorial/blob/main/code/C/os_dimuons/makePlot.C) is recycled code adapted to select ``triggerIsoMu24==1`` and two opposite-sign muons, with the leading muon having $p_\mathrm{T}>25$ plotting the $N_\mathrm{\mu}$ distributions. <img src="https://github.com/theofil/LazyHEPTutorial/blob/main/code/C/os_dimuons/NMuon.png?raw=true" width = 500>
* [C/os_dimuons/os_dimuons_mass/makePlot.C](https://github.com/theofil/LazyHEPTutorial/blob/main/code/C/os_dimuons/os_dimuons_mass/makePlot.C) is build on top of the previous analysis script and applies  the same event selection. In addition, it calculates the invariant mass of the system of the two muons for each event and compares data vs MC. 
<img src="https://github.com/theofil/LazyHEPTutorial/blob/main/code/C/os_dimuons/os_dimuons_mass/os_dimuons_mass.png?raw=true" width = 500>

# Projects
## Project-1  [Trigger Efficiency]
Study the trigger efficiency of the signal, defined here as the semileptonic decay of $t\bar{t}$ pairs, leading to final states with $N_\mathrm{\mu}>=1$. For this project you will mostly need to open just the ``ttbar.root`` , as it's the only sample we have available for which the events that **do not** pass the ``triggerIsoMu24`` bit, i.e.,  events having ``triggerIsoMu24 == false`` are also stored inside the ROOT file. 

Suitably adapt [simple.C](https://github.com/theofil/LazyHEPTutorial/blob/main/code/C/simple.C) to count the $\epsilon_\mathrm{trigger} = \mathrm{pass}/\mathrm{total}$  in bins of the **generated** muon $p_\mathrm{T}$ in MC truth. Select events that have one muon generated ``fabs(MCleptonPDGid) == 13`` and calculate the MC generated $p_\mathrm{T}$ of the muon using the ``MClepton_px`` and ``MClepton_py`` branches. Estimate the efficiency for a generated muon to pass the CMS trigger ``triggerIsoMu24 == true`` as a function of its $p_\mathrm{T}$  (i.e., in bins of pt),  starting with very fine binning e.g., 0.25 or 0.5 GeV in width and increasing it to 1-20 GeV widths at high $p_\mathrm{T}$ for when the available statistics start to be an issue.

  
Thinking needs to be placed for what would be the statistical uncertainty of the $\epsilon_\mathrm{trigger}$. For simplicity we can calculate the uncertainty on the efficiency estimation, in the normal frequentist approximation.  

In the normal frequentist method we assume that  the observed events that pass the selection $(n)$ over the total events $(N)$, $\epsilon = n/N$, is an estimate of the true efficiency $ε_\mathrm{true}$ and that the uncertainty in the efficiency estimation follows normal distribution with $\sigma = [(\epsilon(1-\epsilon)/Ν)]$. Explain why this definition of $\sigma$ is reasonable, starting from the fact that $n\sim \mathrm{binomial} (N, p = ε_\mathrm{true})$ and that we approximate the unknown true efficiency $p = ε_\mathrm{true} \approx ε = n/N$.
 
Furthermore,  verify that the branching ratio we get in ttbar MC for events with exactly 1μ and no other charged leptons in the final state (semi-leptonic final state in the muon channel), is what we expect given that $BR(W\to\mu\nu) \approx 10.6%$.

**Key figures to show in a presentation:**
* $\epsilon_\mathrm{trigger}$ in bins of the generated muon $p_\mathrm{T}$, when taking into account the event weights but ignoring any uncertainty.
*  $\epsilon_\mathrm{trigger}$ in bins of the generated muon $p_\mathrm{T}$, *without* taking into account the event weights and estimating the corresponding uncertainty in the normal frequentist method.
* reconstructed muon $p_\mathrm{T}$ calculated from ``Muon_Px[0]`` and ``Muon_Py[0]`` for data and MC, without any threshold in the muon $p_\mathrm{T}$ for events selected ``triggerIsoMu24 == true``. (For this plot you will need to modify ``makePlot.C`` analysis script and use fine binning of 1 GeV width.) 


## Project-2  [1μ]
The event preselection starts with requiring exactly one muon $(N_\mathrm{\mu} = 1)$ final state, for events with ``triggerIsoMu24==1`` true. 

Study the reconstructed muon $p_\mathrm{T}$ calculated from ``Muon_Px[0]`` and ``Muon_Py[0]`` $p_\mathrm{T}$  without any threshold starting from $p_\mathrm{T}=0$, for events selected ``triggerIsoMu24 == true``. (For this plot you will need to modify ``makePlot.C`` analysis script and use fine binning of 1 GeV width.)  Show that is reasonable to select only those events with a leading muon having  $p_\mathrm{T}>25$ GeV. 

For the selected events (i.e., preselection + $p_\mathrm{T}>25$ GeV requirement), produce the transverse mass $m_\mathrm{T}$ and the MET  distributions as well as the $(N_\mathrm{j})$ and b-jet $(N_\mathrm{bj})$ multiplicity distributions. 

To calculate the transverse mass $m_\mathrm{T}$, we will work under the assumption that the visible MET is solely due to the transverse momentum of one escaping neutrino. We will neglect the muon and the neutrino masses and build their *transverse* 4 vectors such as they are light-like $(P^2 = 0)$, using only the transverse component of their momentum $p^{\mu}=(\sqrt{p_\mathrm{x}^2+p_\mathrm{y}^2}, p_\mathrm{x}, p_\mathrm{y}, 0)$. We will sum the two  *transverse* 4-vectors and calculate the magnitude (mass) of their sum, which is the definition of the $m_\mathrm{T}$. Note that is not possible to calculate the ordinary invariant mass of the $m(\mu, \nu)$ system, since  the $p_\mathrm{z}$ of the $\nu$ is unknown. The $m_\mathrm{T}$ is the closest   quantity we could built to the invariant mass of the system of two particles, having as endpoint the $m(\mu, \nu)$ and being itself also invariant. See also  49.6 [PDG2022](https://pdg.lbl.gov/2022/reviews/rpp2022-rev-kinematics.pdf). 

**Key figures to show in a presentation:**
 * reconstructed muon $p_\mathrm{T}$ calculated from ``Muon_Px[0]`` and ``Muon_Py[0]`` for data and MC, without any threshold in the muon $p_\mathrm{T}$ for events selected ``triggerIsoMu24 == true`` and $N_\mathrm{\mu}==1$, in bins of 1 GeV width.
 * MET in bins of 10 GeV width
 * transverse mass $m_\mathrm{T}$ in bins of 10 GeV width
 * jet multiplicity $N_\mathrm{j}$
 * b-jet multiplicity $N_\mathrm{bj}$
 * event counting statistics summary

## Project-3  [$μ^{+}μ^{-}$]
The event preselection starts with requiring  $N_\mathrm{\mu} \ge 2$ and  leading muon $p_\mathrm{T}>25$ GeV, for events with ``triggerIsoMu24==1`` true. In addition, require that the two muons have opposite charge.

**Key figures to show in a presentation:**
 * invariant mass of the two muons $m(μ^{+}, μ^{-})$ in bins of 0.25 GeV width in the range $[0, 20]$ GeV
 * invariant mass of the two muons $m(μ^{+}, μ^{-})$  in bins of 1 GeV width in the   range $[20, 160]$ GeV
 * MET in bins of 10 GeV width
 * jet multiplicity $N_\mathrm{j}$
 * b-jet multiplicity $N_\mathrm{bj}$
 * event counting statistics summary

**Cross section measurement**

Measure the $t\bar{t}$ cross section for a signal region that you will define by possibly optimizing MET and $N_\mathrm{bj}$, to achieve good signal significance defined here as $S/\sqrt{B}$. Assume that the relative uncertainty   for the signal selection efficiency is $30\%$  (i.e., $\delta\epsilon/\epsilon = 0.3$) and that the luminosity $L$ comes with $5\%$ uncertainty. For the background estimation $B$, assume that is only as large as the corresponding MC statistical uncertainty reported by your program.

To calculate the signal selection efficiency $\epsilon$ we will need also to count how many $t\bar{t}$ (weighted) events we have in our disposal in total ($N_\mathrm{gen}^{\mathrm{tot}}$) inside the ``ttbar.root`` file. The efficiency will simply be $\epsilon = N_\mathrm{sel}/N_\mathrm{gen}^{\mathrm{tot}}$, where $N_\mathrm{sel}$ is the total number of (weighted) events passing the selection cuts of our signal region.

Compare your measurement with the first measurement that CMS ever made, using pretty much the same data http://arxiv.org/abs/1010.5994, http://arxiv.org/abs/1105.5661, http://arxiv.org/abs/1106.0902
http://arxiv.org/abs/1108.3773. What's the main differences among them and how they compare with yours in terms of precision?

## Project-4  [$μ^{\pm}e^{\mp}$]
The event preselection starts with requiring  $N_\mathrm{\mu} \ge 1$ and  leading muon $p_\mathrm{T}>25$ GeV, at least one electron $N_\mathrm{e} \ge 1$, for events with ``triggerIsoMu24==1`` true. In addition, require that the two charged leptons have opposite charge.

**Key figures to show in a presentation:**
 * invariant mass of the two muons $m(l^{+}, l^{-})$  in bins of 1 GeV width in the   range $[0, 160]$ GeV
 * MET in bins of 10 GeV width
 * jet multiplicity $N_\mathrm{j}$
 * b-jet multiplicity $N_\mathrm{bj}$
 * event counting statistics summary

**Cross section measurement**

Measure the $t\bar{t}$ cross section for a signal region that you will define by possibly optimizing $m(l^{+}, l^{-})$ and MET if needed, to achieve good signal significance defined here as $S/\sqrt{B}$. Assume that the relative uncertainty   for the signal selection efficiency is $30\%$  (i.e., $\delta\epsilon/\epsilon = 0.3$) and that the luminosity $L$ comes with $5\%$ uncertainty. For the background estimation $B$, assume that is only as large as the corresponding MC statistical uncertainty reported by your program.

To calculate the signal selection efficiency $\epsilon$ we will need also to count how many $t\bar{t}$ (weighted) events we have in our disposal in total ($N_\mathrm{gen}^{\mathrm{tot}}$) inside the ``ttbar.root`` file. The efficiency will simply be $\epsilon = N_\mathrm{sel}/N_\mathrm{gen}^{\mathrm{tot}}$, where $N_\mathrm{sel}$ is the total number of (weighted) events passing the selection cuts of our signal region.

Compare your measurement with the first measurement that CMS ever made, using pretty much the same data http://arxiv.org/abs/1010.5994, http://arxiv.org/abs/1105.5661, http://arxiv.org/abs/1106.0902
http://arxiv.org/abs/1108.3773. What's the main differences among them and how they compare with yours in terms of precision?

*In this final state we expect significant contribution from the Drell-Yan ($Z/\gamma^{*}$) process, explain why and how the cut on the $m(l^{+}, l^{-})$ might help getting rid of this process.*

## Project-5   [$1μ$ + $\ge 1\mathrm{bjet}$ + MET]
The event preselection starts with requiring  $N_\mathrm{\mu} = 1$ and  leading muon $p_\mathrm{T}>25$ GeV, $N_\mathrm{bj} \ge 1$, for events with ``triggerIsoMu24==1`` true. 

**Key figures to show in a presentation:**
 * muon $p_\mathrm{T}$ in bins of 5 GeV width
 * MET in bins of 10 GeV width
 * jet multiplicity $N_\mathrm{j}$
 * b-jet multiplicity $N_\mathrm{bj}$
 * event counting statistics summary

**Cross section measurement**

Measure the $t\bar{t}$ cross section for a signal region that you will define by possibly optimizing $N_\mathrm{bj}$ and MET if needed, to achieve good signal significance defined here as $S/\sqrt{B}$. Assume that the relative uncertainty   for the signal selection efficiency is $30\%$  (i.e., $\delta\epsilon/\epsilon = 0.3$) and that the luminosity $L$ comes with $5\%$ uncertainty. For the background estimation $B$, assume that is only as large as the corresponding MC statistical uncertainty reported by your program.

To calculate the signal selection efficiency $\epsilon$ we will need also to count how many $t\bar{t}$ (weighted) events we have in our disposal in total ($N_\mathrm{gen}^{\mathrm{tot}}$) inside the ``ttbar.root`` file. The efficiency will simply  be $\epsilon = N_\mathrm{sel}/N_\mathrm{gen}^{\mathrm{tot}}$, where $N_\mathrm{sel}$ is the total number of (weighted) events passing the selection cuts of our signal region.

Compare your measurement with the first measurement that CMS ever made, using pretty much the same data http://arxiv.org/abs/1010.5994, http://arxiv.org/abs/1105.5661, http://arxiv.org/abs/1106.0902
http://arxiv.org/abs/1108.3773. What's the main differences among them and how they compare with yours in terms of precision?

## Project-6   [$1μ$ + $\ge 4\mathrm{jet}$ + $\ge 2\mathrm{bjet}$]

The event preselection starts with requiring  $N_\mathrm{\mu} = 1$ and  leading muon $p_\mathrm{T}>25$ GeV, $N_\mathrm{j} \ge 4$, $N_\mathrm{bj} \ge 2$, for events with ``triggerIsoMu24==1`` true. 

**Key figures to show in a presentation:**
 * muon $p_\mathrm{T}$ in bins of 5 GeV width
 * MET in bins of 10 GeV width
 * jet multiplicity $N_\mathrm{j}$ 
 * jet multiplicity $N_\mathrm{j}$ when no cut on $N_\mathrm{j}$ is placed, but all other cuts are applied
 * b-jet multiplicity $N_\mathrm{bj}$ 
 * b-jet multiplicity $N_\mathrm{bj}$ when no cut on $N_\mathrm{j}$ is placed, but all other cuts are applied
 * event counting statistics summary

**Cross section measurement**

Measure the $t\bar{t}$ cross section for this selection and report the signal significance defined here as $S/\sqrt{B}$. Assume that the relative uncertainty   for the signal selection efficiency is $30\%$  (i.e., $\delta\epsilon/\epsilon = 0.3$) and that the luminosity $L$ comes with $5\%$ uncertainty. For the background estimation $B$, assume that is only as large as the corresponding MC statistical uncertainty reported by your program.

To calculate the signal selection efficiency $\epsilon$ we will need also to count how many $t\bar{t}$ (weighted) events we have in our disposal in total ($N_\mathrm{gen}^{\mathrm{tot}}$) inside the ``ttbar.root`` file. The efficiency will simply  be $\epsilon = N_\mathrm{sel}/N_\mathrm{gen}^{\mathrm{tot}}$, where $N_\mathrm{sel}$ is the total number of (weighted) events passing the selection cuts of our signal region.

Compare your measurement with the first measurement that CMS ever made, using pretty much the same data http://arxiv.org/abs/1010.5994, http://arxiv.org/abs/1105.5661, http://arxiv.org/abs/1106.0902
http://arxiv.org/abs/1108.3773. What's the main differences among them and how they compare with yours in terms of precision?

## Project-7   [$m_\mathrm{top}$]
Measure the $t$ quark and $W$ boson masses, using the same sample of events that have been selected for project 6 (semi-leptonic $t\bar{t}$). Assume the final state $tt\to WWbb\to \mu\nu qqbb$ as fully resolved, where we have omitted charge and anti-particle notation for simplicity. Assume that the first four leading jets can be attributed to $qqbb$. The $qq$ are the two jets that are not b-tagged, while for $bb$ we assign the two jets that pass the b-tagging threshold. 

We interpret the $qq$ pair jets as coming from the hadronic decay of the W boson. Compute the invariant mass distribution of the two $q$ jets $m_\mathrm{qq}$ as well as the invariant mass distribution of the three jet system $m_\mathrm{qqb}$ assuming that is coming from the same parent $t$ quark decay. We don't know which of the two b-jets is the correct one to be paired with the $qq$, i.e., which of the bjets has the same $t$-quark parent as the $q$-jets. Try both combinations and name them $m_\mathrm{qqb_{1}}$ and $m_\mathrm{qqb_{2}}$, where $b_{1}$  and $b_{2}$ is the leading and sub-leading b-jets.


**Key figures to show in a presentation:**
 * muon $p_\mathrm{T}$ in bins of 5 GeV width
 * MET in bins of 10 GeV width
 * jet multiplicity $N_\mathrm{j}$ 
 * b-jet multiplicity $N_\mathrm{bj}$ 
 * $m_\mathrm{qq}$ in bins of 10 GeV width
 * $m_\mathrm{qqb_{1}}$in bins of 10 GeV width
 * $m_\mathrm{qqb_{2}}$in bins of 10 GeV width
 * $m_\mathrm{qqb_{1}}$ and $m_\mathrm{qqb_{2}}$ stacked in the same histogram (e.g., fill the 2 entries in the same histogram for each event)
 * event counting statistics summary
