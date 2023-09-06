<div align="center">
<a href="https://www.glowbuzzer.com">
    <img src="images/tiny-logo.svg" alt="Logo" width="100" height="120">
  </a>
<h3 align="center">glowbuzzer - gbem</h3>
  <h4 align="center">
    <b>EtherCAT master</b>
    <br />
    <br />
    <br />
  </h4>
</div>

# What is it?

GBEM is based on the Open EtherCAT Society's [SOEM](https://github.com/OpenEtherCATsociety/SOEM) (Simple Open EtherCAT
master) project.

GBEM provides a framework to control EtherCAT slaves. It allows you to easily configure a machine (a combination of
EtherCAT IO devices and servo and stepper drives) and provides a simple interface through which you can interact with
that machine without worrying about the nitty-gritty details of how EtherCAT drives and IO devices work.

# Main features

* A CiA 402 based state machine framework to control machines and drives
* A configuration structure in which you can define the layout of your EtherCAT network (i.e. the structure of your
  machine)
* An ability to write your own PLC style functions in the C programming language to control EtherCAT IO
* A simple shared memory interface over which drives and IO can be controlled from your web stack high-level control
  application
* A signal, synchronous with the fieldbus bus cycle, to trigger the cyclic execution of higher level control functions
* Real-time execution - the lowest achievable cycle times depends on hardware. Typically, 0.5ms, 1ms or 4ms cycles are
  used
* A code based suitable for use on both Linux and microcontroller targets
* Open Source code base

# Build

`mkdir build`

`cd build`

`cmake ..`

`make`

# Configure

GBEM needs to have a machine configured. A machine defines the EtherCAT slaves that make up the machine.

See [Configuring machines](https://www.glowbuzzer.com/docs/gbem/configuring_machines) for details on how to do this.

# Run

To scan for slaves on an EtherCAT network using network interface `eth0`:
`./GBEM -n -i eth0`

To run cyclically using network interface `eth0`:
`./GBEM -c -i eth0`

# Documentation

For SOEM documentation see [the SOEM github repository](https://openethercatsociety.github.io/doc/soem/).

For the glowbuzzer framework around SOEM (GBEM)
see [GBEM overivew on the glowbuzzer website](https://www.glowbuzzer.com/docs/gbem/overview).

# Important points

To swap between pre-configured machine configurations search the code for "MACHINE_SWAP" you will find a #define to
toggle. You will need to do this before you compile and run GBEM.

When adding code to support a new drive, you will need to add a library for the new drive and write the required
functions. Two CMakeLists.txt files will need to be updated. Also, a few simple changes will need to made elsewhere in
the code. Search for the code for "NEW_DRIVE" to find the locations where these changes are needed.

When adding a new machine configuration (that uses existing slave and drive functions), you will not need to write any
functions. Small changes are needed to enable the switching between configurations. These can be found by searching
for "NEW_MACHINE".