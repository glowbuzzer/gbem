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
GBEM is based on the Open EtherCAT Society's [SOEM](https://github.com/OpenEtherCATsociety/SOEM) (Simple Open EtherCAT master) project.

GBEM provides a framework to control EtherCAT slaves. It allows you to easily configure a machine (a combination of EtherCAT IO devices and servo and stepper drives) and provides a simple interface through which you can interact with that machine without worrying about the nitty-gritty details of how EtherCAT drives and IO devices work.

# Main features
* A CiA 402 based state machine framework to control machines and drives
* A configuration structure in which you can define the layout of your EtherCAT network (i.e. the structure of your machine)
* An ability to write your own PLC style functions in the C programming language to control EtherCAT IO
* A simple shared memory interface over which drives and IO can be controlled from your web stack high-level control application
* A signal, synchronous with the fieldbus bus cycle, to trigger the cyclic execution of higher level control functions
* Real-time execution - the lowest achievable cycle times depends on hardware. Typically, 0.5ms, 1ms or 4ms cycles are used
* A code based suitable for use on both Linux and microcontroller targets
* Open Source code base
