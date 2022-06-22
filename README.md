# Oil Spill Simulation
The aim of this project is to simulate the event of an oil spill as accurately as possible to use it for developing oill spill detection and thickness estimation algorithms.

# Some ocean waves theory
Mathematically, deep water waves are defined as those occurring in ocean depths greater than twice the wavelength of the wave. For example, if the wavelength of a particular wave is 10 meters, then the wave is considered a deep water wave if the depth of the ocean is greater than 5 meters where the wave is occurring.

- Philips Spectrum:
    - The Phillips spectrum is widely used in the real-time scene simulation of ocean waves. Structural analysis has shown the Phillips spectrum to be an instantaneous spatial spectrum. Its frequency spectrum agrees with the spectrum form raised by Neumann, and is close to the P-M spectrum. (Based on Tessendorf's paper on simulating ocean water)
- Fast Fourier transform:
    - Get frequency of height field of waves → Perform inverse FT to get a time representation of the waves (Height map at time **t**)
    - DFT vs FFT: DFT O(n^2) and FFT O(n log2 (n))
    - FFT algo → Cooley-Turkey (or Butterfly) algorithm

# Some fluid dynamics theory

There are three steps for a basic fluids simulation:
* Add forces from velocities or external factors
* Diffuse
* Move

This is inspired by the paper written by Jos Stam: [**Real-Time Fluid Dynamics for Games**](https://damassets.autodesk.net/content/dam/autodesk/research/publications-assets/pdf/realtime-fluid-dynamics-for.pdf)

To keep it simple:
* Diffusion means that each cell exchanges density with its direct neighbors
* Advection means that the density moves through a static velocity field (We could have some fixed velocity for example to simulate wind)


# To-Do

* Research about shallow water simulations
* Investigate all the mathematical tools that should be used to conduct an accurate simulation
* Decide on the proper parameters to use for the oil spill simulation
* Develop basic 2D simulation in python or c++