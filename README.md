# Stop Dilepton Analysis

## High-level Summary
This repository contains the bulk (soon to be all) of my code (scripting languages, C++, python, etc.) related to my PhD Thesis:

**B. Calvert**, "*Search for Pair Production of Top Squarks in Proton-Proton Collisions at ![equation](http://latex.codecogs.com/svg.latex?\sqrt{s}=8\ \textrm{TeV})*", Nov. 2015, UMD DRUM [doi:10.13016/M2KX54](http://drum.lib.umd.edu/handle/1903/17295), [cds.cern.ch/record/2161071](https://cds.cern.ch/record/2161071?ln=en)

The code within can, in principle, be used to execute almost 100% of the start to finish trajectory of a particle physics analysis, beginning from reconstructed LHC proton-proton collision data and ending with salient particle physics results (that I delivered as part of my PhD) including,

- Statistical exclusions on the existence of a hypothetical new particle
- Calculations of the statistical significance of observed possible signatures of a new particle in the data.

As part of this repository, I have written numerous functions and classes for data visualization (including a novel method for statistical smoothing that I developed -- c.f. Appendix H.2.3 of my dissertation), specific analysis cross-checks (including pure statistical cross-checks as well as cross-checks motivated by physics-specific considerations), and more general data science-y features, like data pipelining and feature engineering.

I'll be the first to admit that this code is not the cleanest code I've ever written; although I proactively refactored my code often over the course of my PhD, technical debt would nevertheless creep in (as it is wont to do). One notable way in which this occurred is that aspects of my specific particle physics scenario (a search for supersymmetry) are interwoven into the general code (e.g. in terms of specific experimental systematic uncertainties or parameter scans), and would need to be separated if one wanted to repurpose those parts.

## Plans for the Repository Code
I am currently updating this repository, notably, among other things,
- Adding in a proper makefile system, rather than the stand-alone ROOT macro compilation scripts that I wrote
- Further splitting the code into proper, separate modules
- General code refactoring

with the goal that other particle physicists could apply parts of my code in order to facilitate various components of their own analyses utilizing LHC collision data.

## Thesis summary

In case you're interested, I've included a semi-short description of my thesis research below.

My thesis presented a search for signatures of the decay of the top-squark, the (hypothetical) supersymmetric partner to the top quark. To quickly motivate this search, supersymmetric extensions to the standard model can solve a number of *current, unresolved issues* in particle physics including,
- Providing a *viable dark matter particle candidate*
- *Naturally resolving* parameter *fine-tuning issues* related to the mass of the Higgs boson.

In most of these models, the top-squark plays an integral role in fixing some of the aforementioned issues. Although the existence of many supersymmetric partner particles have been strongly constrained by experiments, at the time that I wrote my thesis, the existence of the top-squark remained largely unconstrained, particularly in the region of parameter space where the top-squark mass was close to the top quark mass (~175 GeV/c^2).

For my thesis, I specifically considered the *dileptonic* decay signature of pair-produced top-squarks. Lots of buzzwords I know -- here's how it plays out: two top-squarks are produced from the collision of two protons; these top-squarks can decay in a number of different ways, as an example, one of these decay modes includes the direct decay of each top-*squark* to a top *quark* and a *neutralino* (NB, this is the dark matter candidate), as can be seen in the image below,

<p>
    <img src="https://github.com/bcalvert/StopDiLepton2014/files/1116647/T2tt.pdf" alt="T2tt" align="center" height="250">
</p>
    <em>A pseudo-Feynman diagram representation of the decay of pair-produced top-squarks (t with a tilde) into top quarks and neutralinos (the greek chi); supersymmetric particles (i.e. hypothetical particles) are marked in red. </em>


In the above decay mode, the top quarks then decay to a W boson and b-quark, where *each* of the two W bosons then promptly decays to a lepton (hence the **di**-*leptonic* qualifier).

If that sounds like a mouthful, I understand. The most salient point is that, barring the neutralinos, this hypothetical particle pair-production event would look almost exactly like the standard, *known* pair-production of top quarks. As a further complication, for the models I considered in my thesis, the neutralinos produced in the top-squark decay do not interact within our detector at all, meaning that the directly visible experimental signatures of our hypothetical signal look more-or-less **exactly** like our most common background.

### Enter MET and MT2

Fortunately, although they are not directly visible, the presence of these neutralinos can *strongly* affect the correlations between the directly-visible objects in the collision event. The most notable way in which this happens is that these neutralinos alter the relative momentum balance of objects in the plane transverse to the proton-proton collision axis. In a typical LHC collision event, the transverse momentum of all objects produced in the collision perfectly balances out to zero (momentum conservation). When some of these objects are invisible, one will measure a net-*imbalance* in the sum transverse momentum, serving as an indirect means of observing invisible particles. This net imbalance is called missing transverse momentum, or MET for short.

For reasons I won't get into here, for this thesis I did not simply use MET as a distinguishing feature (TL;DR it doesn't buy you enough S/B discrimination), but instead utilized variant constructions of the MT2 variable, a feature that is built by combining (in a nontrivial fashion) the MET vector with the measured transverse momentum (![equation](http://latex.codecogs.com/svg.latex?\vec{p}_{T})) vectors of various visible objects in the event. As an example for what this buys you, consider MT2(ll), an MT2 variant built by combining the MET vector with the two lepton ![equation](http://latex.codecogs.com/svg.latex?\vec{p}_{T}).

When built in this fashion, the distribution of MT2(ll) in the main category of background events (pair-production of top quarks, aka t-tbar) exhibits what is known as a "kinematic-edge" at ~80 GeV -- that is, the frequency of events with specific measured MT2(ll) values rapidly plummets for values greater than 80 GeV. This is in contrast to the signal events, where the presence of the (invisible) neutralinos violates a critical internal-constraint of these decaying particle systems; consequently, there is no kinematic edge at 80 GeV and in fact, the MT2(ll) distribution in our hypothetical signal events is often expected to extend much further beyond 80 GeV.

Both of these phenomena can be observed in the figure below, which shows a comparison between the observed distribution of this variable for our experimental data (black points), the expected distributions for our known backgrounds (filled, colored histograms), and, using unfilled histograms, the expected distributions for two possible top-squark signal scenarios ("tt" refers to the top-squark decay chain mentioned above, while "bbWW" references a very similar top-squark decay chain that results in effectively the same net collection of final, stable particles).

<p>
    <img src="https://github.com/bcalvert/StopDiLepton2014/files/1116848/DilepMT2ll_wSig.pdf" alt="MT2ll" align="center" height="450">
</p>

You can easily observe the kinematic edge (marked with a dashed line) in the t-tbar background (note the logarithmic scale on the y-axis); in contrast, one can see how the distribution for the top-squark signal scenarios extends far beyond the kinematic edge. Unfortunately (if you're a theorist), we don't see any clear "smoking gun" differences between our observed data and the expected background distributions (remember! you have to account for statistical and experimental systematic uncertainties), so there is no clear evidence for top-squark pair-production in this figure.

I've still got to add a fair bit more, because the really juicy bits (including the possible signature of a new particle in my data) have not even been hit yet!